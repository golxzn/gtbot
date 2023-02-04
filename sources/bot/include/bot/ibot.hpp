#pragma once

#include "bot/ibrain.hpp"

namespace golxzn::bot {

class IBot {
public:
	virtual ~IBot() = default;

	template<class Brains, class... Args>
	IBot(Args&&... args) : mBrain{ std::make_unique<Brains>(std::forward<Args>(args)...) } {}

private:
	core::uptr<IBrain> mBrain;
};

} // namespace golxzn::bot
