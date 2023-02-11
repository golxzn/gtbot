#include <core/common>
#include <core/resources/manager.hpp>
#include <neural/dataset.hpp>
#include <gtest/gtest.h>

namespace {

using namespace golxzn::types_literals;

static const std::vector raw_data{
	0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0xF6_b, 0x3F_b, // 1.4  [in]
	0xCD_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xFC_b, 0xBF_b, // -1.8 [in]
	0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0xF0_b, 0x3F_b, // 1.0  [out]

	0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0xF6_b, 0x3F_b, // 1.4  [in]
	0xCD_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xFC_b, 0xBF_b, // -1.8 [in]
	0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0xF0_b, 0x3F_b, // 1.0  [out]

	0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0xF6_b, 0x3F_b, // 1.4  [in]
	0xCD_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xFC_b, 0xBF_b, // -1.8 [in]
	0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0xF0_b, 0x3F_b, // 1.0  [out]

	0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0xF6_b, 0x3F_b, // 1.4  [in]
	0xCD_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xFC_b, 0xBF_b, // -1.8 [in]
	0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0xF0_b, 0x3F_b, // 1.0  [out]

	0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0x66_b, 0xF6_b, 0x3F_b, // 1.4  [in]
	0xCD_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xCC_b, 0xFC_b, 0xBF_b, // -1.8 [in]
	0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0xF0_b, 0x3F_b, // 1.0  [out]
};
static constexpr std::string_view dataset_file{ "res://assets/tests/dataset_test.bin" };
static constexpr std::string_view dataset_with_counts_file{ "res://assets/tests/dataset_test_counts.bin" };

} // anonymous namespace

TEST(DatasetTest, ConstructFromRawData) {
	using namespace golxzn::types_literals;
	using dataset_value_type = golxzn::neural::Dataset::value_type;

	golxzn::neural::Dataset dataset{ raw_data, 2_u32, 1_u32 };
	EXPECT_EQ(dataset.get_input_count(), 2_u32);
	EXPECT_EQ(dataset.get_output_count(), 1_u32);

	const auto &raw_input{ dataset.get_raw_input() };
	EXPECT_EQ(raw_input.size(), 5_u32);
	for (const auto &line : raw_input) {
		EXPECT_EQ(line.size(), 2_u32);
		EXPECT_DOUBLE_EQ(line.at(0), dataset_value_type{ 1.4 });
		EXPECT_DOUBLE_EQ(line.at(1), dataset_value_type{ -1.8 });
	}

	const auto &raw_output{ dataset.get_raw_output() };
	EXPECT_EQ(raw_output.size(), 5_u32);
	for (const auto &line : raw_output) {
		EXPECT_EQ(line.size(), 1_u32);
		EXPECT_DOUBLE_EQ(line.at(0), dataset_value_type{ 1.0 });
	}

	const auto raw{ dataset.raw() };
	EXPECT_EQ(raw.size(), raw_data.size());
	EXPECT_TRUE(std::ranges::equal(raw_data, raw));
}

TEST(DatasetTest, ConstructFromFile) {
	using namespace golxzn::types_literals;
	using dataset_value_type = golxzn::neural::Dataset::value_type;

	golxzn::neural::Dataset dataset{ dataset_file, 2_u32, 1_u32 };
	EXPECT_EQ(dataset.get_input_count(), 2_u32);
	EXPECT_EQ(dataset.get_output_count(), 1_u32);

	const auto &raw_input{ dataset.get_raw_input() };
	EXPECT_EQ(raw_input.size(), 5_u32);
	for (const auto &line : raw_input) {
		EXPECT_EQ(line.size(), 2_u32);
		EXPECT_DOUBLE_EQ(line.at(0), dataset_value_type{ 1.4 });
		EXPECT_DOUBLE_EQ(line.at(1), dataset_value_type{ -1.8 });
	}

	const auto &raw_output{ dataset.get_raw_output() };
	EXPECT_EQ(raw_output.size(), 5_u32);
	for (const auto &line : raw_output) {
		EXPECT_EQ(line.size(), 1_u32);
		EXPECT_DOUBLE_EQ(line.at(0), dataset_value_type{ 1.0 });
	}

	const auto raw{ dataset.raw() };
	EXPECT_EQ(raw.size(), raw_data.size());
	EXPECT_TRUE(std::ranges::equal(raw_data, raw));
}

TEST(DatasetTest, ConstructFromFileWithCounts) {
	using namespace golxzn::types_literals;
	using dataset_value_type = golxzn::neural::Dataset::value_type;

	golxzn::neural::Dataset dataset{ dataset_with_counts_file };
	EXPECT_EQ(dataset.get_input_count(), 2_u32);
	EXPECT_EQ(dataset.get_output_count(), 1_u32);

	const auto &raw_input{ dataset.get_raw_input() };
	EXPECT_EQ(raw_input.size(), 5_u32);
	for (const auto &line : raw_input) {
		EXPECT_EQ(line.size(), 2_u32);
		EXPECT_DOUBLE_EQ(line.at(0), dataset_value_type{ 1.4 });
		EXPECT_DOUBLE_EQ(line.at(1), dataset_value_type{ -1.8 });
	}

	const auto &raw_output{ dataset.get_raw_output() };
	EXPECT_EQ(raw_output.size(), 5_u32);
	for (const auto &line : raw_output) {
		EXPECT_EQ(line.size(), 1_u32);
		EXPECT_DOUBLE_EQ(line.at(0), dataset_value_type{ 1.0 });
	}

	const auto raw{ dataset.raw() };
	EXPECT_EQ(raw.size(), raw_data.size());
	EXPECT_TRUE(std::ranges::equal(raw_data, raw));
}

TEST(DatasetTest, AppendAndErase) {
	using namespace golxzn::types_literals;
	using dataset_value_type = golxzn::neural::Dataset::value_type;

	golxzn::neural::Dataset dataset{};
	dataset
		.append({ 1.4_f32, -1.8_f32 }, { 1.1_f32, 0.5_f32 })
		.append({ 2.4_f32, -1.8_f32 }, { 2.1_f32, 0.5_f32 })
		.append({ 3.4_f32, -1.8_f32 }, { 3.1_f32, 0.5_f32 });

	EXPECT_EQ(dataset.get_input_count(), 2_u32);
	EXPECT_EQ(dataset.get_output_count(), 2_u32);

	const auto raw_input{ dataset.get_raw_input() };
	EXPECT_EQ(raw_input.size(), 3_u32);
	dataset_value_type increaser{};
	for (const auto &line : raw_input) {
		EXPECT_EQ(line.size(), 2_u32);
		EXPECT_DOUBLE_EQ(line.at(0), dataset_value_type{ 1.4 + increaser });
		EXPECT_DOUBLE_EQ(line.at(1), dataset_value_type{ -1.8 });
		increaser += 1.0_f32;
	}

	const auto raw_output{ dataset.get_raw_output() };
	EXPECT_EQ(raw_output.size(), 3_u32);
	increaser = 0.0_f32;
	for (const auto &line : raw_output) {
		EXPECT_EQ(line.size(), 2_u32);
		EXPECT_DOUBLE_EQ(line.at(0), dataset_value_type{ 1.1 + increaser });
		EXPECT_DOUBLE_EQ(line.at(1), dataset_value_type{ 0.5 });
		increaser += 1.0_f32;
	}

	dataset.erase(1_u32);

	const auto modified_raw_input{ dataset.get_raw_input() };
	EXPECT_EQ(modified_raw_input.size(), 2_u32);

	const auto modified_raw_output{ dataset.get_raw_output() };
	EXPECT_EQ(modified_raw_output.size(), 2_u32);
}
