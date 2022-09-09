#include "json.h"
#include "Log.h"
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("json::operator std::string()", "[json::operator std::string()]") {
    logger_info("Unit testing... json::operator std::string()");

    {
        json test;
        test["myData"] = 1;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": 1
        // }
        REQUIRE(r == "{\n    \"myData\": 1\n}");
    }
    {
        json test;
        test["myData"] = "hello";
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": "hello"
        // }
        REQUIRE(r == "{\n    \"myData\": \"hello\"\n}");
    }
    {
        json test;
        std::string text = "hello";
        test["myData"] = text;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": "hello"
        // }
        REQUIRE(r == "{\n    \"myData\": \"hello\"\n}");
    }
    {
        json test;
        test["section1"]["myData"] = 1;
        auto r = static_cast<std::string>(test);

        // {
        //     "section1": {
        //         "myData": 1
        //     }
        // }
        REQUIRE(r == "{\n    \"section1\": {\n        \"myData\": 1\n    }\n}");
    }
    {
        json test;
        test["section1"]["myData"] = 1;
        test["section1"]["myData"] = 10;
        test["myData2"] = 2;
        auto r = static_cast<std::string>(test);

        // {
        //     "section1": {
        //         "myData": 10
        //     },
        //     "myData2": 2
        // }
        REQUIRE(r ==
                "{\n    \"section1\": {\n        \"myData\": 10\n    },\n    "
                "\"myData2\": 2\n}");
    }
    {
        json test;
        test["myData"] = 1;
        test["section1"]["myData2"] = 2;
        test["myData3"] = 3;
        test["section1"]["myData4"] = 4;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": 1,
        //     "section1": {
        //         "myData2": 2,
        //         "myData4": 4
        //     },
        //     "myData3": 3
        // }
        REQUIRE(
            r ==
            "{\n    \"myData\": 1,\n    \"section1\": {\n        \"myData2\": "
            "2,\n        \"myData4\": 4\n    },\n    \"myData3\": 3\n}");
    }
    {
        json test;
        test["myData"] = 1;
        test["section1"]["myData2"] = 2;
        test["myData3"] = 3;
        test["section1"]["section2"]["myData4"] = 4;
        test["section1"]["myData5"] = 5;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": 1,
        //     "section1": {
        //         "myData2": 2,
        //         "section2": {
        //             "myData4": 4
        //         },
        //         "myData5": 5
        //     },
        //     "myData3": 3
        // }
        REQUIRE(
            r ==
            "{\n    \"myData\": 1,\n    \"section1\": {\n        \"myData2\": "
            "2,\n        \"section2\": {\n            \"myData4\": 4\n        "
            "},\n        \"myData5\": 5\n    },\n    \"myData3\": 3\n}");
    }
    {
        json test;
        int arr[2] = {0, 1};
        test["myData"] = arr;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": [
        //         0,
        //         1
        //     ]
        // }
        REQUIRE(r == "{\n    \"myData\": [\n        0,\n        1\n    ]\n}");
    }
    {
        json test;
        std::string arr[2] = {"hello", "world"};
        test["myData"] = arr;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": [
        //         "hello",
        //         "world"
        //     ]
        // }
        REQUIRE(r ==
                "{\n    \"myData\": [\n        \"hello\",\n        \"world\"\n "
                "   ]\n}");
    }
    {
        json test;
        std::vector<int> arr = {0, 1};
        test["myData"] = arr;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": [
        //         0,
        //         1
        //     ]
        // }
        REQUIRE(r == "{\n    \"myData\": [\n        0,\n        1\n    ]\n}");
    }
    {
        json test;
        std::vector<std::string> arr = {"hello", "world"};
        test["myData"] = arr;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": [
        //         "hello",
        //         "world"
        //     ]
        // }
        REQUIRE(r ==
                "{\n    \"myData\": [\n        \"hello\",\n        \"world\"\n "
                "   ]\n}");
    }
    {
        json test;
        std::vector<const char*> arr = {"hello", "world"};
        test["myData"] = arr;
        auto r = static_cast<std::string>(test);

        // {
        //     "myData": [
        //         "hello",
        //         "world"
        //     ]
        // }
        REQUIRE(r ==
                "{\n    \"myData\": [\n        \"hello\",\n        \"world\"\n "
                "   ]\n}");
    }
}

TEST_CASE("json::operator[]", "[json::operator array]") {
    logger_info("Unit testing... json::operator[]");

    json test;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();

    test["section1"];
    REQUIRE(test.GetPath_TEST() == ROOT + DIV + "section1");

    test["section2"];
    REQUIRE(test.GetPath_TEST() == ROOT + DIV + "section1" + DIV + "section2");

    test["section3"]["section4"];
    REQUIRE(test.GetPath_TEST() == ROOT + DIV + "section1" + DIV + "section2" +
                                       DIV + "section3" + DIV + "section4");
}

TEST_CASE("json::SeparateDataFromPath", "[json::SeparateDataFromPath]") {
    logger_info("Unit testing... json::SeparateDataFromPath");

    json _json;
    std::string ROOT = _json.GetRoot_TEST();
    std::string DIV = _json.GetDiv_TEST();

    {
        json test;
        std::pair<std::string, std::string> result;
        result = test.SeparateDataFromPath(test.GetPath_TEST());
        REQUIRE((result.first == "" && result.second == ""));
    }
    {
        json test;
        test["myData"];
        std::pair<std::string, std::string> result;
        result = test.SeparateDataFromPath(test.GetPath_TEST());
        REQUIRE((result.first == ROOT && result.second == "myData"));
    }
    {
        json test;
        test["section1"]["myData"];
        std::pair<std::string, std::string> result;
        result = test.SeparateDataFromPath(test.GetPath_TEST());
        REQUIRE((result.first == ROOT + DIV + "section1" &&
                 result.second == "myData"));
    }
}

TEST_CASE("json::Lookupdata", "[json::Lookupdata]") {
    logger_info("Unit testing... json::Lookupdata");

    {
        json test;
        std::vector<std::string> rData(1, "1");
        jsonvalue::Data tData("root", "myData", rData);
        auto& dataVec = test.GetDataVec_TEST();
        dataVec.push_back(tData);
        auto data = test.LookupData("root", "myData2");
        REQUIRE(data == nullptr);
    }
    {
        json test;
        std::vector<std::string> rData(1, "1");
        jsonvalue::Data tData("root", "myData", rData);
        auto& dataVec = test.GetDataVec_TEST();
        dataVec.push_back(tData);
        auto data = test.LookupData("root", "myData");
        *data->begin() = "2";
        REQUIRE(*dataVec.begin()->GetData().begin() == "2");
    }
}

TEST_CASE("json::GetPathType", "[json::GetPathType]") {
    logger_info("Unit testing... json::GetPathType");

    json test;
    std::string src, toCompare;
    std::string DIV = test.GetDiv_TEST();

    SECTION("'root' is parent of 'root|section1'") {
        src = "root" + DIV + "section1";
        toCompare = "root";
        auto type = test.GetPathType(src, toCompare);
        REQUIRE(type == json::ePathType::Upper);
    }
    SECTION("'root' is parent of 'root|section1|section2'") {
        src = "root" + DIV + "section1" + DIV + "section2";
        toCompare = "root";
        auto type = test.GetPathType(src, toCompare);
        REQUIRE(type == json::ePathType::Upper);
    }
    SECTION("'root|section1' is child of 'root'") {
        src = "root";
        toCompare = "root" + DIV + "section1";
        auto type = test.GetPathType(src, toCompare);
        REQUIRE(type == json::ePathType::Lower);
    }
    SECTION("'root|section1|section2' is child of 'root'") {
        src = "root";
        toCompare = "root" + DIV + "section1" + DIV + "section2";
        auto type = test.GetPathType(src, toCompare);
        REQUIRE(type == json::ePathType::Lower);
    }
    SECTION("'root' and 'root' is same") {
        src = "root";
        toCompare = "root";
        auto type = test.GetPathType(src, toCompare);
        REQUIRE(type == json::ePathType::Same);
    }
    SECTION("'root|section1' and 'root|section2' is different") {
        src = "root" + DIV + "section1";
        toCompare = "root" + DIV + "section2";
        auto type = test.GetPathType(src, toCompare);
        REQUIRE(type == json::ePathType::Else);
    }
}

TEST_CASE("json::UnpackPath", "[json::UnpackPath]") {
    logger_info("Unit testing... json::UnpackPath");

    json test;
    std::string path;
    std::string DIV = test.GetDiv_TEST();

    {
        path = "root";
        auto r = test.UnpackPath(path);
        auto depth = r.size();

        REQUIRE((r[0] == "root" && depth == 1));
    }
    {
        path = "root" + DIV + "section1";
        auto r = test.UnpackPath(path);
        auto depth = r.size();

        REQUIRE((r[0] == "root" && r[1] == "section1" && depth == 2));
    }
    {
        path = "root" + DIV + "section1" + DIV + "section2";
        auto r = test.UnpackPath(path);
        auto depth = r.size();

        REQUIRE((r[0] == "root" && r[1] == "section1" && r[2] == "section2" &&
                 depth == 3));
    }
}

TEST_CASE("json::PackPath", "[json::PackPath]") {
    logger_info("Unit testing... json::PackPath");

    json test;
    std::string DIV = test.GetDiv_TEST();

    {
        std::vector<std::string> pathVec;
        pathVec.push_back("root");
        auto r = test.PackPath(pathVec);
        REQUIRE(r == "root");
    }
    {
        std::vector<std::string> pathVec;
        pathVec.push_back("root");
        pathVec.push_back("section1");
        auto r = test.PackPath(pathVec);
        REQUIRE(r == "root" + DIV + "section1");
    }
}

TEST_CASE("json::WritePath", "[json::WritePath]") {
    logger_info("Unit testing... json::WritePath");

    json test;
    std::string path;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();

    {
        path = ROOT + DIV + "section1";
        auto r = test.WritePath(path, 1);
        REQUIRE(r == "    \"section1\": {\n");
    }
    {
        path = ROOT + DIV + "section1" + DIV + "section2";
        auto r1 = test.WritePath(path, 1);
        auto r2 = test.WritePath(path, 2);
        REQUIRE(r1 == "    \"section1\": {\n");
        REQUIRE(r2 == "        \"section2\": {\n");
    }
}

TEST_CASE("json::WriteData", "[json::WriteData]") {
    logger_info("Unit testing... json::WriteData");

    json test;
    std::string path, name;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();
    {
        path = ROOT;
        name = "myData";
        std::vector<std::string> rData(1, "1");
        jsonvalue::Data tData(path, name, rData);
        auto r = test.WriteData(tData);
        REQUIRE(r == "    \"myData\": 1,\n");
    }
    {
        path = ROOT + DIV + "section1";
        name = "myData";
        std::vector<std::string> rData(1, "1");
        jsonvalue::Data tData(path, name, rData);
        auto r = test.WriteData(tData);
        REQUIRE(r == "        \"myData\": 1,\n");
    }
}

TEST_CASE("json::ClosePath", "[json::ClosePath]") {
    logger_info("Unit testing... json::ClosePath");

    json test;
    std::string path, name, data;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();
    std::string INDENT;

    {
        path = ROOT + DIV + "section1";
        INDENT = test.GetIndent(path, json::eIndentType::Close);
        auto r = test.ClosePath(path);
        REQUIRE(r == INDENT + "},\n");
    }
    {
        path = ROOT + DIV + "section1" + DIV + "section2";
        INDENT = test.GetIndent(path, json::eIndentType::Close);
        auto r = test.ClosePath(path);
        REQUIRE(r == INDENT + "},\n");
    }
}

TEST_CASE("json::DelComma", "[json::DelComma]") {
    logger_info("Unit testing... json::DelComma");

    json test;
    std::string path, result;

    {
        path = "    \"section1\": {\n";
        path += "        \"myData\": 1,\n";

        result = "    \"section1\": {\n";
        result += "        \"myData\": 1\n";

        test.DelComma(&path);
        REQUIRE(path == result);
    }
}

TEST_CASE("json::GetIndent", "[json::GetIndent]") {
    logger_info("Unit testing... json::GetIndent");

    json test;
    std::string path, indent;
    std::string DIV = test.GetDiv_TEST();

    {
        path = "root";
        REQUIRE(test.GetIndent(path, json::eIndentType::WriteData) == "    ");
        REQUIRE(test.GetIndent(path, json::eIndentType::WriteArr) ==
                "        ");
    }
    {
        path = "root" + DIV + "section1";
        REQUIRE(test.GetIndent(path, json::eIndentType::WritePath) == "    ");
        REQUIRE(test.GetIndent(path, json::eIndentType::WriteData) ==
                "        ");
        REQUIRE(test.GetIndent(path, json::eIndentType::WriteArr) ==
                "            ");
        REQUIRE(test.GetIndent(path, json::eIndentType::Close) == "    ");
    }
}

TEST_CASE("json::GetWriteDepth", "[json::GetWriteDepth]") {
    logger_info("Unit testing... json::GetWriteDepth");

    json test;
    std::string prevPath, newPath;
    std::string DIV = test.GetDiv_TEST();

    {
        prevPath = "root";
        newPath = "root" + DIV + "section1";
        auto r = test.GetWriteDepth(prevPath, newPath);
        REQUIRE((r.first == 1 && r.second == 1));
    }
    {
        prevPath = "root" + DIV + "section1";
        newPath = "root" + DIV + "section1" + DIV + "section2";
        auto r = test.GetWriteDepth(prevPath, newPath);
        REQUIRE((r.first == 2 && r.second == 2));
    }
    {
        prevPath = "root";
        newPath = "root" + DIV + "section1" + DIV + "section2";
        auto r = test.GetWriteDepth(prevPath, newPath);
        REQUIRE((r.first == 1 && r.second == 2));
    }
    {
        prevPath = "root";
        newPath = "root";
        auto r = test.GetWriteDepth(prevPath, newPath);
        REQUIRE((r.first == 0 && r.second == 0));
    }
}

TEST_CASE("json::ToUpperPath", "[json::ToUpperPath]") {
    logger_info("Unit testing... json::ToUpperPath");

    json test;
    std::string path;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();

    {
        path = ROOT;
        test.ToUpperPath(&path);
        REQUIRE(path == "");
    }
    {
        path = "root" + DIV + "section1";
        test.ToUpperPath(&path);
        REQUIRE(path == "root");
    }
    {
        path = "root" + DIV + "section1" + DIV + "section2";
        test.ToUpperPath(&path);
        REQUIRE(path == "root" + DIV + "section1");
    }
}

TEST_CASE("json::operator=", "[json::operator=]") {
    logger_info("Unit testing... json::operator=");

    json test;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();

    std::string path, name;

    test["myData"] = 1;
    jsonvalue::Data tData1 = test.GetDataVec_TEST()[0];
    path = tData1.GetPath();
    name = tData1.GetName();
    std::vector<std::string> rData1 = tData1.GetData();
    REQUIRE((path == ROOT && name == "myData" && rData1[0] == "1"));

    test["myData"] = 2;
    jsonvalue::Data tData2 = test.GetDataVec_TEST()[0];
    path = tData2.GetPath();
    name = tData2.GetName();
    std::vector<std::string> rData2 = tData2.GetData();
    REQUIRE((path == ROOT && name == "myData" && rData2[0] == "2"));

    test["section1"]["myData"] = 1;
    jsonvalue::Data tData3 = test.GetDataVec_TEST()[1];
    path = tData3.GetPath();
    name = tData3.GetName();
    std::vector<std::string> rData3 = tData3.GetData();
    REQUIRE((path == ROOT + DIV + "section1" && name == "myData" &&
             rData3[0] == "1"));
}