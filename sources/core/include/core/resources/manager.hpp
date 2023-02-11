#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <core/aliases.hpp>

namespace golxzn::core::resources {

class Manager {
	static constexpr std::string_view class_name{ "resources::Manager" };
	static constexpr std::string_view url_separator{ "//" };

	using LoadHandler = std::function<std::vector<byte>(const std::string_view path)>;
	using SaveHandler = std::function<bool(const std::string_view path, const std::vector<byte>)>;
public:
	static constexpr std::string_view DefaultAssetsDirectory{ "assets" };
	static constexpr std::string_view DefaultAppName{ "gtbot" };

	static constexpr std::string_view ResourcesURL{ "res://" };
	static constexpr std::string_view UserURL{ "user://" };
	static constexpr std::string_view HttpURL{ "http://" };

	static void initialize(const std::string_view application_name = DefaultAppName,
		const std::string_view assets_directory_name = DefaultAssetsDirectory);

	nodis static std::vector<byte> load_file(const std::string_view path);

	static bool save_file(const std::string_view path, const std::vector<byte> &data);
	static bool append_file(const std::string_view path, const std::vector<byte> &data);

private:
	static fs::path assets_root;
	static fs::path user_root;
	static const umap<std::string_view, LoadHandler> load_handlers;
	static const umap<std::string_view, SaveHandler> save_handlers;
	static const umap<std::string_view, SaveHandler> append_handlers;

	static std::vector<byte> load_from(const fs::path &path);
	static std::vector<byte> load_from_http(const fs::path &path);

	static bool save_to(const fs::path &path, const std::vector<byte> &data);
	static bool save_to_http(const fs::path &path, const std::vector<byte> &data);
	static bool append_to(const fs::path &path, const std::vector<byte> &data);

	static fs::path build_path(const fs::path &prefix, const std::string_view path,
		const std::string_view prefix_to_replace);

	static void setup_assets_root(const std::string_view assets_name);
	static void setup_user_root(const std::string_view app_name);
};

} // namespace golxzn::core::resources
