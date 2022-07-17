#pragma once
#include "json.hpp"
struct Rent
{
	int CustomerId;
	int VideoId;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rent, CustomerId, VideoId)
};
