
add_subdirectory(${GTBOT_SOURCES_DIR}/core)
add_subdirectory(${GTBOT_SOURCES_DIR}/bot)

list(APPEND libraries
	golxzn::core
	golxzn::bot
)

list(APPEND sources ${GTBOT_PLATFORM_SOURCES_DIR}/main.cpp)
