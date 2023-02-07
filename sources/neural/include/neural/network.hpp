#pragma once

#include <initializer_list>
#include <neural/layer.hpp>

namespace golxzn::neural {

class Network : std::enable_shared_from_this<Network> {
	template<class T> using vec_t = std::vector<T>;
	template<class T> using double_vec_t = std::vector<std::vector<T>>;
	template<class T> using three_vec_t = std::vector<std::vector<std::vector<T>>>;
public:
	Network() noexcept = default;
	explicit Network(std::initializer_list<Layer::Settings> &&settings) noexcept;

	void generate_values(const bool randomize = true) noexcept;
	void add_layer(const Layer::Settings &settings) noexcept;
	void add_layer(Layer::Settings &&settings) noexcept;

	void clean();

	void set_input(const vec_t<core::f32> &values) noexcept;
	void trigger();
	vec_t<core::f32> output() const noexcept;

	void connect_completely() noexcept;
	void alter_weights(const three_vec_t<core::f32> &weights) noexcept;
	void shift_back_weights(const three_vec_t<core::f32> &weights) noexcept;

	three_vec_t<core::f32> weights() const noexcept;
	three_vec_t<core::sptr<Edge>> edges() const noexcept;
	vec_t<core::sptr<Layer>> layers() const noexcept;

	void randomize(const core::f32 min, const core::f32 max) noexcept;
	void randomize(const core::f32 range = constants::random_max_weight) noexcept;

	core::f32 loss(const vec_t<core::f32> &in, const vec_t<core::f32> &out);
	core::f32 loss(const double_vec_t<core::f32> &in, const double_vec_t<core::f32> &out);

	void shift_weights(const core::f32 range_percentage) noexcept;
	vec_t<core::f32> predict(const vec_t<core::f32> &in) noexcept;

private:
	vec_t<core::sptr<Layer>> mLayers{};

	core::f32 get_loss_coefficient() const noexcept;
};

} // namespace golxzn::neural
