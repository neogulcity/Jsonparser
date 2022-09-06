# jsonparser
C++ Json 라이브러리

## Build Requirements
* [CMake](https://cmake.org/)
* [Catch2](https://github.com/catchorg/Catch2/)
* [spdlog](https://github.com/gabime/spdlog/)

## Example
```cpp
// main.cpp
#include <fstream>
#include "jsonparer/jsonparser.h"

int main()
{
    json config;

    config["myData"] = 10;

    int arr[3] = {1, 2, 3};
    config["myArray"] = arr;

    config["Section1"]["myData2"] = "Hello!";

    std::string outString = static_cast<std::string>(config);
    std::ofstream out("config.json");
    out << outString;
    out.close();

    return 0;
}
```
## Result
```json
{
    "myData": 10,
    "myArray": [
        1,
        2,
        3
    ],
    "Section1": {
        "myData2": "Hello!"
    }
}
```