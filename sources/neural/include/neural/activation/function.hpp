#pragma once

#include <core/aliases.hpp>
#include <string>
#include <string_view>

namespace golxzn::neural::activation {

class IFunction {
public:
	explicit IFunction(const std::string &type) noexcept;
	nodis const std::string &get_type() const noexcept;

	virtual ~IFunction() noexcept = default;

	nodis virtual core::f32 execute(core::f32 x) const noexcept = 0;
	nodis virtual core::f32 derivative(core::f32 x) const noexcept = 0;

	nodis bool is(const std::string_view type) const noexcept;
	nodis bool operator==(const std::string_view type) const noexcept;

	/** @brief Normal call of the function */
	nodis core::f32 operator()(core::f32 x) const noexcept;

	/** @brief Derivative of the function.  */
	nodis core::f32 operator[](core::f32 x) const noexcept;

private:
	const std::string mType;
};

} // namespace golxzn::neural::activation
