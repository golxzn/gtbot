#pragma once

#include <vector>
#include <array>
#include <core/aliases.hpp>

namespace golxzn::neural {

/**
 * @brief The dataset class
 * @details The dataset class using for train and test neural network
 * The file with data must be formatted in the following manner:
 * [data...], where [data] is the value type (core::f32) for the first constructor with input and output sizes
 * or
 * [input_count][output_count][data...], where [input_count] and [output_count] are core::u32 (4 bytes),
 * so the offset will be 8 bytes.
 *
 * Terminology:
 *  - line: the data with the `input_count` count of intput data and `output_count` count of output data:
 *    [input 1][input 2][input 3]...[input `input_count`][output 1][output 2]...[output `output_count`]
 *  - offset: the offset from the start of the line to the beginning of the data
 */
class Dataset {
public:
	template<class T> using data_t = std::vector<std::vector<T>>;
	template<class T> using data_ref_t = std::vector<std::reference_wrapper<const std::vector<T>>>;
	using count_value_type = core::u32;
	using value_type = core::f32;
	static constexpr auto value_size{ sizeof(value_type) };
	static constexpr auto count_value_size{ sizeof(count_value_type) };
	static constexpr std::string_view class_name{ "neural::Dataset" };

	enum class Type : core::u8 {
		Train,
		Test
	};

	Dataset() = default;

	Dataset(const std::vector<core::byte> &raw_data, const core::u32 input_count, const core::u32 output_count = 1);

	Dataset(const std::string_view file, const core::u32 input_count, const core::u32 output_count = 1);

	/**
	 * @brief Construct a new Dataset object using file with stored input and output count data
	 * @param file The specific formatted file
	 * [input_count][output_count][data...], where [input_count] and [output_count] are
	 * core::u32 (4 bytes)), so the offset will be 8 bytes
	 */
	explicit Dataset(const std::string_view file);

	/**
	 * @brief Split the loaded data to the train and test sets
	 * @param ratio the percentage of data to be stored in the train set
	 */
	void split(const core::f32 ratio);

	nodis const data_ref_t<value_type> &get_input(const Type type) const noexcept;
	nodis const data_ref_t<value_type> &get_output(const Type type) const noexcept;

	/**
	 * @brief Append line
	 * @param input_data input data part of line
	 * @param output_data output data part of line
	 */
	Dataset &append(const std::initializer_list<value_type> &input_data,
		const std::initializer_list<value_type> &output_data) noexcept;

	/**
	 * @brief Append line
	 * @param input_data input data part of line
	 * @param output_data output data part of line
	 */
	Dataset &append(std::initializer_list<value_type> &&input_data,
		std::initializer_list<value_type> &&output_data) noexcept;

	void erase(const core::u32 line);

	nodis std::vector<core::byte> raw() const;
	nodis core::u32 get_input_count() const noexcept;
	nodis core::u32 get_output_count() const noexcept;

	nodis const data_t<value_type> &get_raw_input() const noexcept;
	nodis const data_t<value_type> &get_raw_output() const noexcept;

	void clean();
	void clean_split();

#if defined(GOLXZN_DEBUG)
	[[nodiscard]] std::string to_string() const;
	[[nodiscard]] std::string dump() const;
#endif

private:
	core::u32 mLinesCount{};
	data_t<value_type> mInputData;
	data_t<value_type> mOutputData;

	data_ref_t<value_type> mTrainDataInput;
	data_ref_t<value_type> mTrainDataOutput;

	data_ref_t<value_type> mTestDataInput;
	data_ref_t<value_type> mTestDataOutput;

	void load_from_raw_data(const core::byte *raw_data, const core::u32 data_length,
		const core::u32 input_count, const core::u32 output_count);

	bool validate(const std::initializer_list<value_type> &input_data,
		const std::initializer_list<value_type> &output_data) const noexcept;

	static bool is_input_id(const core::u32 id, const core::u32 line_size,
		const core::u32 input_count) noexcept;

	static std::array<count_value_type, 3>  get_input_and_output_count(const std::vector<core::byte> &data);
};

} // namespace golxzn::neural

/*

		if (raw_data.empty()) [[unlikely]] return;

		const auto data_count{ raw_data.size() / value_size };
		const auto line_size{ input_count + output_count };
		mLinesCount = data_count / line_size;

		mInputData.resize(mLinesCount * input_count);
		mOutputData.resize(mLinesCount * output_count);

		const auto value{ reinterpret_cast<const value_type *>(raw_data.data()) };
		for(uint32_t id{}; id < data_count; ++id) {
			if ((id % line_size) < input_count) {
				// std::cout << "when id = " << id  << "\t";
				auto &input = mInputData.at(id / input_count / line_size);
				if (input.capacity() == 0) {
					input.reserve(input_count);
				}
				input.emplace_back(value[id]);
			} else {
				auto &output = mOutputData.at(id / output_count / line_size);
				if (output.capacity() == 0) {
					output.reserve(output_count);
				}
				output.emplace_back(value[id]);
			}
		}
*/
