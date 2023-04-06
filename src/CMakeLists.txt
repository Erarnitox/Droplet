
add_executable(${PROJECT_NAME}
	main.cpp
	language_server.cpp
	database.cpp
)

CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:jtv/libpqxx#7.7.5")

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt pqxx)