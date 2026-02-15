# Supported Data Types

- Boolean type - `bool`
- Numeric types:
  - `int32_t`
  - `uint32_t`
  - `int64_t`
  - `uint64_t`
  - `float`
  - `double`
- String type - `std::string`
- Optional values - `std::optional<T>`
- Arrays - `std::vector<T>`
- Polymorphic objects - `std::unique_ptr<BaseClass>` or `std::shared_ptr<BaseClass>` (using polymorphic macros)
- Object types described using **MacroJson** macros
