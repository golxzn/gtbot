#pragma once

#include <memory>
#include <unordered_map>
#include <ghc/filesystem.hpp>

#define nodis [[nodiscard]]

namespace golxzn::core {

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f16 = float;
using f32 = double;
using f64 = long double;

using byte = u8;

template<class Type, class Deleter = std::default_delete<Type>>
using uptr = std::unique_ptr<Type, Deleter>;

template<class Type>
using sptr = std::shared_ptr<Type>;

template<class Type>
using wptr = std::weak_ptr<Type>;

template<class KeyType, class ValueType,
	class Hasher = std::hash<KeyType>, class KeyEq = std::equal_to<KeyType>>
using umap = std::unordered_map<KeyType, ValueType, Hasher, KeyEq>;

namespace fs {

using namespace ghc::filesystem;
using ifstream = ifstream;
using ofstream = ofstream;
using fstream = fstream;

using bifstream = basic_ifstream<byte, std::char_traits<byte>>;
using bofstream = basic_ofstream<byte, std::char_traits<byte>>;

} // namespace fs


namespace types_literals {

nodis constexpr i8 operator""_i8(const u64 value) noexcept { return static_cast<i8>(value); }
nodis constexpr i16 operator""_i16(const u64 value) noexcept { return static_cast<i16>(value); }
nodis constexpr i32 operator""_i32(const u64 value) noexcept { return static_cast<i32>(value); }
nodis constexpr i64 operator""_i64(const u64 value) noexcept { return static_cast<i64>(value); }

nodis constexpr u8 operator""_u8(const u64 value) noexcept { return static_cast<u8>(value); }
nodis constexpr u16 operator""_u16(const u64 value) noexcept { return static_cast<u16>(value); }
nodis constexpr u32 operator""_u32(const u64 value) noexcept { return static_cast<u32>(value); }
nodis constexpr u64 operator""_u64(const u64 value) noexcept { return value; }

nodis constexpr f16 operator""_f16(const f64 value) noexcept { return static_cast<f16>(value); }
nodis constexpr f32 operator""_f32(const f64 value) noexcept { return static_cast<f32>(value); }
nodis constexpr f64 operator""_f64(const f64 value) noexcept { return value; }

nodis constexpr f16 operator""_f16(const u64 value) noexcept { return static_cast<f16>(value); }
nodis constexpr f32 operator""_f32(const u64 value) noexcept { return static_cast<f32>(value); }
nodis constexpr f64 operator""_f64(const u64 value) noexcept { return static_cast<f64>(value); }

nodis constexpr byte operator""_b(const u64 value) noexcept { return static_cast<byte>(value); }

} // namespace types_literals

} // namespace golxzn::core

namespace golxzn {
namespace types_literals = core::types_literals;
} // namespace golxzn

