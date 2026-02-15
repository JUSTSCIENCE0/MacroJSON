// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "person.hpp"

#include <iostream>

int main() {
    auto person_schema = macrojson::get_json_schema_str<Person>();
    std::cout << "Person JSON Schema:\n" << person_schema << std::endl;

    const char* person_json_str = R"({
        "name": "Mike Smith",
        "age": 21,
        "email": "mike.smith@example.com",
        "employee": {
            "id": 67890,
            "department": "Marketing",
            "position": "Manager"
        }
    })";

    std::string error_descr{};
    macrojson::MJsonErrorCode err_code = macrojson::validate_json_str<Person>(person_json_str, error_descr);
    if (err_code != macrojson::E_MJSON_OK) {
        std::cerr << "JSON does not conform to schema: " << error_descr << std::endl;
        return 1;
    } else {
        std::cout << "JSON is valid according to schema." << std::endl;
    }

    Person person_in{};
    error_descr = "";
    err_code = macrojson::json_str_to_object(person_json_str, person_in, error_descr);
    if (err_code != macrojson::E_MJSON_OK) {
        std::cerr << "Error parsing JSON: " << error_descr << std::endl;
        return 1;
    }

    // work with person_in object...

    // not compilant with schema example
    const char* invalid_person_json_str = R"({
        "name": "Invalid Person",
        "age": 17,
        "email": "invalid-email",
        "employee": {
            "id": 123,
            "department": "Unknown"
        }
    })";
    error_descr = "";
    err_code = macrojson::validate_json_str<Person>(invalid_person_json_str, error_descr);
    if (err_code != macrojson::E_MJSON_OK) {
        std::cout << "JSON does not conform to schema: " << error_descr << std::endl;
    } else {
        std::cerr << "JSON is unexpectedly valid according to schema." << std::endl;
        return 1;
    }

    Person person_out {
        /*name*/  "John Doe",
        /*age*/   30,
        /*email*/ "john.doe@example.com",
        Employee{
            /*id*/ 12345,
            /*department*/ "Engineering"
        },
        /*skills*/ { "C++", "JSON", "MacroJson" }
    };

    std::string person_json = "";
    macrojson::object_to_json_str(person_out, person_json);
    std::cout << "Person JSON:\n" << person_json << std::endl;

    // Polymorphic objects examples
    std::cout << "Polymorphic objects examples:" << std::endl;

    // single object
    std::shared_ptr<Vehicle> car = std::make_shared<Car>();
    auto car_ptr = static_cast<Car*>(car.get());
    car_ptr->id = 1001;
    car_ptr->model = "Toyota Corolla";
    car_ptr->doors = 4;
    car_ptr->fuel_type = "gasoline";
    std::string car_json = "";
    macrojson::object_to_json_str(car, car_json);
    std::cout << "Car JSON:\n" << car_json << std::endl;

    // complex object
    auto products_schema = macrojson::get_json_schema_str<Products>();
    std::cout << "Products schema:\n" << products_schema << std::endl;

    const char* products_example = R"({
    "products": [
        {
            "type": "car",
            "id": 1001,
            "model": "Toyota Corolla",
            "doors": 4,
            "fuel_type": "gasoline"
        },
        {
            "type": "motorcycle",
            "id": 1002,
            "model": "Harley-Davidson Street 750",
            "engine_size": 750
        },
        {
            "type": "truck",
            "id": 1003,
            "model": "Ford F-150",
            "payload_capacity": 1000,
            "cab_type": "crew"
        }
    ]})";

    Products products{};
    error_descr = "";
    err_code = macrojson::json_str_to_object(products_example, products, error_descr);
    if (err_code != macrojson::E_MJSON_OK) {
        std::cerr << "Failed to parse JSON: " << error_descr << std::endl;
        return -1;
    }

    return 0;
}