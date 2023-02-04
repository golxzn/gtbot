
set(type "")

if(WIN32)
	set(type WIN32)
	add_definitions(-DGOLXZN_WINDOWS)
	if(WINDOWS_STORE OR CMAKE_SYSTEM_NAME MATCHES "WindowsStore")
		set(PLATFORM "uwp")
		add_definitions(-DGOLXZN_UWP)
	else()
		set(PLATFORM "win")
	endif()
	set(ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

elseif(ANDROID)
	add_definitions(-DGOLXZN_ANDROID)
	set(PLATFORM "android")
	set(ARCHITECTURE ${ANDROID_ABI})

elseif(IOS OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
	add_definitions(-DGOLXZN_DARWIN)
	set(type MACOSX_BUNDLE)
	set(PLATFORM "ios")

	if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
		set(ARCHITECTURE "i386")
	endif()

	if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm64")
		set(ARCHITECTURE "arm64")
	endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions(-DGOLXZN_LINUX)
	set(PLATFORM "linux")
endif()

if(NOT ARCHITECTURE)
	set(ARCHITECTURE x86_64)
endif()

set(GOLXZN_PLATFORM ${PLATFORM})
set(GOLXZN_ARCHITECTURE ${ARCHITECTURE})
