cmake_minimum_required(VERSION 3.25)
project(mcmv_core)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(mcmv_core src/main.cpp src/math/quaternion.cpp src/math/quaternion.h src/math/vector3.h src/math/vector3.cpp src/math/euler.h src/math/euler.cpp src/armature/joint.h src/armature/joint.cpp src/import/file_reader.h src/import/file_reader.cpp src/import/bvh/bvh_lexer.h src/import/bvh/bvh_lexer.cpp src/import/bvh/bvh_parser.cpp src/import/bvh/bvh_parser.h src/armature/armature_format_adapter.h src/armature/armature_format_adapter.cpp src/thread_pool/thread_pool.cpp src/thread_pool/thread_pool.h src/import/armature_importer.h src/import/bvh/bvh_importer.h src/import/bvh/bvh_importer.cpp src/import/bb/bb_importer.cpp src/import/bb/bb_importer.h)

target_link_libraries(mcmv_core nlohmann_json::nlohmann_json)