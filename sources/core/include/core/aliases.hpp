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

using f32 = float;
using f64 = double;

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


} // namespace golxzn::core
