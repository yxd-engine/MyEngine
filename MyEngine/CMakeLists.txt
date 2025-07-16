add_executable (MyEngine "MyEngine.cpp" "MyEngine.h")

if (CMAKE_VERSION VERSION_GREATER 3.28)
  set_property(TARGET MyEngine PROPERTY CXX_STANDARD 20)
endif()

# --- 明确为 MyEngine 添加所有需要的头文件搜索路径 ---
# 这是解决 "No such file or directory" 错误的关键步骤。
target_include_directories(MyEngine PRIVATE
    # 添加 GLFW 的头文件目录
    ${glfw_SOURCE_DIR}/include

    # 添加 GLM 的头文件目录 (GLM 的根目录就是它的包含目录)
    ${glm_SOURCE_DIR}

    # 添加 spdlog 的头文件目录
    ${spdlog_SOURCE_DIR}/include

    # 添加 Dear ImGui 的头文件目录
    ${imgui_SOURCE_DIR}

    # 添加 GLAD 的头文件目录
    ${glad_SOURCE_DIR}/include
)

# --- Link all required third-party libraries to MyEngine ---
target_link_libraries(MyEngine PRIVATE
    # 核心图形库
    glad
    glfw
    OpenGL::GL

    # 数学库
    glm

    # 日志库
    spdlog::spdlog

    # UI 库
    imgui
    imgui_impl
)