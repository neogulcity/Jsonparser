#include "json.h"

#include "Log.h"

std::ostream& operator<<(std::ostream& _out, const json& _json) {
    _out << _json.m_result;
    return _out;
}

json::operator std::string() {
    if (this->m_dataVec.size() == 0) return "";

    if (this->m_dataVec.size() == 1) {
        this->m_result += "{\n";

        auto data = this->m_dataVec.begin();
        this->m_result += WritePath(std::get<0>(*data));
        this->m_result += WriteData(*this->m_dataVec.begin());
        DelComma(&this->m_result);
        this->m_result += ClosePath(std::get<0>(*data));
        DelComma(&this->m_result);
        this->m_result += "\n}";

        return this->m_result;
    }

    // auto dataVec = this->m_dataVec;
    // std::string work = std::get<0>(*dataVec.begin());
    // for (auto iter = dataVec.begin(); iter != dataVec.end(); iter++) {
    //     auto type = GetPathType(work, std::get<0>(*iter));
    //     switch (type) {
    //         case ePathType::Lower:
    //             break;

    //         case ePathType::Same:
    //             break;

    //         default:
    //             break;
    //     }
    // }

    return this->m_result;
}

json& json::operator[](const std::string& _path) {
    if (this->m_path == "") {
        this->m_path = _ROOT_ + _PATHDIV_ + _path;
    } else {
        this->m_path = this->m_path + _PATHDIV_ + _path;
    }

    return *this;
}

void json::operator=(int _param) {
    if (this->m_path == "") return;

    std::pair<std::string, std::string> pStr;
    pStr = SeparateDataFromPath(this->m_path);
    if (pStr.first == "" && pStr.second == "") return;

    auto data = LookupData(pStr.first, pStr.second);
    if (!data) {
        this->m_dataVec.push_back(
            std::make_tuple(pStr.first, pStr.second, std::to_string(_param)));
    } else {
        *data = std::to_string(_param);
    }

    this->m_path = "";
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

std::string* json::LookupData(const std::string& _path,
                              const std::string& _name) {
    if (_path == "" || _name == "") return nullptr;

    for (auto& elem : this->m_dataVec) {
        std::string path, name;
        path = std::get<0>(elem);
        name = std::get<1>(elem);

        if (path == _path && name == _name) {
            std::string* data;
            data = &std::get<2>(elem);
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

std::string json::WritePath(const std::string& _path) {
    std::string result = "";
    if (_path == "") return result;

    auto pathVec = UnpackPath(_path);
    uint32_t depth = pathVec.size();

    if (pathVec.size() == 0) return result;

    std::string space = "";
    for (int i = 0; i < _INDENT_; i++) {
        space += " ";
    }

    std::vector<std::string> indent(pathVec.size(), "");
    for (int i = 1; i < indent.size(); i++) {
        indent[i] = indent[i - 1] + space;
    }

    for (uint32_t i = 1; i < depth; i++) {
        result += indent[i] + "\"" + pathVec[i] + "\": {\n";
    }

    return result;
}

std::string json::WriteData(
    const std::tuple<std::string, std::string, std::string>& _data) {
    std::string path = std::get<0>(_data);
    std::string name = std::get<1>(_data);
    std::string data = std::get<2>(_data);

    if (path == "" || name == "") return "";

    auto pathVec = UnpackPath(path);
    if (pathVec.size() == 0) return "";

    std::string indent = "";
    auto size = _INDENT_ * pathVec.size();
    for (int i = 0; i < size; i++) {
        indent += " ";
    }

    std::string result = "";
    result += indent + "\"" + name + "\": " + data + ",\n";

    return result;
}

std::string json::ClosePath(const std::string& _path) {
    if (_path == "") return "";

    auto pathVec = UnpackPath(_path);
    if (pathVec.size() == 0 || pathVec.size() == 1) return "";

    assert(pathVec.size() > 1);

    std::string indent = "";
    auto size = _INDENT_ * (pathVec.size() - 1);
    for (int i = 0; i < size; i++) {
        indent += " ";
    }

    std::string result = "";
    result += indent + "},\n";

    return result;
}

void json::DelComma(std::string* _src) {
    _src->erase(_src->end() - 1);
    _src->erase(_src->end() - 1);
    *_src += "\n";
}