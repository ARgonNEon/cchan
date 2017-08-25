CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

LIST(APPEND CMAKE_PREFIX_PATH "/volume/software/common/cmake/$ENV{DLRRM_HOST_PLATFORM}")

PROJECT(cchan_example)

SET(CMAKE_CXX_COMPILER clang)
SET(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(Boost REQUIRED COMPONENTS thread system)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/..)

ADD_EXECUTABLE(cchan_example go_style_channel_example.cc)

TARGET_LINK_LIBRARIES(cchan_example
    ${Boost_LIBRARIES}
    stdc++)

ADD_EXECUTABLE(cchan_example_lambda go_style_channel_lambda_example.cc)


TARGET_LINK_LIBRARIES(cchan_example_lambda
    ${Boost_LIBRARIES}
    stdc++)
