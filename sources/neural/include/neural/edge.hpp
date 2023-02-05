#pragma once

#include <core/aliases.hpp>

namespace golxzn::neural {

class Neuron;

class Edge final {
public:
	static constexpr core::f32 default_weight{};

	Edge(core::sptr<Neuron> previous, core::sptr<Neuron> next, core::f32 weight = default_weight) noexcept;

	nodis core::f32 weight() const noexcept;
	nodis core::f32 last_shift() const noexcept;
	nodis core::f32 back_propagated() const noexcept;

	nodis core::sptr<Neuron> previous() const noexcept;
	nodis core::sptr<Neuron> next() const noexcept;

	void propagate(const core::f32 neuron_out) noexcept;
	void alter_weight(const core::f32 weight) noexcept;
	void shift_weight(const core::f32 shift) noexcept;
	void set_back_propagated(const core::f32 back_propagated) noexcept;
	void reset_shift() noexcept;

private:
	core::f32 mWeight{};
	core::f32 mLastShift{};
	core::f32 mBackPropagated{};

	core::wptr<Neuron> mPrevious;
	core::wptr<Neuron> mNext;

};

} // namespace golxzn::neural
