cmake_minimum_required(VERSION 3.2)
project(ImageRetrievalCpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")

set(SOURCE_FILES
    app/app_data.h
    query/query.h
    utils/inverted_index.h
    configurations.h
    main.cpp
    README.md)

include_directories(.)
include_directories(app)
include_directories(query)
include_directories(utils)

add_executable(ImageRetrievalCpp ${SOURCE_FILES})