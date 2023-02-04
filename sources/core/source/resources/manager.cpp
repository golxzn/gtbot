#include "core/common"
#include "core/resources/manager.hpp"

namespace golxzn::core::resources {

fs::path Manager::assets_root{ fs::current_path() };
fs::path Manager::user_root{ fs::path{ sago::getDataHome() } };

const umap<std::string_view, Manager::LoadHandler> Manager::load_handlers{
	{ Manager::ResourcesURL, [](const std::string_view path) { return Manager::load_from(Manager::build_path(Manager::assets_root, path, Manager::ResourcesURL)); } },
	{ Manager::UserURL,      [](const std::string_view path) { return Manager::load_from(Manager::build_path(Manager::user_root, path, Manager::UserURL)); } },
	{ Manager::HttpURL,      [](const std::string_view path) { return Manager::load_from_http(path); } },
};

const umap<std::string_view, Manager::SaveHandler> Manager::save_handlers{
	{ Manager::ResourcesURL, [](const auto path, const auto &data) { return Manager::save_to(Manager::build_path(Manager::assets_root, path, Manager::ResourcesURL), data); } },
	{ Manager::UserURL,      [](const auto path, const auto &data) { return Manager::save_to(Manager::build_path(Manager::user_root, path, Manager::UserURL), data); } },
	{ Manager::HttpURL,      [](const auto path, const auto &data) { return Manager::save_to_http(path, data); } },
};


void Manager::initialize(const std::string_view application_name, const std::string_view assets_directory_name) {
	spdlog::info("[{}] Initializing with {} and {}",
		class_name, application_name, assets_directory_name);
	setup_assets_root(assets_directory_name);
	setup_user_root(application_name);
}

std::vector<byte> Manager::load_file(const std::string_view path) {
	if (path.empty())
		return {};

	if (const auto url_pos{ path.find(url_separator) }; url_pos != path.npos) {
		const auto url{ path.substr(0, url_pos + url_separator.size()) };
		if (const auto &found{ load_handlers.find(url) }; found != std::end(load_handlers)) {
			return found->second(path);
		} else {
			spdlog::error("[{}]: Unknown URL: '{}' in path '{}'", class_name, url, path);
		}
		return {};
	}
	spdlog::error("[{}]: Cannot find URL in path '{}'", class_name, path);
	return {};
}

bool Manager::save_file(const std::string_view path, const std::vector<byte> &data) {
	if (path.empty() || data.empty())
		return false;

	if (const auto url_pos{ path.find(url_separator) }; url_pos != path.npos) {
		const auto url{ path.substr(0, url_pos + url_separator.size()) };
		if (const auto &found{ save_handlers.find(url) }; found != std::end(save_handlers)) {
			return found->second(path, data);
		} else {
			spdlog::error("[{}]: Unknown URL: '{}' in path '{}'", class_name, url, path);
		}
		return false;
	}
	spdlog::error("[{}]: Cannot find URL in path '{}'", class_name, path);
	return false;
}

std::vector<byte> Manager::load_from(const fs::path &path) {
	if (!fs::exists(path) || !fs::is_regular_file(path))
		return {};

	if (fs::bifstream file{ path, std::ios::binary | std::ios::ate }; file.is_open()) {
		const auto size{ file.tellg() };
		file.seekg(std::ios::beg);
		std::vector<byte> content(size, byte{});
		file.read(&content[0], size);
		return content;
	}
	return {};
}
std::vector<byte> Manager::load_from_http(const fs::path &path) {
	spdlog::error("[{}]: load_from_http isn't implemented yet", class_name);
	return {};
}

bool Manager::save_to(const fs::path &path, const std::vector<byte> &data) {
	if (!path.has_filename())
		return false;

	if (const auto parent_path{ path.parent_path() }; !fs::exists(parent_path)) {
		fs::create_directories(parent_path);
	}

	if (fs::bofstream file{ path, std::ios::binary | std::ios::ate }; file.is_open()) {
		file.write(&data[0], data.size());
		return true;
	}

	return false;
}
bool Manager::save_to_http(const fs::path &path, const std::vector<byte> &data) {
	spdlog::error("[{}]: save_to_http isn't implemented yet", class_name);
	return false;
}

fs::path Manager::build_path(const fs::path &prefix, const std::string_view path,
	const std::string_view prefix_to_replace) {
	return prefix / fs::path{ path.substr(prefix_to_replace.size()) }.make_preferred();
}

void Manager::setup_assets_root(const std::string_view assets_name) {
	const fs::path targetDir{ assets_name.empty() ? DefaultAssetsDirectory : assets_name };

	while (!assets_root.empty()) {
		for (const auto &directory : fs::directory_iterator{ assets_root }) {
			if (!fs::is_directory(directory))
				continue;
			const auto current{ directory / targetDir };
			if (fs::exists(current)) {
				assets_root = current;
				spdlog::info("[{}]: Assets directory was found in '{}'",
					class_name, assets_root.string());
				return;
			}
		}
		assets_root = assets_root.parent_path();
	}
	assets_root = fs::current_path() / targetDir;
	spdlog::warn("[{}]: Cannot find '{}' directory. Creating in '{}'",
		class_name, targetDir.string(), assets_root.string());
	fs::create_directories(assets_root);
}

void Manager::setup_user_root(const std::string_view app_name) {
	user_root /= (app_name.empty() ? DefaultAppName : app_name);

	if (!fs::exists(user_root)) {
		fs::create_directories(user_root);
	}

	spdlog::info("[{}]: User directory was found in '{}'", class_name, user_root.string());
}

} // namespace golxzn::core::resources
