cmake_minimum_required(VERSION 3.20)

set(local_root ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${local_root}/include/*.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${local_root}/source/*.cpp)
list(APPEND headers ${local_root}/include/core/common)

add_library(golxzn_core ${sources} ${headers})
add_library(golxzn::core ALIAS golxzn_core)

target_link_libraries(golxzn_core PUBLIC ${libraries})
target_compile_definitions(golxzn_core PUBLIC $<$<CONFIG:Debug>:GOLXZN_DEBUG>)
target_include_directories(golxzn_core PUBLIC ${local_root}/include ${include_directories})
target_precompile_headers(golxzn_core PUBLIC ${local_root}/include/core/precompiled.hpp)

set_target_properties(golxzn_core PROPERTIES
	CXX_STANDARD ${GTBOT_CPP_STANDARD}
	CXX_STANDARD_REQUIRED ON
)

unset(local_root)
unset(headers)
unset(sources)
