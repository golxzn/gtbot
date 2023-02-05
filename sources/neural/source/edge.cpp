#include "neural/edge.hpp"
#include "neural/neuron.hpp"
#include "neural/constants.hpp"

namespace golxzn::neural {

Edge::Edge(core::sptr<Neuron> previous, core::sptr<Neuron> next, core::f32 weight) noexcept
	: mWeight{ weight }, mPrevious{ previous }, mNext{ next } {}

core::f32 Edge::weight() const noexcept { return mWeight; }
core::f32 Edge::last_shift() const noexcept { return mLastShift; }
core::f32 Edge::back_propagated() const noexcept { return mBackPropagated; }

core::sptr<Neuron> Edge::previous() const noexcept { return mPrevious.lock(); }
core::sptr<Neuron> Edge::next() const noexcept { return mNext.lock(); }

void Edge::propagate(const core::f32 neuron_out) noexcept {
	if (const auto neuron{ next() }; neuron != nullptr) [[likely]]
		neuron->accumulate(neuron_out * weight());
}
void Edge::alter_weight(const core::f32 weight) noexcept { mWeight = weight; }
void Edge::shift_weight(const core::f32 shift) noexcept {
	mWeight += shift * constants::learning_rate;
	mLastShift = shift;
}

void Edge::set_back_propagated(const core::f32 back_propagated) noexcept {
	mBackPropagated = back_propagated;
}

void Edge::reset_shift() noexcept {
	mLastShift = constants::default_shift;
}

} // namespace golxzn::neural
