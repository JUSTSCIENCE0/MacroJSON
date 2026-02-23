# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

import os
import sys
import json
import re

g_objects_counter = 0

def get_properties(descr : dict) -> tuple[dict, set]:
    properties = descr.get("properties", {})
    required = descr.get("required", [])
    required = set(required)
    return properties, required

def generate_object_name() -> str:
    global g_objects_counter
    return f"Object{g_objects_counter}"

def get_title(descr : dict) -> str:
    title = descr.get("title", None)
    if not title:
        return 'nullptr'
    return f'"{title}"'

def get_description(descr : dict) -> str:
    description = descr.get("description", None)
    if not description:
        return 'nullptr'
    return f'"{description}"'

def title_to_identifier(title : str) -> str:
    if title == 'nullptr':
        return generate_object_name()

    name = re.sub(r'[^a-zA-Z0-9 ]', '', title)
    words = name.split()
    object_name = ''.join(word.capitalize() for word in words)
    if not object_name:
        return generate_object_name()
    if object_name[0].isdigit():
        object_name = 'Obj' + object_name
    return object_name

def inference_integer_type(descr : dict) -> str:
    is_signed = True
    is_64 = False
    min_i32 = -2**31
    max_i32 = 2**31 - 1
    min_val = descr.get("minimum", 
                        descr.get("exclusiveMinimum", min_i32))
    max_val = descr.get("maximum", 
                        descr.get("exclusiveMaximum", max_i32))
    if min_val < min_i32 or max_val > max_i32:
        is_64 = True
    if min_val >= 0:
        is_signed = False
    if is_64:
        result = "int64_t"
    else:
        result = "int32_t"
    if not is_signed:
        result = "u" + result
    return result

def unique_identifier(identifier : str, objects : list) -> str:
    global g_objects_counter

    collision = False

    for descr in objects:
        if descr["type"] == "object" or descr["type"] == "enum":
            if descr["identifier"] == identifier:
                collision = True
                break
        elif descr["type"] == "polymorphic_object":
            if descr["base"]["identifier"] == identifier:
                collision = True
                break
            for der_obj in descr["derived"]:
                if der_obj["identifier"] == identifier:
                    collision = True
                    break
            if collision:
                break

    result = identifier
    if collision:
        result += str(g_objects_counter)
    return result

def parse_schema_enum(enum : dict, objects : list) -> str:
    global g_objects_counter
    g_objects_counter += 1

    title = get_title(enum)
    descr_text = get_description(enum)
    identifier = title_to_identifier(title)

    enum_descr = {
        "type": "enum",
        "identifier": "",
        "number": g_objects_counter,
        "title": title,
        "description": descr_text,
        "units": []
    }

    for unit in enum["enum"]:
        unit_descr = {
            "name": "E_" + identifier.upper() + "_" + unit.upper(),
            "short_name": unit
        }
        enum_descr["units"].append(unit_descr)

    # check exists enum
    for descr in objects:
        if descr["type"] == "enum" and descr["units"] == enum_descr["units"]:
            return descr["identifier"]

    identifier = unique_identifier(identifier, objects)
    enum_descr["identifier"] = identifier
    objects.append(enum_descr)
    return identifier

def parse_schema_field(name: str, descr: dict, optional: bool, objects : list) -> dict:
    field_descr = {
        "name": name,
        "title": get_title(descr),
        "description": get_description(descr),
        "optional": optional
    }
    obj_type = descr.get("type")
    if obj_type == "boolean":
        field_descr["type"] = "bool"
    if obj_type == "integer":
        field_descr["type"] = inference_integer_type(descr)
    if obj_type == "number":
        field_descr["type"] = "double"
    if obj_type == "string":
        if "enum" not in descr:
            field_descr["type"] = "std::string"
        else:
            field_descr["type"] = parse_schema_enum(descr, objects)
    if obj_type == "object":
        field_descr["type"] = parse_schema_object(descr, objects)
    if obj_type == "array":
        items_type = descr["items"]["type"]
        if items_type == "object":
            items_type = parse_schema_object(descr["items"], objects)
        if items_type == "string" and "enum" in descr["items"]:
            items_type = parse_schema_enum(descr["items"], objects)
        field_descr["type"] = f"std::vector<{items_type}>"

    return field_descr

def find_enum_description(identifier : str, objects : list) -> dict:
    for obj in objects:
        if obj["type"] == "enum" and obj["identifier"] == identifier:
            return obj

def find_unit_name_in_enum_description(short_name : str, enum_descr : dict) -> str:
    for unit in enum_descr["units"]:
        if unit["short_name"] == short_name:
            return unit["name"]

def parse_schema_polymorphic_object(root : dict, objects : list) -> str:
    global g_objects_counter
    g_objects_counter += 1

    base_object_identifier = title_to_identifier(root.get("title", "Polymorphic Object") + " Base")
    base_object_identifier = unique_identifier(base_object_identifier, objects)

    title = get_title(root)
    descr_text = get_description(root)
    identifier = title_to_identifier(title)
    object_descr = {
        "type": "polymorphic_object",
        "number": g_objects_counter,
        "title": title,
        "description": descr_text,
        "base": {
            "identifier": base_object_identifier,
            "types_enum": "",
            "fields": []
        },
        "derived": []
    }

    # parse base object fields
    props, required = get_properties(root)
    for name, descr in props.items():
        optional = name not in required
        field_descr = parse_schema_field(name, descr, optional, objects)
        object_descr["base"]["fields"].append(field_descr)

    # parse types enum and derived objects
    types_enum = {
        "title": root.get("title", "Polymorphic Object") + " Types",
        "enum": []
    }

    derived_objects_counter = 0
    for derived in root["oneOf"]:
        der_props, der_required = get_properties(derived)
        if "type" not in der_props or "type" not in der_required:
            continue
        if "const" not in der_props["type"]:
            continue
        type_enum_unit_short_name = der_props["type"]["const"]
        types_enum["enum"].append(type_enum_unit_short_name)

        derived_objects_counter += 1
        der_identifier = title_to_identifier(
            root.get("title", "Polymorphic Object") + " " + type_enum_unit_short_name)
        der_identifier = unique_identifier(der_identifier, objects)
        der_object_descr = {
            "identifier": der_identifier,
            "enumerator": type_enum_unit_short_name,
            "fields": []
        }
        for der_name, der_descr in der_props.items():
            if der_name == "type":
                continue
            optional = der_name not in der_required
            der_field_descr = parse_schema_field(der_name, der_descr, optional, objects)
            der_object_descr["fields"].append(der_field_descr)
        object_descr["derived"].append(der_object_descr)

    # generate enum description
    types_enum_name = parse_schema_enum(types_enum, objects)
    object_descr["base"]["types_enum"] = types_enum_name
    types_enum_descr = find_enum_description(types_enum_name, objects)

    # resolve derived type enumerators
    for der in object_descr["derived"]:
        der["enumerator"] = find_unit_name_in_enum_description(der["enumerator"], types_enum_descr)

    objects.append(object_descr)
    return base_object_identifier

def parse_schema_object(root : dict, objects : list) -> str:
    if "oneOf" in root:
        base_object_type = parse_schema_polymorphic_object(root, objects)
        return f'std::shared_ptr<{base_object_type}>'

    global g_objects_counter
    g_objects_counter += 1

    props, required = get_properties(root)
    title = get_title(root)
    descr_text = get_description(root)
    identifier = title_to_identifier(title)
    object_descr = {
        "type": "object",
        "identifier": "",
        "number": g_objects_counter,
        "title": title,
        "description": descr_text,
        "fields": []
    }

    for name, descr in props.items():
        optional = name not in required
        field_descr = parse_schema_field(name, descr, optional, objects)
        object_descr["fields"].append(field_descr)

    # check exists object
    for descr in objects:
        if descr["type"] == "object" and descr["fields"] == object_descr["fields"]:
            return descr["identifier"]

    identifier = unique_identifier(identifier, objects)
    object_descr["identifier"] = identifier
    objects.append(object_descr)
    return identifier

def generate_code(objects: list) -> str:
    result = '// This file is generated by MacroJSON\'s generator_from_schema.py\n'
    result += '// Do not edit this file manually or add it to version control\n\n'

    for obj in objects:
        if obj["type"] == "object":
            result += f'MJSON_OBJECT_BEGIN({obj["identifier"]},\n'
            result += f'        {obj["title"]},\n'
            result += f'        {obj["description"]})\n'
            for field in obj["fields"]:
                field_type = field["type"]
                if field["optional"] and not field_type.startswith('std::vector'):
                    field_type = f'std::optional<{field_type}>'
                result += f'    MJSON_FIELD({field_type}, {field["name"]},\n'
                result += f'        {field["title"]},\n'
                result += f'        {field["description"]}'
                result += f')\n'
            result += f'MJSON_OBJECT_END({obj["identifier"]})\n\n'
        elif obj["type"] == "enum":
            result += f'MJSON_ENUM_BEGIN({obj["identifier"]},\n'
            result += f'        {obj["title"]},\n'
            result += f'        {obj["description"]})\n'
            for unit in obj["units"]:
                result += f'    MJSON_ENUM_UNIT({unit["name"]}, {unit["short_name"]})\n'
            result += f'MJSON_ENUM_END({obj["identifier"]})\n\n'
        elif obj["type"] == "polymorphic_object":
            result += f'#define MJSON_BASE_OBJECT_NAME {obj["base"]["identifier"]}\n'
            result += f'MJSON_POLYMORPHIC_OBJECT_BEGIN(\n'
            result += f'        {obj["title"]},\n'
            result += f'        {obj["description"]})\n'
            result += f'    MJSON_BASE_OBJECT_BEGIN({obj["base"]["types_enum"]})\n'
            for field in obj["base"]["fields"]:
                field_type = field["type"]
                if field["optional"] and not field_type.startswith('std::vector'):
                    field_type = f'std::optional<{field_type}>'
                result += f'      MJSON_BASE_OBJECT_FIELD({field_type}, {field["name"]},\n'
                result += f'        {field["title"]},\n'
                result += f'        {field["description"]}'
                result += f')\n'
            result += f'    MJSON_BASE_OBJECT_END()\n'
            for derived in obj["derived"]:
                result += f'    MJSON_DERIVED_OBJECT_BEGIN({derived["identifier"]}, {derived["enumerator"]})\n'
                for field in derived["fields"]:
                    field_type = field["type"]
                    if field["optional"] and not field_type.startswith('std::vector'):
                        field_type = f'std::optional<{field_type}>'
                    result += f'      MJSON_DERIVED_OBJECT_FIELD({field_type}, {field["name"]},\n'
                    result += f'        {field["title"]},\n'
                    result += f'        {field["description"]}'
                    result += f')\n'
                result += f'    MJSON_DERIVED_OBJECT_END({derived["identifier"]})\n'
            result += f'MJSON_POLYMORPHIC_OBJECT_END()\n'
            result += f'#undef MJSON_BASE_OBJECT_NAME\n\n'

    return result

def main():
    if len(sys.argv) < 1:
        print("Usage: python gen/generator_from_schema.py <schema_file>")
        return -1

    schema_file = sys.argv[1]
    file_path = os.path.abspath(schema_file)

    if not os.path.exists(file_path):
        print(f"Error: The file {file_path} does not exist.")
        return -1

    json_schema = None
    try:
        with open(file_path, 'r') as f:
            json_schema = json.load(f)
    except json.JSONDecodeError as e:
        print(f"Error: Failed to decode JSON from {file_path}. {e}")
        return -1

    objects_list = []
    parse_schema_object(json_schema, objects_list)
    print(objects_list)

    code = generate_code(objects_list)
    print(code)

    return 0

if __name__ == "__main__":
    sys.exit(main())