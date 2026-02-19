# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

import os
import sys
import json

def get_properties(descr : dict) -> tuple[dict, set]:
    properties = descr.get("properties", {})
    required = descr.get("required", [])
    required = set(required)
    return properties, required

objects_counter = 0
def generate_object_name() -> str:
    global objects_counter
    objects_counter += 1
    return f"Object_{objects_counter}"

def get_title(descr : dict) -> str:
    return descr.get("title", generate_object_name())

def inference_integer_type(descr : dict) -> str:
    is_signed = True
    is_64 = False
    min_i32 = -2**31
    max_i32 = 2**31 - 1
    min_val = descr.get("minimum", 
                        descr.get("exclusiveMinimum", min_i32))
    max_val = descr.get("maximum", 
                        descr.get("exclusiveMaximum", max_i32))
    # inference type

def parse_schema_object(root : dict, objects : dict):
    objects_counter += 1
    props, required = get_properties(root)
    title = get_title(root)
    objects[title] = {
        "name": title,
        "fields": []
    }
    for name, descr in props.items():
        field_descr = {}
        if descr.get("type") == "integer":
            pass


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

    print(json_schema)

    objects_dict = {}

    


    return 0

if __name__ == "__main__":
    sys.exit(main())