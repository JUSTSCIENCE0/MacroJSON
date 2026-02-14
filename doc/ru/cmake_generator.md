# Генерация заголовочного файла с помощью CMake

**MacroJson** предоставляет CMake-генератор, который позволяет автоматически генерировать заголовочный файл с описанием структуры данных на основе макросов.

## Генератор

```cmake
macrojson_generate_header(descr_file output_file create_objects)
```
descr_file - путь к файлу с описанием структуры данных, который использует макросы библиотеки MacroJson.\
output_file - имя выходного заголовочного файла, который будет сгенерирован. Сгенерированный файл будет создан в папке проекта в подпапке `generated`. Добавьте `generated` в исключения вашей системы контроля версий.\
create_objects - флаг, указывающий, нужно ли создавать объекты структуры данных. Если `TRUE`, то будут созданы объекты структуры данных, если `FALSE`, то только функции (де)сереализации и схемы.

## Использование

Для использования CMake-генератора необходимо выполнить следующие шаги:
0. Добавьте библиотеку MacroJson в ваш проект. 
1. В CMakeLists.txt вашего проекта добавьте следующий код для подключения CMake-генератора:

```cmake
# include header-generator
include(MacroJSON/cmake/HeaderGenerator.cmake)
```
2. В CMakeLists.txt вашего проекта вызовите функцию `macrojson_generate_header()` для генерации заголовочного файла. Например:

```cmake
# generate header file
macrojson_generate_header(
    person_descr.h
    person.hpp
    TRUE
)
```

3. Добавьте сгенерированный заголовочный файл в список исходных файлов вашего проекта. Например:

```cmake
# add executable
add_executable(my_app main.cpp generated/person.hpp)
```
