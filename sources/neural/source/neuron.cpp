#include <ranges>
#include <cassert>
#include <algorithm>
#include <execution>
#include <exception>

#include <core/utils/random.hpp>

#include "neural/neuron.hpp"
#include "neural/activation/function.hpp"
#include "neural/layer.hpp"
#include "neural/edge.hpp"
#include "neural/constants.hpp"

namespace golxzn::neural {

bool Neuron::valid() const noexcept { return mLayer != nullptr; }
bool Neuron::is_bias() const noexcept { return mBias; }

core::id::type Neuron::id() const noexcept { return mID; }
core::f32 Neuron::in() const noexcept { return mAccumulated; }
core::f32 Neuron::out() const noexcept {
	using namespace core::types_literals;

	if (mBias) return 1.0_f32;
	else if (!valid() || mLayer->is(Layer::Type::Input)) return out_raw();
	else if (mActivationFunction == nullptr) [[unlikely]] return out_raw();

	auto &function{ *mActivationFunction };
	return function(mAccumulated);
}

core::f32 Neuron::out_raw() const noexcept { return mAccumulated; }

core::f32 Neuron::out_derivative() const noexcept {
	using namespace core::types_literals;
	if (mActivationFunction == nullptr) [[unlikely]] return out_raw();

	auto &function{ *mActivationFunction };
	return function[mAccumulated];
}

core::f32 Neuron::threshold() const noexcept { return mThreshold; }
core::sptr<Layer> Neuron::layer() const noexcept { return mLayer; }

std::vector<core::f32> Neuron::weights() const noexcept {
	std::vector<core::f32> weights;
	weights.reserve(mNextEdges.size());
	std::ranges::transform(mNextEdges, std::back_inserter(weights),
		[](auto &edge) { return edge == nullptr ? constants::default_weight : edge->weight(); });

	return weights;
}

const std::vector<core::sptr<Edge>> &Neuron::edges() const noexcept {
	return mNextEdges;
}

void Neuron::clean() noexcept {
	using namespace core::types_literals;
	mAccumulated = 0.0_f32;
}

void Neuron::accumulate(const core::f32 value) noexcept { mAccumulated += value; }

void Neuron::connect(core::sptr<Neuron> next) noexcept {
	using namespace core::utils;
	if (next == nullptr) [[unlikely]] return;

	const auto weight{ random::range<core::f32>(constants::min_weight, constants::max_weight) };
	auto edge{ std::make_shared<Edge>(shared_from_this(), next, weight) };
	mNextEdges.push_back(edge);
	next->add_previous_edge(edge);
}

void Neuron::add_previous_edge(core::sptr<Edge> edge) noexcept {
	if (edge == nullptr) [[unlikely]] return;
	mPreviousEdges.push_back(edge);
}

void Neuron::randomize(const core::f32 min, core::f32 max) {
	using namespace core::utils;

	assert(min < max && "Min must be smaller than or equal to max");
	std::for_each(std::execution::par, std::begin(mNextEdges), std::end(mNextEdges), [min, max](auto &next) {
		if (next == nullptr) [[unlikely]] return;
		next->alter_weight(random::range<core::f32>(min, max));
	});
}

void Neuron::randomize(const core::f32 value) {
	const auto abs_value{ std::abs(value) };
	randomize(-abs_value, abs_value);
}

void Neuron::shift_weights(const core::f32 min, const core::f32 max) {
	using namespace core::utils;

	assert(min < max && "Min must be smaller than or equal to max");
	std::for_each(std::execution::par, std::begin(mNextEdges), std::end(mNextEdges), [min, max](auto &next) {
		if (next == nullptr) [[unlikely]] return;
		next->alter_weight(next->weight() + random::range<core::f32>(min, max));
	});
}

void Neuron::shift_weights(const core::f32 value) {
	const auto abs_value{ std::abs(value) };
	shift_weights(-abs_value, abs_value);
}

void Neuron::shift_back_weights(const std::vector<core::f32> &range) {
	if (range.size() != mPreviousEdges.size()) [[unlikely]] {
		throw std::invalid_argument{ "Neuron::shift_back_weights - Invalid range size" };
	}

	size_t index{};
	std::ranges::for_each(mPreviousEdges, [&range, &index](auto &edge) {
		if (edge == nullptr) [[unlikely]] return;
		edge->shift_weight(range.at(index++));
	});
}

std::vector<core::f32> Neuron::get_back_propagation_shifts(
	[[maybe_unuse]] const std::vector<core::f32> &target_values) {

	using namespace core::types_literals;

	std::vector<core::f32> shifts;
	shifts.reserve(mPreviousEdges.size());

	const auto prop_value{ make_back_propagated(target_values) };
	std::ranges::transform(mPreviousEdges, std::back_inserter(shifts), [prop_value](auto &edge) {
		if (edge == nullptr) [[unlikely]] return 0.0_f32;
		if (auto previous{ edge->previous() }; previous != nullptr) [[likely]] {
			edge->set_back_propagated(prop_value);
			return -prop_value * previous->out();
		}
		plog::warn("Neuron::get_back_propagation_shifts - Previous neuron is nullptr");
		return 0.0_f32;
	}

	return shifts;
}

core::f32 Neuron::make_back_propagated(const std::vector<core::f32> &target_values) {
	if (mLayer->is(Layer::Type::Output)) {
		return (out() - target_values.at(id())) * out_derivative() ;
	}

	return std::accumulate(std::begin(mNextEdges), std::end(mNextEdges), 0.0_f32,
		[](auto acc, auto &edge) {
			if (edge == nullptr) [[unlikely]] return acc;
			return acc + edge->back_propagated() * edge->weight();
		}) * out_derivative();
}

} // namespace golxzn::neural
