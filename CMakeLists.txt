# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MyEngine")

#-----Add third-party library configuration-----
# GLFW 

add_library(glfw_lib INTERFACE)
target_include_directories(glfw_lib INTERFACE ${CMAKE_SOURCE_DIR}/libs/glfw/include)
target_link_libraries(glfw_lib INTERFACE ${CMAKE_SOURCE_DIR}/libs/glfw/lib-vc2022/glfw3.lib)

# Glad
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/libs/glad/src/gl.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/libs/glad/include)

# 包含子项目。
add_subdirectory ("MyEngine")


