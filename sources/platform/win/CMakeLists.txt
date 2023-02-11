cmake_minimum_required(VERSION 3.20)

set(local_root ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${target} ${local_root}/main.cpp)

target_link_libraries(${target} PUBLIC ${libraries})
target_link_directories(${target} PUBLIC ${directories})
target_include_directories(${target} PUBLIC ${include_directories})
set_target_properties(${target} PROPERTIES
	CXX_STANDARD ${GTBOT_CPP_STANDARD}
	CXX_STANDARD_REQUIRED ON
	PRECOMPILE_HEADERS_REUSE_FROM golxzn_core

	RUNTIME_OUTPUT_DIRECTORY ${GTBOT_RUNTIME_OUTPUT_DIRECTORY}
	LIBRARY_OUTPUT_DIRECTORY ${GTBOT_LIBRARY_OUTPUT_DIRECTORY}
	ARCHIVE_OUTPUT_DIRECTORY ${GTBOT_ARCHIVE_OUTPUT_DIRECTORY}
)

target_compile_definitions(${target} PUBLIC ${definitions})

unset(local_root)

