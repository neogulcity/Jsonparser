#include "json.h"

template <typename T>
void json::operator=(const T& _param) {
    if (this->m_path == "") return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec(1, std::to_string(_param));
        Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec(1, std::to_string(_param));
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
        Data newData(pStr.first, pStr.second, dataVec);
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
