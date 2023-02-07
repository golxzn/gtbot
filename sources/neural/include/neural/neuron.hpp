#pragma once

#include <core/types/id.hpp>
#include "activation/function.hpp"

namespace golxzn::neural {

class Layer;
class Edge;

class Neuron final : public std::enable_shared_from_this<Neuron> {
public:

	Neuron(core::id::type id, core::sptr<Layer> layer, core::sptr<activation::IFunction> function,
		bool is_bias = false) noexcept;

	template<class Function, std::enable_if_t<std::is_base_of_v<activation::IFunction, Function>, bool> = false>
	Neuron(core::id::type id, core::sptr<Layer> layer, bool is_bias = false) noexcept
		: Neuron{ id, layer, std::make_shared<Function>(), is_bias } {}

	nodis bool valid() const noexcept;
	nodis bool is_bias() const noexcept;

	nodis core::id::type id() const noexcept;
	nodis core::f32 in() const noexcept;
	nodis core::f32 out() const noexcept;
	nodis core::f32 out_raw() const noexcept;
	nodis core::f32 out_derivative() const noexcept;
	nodis core::f32 threshold() const noexcept;

	nodis core::sptr<Layer> layer() const noexcept;
	nodis std::vector<core::f32> weights() const noexcept;
	nodis const std::vector<core::sptr<Edge>> &edges() const noexcept;

	void clean() noexcept;

	void set_accumulate(const core::f32 value) noexcept;
	void accumulate(const core::f32 value) noexcept;
	void connect(core::sptr<Neuron> next) noexcept;
	void add_previous_edge(core::sptr<Edge> edge) noexcept;

	void randomize(const core::f32 min, const core::f32 max);
	void randomize(const core::f32 range);

	void trigger();

	void shift_weights(const core::f32 min, const core::f32 max);
	void shift_weights(const core::f32 range);

	void alter_weights(const std::vector<core::f32> &values);
	void shift_back_weights(const std::vector<core::f32> &values);

	nodis std::vector<core::f32> get_back_propagation_shifts(const std::vector<core::f32> &target_values);

private:
	core::id::type mID{};
	core::f32 mAccumulated{};
	core::f32 mThreshold{};
	bool mBias{ false };

	core::sptr<Layer> mLayer;
	core::sptr<activation::IFunction> mActivationFunction;
	std::vector<core::sptr<Edge>> mNextEdges;
	std::vector<core::sptr<Edge>> mPreviousEdges;

	core::f32 make_back_propagated(const std::vector<core::f32> &target_values) const;
};

} // namespace golxzn::neural
