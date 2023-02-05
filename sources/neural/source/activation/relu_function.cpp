#include "neural/activation/relu_function.hpp"

namespace golxzn::neural::activation {

ReLUFunction::ReLUFunction() noexcept : IFunction{ std::string{ type } } {}

core::f32 ReLUFunction::execute(core::f32 x) const noexcept {
	using namespace core::types_literals;
	return x > 0.0_f32 ? x : 0.0_f32;
}

core::f32 ReLUFunction::derivative(core::f32 x) const noexcept {
	using namespace core::types_literals;
	return x > 0.0_f32 ? 1.0_f32 : 0.0_f32;
}

} // namespace golxzn::neural::activation
