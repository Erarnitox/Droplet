set(BUILD_SHARED_LIBS OFF)
set(CXX_EXTENSIONS OFF)
set(DPP_INSTALL OFF)
set(DPP_CORO ON)
set(BUILD_VOICE_SUPPORT OFF)

add_subdirectory(DPP)

set_target_properties(dpp PROPERTIES
	CXX_STANDARD 20
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(dpp PRIVATE
		-Wno-deprecated-literal-operator
		-Wno-mismatched-tags
		-Wno-unused-variable
	)
endif()
