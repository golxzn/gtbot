#include <execution>
#include <core/common>
#include <core/utils/random.hpp>
#include <core/resources/manager.hpp>

#include "neural/dataset.hpp"

namespace golxzn::neural {

Dataset::Dataset(const std::vector<core::byte> &raw_data, const core::u32 input_count, const core::u32 output_count) {
	if (raw_data.empty()) [[unlikely]] return;

	load_from_raw_data(raw_data.data(), raw_data.size(), input_count, output_count);
}

Dataset::Dataset(const std::string_view file, const core::u32 input_count, const core::u32 output_count)
	: Dataset{ core::resources::Manager::load_file(file), input_count, output_count } { }

Dataset::Dataset(const std::string_view file) {
	const auto raw_data{ core::resources::Manager::load_file(file) };
	if (raw_data.empty()) [[unlikely]] return;

	const auto [offset, input_size, output_size]{ get_input_and_output_count(raw_data) };
	const auto data_length{ raw_data.size() - offset };

	load_from_raw_data(raw_data.data() + offset, data_length, input_size, output_size);
}

void Dataset::split(const core::f32 ratio) {
	using namespace core::types_literals;

	mTrainDataInput.clear();
	mTrainDataOutput.clear();
	mTestDataInput.clear();
	mTestDataOutput.clear();

	/// @todo: it could cause out of range crush
	for (core::u32 id{}; id < mInputData.size(); ++id) {
		if (core::utils::random::range(0.0_f32, 1.0_f32) < ratio) {
			mTrainDataInput.emplace_back(std::cref(mInputData.at(id)));
			mTrainDataOutput.emplace_back(std::cref(mOutputData.at(id)));
		} else {
			mTestDataInput.emplace_back(std::cref(mInputData.at(id)));
			mTestDataOutput.emplace_back(std::cref(mOutputData.at(id)));
		}
	}

	// for (core::u32 id{}; id < mOutputData.size(); ++id) {
	// 	if (core::utils::random::range(0.0_f32, 1.0_f32) < ratio) {
	// 		mTrainDataOutput.emplace_back(std::cref(mOutputData.at(id)));
	// 	} else {
	// 		mTestDataOutput.emplace_back(std::cref(mOutputData.at(id)));
	// 	}
	// }
}

const Dataset::data_ref_t<Dataset::value_type> &Dataset::get_input(const Type type) const noexcept {
	if (type == Type::Train) {
		return mTrainDataInput;
	}
	return mTestDataInput;
}

const Dataset::data_ref_t<Dataset::value_type> &Dataset::get_output(const Type type) const noexcept {
	if (type == Type::Train) {
		return mTrainDataOutput;
	}
	return mTestDataOutput;
}

Dataset &Dataset::append(const std::initializer_list<value_type> &input, const std::initializer_list<value_type> &output) noexcept {
	if ((mInputData.empty() && mOutputData.empty()) || validate(input, output)) {
		clean_split();
		mInputData.emplace_back(input);
		mOutputData.emplace_back(output);
		++mLinesCount;
	}
	return *this;
}

Dataset &Dataset::append(std::initializer_list<value_type> &&input, std::initializer_list<value_type> &&output) noexcept {
	if ((mInputData.empty() && mOutputData.empty()) || validate(input, output)) {
		clean_split();
		mInputData.emplace_back(std::move(input));
		mOutputData.emplace_back(std::move(output));
		++mLinesCount;
	}
	return *this;
}

void Dataset::erase(const core::u32 line) {
	if (line >= mInputData.size()) [[unlikely]] return;

	clean_split();
	mInputData.erase(mInputData.begin() + line);
	mOutputData.erase(mOutputData.begin() + line);
	--mLinesCount;
}

std::vector<core::byte> Dataset::raw() const {
	if (mLinesCount == 0) [[unlikely]] return {};
	if (mInputData.empty() || mOutputData.empty()) [[unlikely]] return {};

	const auto input_count{ get_input_count() };
	const auto output_count{ get_output_count() };
	if (input_count == 0 || output_count == 0) [[unlikely]] return {};

	const auto line_size{ input_count + output_count };
	const auto data_count{ mLinesCount * line_size };

	std::vector<core::byte> raw_data(data_count * value_size, core::byte{});
	auto value{ reinterpret_cast<value_type *>(raw_data.data()) };
	for (core::u32 id{}; id < data_count; ++id) {
		if (is_input_id(id, line_size, input_count)) {
			value[id] = mInputData.at(id / line_size).at(id % line_size);
		} else {
			value[id] = mOutputData.at(id / line_size).at(id % line_size - input_count);
		}
	}

	return raw_data;
}

core::u32 Dataset::get_input_count() const noexcept {
	return mInputData.empty() ? core::u32{} : static_cast<core::u32>(mInputData.front().size());
}
core::u32 Dataset::get_output_count() const noexcept {
	return mOutputData.empty() ? core::u32{} : static_cast<core::u32>(mOutputData.front().size());
}

const Dataset::data_t<Dataset::value_type> &Dataset::get_raw_input() const noexcept {
	return mInputData;
}
const Dataset::data_t<Dataset::value_type> &Dataset::get_raw_output() const noexcept {
	return mOutputData;
}

void Dataset::clean() {
	clean_split();
	mInputData.clear();
	mOutputData.clear();
	mLinesCount = 0;
}

void Dataset::clean_split() {
	mTrainDataInput.clear();
	mTrainDataOutput.clear();
	mTestDataInput.clear();
	mTestDataOutput.clear();
}

#if defined(GOLXZN_DEBUG)

std::string Dataset::to_string() const {
	std::stringstream out;
	out << "   | ";
	for (core::u32 i{}; i < get_input_count(); ++i) {
		out << "[ in ]";
	}
	out << " ";
	for (core::u32 i{}; i < get_output_count(); ++i) {
		out << "[ out]";
	}
	out << "\n";
	for (core::u32 i{}; i < mInputData.size(); ++i) {
		out << std::setw(3) << i << "| ";
		for (const auto &v : mInputData.at(i)) {
			out << '[' << std::setw(4) << v << ']';
		}
		out << " ";
		for (const auto &v : mOutputData.at(i)) {
			out << '[' << std::setw(4) << v << ']';
		}
		out << '\n';
	}
	out << '\n';
	return out.str();
}

std::string Dataset::dump() const {
	std::stringstream out;
	const auto input_count{ get_input_count() };
	const auto output_count{ get_output_count() };
	if (input_count == 0 || output_count == 0) [[unlikely]] return {};

	const auto value{ raw() };
	for (core::u32 i{}; i < value.size(); ++i) {
		out << "0x" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex
			<< static_cast<uint32_t>(value[i] & 0xff) << ' ';
		if ((i + 1) % value_size == 0) {
			out << " | (" << std::setfill(' ') << std::setw(4)
				<< *(reinterpret_cast<const value_type*>(value.data()) + (i / value_size))
				<< ")" << "\n";
		}
	}
	return out.str();
}

#endif // GOLXZN_DEBUG


void Dataset::load_from_raw_data(const core::byte *raw_data, const core::u32 data_length,
		const core::u32 input_count, const core::u32 output_count) {
	if (data_length == 0 || raw_data == nullptr) [[unlikely]] return;

	const auto data_count{ data_length / value_size };
	const auto line_size{ input_count + output_count };
	mLinesCount = data_count / line_size;
	mInputData.resize(mLinesCount);
	mOutputData.resize(mLinesCount);

	const auto output_divider{ output_count * line_size };
	const auto value{ reinterpret_cast<const value_type *>(raw_data) };
	for(core::u32 id{}; id < data_count; ++id) {
		if (is_input_id(id, line_size, input_count)) {
			auto &input = mInputData.at(id / line_size);
			if (input.capacity() == 0) {
				input.reserve(input_count);
			}
			input.emplace_back(value[id]);
		} else {
			auto &output = mOutputData.at(id / line_size);
			if (output.capacity() == 0) {
				output.reserve(output_count);
			}
			output.emplace_back(value[id]);
		}
	}
}

bool Dataset::validate(const std::initializer_list<value_type> &input_data,
		const std::initializer_list<value_type> &output_data) const noexcept {

	if (const auto count{ get_input_count() }; count != input_data.size()) [[unlikely]] {
		spdlog::error("[{}]: Cannot append input data with different size ({} != {})",
			class_name.data(), count, input_data.size());
		return false;
	}

	if (const auto count{ get_output_count() }; count != output_data.size()) [[unlikely]] {
		spdlog::error("[{}]: Cannot append output data with different size ({} != {})",
			class_name.data(), count, output_data.size());
		return false;
	}
	return true;
}

bool Dataset::is_input_id(const core::u32 id, const core::u32 line_size, const core::u32 input_count) noexcept {
	return (id % line_size) < input_count;
}

std::array<Dataset::count_value_type, 3> Dataset::get_input_and_output_count(const std::vector<core::byte> &data) {

	static constexpr count_value_type count_fields_size{ count_value_size * 2 };
	if (data.size() < count_fields_size) [[unlikely]] {
		return {};
	}

	const auto value{ reinterpret_cast<const count_value_type *>(data.data()) };
	return std::array<count_value_type, 3>{ count_fields_size, value[0], value[1] };
}

} // namespace golxzn::neural

