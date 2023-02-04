#include "core/types/name_holder.hpp"

namespace golxzn::core::types {

NameHolder::NameHolder(const std::string_view name) noexcept : mName{ name } {}

const std::string &NameHolder::name() const noexcept {
	return mName;
}

} // namespace golxzn::core::types
