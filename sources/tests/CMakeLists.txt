cmake_minimum_required(VERSION 3.20)

project(gtbot_tests LANGUAGES CXX)
set(local_root ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

CPMAddPackage(
	NAME googletest
	GITHUB_REPOSITORY google/googletest
	VERSION 1.13.0
	OPTIONS
		"INSTALL_GTEST OFF"
		"gtest_force_shared_crt ON"
)
CPMAddPackage(
	NAME benchmark
	GITHUB_REPOSITORY google/benchmark
	VERSION 1.7.1
	OPTIONS
		"BENCHMARK_ENABLE_TESTING Off"
		"BENCHMARK_USE_LIBCXX ON"
)

if(benchmark_ADDED)
	# patch benchmark target
	set_target_properties(benchmark PROPERTIES CXX_STANDARD 17)
endif()

file(GLOB_RECURSE ${target}_sources CONFIGURE_DEPENDS ${local_root}/sources/*.c*)
file(GLOB_RECURSE ${target}_headers CONFIGURE_DEPENDS ${local_root}/sources/*.h*)

add_executable(${target}
	${${target}_sources}
	${${target}_sources}
)

target_link_libraries(${target} PUBLIC
	${libraries}
	gtest_main
	benchmark::benchmark
)
set_target_properties(${target} PROPERTIES
	CXX_STANDARD ${GTBOT_CPP_STANDARD}
	CXX_STANDARD_REQUIRED ON
)

unset(local_root)
