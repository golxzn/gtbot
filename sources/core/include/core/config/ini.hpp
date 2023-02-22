#pragma once

#include <any>
#include <cmath>
#include <bitset>
#include <string>
#include <string_view>
#include <core/utils/traits.hpp>

namespace golxzn::core::config {

class ini {
	friend class writer;
public:
	class value;

	using container_type = umap<std::string, umap<std::string, value>>;

	GOLXZN_DEFAULT_CLASS(ini);

	explicit ini(container_type &&data) noexcept;

	umap<std::string, value> &operator[](const std::string &key) noexcept;
	const umap<std::string, value> &at(const std::string &key) const;

	nodis bool has(const std::string &section) const noexcept;
	nodis bool has(const std::string &section, const std::string &key) const noexcept;

private:
	container_type mData;
};

class ini::value {
	friend class writer;
public:
	enum class data_type {
		none,
		string,
		integer,
		real,
		boolean,
		other,
	};
	GOLXZN_DEFAULT_CLASS(value);

	value(data_type type, std::string &&raw, std::string &&comment, std::any &&value) noexcept;

	nodis data_type type() const noexcept;
	nodis std::string_view raw() const noexcept;
	nodis std::string_view comment() const noexcept;

	void set_comment(const std::string_view value) noexcept;

	nodis explicit operator const std::string &() const noexcept;

	value &operator=(const std::string_view value) noexcept;
	value &operator=(std::integral auto value) noexcept;
	value &operator=(std::floating_point auto value) noexcept;
	value &operator=(bool value) noexcept;

	template<class T>
	nodis std::optional<T> as() const noexcept {
		if (!mData.has_value()) {
			return std::nullopt;
		}
		if constexpr (std::is_same_v<T, bool>) {
			if (const auto pData{ std::any_cast<T>(&mData) }; pData != nullptr) {
				return std::make_optional(*pData);
			}
			if (mType == data_type::integer) {
				return as_integer() != 0;
			}
			if (mType == data_type::real) {
				return std::abs(as_real()) >= std::numeric_limits<f64>::epsilon();
			}
			if (mType == data_type::string) {
				return !as_string().empty();
			}
		} else if constexpr (std::is_integral_v<T>) {
			if constexpr (std::is_unsigned_v<T>) {
				return static_cast<T>(as_unsigned_integer());
			}
			return static_cast<T>(as_integer());
		} else if constexpr (std::is_floating_point_v<T>) {
			return static_cast<T>(as_real());
		}
		if (auto pData{ std::any_cast<T>(&mData) }; pData != nullptr) {
			return *pData;
		}
		return std::nullopt;
	}

	nodis u64 as_unsigned_integer(const u64 default_value = u64{}) const noexcept;
	nodis i64 as_integer(const i64 default_value = i64{}) const noexcept;
	nodis f64 as_real(const f64 default_value = f64{}) const noexcept;
	nodis std::string as_string(const std::string_view default_value = {}) const noexcept;
private:
	data_type mType{ data_type::none };
	std::string mRaw{};
	std::string mComment{};
	std::any mData{};
};

using value_type = ini::value::data_type;

class parser {
public:
	GOLXZN_STATIC_CLASS(parser);

	struct ini_value;
	struct ini_document;

	[[nodiscard]] static ini parse(const std::string_view content);
};

struct parser::ini_value {
	GOLXZN_STATIC_CLASS(ini_value);

	static ini::value parse(const std::string_view value, const std::string_view comment);
	static bool is_number(std::string_view value, bool &is_real) noexcept;
};

class parser::ini_document {
public:
	static constexpr auto line_types_count{ 5ull };
	static constexpr std::string_view comments{ ";#" };
	static constexpr std::string_view eol{ "\n\r" };
	static constexpr std::string_view whitespace{ " \t" };
	static constexpr auto equal{ '=' };
	static constexpr auto section_beg{ '[' };
	static constexpr auto section_end{ ']' };

	using line_type_t = std::bitset<line_types_count>;

	GOLXZN_STATIC_CLASS(ini_document);

	enum class line_type : u8 {
		empty,
		normal,
		section,
		comment,
		syntax_error,
	};

	[[nodiscard]] static ini parse(const std::string_view content);

private:

	static line_type_t detect_type(const std::string_view line);

	static std::string_view trim(const std::string_view value);
	static std::string_view get_section(const std::string_view value);

	static std::pair<std::string, ini::value> parse_line(
		std::string &section, std::string_view line, const line_type_t &type);
};

class writer {
public:
	static constexpr auto comments{ ';' };
	static constexpr auto eol{ '\n' };
	static constexpr auto whitespace{ ' ' };
	static constexpr auto equal{ '=' };
	static constexpr auto section_beg{ '[' };
	static constexpr auto section_end{ ']' };


	GOLXZN_STATIC_CLASS(writer);

	[[nodiscard]] static std::string write(const ini &ini);
private:
	static std::string make_line(const std::string &key, const ini::value &value);
};

} // namespace golxzn::core::config
