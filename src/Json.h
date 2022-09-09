#pragma once

#include <cassert>
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

    template <typename T>
    void operator=(const std::vector<T>& _param);

    template <typename T, size_t N>
    void operator=(const T (&arr)[N]);

    template <size_t N>
    void operator=(const std::string (&arr)[N]);

    void operator=(const std::string& _param);
    void operator=(const char* _param);
    void operator=(const std::vector<std::string>& _param);
    void operator=(const std::vector<const char*>& _param);

// For testing purpose functions
#ifdef JSONPARSER_TEST
   public:
    enum class ePathType { Same, Upper, Lower, Else };
    enum class eIndentType { WritePath, WriteData, WriteArr, Close };

    std::string GetPath_TEST() { return this->m_path; }
    std::string GetRoot_TEST() { return this->_ROOT_; }
    std::string GetDiv_TEST() { return this->_PATHDIV_; }
    std::vector<jsonvalue::Data>& GetDataVec_TEST() { return this->m_dataVec; }

    /**
     * Separate the string to path and data name from given path.
     *
     * @param path The string to separate.
     * @return pair<string, string> - The first is path and the second, data
     * name.
     */
    std::pair<std::string, std::string> SeparateDataFromPath(
        const std::string& _path);

    /**
     * Lookup the data in data member m_dataVec by given path and data name.
     *
     * @param path The string of path to lookup data.
     * @param name The string of name to lookup data.
     * @return vector<string>* - Address of the container which hold stringized
     * data.
     */
    std::vector<std::string>* LookupData(const std::string& _path,
                                         const std::string& _name);

    /**
     * Get "Path Type" from the Source string compared with given string.
     *
     * @param src The string of source to compare.
     * @param toCompare The string to get Path Type.
     * @return ePathType - The relation between given strings.
     */
    ePathType GetPathType(const std::string& _src,
                          const std::string& _toCompare);

    /**
     * Unpack given string by path divider json::_PATHDIV_.
     *
     * @param path The string of path to unpack.
     * @return vector<string> - The container of unpacked strings.
     */
    std::vector<std::string> UnpackPath(const std::string& _path);

    /**
     * Pack given container of strings by path divider json::_PATHDIV_.
     *
     * @param path The container of strings to pack.
     * @return string - The string packaged with path divider.
     */
    std::string PackPath(const std::vector<std::string>& _path);

    /**
     * Write string of path from given depth.
     *
     * @param path The string of path to write.
     * @param depth The number to write path.
     * @return string - The string to write with json structure.
     */
    std::string WritePath(const std::string& _path, const uint32_t& _depth);

    /**
     * Write stringized data from given Data class.
     *
     * @param data The data to write.
     * @return string - The string to write with json structure.
     */
    std::string WriteData(jsonvalue::Data _data);

    /**
     * Add end bracket to close given path.
     *
     * @param path The string of path to close.
     * @return string - The string to write with json structure.
     */
    std::string ClosePath(const std::string& _path);

    /**
     * Delete comma from given string to end writting data.
     *
     * @param path The string of source to delete comma.
     */
    void DelComma(std::string* _src);

    /**
     * Get indent size of path by eIndentType and depth.
     *
     * @param path The string of path to get indent size.
     * @param eIndentType The type of writing.
     * @param depth The number to write path.
     * @return string - The stringized indent.
     */
    std::string GetIndent(const std::string& _path, eIndentType _type,
                          const uint32_t& _depth = 0);

    /**
     * Get paired depth to start writing path and end.
     *
     * @param prev The string of path previously written.
     * @param new The string of path to write.
     * @return pair<uint32_t, uint32_t> - The first is depth to start writing
     * and the second, end of depth to write.
     */
    std::pair<uint32_t, uint32_t> GetWriteDepth(const std::string& _prev,
                                                const std::string& _new);

    /**
     * Move current path to it's parent.
     *
     * @param path The string of path to move upper.
     */
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
    std::string WriteData(jsonvalue::Data _data);
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
    std::vector<jsonvalue::Data> m_dataVec;
};

template <typename T>
void json::operator=(const T& _param) {
    if (this->m_path == "") return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec(1, std::to_string(_param));
        jsonvalue::Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec(1, std::to_string(_param));
        *data = dataVec;
    }

    this->m_path = "";
}

template <typename T>
void json::operator=(const std::vector<T>& _param) {
    if (this->m_path == "") return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec;
        for (const auto& elem : _param) {
            dataVec.push_back(std::to_string(elem));
        }
        jsonvalue::Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec;
        for (const auto& elem : _param) {
            dataVec.push_back(std::to_string(elem));
        }
        *data = dataVec;
    }

    this->m_path = "";
}

template <typename T, size_t N>
void json::operator=(const T (&arr)[N]) {
    if (this->m_path == "") return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec;
        for (int i = 0; i < N; i++) {
            dataVec.push_back(std::to_string(arr[i]));
        }
        jsonvalue::Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec;
        for (int i = 0; i < N; i++) {
            dataVec.push_back(std::to_string(arr[i]));
        }
        *data = dataVec;
    }

    this->m_path = "";
}

template <size_t N>
void json::operator=(const std::string (&arr)[N]) {
    if (this->m_path == "") return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec;
        for (int i = 0; i < N; i++) {
            std::string newStr = arr[i];
            newStr = "\"" + newStr + "\"";
            dataVec.push_back(newStr);
        }
        jsonvalue::Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec;
        for (int i = 0; i < N; i++) {
            std::string newStr = arr[i];
            newStr = "\"" + newStr + "\"";
            dataVec.push_back(newStr);
        }
        *data = dataVec;
    }

    this->m_path = "";
}