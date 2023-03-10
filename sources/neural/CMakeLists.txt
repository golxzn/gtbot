cmake_minimum_required(VERSION 3.20)

set(local_root ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${local_root}/include/*.hpp)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${local_root}/source/*.cpp)

add_library(golxzn_neural ${sources} ${headers})
add_library(golxzn::neural ALIAS golxzn_neural)

target_link_libraries(golxzn_neural PRIVATE ${libraries} golxzn::core)
target_compile_definitions(golxzn_neural PUBLIC $<$<CONFIG:Debug>:GOLXZN_DEBUG>)
target_include_directories(golxzn_neural PUBLIC ${local_root}/include ${include_directories})

set_target_properties(golxzn_neural PROPERTIES
	CXX_STANDARD ${GTBOT_CPP_STANDARD}
	CXX_STANDARD_REQUIRED ON
)

unset(local_root)
unset(headers)
unset(sources)
