cmake_minimum_required(VERSION 3.25)
project(mcmv_core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/src)


message(STATUS "Downloading required libraries...")

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/imgui)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/imguifiledialog)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/glfw)

execute_process(COMMAND git clone "https://github.com/ocornut/imgui.git" ${CMAKE_SOURCE_DIR}/libs/imgui)
execute_process(COMMAND git clone "https://github.com/aiekick/ImGuiFileDialog.git" ${CMAKE_SOURCE_DIR}/libs/imguifiledialog)
execute_process(COMMAND git clone "https://github.com/glfw/glfw.git" ${CMAKE_SOURCE_DIR}/libs/glfw)



# include directories for imgui and glfw
include_directories(${CMAKE_SOURCE_DIR}/libs/imgui)
include_directories(${CMAKE_SOURCE_DIR}/libs/imgui/backends)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw)
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw/include)
# move pthead file into build folder
configure_file(pthread.lib pthread.lib COPYONLY)

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
    src/armature/joint.h 
    src/armature/joint.cpp 
    src/armature/armature_format_adapter.h 
    src/armature/armature_format_adapter.cpp 
    src/export/bedrock/bedrock_exporter.cpp 
    src/export/bedrock/bedrock_exporter.h 
    src/export/bedrock/armature_exporter.h
    src/import/file_reader.h 
    src/import/file_reader.cpp 
    src/import/bvh/bvh_lexer.h 
    src/import/bvh/bvh_lexer.cpp 
    src/import/bvh/bvh_parser.cpp 
    src/import/bvh/bvh_parser.h 
    src/import/armature_importer.h 
    src/import/bvh/bvh_importer.h 
    src/import/bvh/bvh_importer.cpp 
    src/import/bb/bb_importer.cpp 
    src/import/bb/bb_importer.h 
    src/import/bb/bb_parser.cpp 
    src/import/bb/bb_parser.h 
    src/math/quaternion.cpp
    src/math/quaternion.h
    src/math/vector3.h 
    src/math/vector3.cpp 
    src/math/euler.h 
    src/math/euler.cpp 
    src/thread_pool/thread_pool.cpp 
    src/thread_pool/thread_pool.h

)

add_executable(mcmv_core src/main.cpp ${SOURCE_FILES} resources/mcmv_core.rc) # Build mcmv_core project


# link libraries to executable
target_link_libraries(mcmv_core PRIVATE nlohmann_json::nlohmann_json pthread)

# set compiler library linker to MultiThreadedDLL
if(WIN32)
  target_compile_options(mcmv_core PRIVATE "/MD")
endif(WIN32)

enable_testing()

add_executable(
        m_test
        tests/functional/f_line.cpp
        ${SOURCE_FILES}
        tests/helper/helper.h tests/manual/m_line.cpp)
target_link_libraries(
        m_test PRIVATE
        GTest::gtest_main
        GTest::gmock_main
        nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(m_test)


# BEGIN GUI TARGET ###################################################################################################################

# find required header files
find_package(OpenGL REQUIRED)

# set appropriate sources
set(GUI_SOURCES
  ${SOURCE_FILES}
	src/ui/ui_pages.cpp
  src/ui/makeui.cpp
  src/ui/file_dialog.cpp
  src/ui/joint_combo.cpp
  src/ui/joint_list_builder.cpp
)

# make executeable target
add_executable(mcmv_gui src/main_gui.cpp ${GUI_SOURCES} resources/mcmv_gui.rc)  # Build mcmv_gui project



# adding sources for imgui
list(APPEND IMGUI_SOURCES
    libs/imgui/imgui.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
    libs/imguifiledialog/ImGuiFileDialog.cpp
)

# set imgui sources to target
target_sources(mcmv_gui PRIVATE ${IMGUI_SOURCES})
# no command prompt when application is opened
set_target_properties(mcmv_gui PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

# set glfw optional build args
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


target_include_directories(mcmv_gui PRIVATE libs/imgui)  # Include the IMGUI headers
target_include_directories(mcmv_gui PRIVATE libs/glfw/include)  # Include the GLFW headers
target_include_directories(mcmv_gui PRIVATE libs/imguifiledialog)  # Include the File Dialog headers


# set compiler library linker to MultiThreadedDLL
if(WIN32)
  target_compile_options(mcmv_gui PRIVATE "/MD")
endif(WIN32)

# link all libraries to target
target_link_libraries(mcmv_gui PRIVATE nlohmann_json::nlohmann_json pthread glfw ${GLFW_LIBRARIES} OpenGL::GL)

# END GUI TARGET ################################################################################################################### 