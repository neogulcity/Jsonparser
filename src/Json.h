#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Data.h"

class json {
   public:
    json() = default;
    ~json() = default;
    json& operator=(const json& _json) = delete;
    json(const json& _json) = delete;

    friend std::ostream& operator<<(std::ostream& _out, const json& _json);
    operator std::string();
    json& operator[](const std::string& _path);

    template <typename T>
    void operator=(const T& _param);

    template <typename T, size_t N>
    void operator=(const T (&arr)[N]);

// For testing purpose functions
#ifdef _TEST
   public:
    enum class ePathType { Same, Upper, Lower, Else };
    enum class eIndentType { WritePath, WriteData, WriteArr, Close };
    std::string GetPath_TEST() { return this->m_path; }
    std::string GetRoot_TEST() { return this->_ROOT_; }
    std::string GetDiv_TEST() { return this->_PATHDIV_; }
    std::vector<Data>& GetDataVec_TEST() { return this->m_dataVec; }
    std::pair<std::string, std::string> SeparateDataFromPath(
        const std::string& _path);
    std::vector<std::string>* LookupData(const std::string& _path,
                                         const std::string& _name);
    ePathType GetPathType(const std::string& _src,
                          const std::string& _toCompare);
    std::vector<std::string> UnpackPath(const std::string& _path);
    std::string PackPath(const std::vector<std::string>& _path);
    std::string WritePath(const std::string& _path, const uint32_t& _depth);
    std::string WriteData(Data _data);
    std::string ClosePath(const std::string& _path);
    void DelComma(std::string* _src);
    std::string GetIndent(const std::string& _path, eIndentType _type,
                          const uint32_t& _depth = 0);
    std::pair<uint32_t, uint32_t> GetWriteDepth(const std::string& _prev,
                                                const std::string& _new);
    void ToUpperPath(std::string* _path);

#else
   private:
    enum class ePathType { Same, Upper, Lower, Else };
    enum class eIndentType { WritePath, WriteData, WriteArr, Close };
    std::pair<std::string, std::string> SeparateDataFromPath(
        const std::string& _path);
    std::vector<std::string>* LookupData(const std::string& _path,
                                         const std::string& _name);
    ePathType GetPathType(const std::string& _src,
                          const std::string& _toCompare);
    std::vector<std::string> UnpackPath(const std::string& _path);
    std::string PackPath(const std::vector<std::string>& _path);
    std::string WritePath(const std::string& _path, const uint32_t& _depth);
    std::string WriteData(Data _data);
    std::string ClosePath(const std::string& _path);
    void DelComma(std::string* _src);
    std::string GetIndent(const std::string& _path, eIndentType _type,
                          const uint32_t& _depth = 0);
    std::pair<uint32_t, uint32_t> GetWriteDepth(const std::string& _prev,
                                                const std::string& _new);
    void ToUpperPath(std::string* _path);
#endif

   private:
    const std::string _ROOT_{"_root_"};
    const std::string _PATHDIV_{"|"};
    const size_t _INDENT_{4};

   private:
    std::string m_result;
    std::string m_path;
    std::vector<Data> m_dataVec;
};

#include "json.hpp"