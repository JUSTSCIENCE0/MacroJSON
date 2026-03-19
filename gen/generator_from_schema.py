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

def determine_object(root: dict, objects_list: list) -> str:
    if "oneOf" in root:
        obj = PolymorphicObjectParser(root, objects_list)
        return obj.base["identifier"]
    else:
        obj = ObjectParser(root, objects_list)
        return obj.identifier

def determine_string(root: dict, objects_list: list) -> str:
    if "enum" not in root:
        return "std::string"
    enum_obj = EnumParser(root, objects_list)
    return enum_obj.identifier

# parsers
class BaseParser:
    def __init__(self, root: dict, objects_list: list):
        self.title = get_title(root)
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
            field_descr["type"] = determine_string(root, objects_list)
        if obj_type == "object":
            field_descr["type"] = determine_object(root, objects_list)
        if obj_type == "array":
            items_type = root["items"]["type"]
            if items_type == "object":
                items_type = determine_object(root["items"], objects_list)
            if items_type == "string":
                items_type = determine_string(root["items"], objects_list)
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

    def generate_code(self) -> str:
        result  = f'MJSON_OBJECT_BEGIN({self.identifier},\n'
        result += f'        {self.title},\n'
        result += f'        {self.description})\n'
        for field in self.fields:
            field_type = field["type"]
            if field["optional"] and not field_type.startswith('std::vector'):
                field_type = f'std::optional<{field_type}>'
            result += f'    MJSON_FIELD({field_type}, {field["name"]},\n'
            result += f'        {field["title"]},\n'
            result += f'        {field["description"]}'
            result += f')\n'
        result += f'MJSON_OBJECT_END({self.identifier})\n\n'
        return result

class EnumParser(BaseParser):
    def __init__(self: dict, root, objects_list: list):
        self.type = "enum"
        super().__init__(root, objects_list)
        self.units = []

        self.identifier = title_to_identifier(self.title, len(objects_list))

        for unit in root["enum"]:
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
        return False

    def is_identifier_collision(self, identifier: str) -> bool:
        return self.identifier == identifier

    def generate_code(self) -> str:
        result  = f'MJSON_ENUM_BEGIN({self.identifier},\n'
        result += f'        {self.title},\n'
        result += f'        {self.description})\n'
        for unit in self.units:
            result += f'    MJSON_ENUM_UNIT({unit["name"]}, {unit["short_name"]})\n'
        result += f'MJSON_ENUM_END({self.identifier})\n\n'
        return result

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

        # generate enum description
        parsed_types_enum = EnumParser(types_enum, objects_list)
        self.base["types_enum"] = parsed_types_enum.identifier
        types_enum_descr = self.find_enum_description(parsed_types_enum.identifier, objects_list)

        # resolve derived type enumerators
        for der in self.derived:
            der["enumerator"] = self.find_unit_name_in_enum_description(der["enumerator"], types_enum_descr)

        # check exists object
        for obj in objects_list:
            if self.is_same(obj):
                self.base["identifier"] = obj.base["identifier"]

                check_derived = lambda lhs, rhs: (
                    lhs["enumerator"] == rhs["enumerator"] and
                    lhs["fields"] == rhs["fields"]
                )
                for self_der in self.derived:
                    for other_der in obj.derived:
                        if check_derived(self_der, other_der):
                            self_der["identifier"] = other_der["identifier"]
                            break
                return

        objects_list.append(self)

    @staticmethod
    def find_enum_description(identifier : str, objects_list: list) -> dict:
        for obj in objects_list:
            if isinstance(obj, EnumParser) and obj.identifier == identifier:
                return obj
        return None

    @staticmethod
    def find_unit_name_in_enum_description(short_name : str, enum_descr : EnumParser) -> str:
        for unit in enum_descr.units:
            if unit["short_name"] == short_name:
                return unit["name"]
        raise ValueError(f"Error: Unit name '{short_name}' not found in enum description.")

    def is_same(self, other) -> bool:
        if not isinstance(other, PolymorphicObjectParser):
            return False

        same_base = (
            self.base["fields"] == other.base["fields"] and
            self.base["types_enum"] == other.base["types_enum"]
        )
        if not same_base:
            return False

        if len(self.derived) != len(other.derived):
            return False

        check_derived = lambda lhs, rhs: (
            lhs["enumerator"] == rhs["enumerator"] and
            lhs["fields"] == rhs["fields"]
        )
        for self_der in self.derived:
            found_same = False
            for other_der in other.derived:
                if check_derived(self_der, other_der):
                    found_same = True
                    break
            if not found_same:
                return False

        return True

    def is_identifier_collision(self, identifier: str) -> bool:
        if self.base["identifier"] == identifier:
            return True
        return any(der["identifier"] == identifier for der in self.derived)

    def generate_code(self) -> str:
        result  = f'#define MJSON_BASE_OBJECT_NAME {self.base["identifier"]}\n'
        result += f'MJSON_POLYMORPHIC_OBJECT_BEGIN(\n'
        result += f'        {self.title},\n'
        result += f'        {self.description})\n'
        result += f'    MJSON_BASE_OBJECT_BEGIN({self.base["types_enum"]})\n'
        for field in self.base["fields"]:
            field_type = field["type"]
            if field["optional"] and not field_type.startswith('std::vector'):
                field_type = f'std::optional<{field_type}>'
            result += f'      MJSON_BASE_OBJECT_FIELD({field_type}, {field["name"]},\n'
            result += f'        {field["title"]},\n'
            result += f'        {field["description"]}'
            result += f')\n'
        result += f'    MJSON_BASE_OBJECT_END()\n'
        for derived in self.derived:
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
        root_obj = determine_object(self.json_schema, self.objects)
        for obj in self.objects:
            print(vars(obj))
        code = self.generate_code()
        print(code)

    def find_enum_description(self, identifier : str) -> dict:
        for obj in self.objects:
            if obj["type"] == "enum" and obj["identifier"] == identifier:
                return obj

    def generate_code(self) -> str:
        result  = '// This file is generated by MacroJSON\'s generator_from_schema.py\n'
        result += '// Do not edit this file manually or add it to version control\n\n'

        for obj in self.objects:
            result += obj.generate_code()

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
