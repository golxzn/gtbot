#include "neural/activation/function.hpp"
#include <cassert>

namespace golxzn::neural::activation {

IFunction::IFunction(const std::string &type) noexcept : mType{ type } {
	assert(!mType.empty() && "Function type must not be empty.");
}

const std::string &IFunction::get_type() const noexcept { return mType; }

bool IFunction::is(const std::string_view type) const noexcept { return mType == type; }
bool IFunction::operator==(const std::string_view type) const noexcept { return mType == type; }
core::f32 IFunction::operator()(core::f32 x) const noexcept { return execute(x); }
core::f32 IFunction::operator[](core::f32 x) const noexcept { return derivative(x); }

} // namespace golxzn::neural::activation
