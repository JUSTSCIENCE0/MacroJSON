# Description of validation parameters

## Validation of string fields
### Regular expressions
To set a mandatory regular expression for a string, use the `StringRegex` structure. The `pattern` field must contain a string representing the regular expression. If an empty regular expression is specified, validation against the pattern will not be performed and the `pattern` field will be omitted in the generated JSON schema.
```cpp
struct StringRegex {
    std::string pattern = "";
};
```

### String Length
The minimum and maximum length of a string can be set using the `StringLength` structure. The `minLength` field should contain the minimum length of the string, and the `maxLength` field should contain the maximum length. If the value of a field is `-1`, then the corresponding check will not be performed and the field will be omitted in the generated JSON schema.
```cpp
struct StringLength {
    int minLength = -1;
    int maxLength = -1;
};
```

## Validation of numeric fields
### Multiplicity
To set the multiplicity of a numeric field, use the `MultipleOf` structure. The `multipleOf` field must contain the value to which the field's value should be a multiple. If the value of this field is `0`, then the corresponding check will not be performed and the field will be omitted in the generated JSON schema.
```cpp
template<typename NUM,
            std::enable_if_t<std::is_arithmetic_v<NUM>, bool> = true>
struct MultipleOf {
    NUM multipleOf{};
};
```

### Minimum and maximum value
The minimum and maximum value of a numeric field can be set using the `Range` structure. The `minimum` and `maximum` fields should contain the minimum and maximum allowable values, respectively. The `isExclusiveMinimum` and `isExclusiveMaximum` fields determine whether the boundaries include the minimum and maximum values in the allowable range.
```cpp
template<typename NUM,
            std::enable_if_t<std::is_arithmetic_v<NUM>, bool> = true>
struct Range {
    NUM minimum = std::numeric_limits<NUM>::min();
    bool isExclusiveMinimum = false;
    NUM maximum = std::numeric_limits<NUM>::max();
    bool isExclusiveMaximum = false;
};
```

## Validation of arrays
Parameters for array validation are specified using the `ArrayParams` structure. The `minItems` and `maxItems` fields define the minimum and maximum number of elements in the array, respectively. The `uniqueItems` field indicates whether the elements in the array must be unique.
If the `minItems` field is set to `0`, the array can be empty. In this case, the `minItems` field will not be included in the generated JSON schema.
If the `maxItems` field is set to `-1`, there is no limit on the number of elements. In this case, the `maxItems` field will not be included in the generated JSON schema.
If the `uniqueItems` field is set to `false`, array elements can repeat. In this case, the `uniqueItems` field will not be included in the generated JSON schema.

```cpp
struct ArrayParams {
    int minItems = 0;
    int maxItems = -1;
    bool uniqueItems = false;
};
```

**MacroJson** allows specifying validation parameters for an array as a whole and for the type of elements in the array. In this case, you must first specify `ArrayParams` as the first argument to validation, followed by the validation parameters for the type of elements in the array.
If you do not need to validate the array parameters, you can omit `ArrayParams` and directly pass the validation parameters for the type of elements in the array.


Is code the best documentation? Check out [full functionality demonstration](../apps/base-demo)

