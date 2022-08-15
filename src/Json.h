#pragma once

#include <string>

using namespace std;

namespace jsonparser {
    class json {
    public:
        json()
        : mstrFile("")
        , mstrText("")
        , mstrSpace("    ")
        , mbInArray(false)
        {}
        json(string _param)
        : mstrFile(_param)
        , mstrText("")
        , mstrSpace("    ")
        , mbInArray(false)
        {}

        // 데이터 멤버 mstrFile 값 반환.
        const string& GetFile() const;

        // 데이터 멤버 mstrText 값 반환.
        const string& GetText() const;

        // 데이터 멤버 mstrSpace 값 반환.
        const string& GetSpace() const;

        // 데이터 멤버 mstrFile 값 설정.
        void SetFile(const string&);

        // 데이터 멤버 mstrText 값 설정.
        void SetText(const string&);

        // 데이터 멤버 mstrSpace 값 설정.
        void SetSpace(const string&);

        // 데이터 멤버 mstrText 문자열 결합.
        void AddText(const string&);

        // 데이터 멤버 mstrSpace 들여쓰기 값 증가.
        void AddSpace();

        // 데이터 멤버 mstrSpace 들여쓰기 값 감소.
        void ResolveSpace();

        template <typename T>
        void Serialize(const string&, const T&);

        // template <typename T>
        // void Serialize(const string&, T*);

        template <typename T, size_t N>
        void Serialize(const string&, const T (&arr)[N]);

        void SerializeObjectStart(const string&);
        void SerializeObjectEnd(const string&);

        void Write();

    private:
        string mstrFile; // json 파일 경로를 갖는 데이터 멤버.
        string mstrText; // json 파일에 작성할 실제 내용을 갖는 데이터 멤버.
        string mstrSpace; // 현재 들여쓰기 상태를 갖는 데이터 멤버.
        bool mbInArray;
    };

    // Read 테스트를 위한 메인 함수.
    void main_read();

    // Write 테스트를 위한 메인 함수.
    void main_write();
}