#pragma

#include <string>
#include <string_view>

namespace golxzn::core::types {

class NameHolder {
public:
	explicit NameHolder(const std::string_view name) noexcept;

	[[nodiscard]] const std::string &name() const noexcept;

private:
	std::string mName;
};

} // namespace golxzn::core::types
