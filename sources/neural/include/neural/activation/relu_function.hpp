#pragma once

#include "neural/activation/function.hpp"
#include <string_view>

namespace golxzn::neural::activation {

/** @brief Rectified Linear Unit function implementation. */
class ReLUFunction final : public IFunction {
public:
	static constexpr std::string_view type{ "relu" };
	ReLUFunction() noexcept;

	core::f32 execute(core::f32 x) const noexcept override;
	core::f32 derivative(core::f32 x) const noexcept override;
};

} // namespace golxzn::neural::activation
