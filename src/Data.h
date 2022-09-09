#pragma once

#include <string>
#include <vector>

namespace jsonvalue {
    class Data {
       public:
        Data(const std::string& _path, const std::string& _name,
             const std::vector<std::string>& _data)
            : m_path(_path), m_name(_name), m_data(_data) {}
        ~Data() = default;

        std::string GetPath();
        std::string GetName();
        std::vector<std::string> GetData();
        std::vector<std::string>* pGetData();
        void SetPath(const std::string& _path);
        void SetName(const std::string& _name);
        void SetData(const std::vector<std::string>& _data);

       private:
        std::string m_path;
        std::string m_name;
        std::vector<std::string> m_data;
    };
}