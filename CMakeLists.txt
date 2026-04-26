cmake_minimum_required(VERSION 3.28)
project(JsonManager)

set(CMAKE_CXX_STANDARD 17)

add_executable(JsonManager main.cpp
        model/Person.h
        utils/Function.h
        model/Worker.h
        model/Worker.cpp
        utils/JsonParamFeatures.h
        parser/JsonParser.h
        storage/Storage.h
        storage/Storage.cpp)
