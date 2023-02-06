#pragma once

#include <core/aliases.hpp>

namespace golxzn::neural::constants {

using namespace core::types_literals;

static constexpr core::f32 default_weight{};
static constexpr core::f32 default_shift{};
static constexpr core::f32 learning_rate{ 0.5_f32 };

static constexpr core::f32 min_weight{ 0.01_f32 };
static constexpr core::f32 max_weight{ 0.99_f32 };

static constexpr core::u32 default_neuron_count{ 3_u32 };

} // namespace golxzn::neural::constants
