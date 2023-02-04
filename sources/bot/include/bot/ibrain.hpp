#pragma once

#include <core/aliases.hpp>
#include <optional>

namespace golxzn::bot {

class IBrain {
public:
	struct ThinkResult {};

	virtual ~IBrain() = default;

	virtual bool memorize() = 0;
	virtual std::optional<ThinkResult> think() = 0;
};

} // namespace golxzn::bot