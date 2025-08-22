add_executable (MyEngine "MyEngine.cpp" "MyEngine.h" "Renderer.h" "Renderer.cpp" "VertexBuffer.h" "VertexBuffer.cpp" "IndexBuffer.h" "IndexBuffer.cpp" "VertexBufferLayout.h" "VertexArray.h" "VertexArray.cpp")

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


# --- 添加自定义命令，在构建后自动复制资源文件夹 ---
add_custom_command(
    TARGET MyEngine # 指定这个命令是附加到 MyEngine 这个目标上的
    POST_BUILD      # 指定在构建成功之后执行

    # 要执行的命令：使用 CMake 内置的、跨平台的 copy_directory 命令
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res" # 源文件夹：当前 CMakeLists.txt 所在的目录下的 res 文件夹
        "$<TARGET_FILE_DIR:MyEngine>/res" # 目标文件夹：MyEngine.exe 所在的目录下的 res 文件夹
    
    COMMENT "Copying resources to output directory" # 在构建日志中显示的提示信息
)