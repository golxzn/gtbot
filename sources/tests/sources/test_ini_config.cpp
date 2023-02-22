#include <core/common>
#include <core/config/ini.hpp>
#include <core/resources/manager.hpp>
#include <iostream>

#include <gtest/gtest.h>

TEST(ini_config, basic) {
	using namespace golxzn::core;
	using namespace golxzn::types_literals;

	static constexpr std::string_view filename{ "res://assets/tests/basic_test.ini" };

	const auto content{ resources::manager::load_string(filename) };
	if (content.empty()) {
		spdlog::error("Something wrong with resource manager or file {}", filename);
	}

	const auto ini{ config::parser::parse(content) };
	EXPECT_TRUE(ini.has("types checks"));
	EXPECT_TRUE(ini.has("types checks", "string"));
	EXPECT_TRUE(ini.has("types checks", "integer"));
	EXPECT_TRUE(ini.has("types checks", "real"));
	EXPECT_TRUE(ini.has("types checks", "boolean"));

	EXPECT_TRUE(ini.has("types checks", "string"));
	EXPECT_TRUE(ini.has("types checks", "integer"));
	EXPECT_TRUE(ini.has("types checks", "real"));
	EXPECT_TRUE(ini.has("types checks", "boolean"));

	EXPECT_EQ(ini.at("").at("unsegmented").type(), config::value_type::string);
	EXPECT_EQ(ini.at("types checks").at("string").type(), config::value_type::string);
	EXPECT_EQ(ini.at("types checks").at("integer").type(), config::value_type::integer);
	EXPECT_EQ(ini.at("types checks").at("real").type(), config::value_type::real);
	EXPECT_EQ(ini.at("types checks").at("boolean").type(), config::value_type::boolean);

	EXPECT_EQ(ini.at("").at("unsegmented").as<std::string>().value(), "value");
	EXPECT_EQ(ini.at("types checks").at("string").as<std::string>().value(), "value1");
	EXPECT_EQ(ini.at("types checks").at("integer").as<i32>().value(), 42_i32);
	EXPECT_EQ(ini.at("types checks").at("real").as<f32>().value(), 3.14_f32);
	EXPECT_EQ(ini.at("types checks").at("boolean").as<bool>().value(), true);
	EXPECT_EQ(ini.at("types checks").at("boolean").comment(), "This is another comment");

	const std::string inistr{ config::writer::write(ini) };
	std::cout << inistr << "\n";
}
