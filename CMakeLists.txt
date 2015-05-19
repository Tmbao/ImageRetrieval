cmake_minimum_required(VERSION 3.2)
project(ImageRetrievalCpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")

set(SOURCE_FILES
    app/appdata.h
    query/query.h
    utils/invertedindex.h
    extract/extract.h
    quantize/quantize.h
    configurations.h
    main.cpp
    README.md)

add_executable(ImageRetrievalCpp ${SOURCE_FILES})