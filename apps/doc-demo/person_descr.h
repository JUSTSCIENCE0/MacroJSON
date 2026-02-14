// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

MJSON_OBJECT_BEGIN(Employee,
        "Employee Info", nullptr)
    MJSON_FIELD(int, id,
        "Employee ID",
        "Unique identifier for the employee")
    MJSON_FIELD(std::string, department,
        "Department",
        "Department where the employee works")
    MJSON_FIELD(std::optional<std::string>, position,
        "Position",
        "Job position of the employee")
MJSON_OBJECT_END(Employee)

MJSON_OBJECT_BEGIN(Person,
        "Person Info",
        "Information about a person, including their name, age, email, and employment details")
    MJSON_FIELD(std::string, name,
        "Name",
        "Full name of the person")
    MJSON_FIELD(int, age,
        "Age",
        "Age of the person in years",
        macrojson::Range<int>{18, false, 100, true})
    MJSON_FIELD(std::string, email,
        "Email",
        "Email address of the person",
        macrojson::StringRegex{ R"(^\S+@\S+\.\S+$)" },
        macrojson::StringLength{ 5, 100 })
    MJSON_FIELD(Employee, employee,
        "Employee Information",
        "Information about the person's employment")
    MJSON_FIELD(std::vector<std::string>, skills,
        "Skills",
        "List of skills the person has",
        macrojson::ArrayParams{ 0, 10, true },
        macrojson::StringLength{ 1, 50 })
MJSON_OBJECT_END(Person)
