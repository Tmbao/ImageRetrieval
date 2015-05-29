cmake_minimum_required(VERSION 3.2)
project(ImageRetrievalCpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")

find_package(OpenCV REQUIRED)
find_package(HDF5 REQUIRED)
find_package(Boost 1.49 COMPONENTS filesystem system REQUIRED)

set(SOURCE_FILES
    app/appdata.h
    query/query.h
    utils/invertedindex.h
    extract/extract.h
    quantize/quantize.h
    utils/utils.h
    utils/score.h
    configurations.h
    main.cpp
    README.md)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${HDF5_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(ImageRetrievalCpp ${SOURCE_FILES})
target_link_libraries(ImageRetrievalCpp ${OpenCV_LIBRARIES} ${HDF5_LIBRARIES} ${Boost_LIBRARIES})
