#include "neural/activation/linear_function.hpp"

namespace golxzn::neural::activation {
LinearFunction::LinearFunction() noexcept : IFunction{ std::string{ type } } {}

core::f32 LinearFunction::execute(core::f32 x) const noexcept { return x; }
core::f32 LinearFunction::derivative(core::f32 x) const noexcept {
	using namespace core::types_literals;
	return 1.0_f32;
}

} // namespace golxzn::neural::activation
