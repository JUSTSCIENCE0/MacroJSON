# Advanced Features

## Polymorphic Objects

Let's say that one of the fields in a JSON object is an array of similar but heterogeneous objects. Each object extends from some base class and has its own unique fields.\
For example:
```json
{
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
  ]
}
```

The **MacroJson** library allows for the automatic generation of code to work with such JSON objects. \
We can identify common characteristics that are shared by all these objects and define a parent type `Vehicle` with an additional field `type`, which will determine the specific type of vehicle. Additional properties will be distributed among the corresponding child classes. \
In terms of **MacroJson** semantics, this could look like the following:
```cpp
#define MJSON_BASE_OBJECT_NAME Vehicle
MJSON_POLYMORPHIC_OBJECT_BEGIN(
        "Vehicle",
        "Person's Vehicle")
    MJSON_BASE_OBJECT_BEGIN(VehicleType)
      MJSON_BASE_OBJECT_FIELD(int, id,
        "ID",
        "Unique identifier of the vehicle")
      MJSON_BASE_OBJECT_FIELD(std::string, model,
        "Model",
        "Vehicle model name")
    MJSON_BASE_OBJECT_END()
    MJSON_DERIVED_OBJECT_BEGIN(Car, E_VEHICLE_CAR)
      MJSON_DERIVED_OBJECT_FIELD(int, doors,
        "Doors",
        "Number of doors")
      MJSON_DERIVED_OBJECT_FIELD(std::string, fuel_type,
        "Fuel Type",
        "Type of fuel used by the vehicle")
    MJSON_DERIVED_OBJECT_END(Car)
    MJSON_DERIVED_OBJECT_BEGIN(Motorcycle, E_VEHICLE_MOTORCYCLE)
      MJSON_DERIVED_OBJECT_FIELD(int, engine_size,
        "Engine Size",
        "Size of the motorcycle's engine")
    MJSON_DERIVED_OBJECT_END(Motorcycle)
    MJSON_DERIVED_OBJECT_BEGIN(Truck, E_VEHICLE_TRUCK)
      MJSON_DERIVED_OBJECT_FIELD(int, payload_capacity,
        "Payload Capacity",
        "Maximum weight the truck can carry")
      MJSON_DERIVED_OBJECT_FIELD(std::string, cab_type,
        "Cab Type",
        "Type of cab for the truck")
    MJSON_DERIVED_OBJECT_END(Truck)
MJSON_POLYMORPHIC_OBJECT_END()
#undef MJSON_BASE_OBJECT_NAME
```

Polymorphic objects are stored by pointer, and memory management is handled using `std::unique_ptr` or `std::shared_ptr`.\
An object that has an array of polymorphic objects as a field will look like this:
```cpp
MJSON_OBJECT_BEGIN(Products, nullptr, nullptr)
    MJSON_FIELD(std::vector<std::unique_ptr<Vehicle>>, products,
        "Products",
        "List of vehicles")
MJSON_OBJECT_END(Products)
```

Is code the best documentation? Check out [demo example](../apps/doc-demo)
