#include "core/common"
#include "core/utils/traits.hpp"
#include "core/config/ini.hpp"

namespace {

template<class T, std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, bool> = false>
static constexpr size_t as_pos(const T value) noexcept {
	return static_cast<size_t>(value);
}

template<std::integral T>
static constexpr T extract_integral(const std::any &any, const T default_value) noexcept {
	if (!any.has_value()) [[unlikely]] {
		return default_value;
	}
	using namespace golxzn::core;

	if (auto value{ std::any_cast<i64>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<i32>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<i16>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<i8>(&any) }; value != nullptr) return static_cast<T>(*value);

	if (auto value{ std::any_cast<u64>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<u32>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<u16>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<u8>(&any) }; value != nullptr) return static_cast<T>(*value);

	if (auto value{ std::any_cast<f64>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<f32>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<f16>(&any) }; value != nullptr) return static_cast<T>(*value);

	return default_value;
}

template<std::floating_point T>
static constexpr T extract_real(const std::any &any, const T default_value) {
	if (!any.has_value()) [[unlikely]] {
		return default_value;
	}
	using namespace golxzn::core;

	if (auto value{ std::any_cast<f64>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<f32>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<f16>(&any) }; value != nullptr) return static_cast<T>(*value);

	if (auto value{ std::any_cast<i64>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<i32>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<i16>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<i8>(&any) }; value != nullptr) return static_cast<T>(*value);

	if (auto value{ std::any_cast<u64>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<u32>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<u16>(&any) }; value != nullptr) return static_cast<T>(*value);
	if (auto value{ std::any_cast<u8>(&any) }; value != nullptr) return static_cast<T>(*value);

	return default_value;
}

} // anonymous namespace

namespace golxzn::core::config {

//============================================== ini =============================================//

ini::ini(ini::container_type &&data) noexcept : mData{ std::move(data) } {}

umap<std::string, ini::value> &ini::operator[](const std::string &key) noexcept {
	return mData[key];
}
const umap<std::string, ini::value> &ini::at(const std::string &key) const {
	return mData.at(key);
}

[[nodiscard]] bool ini::has(const std::string &section) const noexcept {
	return mData.contains(section);
}
[[nodiscard]] bool ini::has(const std::string &section, const std::string &key) const noexcept {
	if (auto found{ mData.find(section) }; found != std::end(mData)) {
		return found->second.contains(key);
	}
	return false;
}

//============================================ parser ============================================//

ini parser::parse(const std::string_view content) { return ini_document::parse(content); }

//========================================== ini::value ==========================================//

ini::value::value(data_type type, std::string &&raw, std::string &&comment, std::any &&value) noexcept
	: mType{ type }, mRaw{ std::move(raw) }, mComment{ std::move(comment) }, mData{ std::move(value) } {}

ini::value::data_type ini::value::type() const noexcept { return mType; }
std::string_view ini::value::raw() const noexcept { return mRaw; }
std::string_view ini::value::comment() const noexcept { return mComment; }

void ini::value::set_comment(const std::string_view value) noexcept { mComment = value; }

ini::value::operator const std::string &() const noexcept {
	return mRaw;
}

ini::value &ini::value::operator=(const std::string_view value) noexcept {
	mRaw = value;
	mType = data_type::string;
	mData = std::string{ mRaw };
	return *this;
}

ini::value &ini::value::operator=(std::integral auto value) noexcept {
	mRaw = std::to_string(value);
	mType = data_type::integer;
	mData = value;
	return *this;
}

ini::value &ini::value::operator=(std::floating_point auto value) noexcept {
	mRaw = std::to_string(value);
	mType = data_type::real;
	mData = value;
	return *this;
}

ini::value &ini::value::operator=(bool value) noexcept {
	mRaw = value ? "true" : "false";
	mType = data_type::boolean;
	mData = value;
	return *this;
}

u64 ini::value::as_unsigned_integer(const u64 default_value) const noexcept {
	if (mType != data_type::integer) [[unlikely]] {
		return default_value;
	}
	return extract_integral<u64>(mData, default_value);
}
i64 ini::value::as_integer(const i64 default_value) const noexcept {
	if (mType != data_type::integer) [[unlikely]] {
		return default_value;
	}
	return extract_integral<i64>(mData, default_value);
}
f64 ini::value::as_real(const f64 default_value) const noexcept {
	if (mType != data_type::real) [[unlikely]] {
		return default_value;
	}
	return extract_real<f64>(mData, default_value);
}
std::string ini::value::as_string(const std::string_view default_value) const noexcept {
	if (mType != data_type::string) [[unlikely]] {
		return std::string{ default_value };
	}
	if (auto value{ std::any_cast<std::string>(&mData) }; value != nullptr) return *value;
	if (auto value{ std::any_cast<const char *>(&mData) }; value != nullptr) return std::string{ *value };
	if (auto value{ std::any_cast<std::string_view>(&mData) }; value != nullptr) return std::string{ *value };

	return std::string{ default_value };
}

//======================================= parser::ini_value ======================================//

ini::value parser::ini_value::parse(const std::string_view value, const std::string_view comment) {
	using type_t = ini::value::data_type;
	if (value.empty()) {
		if (comment.empty()) {
			return {};
		}
		return ini::value{ type_t::none, {}, std::string{ comment }, {} };
	}

	using namespace std::string_view_literals;
	static constexpr auto npos = std::string::npos;

	if (bool is_real{ false }; is_number(value, is_real)) {
		return ini::value{
			is_real ? type_t::real : type_t::integer,
			std::string{ value },
			std::string{ comment },
			is_real
				? std::make_any<f32>(std::strtod(value.data(), nullptr))
				: std::make_any<i32>(std::strtol(value.data(), nullptr, 10))
		};
	}

	std::string lower;
	lower.reserve(value.size());
	std::transform(std::begin(value), std::end(value), std::back_inserter(lower),
		[](const auto c){ return std::tolower(c); });

	if (traits::any_from(lower, "true"sv, "ok"sv, "yes"sv, "enable"sv, "enabled"sv)) {
		return ini::value{ type_t::boolean, std::string{ value }, std::string{ comment }, true };
	} else if (traits::any_from(lower, "false"sv, "no"sv, "disable"sv, "disabled"sv)) {
		return ini::value{ type_t::boolean, std::string{ value }, std::string{ comment }, false };
	}
	return ini::value{ type_t::string,
		std::string{ value },
		std::string{ comment },
		std::string{ value }
	};
}

bool parser::ini_value::is_number(std::string_view text, bool &is_real) noexcept {
	if (text.empty()) [[unlikely]] return false;
	if (text.front() == '-' || text.front() == '+') [[unlikely]]
		text.remove_prefix(1);

	size_t dot_count{};
	for (const auto c : text) {
		if (c == '.') [[unlikely]] {
			++dot_count;
			if (dot_count > 1) {
				return false;
			}
			continue;
		}
		if (!std::isdigit(c)) [[unlikely]] return false;
	}
	is_real = dot_count != 0;
	return true;
}

//===================================== parser::ini_document =====================================//

ini parser::ini_document::parse(const std::string_view content) {

	ini::container_type data{};
	std::string current_section{};
	for (std::string_view::size_type beg{}; beg != std::string_view::npos;) {
		const auto end{ content.find_first_of(eol, beg) };
		const auto line{ content.substr(beg, end - beg) };
		beg = end != std::string_view::npos ? end + 1 : end;

		const auto type{ detect_type(line) };
		auto [key, value]{ parse_line(current_section, line, type) };
		if (key.empty()) continue;

		data[current_section].insert_or_assign(std::move(key), std::move(value));
	}

	return ini{ std::move(data) };
}

parser::ini_document::line_type_t parser::ini_document::detect_type(const std::string_view line) {
	line_type_t type{};
	const auto trimmed{ trim(line) };
	if (trimmed.empty()) {
		type.set(as_pos(line_type::empty));
		return type;
	}

	static constexpr auto npos{ std::string_view::npos };

	if (trimmed.find_first_of(comments) != npos) {
		type.set(as_pos(line_type::comment));
	}
	if (trimmed.find_first_of(section_beg) == 0) {
		if (trimmed.find_last_of(section_end) != npos) {
			type.set(as_pos(line_type::section));
		} else {
			type.set(as_pos(line_type::syntax_error));
		}
		if (std::count(std::begin(trimmed), std::end(trimmed), section_beg) > 1 ||
			std::count(std::begin(trimmed), std::end(trimmed), section_end) > 1) {
			type.set(as_pos(line_type::syntax_error));
		}
		return type;
	}
	if (trimmed.find_first_of(equal) != npos) {
		if (std::count(std::begin(trimmed), std::end(trimmed), equal) > 1) {
			type.set(as_pos(line_type::syntax_error));
			return type;
		}
		type.set(as_pos(line_type::normal));
	}
	return type;
}

std::string_view parser::ini_document::trim(const std::string_view value) {
	static constexpr auto npos{ std::string_view::npos };

	const auto beg{ value.find_first_not_of(whitespace) };
	const auto end{ value.find_last_not_of(whitespace) };
	if (beg == npos && end == npos) {
		return value;
	}
	if (beg == npos && end != npos) {
		return value.substr(0, end + 1);
	}
	if (beg != npos && end == npos) {
		return value.substr(beg);
	}

	return value.substr(beg, end - beg + 1);
}

std::string_view parser::ini_document::get_section(const std::string_view line) {
	const auto beg{ line.find_first_of(section_beg) + 1 };
	const auto end{ line.find_last_of(section_end) };
	return line.substr(beg, end - beg);
}

std::pair<std::string, ini::value> parser::ini_document::parse_line(
	std::string &section, std::string_view line, const line_type_t &type) {

	if (type.test(as_pos(line_type::empty))) {
		return {};
	}
	if (type.test(as_pos(line_type::syntax_error))) {
		spdlog::error("[parser::ini_document]: syntax error at line: '{}'", line);
		return {};
	}

	std::string_view comment{};
	if (type.test(as_pos(line_type::comment))) {
		const auto pos{ line.find_first_of(comments) };
		comment = trim(line.substr(pos + 1));
		line.remove_suffix(line.size() - pos);
	}

	if (type.test(as_pos(line_type::section))) {
		const auto sect{ get_section(line) };
		section = std::string{ sect };
		return {};
	}

	if (type.test(as_pos(line_type::normal))) {
		const auto pos{ line.find_first_of(equal) };
		const auto key{ trim(line.substr(0, pos)) };
		const auto value{ trim(line.substr(pos + 1)) };
		return std::make_pair(std::string{ key }, ini_value::parse(value, comment));
	}

	return std::make_pair(std::string{}, ini_value::parse(std::string{}, comment));
}

//============================================ writer ============================================//

std::string writer::write(const ini &ini) {
	std::string result;

	if (ini.has("")) {
		for (const auto &[name, value] : ini.at("")) {
			auto line{ make_line(name, value) };
			result.reserve(result.size() + line.size());
			result.append(std::move(line));
		}
	}

	for (const auto &[section, values] : ini.mData) {
		if (section.empty()) [[unlikely]] {
			continue;
		}
		constexpr auto environment_size{ sizeof(section_beg) + sizeof(section_end) + sizeof(eol) };
		result.reserve(result.size() + section.size() + environment_size);
		result += section_beg;
		result += section;
		result += section_end;
		result += eol;

		for (const auto &[name, value] : values) {
			auto line{ make_line(name, value) };
			result.reserve(result.size() + line.size());
			result.append(std::move(line));
		}
	}
	return result;
}

std::string writer::make_line(const std::string &key, const ini::value &value) {
	std::string result;

	if (!key.empty()) {
		result.reserve(result.size() + key.size() + sizeof(equal) + value.mRaw.size());
		result += key;
		result += equal;
		result += value.mRaw;
	}
	if (!value.mComment.empty()) {
		result.reserve(result.size() + sizeof(whitespace) * 2 + sizeof(comments) + value.mComment.size());
		result += whitespace;
		result += comments;
		result += whitespace;
		result += value.mComment;
	}
	result.reserve(result.size() + sizeof(eol));
	result += eol;
	return result;
}

} // namespace golxzn::core::config


