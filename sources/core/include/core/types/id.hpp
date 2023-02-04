#pragma once

#include <core/constants.hpp>

namespace golxzn::core::id {

using type = u32;

constexpr type generation_bits{ 8 };
constexpr type index_bits{ sizeof(type) * 8 - generation_bits };
constexpr type index_mask{ (type{1} << index_bits) - 1 };
constexpr type generation_mask{ (type{1} << generation_bits) - 1 };
constexpr type mask{ invalid_id<type>() };

using generation_type = std::conditional_t<generation_bits < 16, std::conditional<generation_bits < 8, u8, u16>, u32>;


} // namespace golxzn::core::id
