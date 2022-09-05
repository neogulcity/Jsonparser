#include "json.h"

#include "Log.h"
#include "catch2/catch_template_test_macros.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("json::operator std::string()", "[json::operator std::string()]") {
    logger_info("Unit testing... json::operator std::string()");

    json test;
    test["section1"]["section2"]["myData"] = 1;
    std::string strTest = static_cast<std::string>(test);
    logger_info("\n{}", strTest);
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
        auto& dataVec = test.GetDataVec_TEST();
        dataVec.push_back(std::make_tuple("root", "myData", "1"));
        auto data = test.LookupData("root", "myData2");
        REQUIRE(data == nullptr);
    }
    {
        json test;
        auto& dataVec = test.GetDataVec_TEST();
        dataVec.push_back(std::make_tuple("root", "myData", "1"));
        auto data = test.LookupData("root", "myData");
        *data = "2";
        REQUIRE(std::get<2>(dataVec[0]) == "2");
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

TEST_CASE("json::WritePath", "[json::WritePath]") {
    logger_info("Unit testing... json::WritePath");

    json test;
    std::string path;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();
    std::string SPACE = "";
    for (int i = 0; i < test.GetIndent_TEST(); i++) SPACE += " ";

    {
        path = ROOT + DIV + "section1";
        auto r = test.WritePath(path);
        REQUIRE(r == SPACE + "\"section1\": {\n");
    }
    {
        path = ROOT + DIV + "section1" + DIV + "section2";
        auto r = test.WritePath(path);
        REQUIRE(r == SPACE + "\"section1\": {\n" + SPACE + SPACE +
                         "\"section2\": {\n");
    }
}

TEST_CASE("json::WriteData", "[json::WriteData]") {
    logger_info("Unit testing... json::WriteData");

    json test;
    std::string path, name, data;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();
    std::string SPACE = "";
    for (int i = 0; i < test.GetIndent_TEST(); i++) SPACE += " ";

    {
        path = ROOT;
        name = "myData";
        data = "1";
        auto r = test.WriteData(std::make_tuple(path, name, data));
        REQUIRE(r == SPACE + "\"myData\": 1,\n");
    }
    {
        path = ROOT + DIV + "section1";
        name = "myData";
        data = "1";
        auto r = test.WriteData(std::make_tuple(path, name, data));
        REQUIRE(r == SPACE + SPACE + "\"myData\": 1,\n");
    }
}

TEST_CASE("json::ClosePath", "[json::ClosePath]") {
    logger_info("Unit testing... json::ClosePath");

    json test;
    std::string path, name, data;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();
    std::string SPACE = "";
    for (int i = 0; i < test.GetIndent_TEST(); i++) SPACE += " ";

    {
        path = ROOT + DIV + "section1";
        auto r = test.ClosePath(path);
        REQUIRE(r == SPACE + "},\n");
    }
    {
        path = ROOT + DIV + "section1" + DIV + "section2";
        auto r = test.ClosePath(path);
        REQUIRE(r == SPACE + SPACE + "},\n");
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

TEST_CASE("json::operator=", "[json::operator=]") {
    logger_info("Unit testing... json::operator=");

    json test;
    std::string ROOT = test.GetRoot_TEST();
    std::string DIV = test.GetDiv_TEST();

    std::tuple<std::string, std::string, std::string> elem;
    std::string path, name, data;

    test["myData"] = 1;
    elem = test.GetDataVec_TEST()[0];
    path = std::get<0>(elem);
    name = std::get<1>(elem);
    data = std::get<2>(elem);
    REQUIRE((path == ROOT && name == "myData" && data == "1"));

    test["myData"] = 2;
    elem = test.GetDataVec_TEST()[0];
    path = std::get<0>(elem);
    name = std::get<1>(elem);
    data = std::get<2>(elem);
    REQUIRE((path == ROOT && name == "myData" && data == "2"));

    test["section1"]["myData"] = 1;
    elem = test.GetDataVec_TEST()[1];
    path = std::get<0>(elem);
    name = std::get<1>(elem);
    data = std::get<2>(elem);
    REQUIRE(
        (path == ROOT + DIV + "section1" && name == "myData" && data == "1"));
}