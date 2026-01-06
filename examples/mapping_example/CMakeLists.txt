# Q25 SDK - Mapping Example CMakeLists.txt
# Windows平台构建配置

cmake_minimum_required(VERSION 3.10)
project(mapping_example)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置SDK路径
set(SDK_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(SDK_INCLUDE_DIR "${SDK_ROOT_DIR}/include")
set(SDK_LIB_DIR "${SDK_ROOT_DIR}/lib")

# 包含SDK头文件
include_directories(${SDK_INCLUDE_DIR})

# Windows SDK库
set(SDK_LIB "${SDK_LIB_DIR}/windows/robot_sdk.lib")

# 定义可执行文件
add_executable(mapping_example main.cpp)

# 链接SDK库
target_link_libraries(mapping_example ${SDK_LIB})

# Windows特定设置
target_compile_definitions(mapping_example PRIVATE _CRT_SECURE_NO_WARNINGS)
# 设置为控制台应用程序
set_target_properties(mapping_example PROPERTIES WIN32_EXECUTABLE FALSE)

# 输出目录设置
set_target_properties(mapping_example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
