#include "json.h"

using namespace jsonvalue;

std::ostream& operator<<(std::ostream& _out, const json& _json) {
    _out << _json.m_result;
    return _out;
}

json::operator std::string() {
    if (this->m_dataVec.size() == 0) return "";

    this->m_result = "";

    this->m_result += "{\n";

    // Start of writting json format string. Start from root and Iterate all
    // data in m_dataVec. While iterating data, check it's path and compare with
    // current path.
    auto dataVec = this->m_dataVec;
    std::string curPath = _ROOT_;
    while (curPath != "") {
        auto iter = dataVec.begin();
        while (iter != dataVec.end()) {
            auto type = GetPathType(curPath, iter->GetPath());
            std::pair<uint32_t, uint32_t> depth;
            switch (type) {
                // The data located path is child of current path. So, write the
                // path and data. Then move current path lower.
                case ePathType::Lower:
                    depth = GetWriteDepth(curPath, iter->GetPath());
                    for (uint32_t i = depth.first; i <= depth.second; i++) {
                        this->m_result += WritePath(iter->GetPath(), i);
                    }
                    this->m_result += WriteData(*iter);

                    curPath = iter->GetPath();

                    iter = dataVec.erase(iter);
                    break;

                // The data located in same path with current. So, write the
                // data and keep searching next.
                case ePathType::Same:
                    this->m_result += WriteData(*iter);

                    iter = dataVec.erase(iter);
                    break;

                default:
                    iter++;
                    break;
            }
        }

        // Now, all the same path data written in current path. End writting
        // data and close the path. Then move current path to upper and keep
        // searching next data.
        DelComma(&this->m_result);
        if (curPath != _ROOT_) {
            this->m_result += ClosePath(curPath);
        }
        ToUpperPath(&curPath);
    }

    this->m_result += "}";
    return this->m_result;
}

void json::operator=(const std::string& _param) {
    if (this->m_path == "") return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    std::string param = "\"" + _param + "\"";

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec(1, param);
        Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec(1, param);
        *data = dataVec;
    }

    this->m_path = "";
}

void json::operator=(const char* _param) {
    if (this->m_path == "") return;

    if (!_param) return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    std::string param(_param);
    param = "\"" + param + "\"";

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec(1, param);
        Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec(1, param);
        *data = dataVec;
    }

    this->m_path = "";
}

void json::operator=(const std::vector<std::string>& _param) {
    if (this->m_path == "") return;

    if (_param.size() == 0) return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec;
        for (const auto& elem : _param) {
            std::string param = elem;
            param = "\"" + param + "\"";
            dataVec.push_back(param);
        }
        Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec;
        for (const auto& elem : _param) {
            std::string param = elem;
            param = "\"" + param + "\"";
            dataVec.push_back(param);
        }
        *data = dataVec;
    }

    this->m_path = "";
}

void json::operator=(const std::vector<const char*>& _param) {
    if (this->m_path == "") return;

    if (_param.size() == 0) return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        std::vector<std::string> dataVec;
        for (const auto& elem : _param) {
            std::string param(elem);
            param = "\"" + param + "\"";
            dataVec.push_back(param);
        }
        Data newData(pStr.first, pStr.second, dataVec);
        this->m_dataVec.push_back(newData);
    } else {
        std::vector<std::string> dataVec;
        for (const auto& elem : _param) {
            std::string param(elem);
            param = "\"" + param + "\"";
            dataVec.push_back(param);
        }
        *data = dataVec;
    }

    this->m_path = "";
}

json& json::operator[](const std::string& _path) {
    if (this->m_path == "") {
        this->m_path = _ROOT_ + _PATHDIV_ + _path;
    } else {
        this->m_path = this->m_path + _PATHDIV_ + _path;
    }

    return *this;
}

std::pair<std::string, std::string> json::SeparateDataFromPath(
    const std::string& _path) {
    std::pair<std::string, std::string> _NULL_ = {"", ""};
    std::string path = _path;

    if (path == "") return _NULL_;

    auto pos = path.rfind(_PATHDIV_);
    if (pos == std::string::npos) return _NULL_;

    std::string result1, result2;
    result1 = path;
    result1.erase(pos, result1.length());

    result2 = path;
    result2.erase(0, pos + _PATHDIV_.length());

    return {result1, result2};
}

std::vector<std::string>* json::LookupData(const std::string& _path,
                                           const std::string& _name) {
    if (_path == "" || _name == "") return nullptr;

    for (auto& elem : this->m_dataVec) {
        std::string path, name;
        path = elem.GetPath();
        name = elem.GetName();

        if (path == _path && name == _name) {
            std::vector<std::string>* data;
            data = elem.pGetData();
            return data;
        }
    }

    return nullptr;
}

json::ePathType json::GetPathType(const std::string& _src,
                                  const std::string& _toCompare) {
    if (_src.find(_toCompare) != std::string::npos) {
        if (_src.length() != _toCompare.length())
            return ePathType::Upper;
        else
            return ePathType::Same;
    }

    if (_toCompare.find(_src) != std::string::npos) {
        if (_toCompare.length() != _src.length()) return ePathType::Lower;
    }

    return ePathType::Else;
}

std::vector<std::string> json::UnpackPath(const std::string& _path) {
    std::vector<std::string> pathVec;

    std::string path = _path;
    while (path.length() > 0) {
        auto pos = path.find(_PATHDIV_);
        if (pos == std::string::npos) {
            pathVec.push_back(path);
            break;
        }

        std::string tempPath = path;
        tempPath.erase(pos, tempPath.length());
        path.erase(0, pos + _PATHDIV_.length());
        pathVec.push_back(tempPath);
    }

    return pathVec;
}

std::string json::PackPath(const std::vector<std::string>& _path) {
    if (_path.size() == 0) return "";

    std::string result = "";
    for (auto iter = _path.begin(); iter != _path.end(); iter++) {
        result += *iter + _PATHDIV_;
    }

    auto pos = result.rfind(_PATHDIV_);
    if (pos != std::string::npos) {
        result.erase(pos, result.length());
    }

    return result;
}

std::string json::WritePath(const std::string& _path, const uint32_t& _depth) {
    if (_path == "") return "";

    auto pathVec = UnpackPath(_path);
    if (pathVec.size() == 0) return "";

    auto indent = GetIndent(_path, eIndentType::WritePath, _depth);
    if (indent == "") return "";

    std::string result = "";
    result += indent + "\"" + pathVec[_depth] + "\": {\n";

    return result;
}

std::string json::WriteData(Data _data) {
    std::string path = _data.GetPath();
    std::string name = _data.GetName();
    std::vector<std::string> data = _data.GetData();

    assert(data.size() > 0);

    if (path == "" || name == "") return "";

    std::string result = "";
    if (data.size() == 1) {
        auto indent = GetIndent(path, eIndentType::WriteData);
        if (indent == "") return "";

        result += indent + "\"" + name + "\": " + *data.begin() + ",\n";
    } else {
        auto arrIndent = GetIndent(path, eIndentType::WriteData);
        if (arrIndent == "") return "";

        auto dataIndent = GetIndent(path, eIndentType::WriteArr);
        if (dataIndent == "") return "";

        result += arrIndent + "\"" + name + "\": [\n";
        for (const auto& elem : data) {
            result += dataIndent + elem + ",\n";
        }
        result.erase(result.end() - 1);
        result.erase(result.end() - 1);
        result += "\n";
        result += arrIndent + "],\n";
    }

    return result;
}

std::string json::ClosePath(const std::string& _path) {
    if (_path == "") return "";

    auto indent = GetIndent(_path, eIndentType::Close);
    if (indent == "") return "";

    std::string result = "";
    result += indent + "},\n";

    return result;
}

void json::DelComma(std::string* _src) {
    _src->erase(_src->end() - 1);
    _src->erase(_src->end() - 1);
    *_src += "\n";
}

std::string json::GetIndent(const std::string& _path, eIndentType _type,
                            const uint32_t& _depth) {
    if (_path == "") return "";

    std::string result = "";
    auto pathVec = UnpackPath(_path);
    auto size = pathVec.size();

    assert(size > 0);

    switch (_type) {
        case eIndentType::WritePath:
            if (_depth == 0) {
                assert(size > 1);
                size = (size - 1) * _INDENT_;
            } else {
                assert(size > 1 && _depth > 0);
                size = _depth * _INDENT_;
            }
            break;

        case eIndentType::WriteData:
            size = size * _INDENT_;
            break;

        case eIndentType::WriteArr:
            size = (size + 1) * _INDENT_;
            break;

        case eIndentType::Close:
            assert(size > 1);
            size = (size - 1) * _INDENT_;
            break;
    }

    for (int i = 0; i < size; i++) {
        result += " ";
    }

    return result;
}

std::pair<uint32_t, uint32_t> json::GetWriteDepth(const std::string& _prev,
                                                  const std::string& _new) {
    if (_prev == "" || _new == "") return {0, 0};

    if (_prev == _new) return {0, 0};

    auto prevVec = UnpackPath(_prev);
    if (prevVec.size() == 0) return {0, 0};

    auto newVec = UnpackPath(_new);
    if (newVec.size() == 0) return {0, 0};

    uint32_t result = 0;
    auto prev = *(prevVec.end() - 1);
    for (int i = 0; i < newVec.size(); i++) {
        if (newVec[i] == prev) {
            result = i;
        }
    }

    return {result + 1, newVec.size() - 1};
}

void json::ToUpperPath(std::string* _path) {
    if (!_path) return;

    if (*_path == "") return;

    if (*_path == _ROOT_) {
        *_path = "";
        return;
    }

    auto pathVec = UnpackPath(*_path);
    pathVec.erase(pathVec.end() - 1);
    *_path = PackPath(pathVec);
}