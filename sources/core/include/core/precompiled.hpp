#pragma once

#ifdef __APPLE__
#	include <Availability.h> // for deployment target to support pre-catalina targets without std::fs
#endif

#include <initializer_list>
#include <unordered_map>
#include <type_traits>
#include <string_view>
#include <filesystem>
#include <functional>
#include <algorithm>
#include <concepts>
#include <optional>
#include <cstdint>
#include <numeric>
#include <fstream>
#include <cassert>
#include <codecvt>
#include <bitset>
#include <chrono>
#include <cstdio>
#include <locale>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <set>
#include <map>

#include <fmt/core.h>
#include <platform_folders.h>
#include <spdlog/spdlog.h>
#include <ghc/filesystem.hpp>
// #include <chaiscript/chaiscript.hpp>
