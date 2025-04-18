# 设置cmake的最低版本号，项目名称，以及C++标准
cmake_minimum_required(VERSION 3.20.0)
project(UserRecommendationSystem LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# 存放头文件的目录，存放可执行文件的目录
include_directories(${CMAKE_SOURCE_DIR}/include)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 汇总源文件，存放在src目录下
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_LIST)

# 生成可以执行的文件
add_executable(
    UserRecommendationSystem
    ${SRC_LIST}
)

if(WIN32)
    add_compile_options(-fexec-charset=GBK) # 让 GCC 输出 GBK 编码（适用于 Windows 终端）

    # 或者：
    # add_compile_options(-fexec-charset=UTF-8)  # 让 GCC 输出 UTF-8（适用于现代终端）
endif()