#pragma once

#include <core/aliases.hpp>

namespace golxzn::neural {

class Layer {
public:
	enum class Type : core::u8 {
		Input,
		Hidden,
		Output
	};

	[[nodiscard]] constexpr bool is(const Type type) const noexcept { return mType == type; }

private:
	Type mType{ Type::Hidden };
};

} // namespace golxzn::neural
