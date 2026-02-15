# Поддерживаемые типы данных

- Логический тип - `bool`
- Числовые типы:
  - `int32_t`
  - `uint32_t`
  - `int64_t`
  - `uint64_t`
  - `float`
  - `double`
- Строковый тип - `std::string`
- Опциональные значения - `std::optional<T>`
- Массивы - `std::vector<T>`
- Полиморфные объекты - `std::unique_ptr<BaseClass>` или `std::shared_ptr<BaseClass>` (с использованием полиморфных макросов)
- Типы объектов, описанные с помощью макросов **MacroJson**
