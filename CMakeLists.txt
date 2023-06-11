cmake_minimum_required(VERSION 3.25)
project(mcmv_core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

include(FetchContent)


FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCE_FILES
         src/math/quaternion.cpp src/math/quaternion.h src/math/vector3.h src/math/vector3.cpp src/math/euler.h src/math/euler.cpp src/armature/joint.h src/armature/joint.cpp src/import/file_reader.h src/import/file_reader.cpp src/import/bvh/bvh_lexer.h src/import/bvh/bvh_lexer.cpp src/import/bvh/bvh_parser.cpp src/import/bvh/bvh_parser.h src/armature/armature_format_adapter.h src/armature/armature_format_adapter.cpp src/thread_pool/thread_pool.cpp src/thread_pool/thread_pool.h src/import/armature_importer.h src/import/bvh/bvh_importer.h src/import/bvh/bvh_importer.cpp src/import/bb/bb_importer.cpp src/import/bb/bb_importer.h src/import/bb/bb_parser.cpp src/import/bb/bb_parser.h src/export/bedrock/bedrock_exporter.cpp src/export/bedrock/bedrock_exporter.h src/export/bedrock/armature_exporter.h
        )

add_executable(mcmv_core src/main.cpp ${SOURCE_FILES})

target_link_libraries(mcmv_core PRIVATE nlohmann_json::nlohmann_json pthread)


enable_testing()

add_executable(
        m_test
        tests/functional/f_line.cpp
        ${SOURCE_FILES}
)
target_link_libraries(
        m_test PRIVATE
        GTest::gtest_main
        nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(m_test)
