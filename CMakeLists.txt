cmake_minimum_required(VERSION 3.20)

include(cmake/prelude.cmake)

project(gtbot
	VERSION 0.2.0
	DESCRIPTION "Golxzn Telegram Bot"
	HOMEPAGE_URL "none"
	LANGUAGES C CXX
)

include(cmake/options.cmake)

include(${GTBOT_LIBRARIES_DIR}/deps.cmake)
include(${GTBOT_SOURCES_DIR}/source.cmake)

