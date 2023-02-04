# ---- In-source guard ----

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR
		"In-source builds are not supported. "
		"Please read the BUILDING document before trying to build this project. "
		"You may need to delete 'CMakeCache.txt' and 'CMakeFiles/' first."
	)
endif()

include(cmake/utils/detect_platform.cmake)
include(cmake/utils/functions.cmake)

get_cpm(VERSION 0.36.0)
get_ccache(VERSION 4.7.4)
