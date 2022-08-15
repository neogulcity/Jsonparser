#include <fstream>
#include <string>
#include <sstream>

#include "Json.h"
#include "Log.h"

using namespace std;

namespace jsonparser {
    void json::AddText(const string& _text)
    {
        mstrText = mstrText + _text;
    }

    void json::AddSpace()
    {
        mstrSpace = mstrSpace + (string)"    ";
    }

    void json::ResolveSpace()
    {
        mstrSpace.erase(0, 4);
    }

    template <typename T>
    void json::Serialize(const string& _name, const T& _data)
    {
        log_debug("Basic Function Called");

        string strData;
        strData = to_string(_data);
        
        string strText;
        strText = GetText();

        // Case #1: Nothing serialized yet.
        if (strText.front() == '\0') {
            strText = (string)"{\n" + GetSpace() + (string)"\"" + _name + (string)"\": " + strData + (string)",";
        }

        // Case #2: Serializing object but nothing serialized yet.
        else if (strText.back() == '{') {
            strText = strText + (string)"\n" + GetSpace() + (string)"\"" + _name + (string)"\": " + strData + (string)",";
        }
        
        // Case #3: Soemthing serialized and trying to serialize more.
        // Process seems same with Case #2. Need to merge it later.
        else if (strText.back() == ',') {
            strText = strText + (string)"\n" + GetSpace() + (string)"\"" + _name + (string)"\": " + strData + (string)",";
        }
        SetText(strText);
    }

    // template <typename T>
    // void json::Serialize(const string& _name, T* _data)
    // {
    //     log_debug("pointer Function Called");
    //     log_warn("Serializing pointer is not allowed.");
    // }

    template <typename T, size_t N>
    void json::Serialize(const string& _name, const T (&arr)[N])
    {
        for (int i = 0; i < N; i++) {
            Serialize(_name, arr[i]);
        }
        
        // log_debug("int: {}", (void*)_data);
    }

    void json::SerializeObjectStart(const string& _name)
    {
        string strText;
        strText = GetText();

        // Case #1: Nothing serialized yet.
        if (strText.front() == '\0') {
            strText = (string)"{\n" + GetSpace() + (string)"\"" + _name + (string)"\": " + (string)"{";

        }

        // Case #2: Soemthing serialized or parent object serialized and trying to serialize more.
        else if (strText.back() == ',') {
            strText = strText + (string)"\n" + GetSpace() + (string)"\"" + _name + (string)"\": " + (string)"{";

        }

        // Case #3: parent object start serializing but nothing serialized.
        else if (strText.back() == '{') {
            strText = strText + (string)"\n" + GetSpace() + (string)"\"" + _name + (string)"\": " + (string)"{";

        }

        AddSpace();
        SetText(strText);
    }

    void json::SerializeObjectEnd(const string& _name)
    {
        string strText;
        strText = GetText();

        // Nothing serialized by object.
        if (strText.back() != '{') {
            strText.pop_back();
        }
        
        ResolveSpace();
        strText = strText + (string)"\n" + GetSpace() + (string)"},";

        SetText(strText);
    }

    void json::Write()
    {
        string strText;
        strText = GetText();

        strText.pop_back();
        strText = strText + "\n}";

        SetText(strText);

        ofstream out("result.json");
        out << GetText();
        out.close();
    }

    class childClass{
    public:
        childClass() : myChildData(5) {}

        void SerializeObject(const string& object, json* const file) {
            file->SerializeObjectStart(object);
            file->Serialize("myChildData", myChildData);
            file->SerializeObjectEnd(object);
        }

    private:
        int myChildData;
    };

    class myClass{
    public:
        myClass() : myData(0), myDouble(3.3) {}

        void SerializeObject(const string& object, json* const file) {
            file->SerializeObjectStart(object);
            test.SerializeObject("myChildObject", file);
            file->Serialize("myData", myData);
            file->Serialize("myDouble", myDouble);
            file->SerializeObjectEnd(object);
        }
    private:
        int myData;
        double myDouble;
        childClass test;
    };

    void main_write()
    {
        // log_debug("Write의 기능 테스트를 위한 main 함수입니다.");

        json file;

        int myArray[5] = {0, 1, 2, 3, 4};

        file.Serialize("myArray", myArray);

        file.Write();
    }
}