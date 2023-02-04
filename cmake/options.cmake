
set(root ${CMAKE_SOURCE_DIR})
set(bin_dir ${root}/bin)
set(target gtbot)
set(libraries "")
set(sources "")
set(include_directories "")
set(definitions "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile_commands.json" FORCE)

set(GTBOT_RUNTIME_OUTPUT_DIRECTORY ${bin_dir})
set(GTBOT_LIBRARY_OUTPUT_DIRECTORY ${bin_dir})
set(GTBOT_ARCHIVE_OUTPUT_DIRECTORY ${bin_dir})

set(GTBOT_CPP_STANDARD 20 CACHE STRING "C++ standard")
set(GTBOT_SOURCES_DIR ${root}/sources CACHE PATH "Sources directory")
set(GTBOT_PLATFORM_SOURCES_DIR ${GTBOT_SOURCES_DIR}/platform/${PLATFORM} CACHE PATH "Platform sources")
set(GTBOT_LIBRARIES_DIR ${root}/libraries CACHE PATH "Libraries directory")

set(GTBOT_ZLIB_VERSION             "1.2.13"         CACHE STRING "zlib version"                  )
set(GTBOT_FMT_VERSION              "9.1.0"          CACHE STRING "FMT version"                   )
set(GTBOT_SPDLOG_VERSION           "1.11.0"         CACHE STRING "SPDLOG library version"        )
set(GTBOT_PLATFORM_FOLDERS_VERSION "4.2.0"          CACHE STRING "PlatformFolders version"       )
set(GTBOT_FILESYSTEM_VERSION       "1.5.12"         CACHE STRING "Gulrak Filesystem version"     )
set(GTBOT_TDLIB_VERSION            "1.7.0"          CACHE STRING "TDLib version"                 )

mark_as_advanced(
	GTBOT_PLATFORM_SOURCES_DIR
	CMAKE_EXPORT_COMPILE_COMMANDS
)

include(cmake/utils/config.cmake)

message(STATUS "-- -- -- -- -- -- -- -- -- options -- -- -- -- -- -- -- -- -- --")
message(STATUS "Target:                          | ${target}")
message(STATUS "Platform:                        | ${PLATFORM} (${ARCHITECTURE})")
message(STATUS "Build types:                     | ${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "C++ standard:                    | ${GTBOT_CPP_STANDARD}")
message(STATUS "Directories:                     |")
message(STATUS "    Sources:                     | ${GTBOT_SOURCES_DIR}")
message(STATUS "    Platform:                    | ${GTBOT_PLATFORM_SOURCES_DIR}")
message(STATUS "    Libraries:                   | ${GTBOT_LIBRARIES_DIR}")
message(STATUS "Libraries:                       |")
message(STATUS "    Magic Enum version:          | ${GTBOT_MAGIC_ENUM_VERSION}")
message(STATUS "    FMT version:                 | ${GTBOT_FMT_VERSION}")
message(STATUS "    SPD log version:             | ${GTBOT_SPDLOG_VERSION}")
message(STATUS "    PlarformFolders version:     | ${GTBOT_PLATFORM_FOLDERS_VERSION}")
message(STATUS "    Gulrack filesystem version:  | ${GTBOT_FILESYSTEM_VERSION}")
message(STATUS "    Telegram bot api version:    | ${GTBOT_TDLIB_VERSION}")
message(STATUS "    ZLib version:                | ${GTBOT_ZLIB_VERSION}")
message(STATUS "-- -- -- -- -- -- -- -- -- -- - -- -- -- -- -- -- -- -- -- -- --")