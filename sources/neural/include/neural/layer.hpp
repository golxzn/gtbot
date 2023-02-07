#pragma once

#include <core/aliases.hpp>
#include <core/types/id.hpp>

#include "neural/constants.hpp"
#include "activation/function.hpp"

namespace golxzn::neural {

class Edge;
class Neuron;
class Network;

class Layer : std::enable_shared_from_this<Layer> {
	template<class T> using dvec_t = std::vector<std::vector<T>>;
public:
	enum class Type : core::u8 {
		Input,
		Hidden,
		Output
	};
	struct Settings {
		Type type{ Type::Hidden };
		core::u32 neuron_count{ constants::default_neuron_count };
		core::sptr<activation::IFunction> activation{};
	};

	Layer(const core::id::type id, core::sptr<Network> network, const Settings &settings) noexcept;
	Layer(const core::id::type id, core::sptr<Network> network, Settings &&settings) noexcept;

	bool initialize();
	void clean();

	nodis core::id::type id() const noexcept;
	nodis Type type() const noexcept;
	nodis core::sptr<Network> network() const noexcept;
	nodis const std::vector<core::sptr<Neuron>> &neurons() const noexcept;

	nodis core::u32 neuron_count() const noexcept;
	nodis core::sptr<Neuron> neuron(const core::id::type id) const noexcept;
	nodis core::sptr<activation::IFunction> activation() const noexcept;

	nodis bool is(const Type type) const noexcept;

	nodis dvec_t<core::f32> weights() const noexcept;
	nodis dvec_t<core::f32> back_propagation_shifts(const std::vector<core::f32> &target_values) const noexcept;
	nodis dvec_t<core::sptr<Edge>> edges() const noexcept;

	nodis std::vector<core::f32> output() const noexcept;

	void trigger();

	void set_accumulate(const std::vector<core::f32> &value) noexcept;

	void connect_completely(const core::sptr<Layer> &layer);

	void alter_weights(const dvec_t<core::f32> &weights);
	void shift_back_weights(const dvec_t<core::f32> &weights);

	void shift_weights(const core::f32 factor);

	void randomize(const core::f32 min, const core::f32 max);
	void randomize(const core::f32 range);

private:
	core::id::type mID{};
	Settings mSettings{};
	core::wptr<Network> mNetwork{};
	std::vector<core::sptr<Neuron>> mNeurons{};
};

} // namespace golxzn::neural
