// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <macrojson/fundamental.hpp>
#include <macrojson/aggregative.hpp>

namespace macrojson {
    template <typename T>
    static inline void object_to_json_str(const T& object, std::string& out) {
        rapidjson::Document doc;
        doc.SetObject();
        write_to_json(nullptr, object, doc.GetAllocator(), doc);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        out.assign(buffer.GetString(), buffer.GetSize());
    }

    template <typename T>
    static inline MJsonErrorCode object_to_json_file(
            const T& object, const std::filesystem::path& json_file) {
        rapidjson::Document doc;
        doc.SetObject();
        write_to_json(nullptr, object, doc.GetAllocator(), doc);

        try {
            std::ofstream ofs(json_file);
            if (!ofs)
                return E_MJSON_FILE_ERROR;

            rapidjson::OStreamWrapper osw(ofs);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
            doc.Accept(writer);
        }
        catch (const std::filesystem::filesystem_error& e) {
            return E_MJSON_FILE_ERROR;
        }

        return E_MJSON_OK;
    }

    template <typename T>
    static inline MJsonErrorCode json_doc_to_object(
            const rapidjson::Document& doc, T& object, std::string& error_descr) {
        if (doc.HasParseError()) {
            error_descr = "json parse error: ";
            error_descr += rapidjson::GetParseError_En(doc.GetParseError());
            error_descr += ", error offset: ";
            error_descr += std::to_string(doc.GetErrorOffset());
            return E_MJSON_PARSE_FAILED;
        }
        return read_from_json(nullptr, doc, object);
    }

    template <typename T>
    static inline MJsonErrorCode json_str_to_object(
            const std::string& json, T& object, std::string& error_descr) {
        rapidjson::Document doc;
        doc.Parse(json.c_str(), json.size());
        return json_doc_to_object(doc, object, error_descr);
    }

    template <typename T>
    static inline MJsonErrorCode json_file_to_object(
            const std::filesystem::path& json_file, T& object, std::string& error_descr) {
        rapidjson::Document doc;
        try {
            std::ifstream ifs(json_file);
            if (!ifs)
                return E_MJSON_FILE_ERROR;
            rapidjson::IStreamWrapper isw(ifs);
            doc.ParseStream(isw);
        }
        catch (const std::filesystem::filesystem_error& e) {
            error_descr = e.what();
            return E_MJSON_FILE_ERROR;
        }

        return json_doc_to_object(doc, object, error_descr);
    }
}
