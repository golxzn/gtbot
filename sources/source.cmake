
add_subdirectory(${GTBOT_SOURCES_DIR}/core)
add_subdirectory(${GTBOT_SOURCES_DIR}/neural)
add_subdirectory(${GTBOT_SOURCES_DIR}/bot)

list(APPEND libraries
	golxzn::core
	golxzn::neural
	golxzn::bot
)

if(GTBOT_CONFIGURE_MODULE MATCHES Platform)
	add_subdirectory(${GTBOT_PLATFORM_SOURCES_DIR})
elseif(GTBOT_CONFIGURE_MODULE MATCHES Tests)
	add_subdirectory(${GTBOT_SOURCES_DIR}/tests)
endif()
