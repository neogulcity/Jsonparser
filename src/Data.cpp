#include "Data.h"

namespace jsonvalue {
    std::string Data::GetPath() { return this->m_path; }
    std::string Data::GetName() { return this->m_name; }
    std::vector<std::string> Data::GetData() { return this->m_data; }
    std::vector<std::string>* Data::pGetData() { return &this->m_data; }
    void Data::SetPath(const std::string& _path) { this->m_path = _path; }
    void Data::SetName(const std::string& _name) { this->m_name = _name; }
    void Data::SetData(const std::vector<std::string>& _data) {
        this->m_data = _data;
    }
}