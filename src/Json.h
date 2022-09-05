#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class json {
   public:
    json() = default;
    ~json() = default;
    json& operator=(const json& _json) = delete;
    json(const json& _json) = delete;

    friend std::ostream& operator<<(std::ostream& _out, const json& _json);
    operator std::string();
    json& operator[](const std::string& _path);
    void operator=(int _param);

// For testing purpose functions
#ifdef _TEST
   public:
    enum class ePathType { Same, Upper, Lower, Else };
    std::string GetPath_TEST() { return this->m_path; }
    std::string GetRoot_TEST() { return this->_ROOT_; }
    std::string GetDiv_TEST() { return this->_PATHDIV_; }
    size_t GetIndent_TEST() { return this->_INDENT_; }
    std::vector<std::tuple<std::string, std::string, std::string>>&
    GetDataVec_TEST() {
        return this->m_dataVec;
    }
    std::pair<std::string, std::string> SeparateDataFromPath(
        const std::string& _path);
    std::string* LookupData(const std::string& _path, const std::string& _name);
    ePathType GetPathType(const std::string& _src,
                          const std::string& _toCompare);
    std::vector<std::string> UnpackPath(const std::string& _path);
    std::string WritePath(const std::string& _path);
    std::string WriteData(
        const std::tuple<std::string, std::string, std::string>& _data);
    std::string ClosePath(const std::string& _path);
    void DelComma(std::string* _src);

#else
   private:
    enum class ePathType { Same, Upper, Lower, Else };
    std::pair<std::string, std::string> SeparateDataFromPath(
        const std::string& _path);
    std::string* LookupData(const std::string& _path, const std::string& _name);
    ePathType GetPathType(const std::string& _src,
                          const std::string& _toCompare);
    td::vector<std::string> UnpackPath(const std::string& _path);
    std::string WritePath(const std::string& _path);
    std::string WriteData(
        const std::tuple<std::string, std::string, std::string>& _data);
    std::string ClosePath(const std::string& _path);
    void DelComma(std::string* _src);
#endif

   private:
    const std::string _ROOT_{"_root_"};
    const std::string _PATHDIV_{"|"};
    const size_t _INDENT_{4};

   private:
    std::string m_result;
    std::string m_path;
    std::vector<std::tuple<std::string, std::string, std::string>> m_dataVec;
};