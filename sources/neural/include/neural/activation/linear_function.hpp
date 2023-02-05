#pragma once

#include "neural/activation/function.hpp"

namespace golxzn::neural::activation {

class LinearFunction final : public IFunction {
public:
	static constexpr std::string_view type{ "linear" };
	LinearFunction() noexcept;

	core::f32 execute(core::f32 x) const noexcept override;
	core::f32 derivative(core::f32 x) const noexcept override;
};

} // namespace golxzn::neural::activation
