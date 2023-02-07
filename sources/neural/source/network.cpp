#include <ranges>
#include <algorithm>
#include <execution>

#include "neural/network.hpp"
#include "neural/edge.hpp"
#include "neural/activation/sigmoid_function.hpp"

namespace golxzn::neural {

Network::Network(std::initializer_list<Layer::Settings> &&settings) noexcept {
	mLayers.reserve(settings.size());
	for (auto &&setting : settings)
		add_layer(std::move(setting));
}

void Network::generate_values(const bool run_randomize) noexcept {
	connect_completely();
	if (run_randomize)
		randomize();
}

void Network::add_layer(const Layer::Settings &settings) noexcept {
	mLayers.emplace_back(std::make_shared<Layer>(
		static_cast<core::id::type>(mLayers.size()), shared_from_this(), settings
	));
}
void Network::add_layer(Layer::Settings &&settings) noexcept {
	mLayers.emplace_back(std::make_shared<Layer>(
		static_cast<core::id::type>(mLayers.size()), shared_from_this(), std::move(settings)
	));
}

void Network::clean() {
	std::for_each(std::execution::par, std::begin(mLayers), std::end(mLayers),
		[](auto &&layer) { layer->clean(); }
	);
}

void Network::set_input(const vec_t<core::f32> &values) noexcept {
	if (mLayers.empty()) [[unlikely]] return;
	clean();
	if (auto &layer{ mLayers.front() }; layer != nullptr) [[likely]] {
		layer->set_accumulate(values);
	}
}

void Network::trigger() {
	std::for_each(std::execution::par, std::begin(mLayers), std::end(mLayers), // maybe std::begin(mLayers) + 1
		[](auto &&layer) { layer->trigger(); }
	);
}

Network::vec_t<core::f32> Network::output() const noexcept {
	if (mLayers.empty()) [[unlikely]] return {};

	if (auto &layer{ mLayers.back() }; layer != nullptr) [[likely]] {
		return layer->output();
	}
	return {};
}

void Network::connect_completely() noexcept {
	const auto prev_end{ std::prev(std::end(mLayers)) };

	for (auto it{ std::begin(mLayers) }; it != prev_end; ++it) {
		if (auto &&layer{ *it }; layer != nullptr) [[likely]] {
			layer->connect_completely(*std::next(it));
		}
	}
}

void Network::alter_weights(const three_vec_t<core::f32> &weights) noexcept {
	if (mLayers.empty() || weights.size() != mLayers.size()) [[unlikely]] return;
	using size_type = three_vec_t<core::f32>::size_type;

	std::ranges::for_each(std::views::iota(size_type{}, weights.size()), [&](auto &&index) {
		if (auto &&layer{ mLayers.at(index) }; layer != nullptr) [[likely]] {
			layer->alter_weights(weights.at(index));
		}
	});
}

void Network::shift_back_weights(const three_vec_t<core::f32> &weights) noexcept {
	if (mLayers.empty() || weights.size() != mLayers.size()) [[unlikely]] return;
	using size_type = three_vec_t<core::f32>::size_type;

	std::ranges::for_each(std::views::iota(size_type{}, weights.size()), [&](auto &&index) {
		if (auto &&layer{ mLayers.at(index) }; layer != nullptr) [[likely]] {
			layer->shift_back_weights(weights.at(index));
		}
	});
}

Network::three_vec_t<core::f32> Network::weights() const noexcept {
	if (mLayers.empty()) [[unlikely]] return {};
	three_vec_t<core::f32> result;
	result.reserve(mLayers.size());

	std::ranges::transform(mLayers, std::back_inserter(result), [](auto &&layer) {
		if (layer != nullptr) [[likely]] {
			return layer->weights();
		}
		return double_vec_t<core::f32>{};
	});

	return result;
}

Network::three_vec_t<core::sptr<Edge>> Network::edges() const noexcept {
	if (mLayers.empty()) [[unlikely]] return {};
	three_vec_t<core::sptr<Edge>> result;
	result.reserve(mLayers.size());

	std::ranges::transform(mLayers, std::back_inserter(result), [](auto &&layer) {
		if (layer != nullptr) [[likely]] {
			return layer->edges();
		}
		return double_vec_t<core::sptr<Edge>>{};
	});

	return result;
}

Network::vec_t<core::sptr<Layer>> Network::layers() const noexcept { return mLayers; }

void Network::randomize(const core::f32 min, const core::f32 max) noexcept {
	std::for_each(std::execution::par_unseq, std::begin(mLayers), std::end(mLayers),
		[min, max](auto &&layer) { layer->randomize(min, max); }
	);
}

void Network::randomize(const core::f32 range) noexcept {
	const auto abs_range{ std::abs(range) };
	randomize(-abs_range, abs_range);
}

/*
T inner_product(InputIt1 first1, InputIt1 last1,
                InputIt2 first2, T init,
                BinaryOperation1 op1
                BinaryOperation2 op2)
{
    while (first1 != last1)
    {
         init = op1(std::move(init), op2(*first1, *first2)); // std::move since C++20
         ++first1;
         ++first2;
    }
*/

core::f32 Network::loss(const vec_t<core::f32> &in, const vec_t<core::f32> &out) {
	using namespace core::types_literals;
	if (mLayers.empty() || in.size() != out.size()) [[unlikely]] return 0.0_f32;

	const auto predicted{ predict(in) };
	const core::f32 coeff{ get_loss_coefficient() };
	return std::inner_product( std::begin(out), std::end(out), std::begin(predicted), 0.0_f32,
		std::plus<core::f32>{},
		[coeff](auto lhs, auto rhs) { return coeff * (lhs - rhs) * (lhs - rhs); }
	);
}

core::f32 Network::loss(const double_vec_t<core::f32> &in, const double_vec_t<core::f32> &out) {
	using namespace core::types_literals;
	if (mLayers.empty() || in.size() != out.size()) [[unlikely]] return 0.0_f32;
	return std::inner_product( std::begin(in), std::end(in), std::begin(out), 0.0_f32,
		std::plus<core::f32>{},
		[this](const auto &lhs, const auto &rhs) { return this->loss(lhs, rhs); }
	);
}

void Network::shift_weights(const core::f32 range_percentage) noexcept {
	if (mLayers.empty()) [[unlikely]] return;

	const auto range{ range_percentage * constants::random_max_weight * 2 };
	std::for_each(std::execution::par_unseq, std::begin(mLayers), std::end(mLayers),
		[range](auto &&layer) { layer->shift_weights(range); }
	);
}

Network::vec_t<core::f32> Network::predict(const vec_t<core::f32> &in) noexcept {
	if (mLayers.empty()) [[unlikely]] return {};

	set_input(in);
	trigger();
	return output();
}

core::f32 Network::get_loss_coefficient() const noexcept {
	using namespace core::types_literals;
	if (mLayers.empty()) [[unlikely]] return 1.0_f32;

	if (auto layer{ mLayers.back() }; layer != nullptr) [[likely]] {
		if (auto activation{ layer->activation() }; activation != nullptr) [[likely]] {
			return activation->is(activation::SigmoidFunction::type) ? 0.5_f32 : 1.0_f32;
		}
	}
	return 1.0_f32;
}

} // namespace golxzn::neural

