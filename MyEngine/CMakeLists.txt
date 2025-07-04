add_executable (MyEngine "MyEngine.cpp" "MyEngine.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MyEngine PROPERTY CXX_STANDARD 20)
endif()

# --- Link all required third-party libraries to MyEngine ---
target_link_libraries(MyEngine PRIVATE
	#link GLFW
	glfw_lib

	glad
)