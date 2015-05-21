cmake_minimum_required(VERSION 3.2)
project(ImageRetrievalCpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")

find_package(OpenCV REQUIRED)
find_package(HDF5 REQUIRED)

set(SOURCE_FILES
    app/appdata.h
    query/query.h
    utils/invertedindex.h
    extract/extract.h
    quantize/quantize.h
    utils/utils.h
    utils/distance.h
    configurations.h
    main.cpp
    README.md)

include_directories(${HDF5_INCLUDE_DIR})

add_executable(ImageRetrievalCpp ${SOURCE_FILES})
target_link_libraries(ImageRetrievalCpp ${OpenCV_LIBRARIES} ${HDF5_LIBRARIES})
