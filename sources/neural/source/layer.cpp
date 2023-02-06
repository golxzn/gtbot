#include <ranges>
#include <algorithm>
#include <execution>
#include <core/constants.hpp>

#include "neural/layer.hpp"
#include "neural/neuron.hpp"

namespace golxzn::neural {

Layer::Layer(core::id::type id, core::sptr<Network> &network, const Settings &settings) noexcept
	: mID{ id }, mSettings{ settings }, mNetwork{ network } {
	initialize();
}

bool Layer::initialize() {
	using namespace core::types_literals;

	if (mSettings.neuron_count == 0) {
		return false;
	}
	clean();

	const auto not_output{ !is(Type::Output) };
	const auto bias_id{ not_output ? mSettings.neuron_count : core::invalid_id<core::u32>() };
	if (not_output) {
		mSettings.neuron_count += 1; // bias
	}

	mNeurons.reserve(mSettings.neuron_count);
	std::ranges::transform(std::views::iota(0_u32, mSettings.neuron_count),
		std::back_inserter(mNeurons),
		[&](const auto index) {
			return std::make_shared<Neuron>(
				index, shared_from_this(), mSettings.activation, index == bias_id);
		}
	);

	return true;
}

void Layer::clean() {
	mNeurons.clear();
}

core::id::type Layer::id() const noexcept { return mID; }
Layer::Type Layer::type() const noexcept { return mSettings.type; }
core::sptr<Network> Layer::network() const noexcept { return mNetwork.lock(); }
const std::vector<core::sptr<Neuron>> &Layer::neurons() const noexcept { return mNeurons; }
core::u32 Layer::neuron_count() const noexcept { return static_cast<core::u32>(mNeurons.size()); }
core::sptr<Neuron> Layer::neuron(core::id::type id) const noexcept { return mNeurons.at(id); }
core::sptr<activation::IFunction> Layer::activation() const noexcept { return mSettings.activation; }
bool Layer::is(const Type type) const noexcept { return mSettings.type == type; }

Layer::dvec_t<core::f32> Layer::weights() const noexcept {
	if (mNeurons.empty()) [[unlikely]] return {};

	dvec_t<core::f32> weights;
	weights.reserve(mNeurons.size());
	std::transform(std::begin(mNeurons), std::end(mNeurons), std::back_inserter(weights),
		[](const auto &neuron) { return neuron->weights(); }
	);

	return weights;
}

Layer::dvec_t<core::f32> Layer::back_propagation_shifts(
	const std::vector<core::f32> &target_values) const noexcept {

	if (mNeurons.empty()) [[unlikely]] return {};

	dvec_t<core::f32> shifts;
	shifts.reserve(mNeurons.size());
	std::transform(std::begin(mNeurons), std::end(mNeurons), std::back_inserter(shifts),
		[&target_values](const auto &neuron) {
			return neuron->get_back_propagation_shifts(target_values);
		}
	);

	return shifts;
}

Layer::dvec_t<core::sptr<Edge>> Layer::edges() const noexcept {
	if (mNeurons.empty()) [[unlikely]] return {};

	dvec_t<core::sptr<Edge>> edges;
	edges.reserve(mNeurons.size());
	std::transform(std::begin(mNeurons), std::end(mNeurons), std::back_inserter(edges),
		[](const auto &neuron) { return neuron->edges(); }
	);

	return edges;
}

std::vector<core::f32> Layer::output() const noexcept {
	if (mNeurons.empty()) [[unlikely]] return {};

	std::vector<core::f32> output_values;
	output_values.reserve(mNeurons.size());
	std::transform(std::begin(mNeurons), std::end(mNeurons), std::back_inserter(output_values),
		[](const auto &neuron) { return neuron->out(); }
	);

	return output_values;
}

void Layer::trigger() {
	std::for_each(std::execution::par_unseq, std::begin(mNeurons), std::end(mNeurons),
		[](auto &neuron) {
			if (neuron == nullptr) [[unlikely]] return;
			neuron->trigger();
		}
	);
}

void Layer::connect_completely(const core::sptr<Layer> &layer) {
	if (mNeurons.empty() || layer == nullptr) [[unlikely]] return;

	std::for_each(std::execution::par_unseq, std::begin(mNeurons), std::end(mNeurons),
		[&](auto &neuron) {
			if (neuron == nullptr) [[unlikely]] return;
			std::ranges::for_each(layer->mNeurons, [&neuron](auto &other) {
				if (other == nullptr || other->is_bias()) [[unlikely]] return;
				neuron->connect(other);
			});
		}
	);
}

void Layer::alter_weights(const dvec_t<core::f32> &weights) {
	if (mNeurons.empty() || weights.empty()) [[unlikely]] return;

	std::ranges::for_each(std::views::iota(dvec_t<core::f32>::size_type{}, weights.size()),
		[&weights, this](auto index) {
			if (auto &neuron{ mNeurons.at(index) }; neuron != nullptr) [[likely]] {
				neuron->alter_weights(weights.at(index));
			}
		}
	);
}

void Layer::shift_back_weights(const dvec_t<core::f32> &weights) {
	if (mNeurons.empty() || weights.empty()) [[unlikely]] return;

	std::ranges::for_each(std::views::iota(dvec_t<core::f32>::size_type{}, weights.size()),
		[&weights, this](auto index) {
			if (auto &neuron{ mNeurons.at(index) }; neuron != nullptr) [[likely]] {
				neuron->shift_back_weights(weights.at(index));
			}
		}
	);
}

void Layer::shift_weights(const core::f32 factor) {
	if (mNeurons.empty()) [[unlikely]] return;

	std::for_each(std::execution::par_unseq, std::begin(mNeurons), std::end(mNeurons),
		[factor](auto &neuron) {
			if (neuron == nullptr) [[unlikely]] return;
			neuron->shift_weights(factor);
		}
	);
}

} // namespace golxzn::neural
