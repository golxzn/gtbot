

set(ALLOW_DUPLICATE_CUSTOM_TARGETS ON)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

CPMAddPackage("gh:fmtlib/fmt#${GTBOT_FMT_VERSION}")
CPMAddPackage("gh:gabime/spdlog#v${GTBOT_SPDLOG_VERSION}")
CPMAddPackage("gh:sago007/PlatformFolders#${GTBOT_PLATFORM_FOLDERS_VERSION}")
CPMAddPackage("gh:gulrak/filesystem#v${GTBOT_FILESYSTEM_VERSION}")
CPMAddPackage("gh:tdlib/td#v${GTBOT_TDLIB_VERSION}")

list(APPEND libraries
	fmt::fmt
	spdlog::spdlog
	sago::platform_folders
	ghc_filesystem
)

list(APPEND telegram_bot_libraries
	Td::TdStatic
)
