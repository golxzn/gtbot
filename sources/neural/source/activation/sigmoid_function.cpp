#include "neural/activation/sigmoid_function.hpp"

#include <cmath>

namespace golxzn::neural::activation {

SigmoidFunction::SigmoidFunction() noexcept : IFunction{ std::string{ type } } {}

core::f32 SigmoidFunction::execute(core::f32 x) const noexcept {
	using namespace core::types_literals;
	return 1.0_f32 / (1.0_f32 + std::exp(-x));
}

core::f32 SigmoidFunction::derivative(core::f32 x) const noexcept {
	using namespace core::types_literals;
	return execute(x) * (1.0_f32 - execute(x));
}

} // namespace golxzn::neural::activation
