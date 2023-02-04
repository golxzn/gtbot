#pragma once

#include <limits>
#include <type_traits>
#include <core/aliases.hpp>

#define old_max(a, b) max(a, b)
#undef max

namespace golxzn::core {

template<class T, std::enable_if_t<std::is_unsigned_v<T>, bool> = false>
nodis constexpr T invalid_id() noexcept {
	return std::numeric_limits<T>::max();
}

} // namespace golxzn::core

#define max(a, b) old_max(a, b)
#undef old_max