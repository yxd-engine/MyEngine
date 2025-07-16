# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.28)
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MyEngine")

#-----Add third-party library configuration-----
# --- 依赖项配置 (Dependencies) ---

# 1. GLFW
find_package(glfw 3.4 QUIET)
if(NOT glfw_FOUND)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
    )
    FetchContent_MakeAvailable(glfw)
endif()
# 在 Visual Studio 中将目标整理到 "Dependencies" 文件夹下
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")

# 2. GLM
find_package(glm 1.0.1 QUIET)
if(NOT glm_FOUND)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
    )
    FetchContent_MakeAvailable(glm)
endif()
#set_target_properties(glm PROPERTIES FOLDER "Dependencies")
# GLM 是纯头文件库，没有可以设置 FOLDER 的具体目标，它通过 target_link_libraries 传递

# 3. spdlog
find_package(spdlog 1.14.1 QUIET)
if(NOT spdlog_FOUND)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.14.1
    )
    FetchContent_MakeAvailable(spdlog)
endif()
set_target_properties(spdlog PROPERTIES FOLDER "Dependencies")

# 4. Dear ImGui (最终修复)
# Dear ImGui 不是一个 CMake 项目，我们不能用 add_subdirectory。
# 我们需要手动创建一个库目标，并添加它所有的源文件。
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.90.8
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()
# 手动创建一个名为 "imgui" 的库，并添加其核心源文件
add_library(imgui
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    #${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    #${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
# 将 imgui 的根目录添加到其头文件搜索路径中，这样我们才能 #include "imgui.h"
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
# 现在，imgui 目标肯定存在了，可以安全地设置它的属性
set_target_properties(imgui PROPERTIES FOLDER "Dependencies")

# 5. GLAD
# GLAD 也没有标准的 FindXXX.cmake，我们直接 Fetch 并构建
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.8
)
# 使用 FetchContent_MakeAvailable 来避免废弃警告
FetchContent_MakeAvailable(glad)
# 在 MakeAvailable 之后，再执行 GLAD 特有的配置步骤
add_subdirectory(${glad_SOURCE_DIR}/cmake glad_cmake)
glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6)
set_target_properties(glad PROPERTIES FOLDER "Dependencies")

# 6. OpenGL
# 查找系统自带的 OpenGL 库
find_package(OpenGL REQUIRED)

# --- Dear ImGui 后端配置 ---
# 这部分逻辑不变，但现在它依赖于上面已经获取的 imgui 和 glfw 目标
add_library(imgui_impl
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
# 明确地将 GLFW 的头文件路径添加到 imgui_impl
target_include_directories(imgui_impl PUBLIC
    ${imgui_SOURCE_DIR}/backends
    ${glfw_SOURCE_DIR}/include
)

target_link_libraries(imgui_impl PUBLIC
    imgui
    glfw
    OpenGL::GL # ImGui 后端需要链接到 OpenGL
)
target_include_directories(imgui_impl PUBLIC
    ${imgui_SOURCE_DIR}/backends
)
set_target_properties(imgui_impl PROPERTIES FOLDER "Dependencies")


# --- 包含我们自己的子项目 ---
add_subdirectory(MyEngine)



