// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <macrojson/fundamental.hpp>

namespace macrojson {
    // Aggregative types writers
    template<typename T>
    void write_to_json(
            const char* name, const std::optional<T>& val, Document::AllocatorType& alloc, Value& root) {
        if (val.has_value()) {
            write_to_json(name, val.value(), alloc, root);
        }
    }

    template<typename T>
    void write_to_json(
            const char* name, const std::vector<T>& val, Document::AllocatorType& alloc, Value& root) {
        Value jarr(kArrayType);
        for (const auto& item : val) {
            Value jval;
            write_to_json(nullptr, item, alloc, jval);
            jarr.PushBack(jval, alloc);
        }
        write_to_json(name, std::move(jarr), alloc, root);
    }

    template<typename T>
    void write_to_json(
            const char* name, const std::unique_ptr<T>& val, Document::AllocatorType& alloc, Value& root) {
        write_to_json(name, val.get(), alloc, root);
    }

    template<typename T>
    void write_to_json(
            const char* name, const std::shared_ptr<T>& val, Document::AllocatorType& alloc, Value& root) {
        write_to_json(name, val.get(), alloc, root);
    }

    // Aggregative types readers
    template<typename T>
    MJsonErrorCode read_from_json(const char* name, const Value& root, std::vector<T>& val) {
        val.clear();

        if (name && !root.HasMember(name)) {
            return E_MJSON_NOT_EXISTS;
        }
        const Value& jval = name ? root[name] : root;
        if (!jval.IsArray()) {
            return E_MJSON_TYPE_MISMATCH;
        }

        for (const auto& item : jval.GetArray()) {
            T elem{};
            MJSON_CHECK_ERROR(read_from_json(nullptr, item, elem));
            val.emplace_back(std::move(elem));
        }

        return E_MJSON_OK;
    }

    template<typename T>
    MJsonErrorCode read_from_json(const char* name, const Value& root, std::optional<T>& val) {
        val.reset();

        if (name) {
            if (!root.HasMember(name)) {
                return E_MJSON_OK;
            }
            else if (root[name].IsNull()) {
                return E_MJSON_OK;
            }
        }
        else if (root.IsNull()) {
            return E_MJSON_OK;
        }

        T temp{};
        MJSON_CHECK_ERROR(read_from_json(name, root, temp));
        val = std::move(temp);

        return E_MJSON_OK;
    }

    template<typename T>
    MJsonErrorCode read_from_json(const char* name, const Value& root, std::shared_ptr<T>& val) {
        val.reset();

        std::unique_ptr<T> tmp{};
        MJSON_CHECK_ERROR(read_from_json(name, root, tmp));

        val = std::move(tmp);
        return E_MJSON_OK;
    }

    // Aggregative types schemas
    template<typename T, typename... Validators>
    void generate_schema(
            const char* name, const char* title, const char* description,
            Document::AllocatorType& alloc, Value& schema,
            Validators... validators) {
        if constexpr (std::is_same_v<T, std::string>) {
            generate_schema_base(name, title, description, "string", alloc, schema);
            Value& jobj = name ? schema[name] : schema;
            add_validation_fields<std::string>(alloc, jobj, validators...);
        } else if constexpr (is_std_optional_v<T>) {
            generate_schema<typename T::value_type>(name, title, description, alloc, schema, validators...);
        }
        else if constexpr (is_std_vector_v<T>) {
            generate_schema_base(name, title, description, "array", alloc, schema);
            Value& jobj = name ? schema[name] : schema;
            generate_schema<typename T::value_type>("items", nullptr, nullptr, alloc, jobj, validators...);
        }
        else if constexpr (is_std_unique_ptr_v<T> || is_std_shared_ptr_v<T>) {
            generate_schema<typename T::element_type>(name, title, description, alloc, schema, validators...);
        } else
            static_assert(false, "unsupported type");
    }

    // Aggregative types validators
    // TODO
}
