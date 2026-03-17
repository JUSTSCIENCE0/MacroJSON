# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

import os
import sys
import json
import re

# helpers
def get_properties(descr : dict) -> tuple[dict, set]:
    properties = descr.get("properties", {})
    required = descr.get("required", [])
    required = set(required)
    return properties, required

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

def find_unit_name_in_enum_description(short_name : str, enum_descr : dict) -> str:
    for unit in enum_descr["units"]:
        if unit["short_name"] == short_name:
            return unit["name"]
    raise ValueError(f"Error: Unit name '{short_name} not found in enum description.")

def generate_object_name(objects_amount : int) -> str:
    return f"Object{objects_amount}"

def title_to_identifier(title : str, objects_amount : int) -> str:
    if title == 'nullptr':
        return generate_object_name(objects_amount)

    name = re.sub(r'[^a-zA-Z0-9 ]', '', title)
    words = name.split()
    object_name = ''.join(word.capitalize() for word in words)
    if not object_name:
        return generate_object_name(objects_amount)
    if object_name[0].isdigit():
        object_name = 'Obj' + object_name
    return object_name

# parsers
class BaseParser:
    def __init__(self, root: dict, objects_list: list):
        self.title = get_title(get_title(root))
        self.description = get_description(root)

    def parse_field(self, name: str, root: dict, is_optional: bool, objects_list: list) -> dict:
        field_descr = {
            "name": name,
            "title": get_title(root),
            "description": get_description(root),
            "optional": is_optional
        }
        obj_type = root.get("type")
        if obj_type == "boolean":
            field_descr["type"] = "bool"
        if obj_type == "integer":
            field_descr["type"] = inference_integer_type(root)
        if obj_type == "number":
            field_descr["type"] = "double"
        if obj_type == "string":
            if "enum" not in root:
                field_descr["type"] = "std::string"
            # else:
            #     field_descr["type"] = self.parse_schema_enum(root)
        if obj_type == "object":
            obj = ObjectParser(root, objects_list)
            field_descr["type"] = obj.identifier
        if obj_type == "array":
            items_type = root["items"]["type"]
            if items_type == "object":
                obj = ObjectParser(root["items"], objects_list)
                items_type = obj.identifier
            # if items_type == "string" and "enum" in root["items"]:
            #     items_type = self.parse_schema_enum(root["items"])
            field_descr["type"] = f"std::vector<{items_type}>"

        return field_descr

    @staticmethod
    def make_unique_identifier(identifier, objects_list: list) -> str:
        for obj in objects_list:
            if obj.is_identifier_collision(identifier):
                identifier += str(len(objects_list))
                break
        return identifier

class ObjectParser(BaseParser):
    def __init__(self, root: dict, objects_list: list):
        self.type = "object"
        super().__init__(root, objects_list)
        self.fields = []

        props, required = get_properties(root)
        for name, descr in props.items():
            optional = name not in required
            field_descr = self.parse_field(name, descr, optional, objects_list)
            self.fields.append(field_descr)

        for obj in objects_list:
            if self.is_same(obj):
                self.identifier = obj.identifier
                return

        self.identifier = title_to_identifier(self.title, len(objects_list))
        self.identifier = self.make_unique_identifier(self.identifier, objects_list)
        objects_list.append(self)

    def is_same(self, other) -> bool:
        if isinstance(other, ObjectParser):
            return self.fields == other.fields
        return False

    def is_identifier_collision(self, identifier: str) -> bool:
        return self.identifier == identifier

class EnumParser(BaseParser):
    def __init__(self: dict, root, objects_list: list):
        self.type = "enum"
        super().__init__(root, objects_list)
        self.units = []

        self.identifier = title_to_identifier(self.title, len(objects_list))

        for unit in root["units"]:
            unit_descr = {
                "name": "E_" + self.identifier.upper() + "_" + unit.upper(),
                "short_name": unit
            }
            self.units.append(unit_descr)

        for obj in objects_list:
            if self.is_same(obj):
                self.identifier = obj.identifier
                return

        self.identifier = self.make_unique_identifier(self.identifier, objects_list)
        objects_list.append(self)

    def is_same(self, other) -> bool:
        if isinstance(other, EnumParser):
            return self.units == other.units

    def is_identifier_collision(self, identifier: str) -> bool:
        return self.identifier == identifier

class PolymorphicObjectParser(BaseParser):
    def __init__(self, root: dict, objects_list: list):
        self.type = "polymorphic_object"
        super().__init__(root, objects_list)

        base_object_identifier = title_to_identifier(root.get("title", "Polymorphic Object") + " Base", len(objects_list))
        base_object_identifier = self.make_unique_identifier(base_object_identifier, objects_list)

        # parse base object fields
        self.base = {
            "identifier": base_object_identifier,
            "types_enum": "",
            "fields": []
        }
        props, required = get_properties(root)
        for name, descr in props.items():
            optional = name not in required
            field_descr = self.parse_field(name, descr, optional, objects_list)
            self.base["fields"].append(field_descr)

        # parse types enum and derived objects
        types_enum = {
            "title": root.get("title", "Polymorphic Object") + " Types",
            "enum": []
        }
        self.derived = []
        for derived in root["oneOf"]:
            der_props, der_required = get_properties(derived)
            if "type" not in der_props or "type" not in der_required:
                continue
            if "const" not in der_props["type"]:
                continue
            type_enum_unit_short_name = der_props["type"]["const"]
            types_enum["enum"].append(type_enum_unit_short_name)

            der_identifier = title_to_identifier(
                root.get("title", "Polymorphic Object") + " " + type_enum_unit_short_name,
                len(objects_list)
            )
            der_identifier = self.make_unique_identifier(der_identifier, objects_list)
            der_object_descr = {
                "identifier": der_identifier,
                "enumerator": type_enum_unit_short_name,
                "fields": []
            }
            for der_name, der_descr in der_props.items():
                if der_name == "type":
                    continue
                optional = der_name not in der_required
                der_field_descr = self.parse_field(der_name, der_descr, optional, objects_list)
                der_object_descr["fields"].append(der_field_descr)
            self.derived.append(der_object_descr)

            # checkpoint

            # generate enum description
            types_enum_name = self.parse_schema_enum(types_enum)
            object_descr["base"]["types_enum"] = types_enum_name
            types_enum_descr = self.find_enum_description(types_enum_name)

            # resolve derived type enumerators
            for der in object_descr["derived"]:
                der["enumerator"] = find_unit_name_in_enum_description(der["enumerator"], types_enum_descr)

            # TODO: check exists object

            self.objects.append(object_descr)
            #return base_object_identifier

    @staticmethod
    def find_enum_description(identifier : str, objects_list: list) -> dict:
        for obj in objects_list:
            if isinstance(obj, EnumParser) and obj.identifier == identifier:
                return obj
        return None

# main class
class Generator:
    def __init__(self, schema_file: str):
        file_path = os.path.abspath(schema_file)

        if not os.path.exists(file_path):
            raise ValueError(f"Error: The file {file_path} does not exist.")

        try:
            with open(file_path, 'r') as f:
                self.json_schema = json.load(f)
        except json.JSONDecodeError as e:
            raise ValueError(f"Error: Failed to decode JSON from {file_path}. {e}")

        self.objects = []
        self.objects_counter = 0

    def generate_header(self, output_dir: str):
        self.parse_schema_object(self.json_schema)
        print(self.objects)
        code = self.generate_code()
        print(code)

    def unique_identifier(self, identifier : str) -> str:
        collision = False

        for descr in self.objects:
            # TODO: refactor this code
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
            result += str(self.objects_counter)
        return result

    # TODO: refactor this method
    def parse_schema_enum(self, enum : dict) -> str:
        self.objects_counter += 1

        title = get_title(enum)
        descr_text = get_description(enum)
        identifier = title_to_identifier(title, len(self.objects))

        enum_descr = {
            "type": "enum",
            "identifier": "",
            "number": self.objects_counter,
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
        for descr in self.objects:
            if descr["type"] == "enum" and descr["units"] == enum_descr["units"]:
                return descr["identifier"]

        identifier = self.unique_identifier(identifier)
        enum_descr["identifier"] = identifier
        self.objects.append(enum_descr)
        return identifier

    def parse_schema_field(self, name: str, descr: dict, optional: bool) -> dict:
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
                field_descr["type"] = self.parse_schema_enum(descr)
        if obj_type == "object":
            field_descr["type"] = self.parse_schema_object(descr)
        if obj_type == "array":
            items_type = descr["items"]["type"]
            if items_type == "object":
                items_type = self.parse_schema_object(descr["items"])
            if items_type == "string" and "enum" in descr["items"]:
                items_type = self.parse_schema_enum(descr["items"])
            field_descr["type"] = f"std::vector<{items_type}>"

        return field_descr

    def find_enum_description(self, identifier : str) -> dict:
        for obj in self.objects:
            if obj["type"] == "enum" and obj["identifier"] == identifier:
                return obj

    # TODO: refactor this method
    def parse_schema_polymorphic_object(self, root : dict) -> str:
        self.objects_counter += 1

        base_object_identifier = title_to_identifier(root.get("title", "Polymorphic Object") + " Base", len(self.objects))
        base_object_identifier = self.unique_identifier(base_object_identifier)

        title = get_title(root)
        descr_text = get_description(root)
        identifier = title_to_identifier(title, len(self.objects))
        object_descr = {
            "type": "polymorphic_object",
            "number": self.objects_counter,
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
            field_descr = self.parse_schema_field(name, descr, optional)
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
                root.get("title", "Polymorphic Object") + " " + type_enum_unit_short_name,
                len(self.objects)
            )
            der_identifier = self.unique_identifier(der_identifier)
            der_object_descr = {
                "identifier": der_identifier,
                "enumerator": type_enum_unit_short_name,
                "fields": []
            }
            for der_name, der_descr in der_props.items():
                if der_name == "type":
                    continue
                optional = der_name not in der_required
                der_field_descr = self.parse_schema_field(der_name, der_descr, optional)
                der_object_descr["fields"].append(der_field_descr)
            object_descr["derived"].append(der_object_descr)

        # generate enum description
        types_enum_name = self.parse_schema_enum(types_enum)
        object_descr["base"]["types_enum"] = types_enum_name
        types_enum_descr = self.find_enum_description(types_enum_name)

        # resolve derived type enumerators
        for der in object_descr["derived"]:
            der["enumerator"] = find_unit_name_in_enum_description(der["enumerator"], types_enum_descr)

        # TODO: check exists object

        self.objects.append(object_descr)
        return base_object_identifier

    # TODO: refactor this method
    def parse_schema_object(self, root : dict) -> str:
        if "oneOf" in root:
            base_object_type = self.parse_schema_polymorphic_object(root)
            return f'std::shared_ptr<{base_object_type}>'

        self.objects_counter += 1

        props, required = get_properties(root)
        title = get_title(root)
        descr_text = get_description(root)
        identifier = title_to_identifier(title, len(self.objects))
        object_descr = {
            "type": "object",
            "identifier": "",
            "number": self.objects_counter,
            "title": title,
            "description": descr_text,
            "fields": []
        }

        for name, descr in props.items():
            optional = name not in required
            field_descr = self.parse_schema_field(name, descr, optional)
            object_descr["fields"].append(field_descr)

        # check exists object
        for descr in self.objects:
            if descr["type"] == "object" and descr["fields"] == object_descr["fields"]:
                return descr["identifier"]

        identifier = self.unique_identifier(identifier)
        object_descr["identifier"] = identifier
        self.objects.append(object_descr)
        return identifier

    def generate_code(self) -> str:
        result = '// This file is generated by MacroJSON\'s generator_from_schema.py\n'
        result += '// Do not edit this file manually or add it to version control\n\n'

        for obj in self.objects:
            # TODO: refactore this
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
    generator = Generator(schema_file)
    generator.generate_header(None)

    return 0

if __name__ == "__main__":
    sys.exit(main())