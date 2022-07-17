#pragma once
#include <list>

#include "json.hpp"
#include "Movie.h"

class Customer
{
private:
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Customer, Id, Name, Address, RentedVideos)

public:

	Customer();
	Customer(int id, std::string name, std::string address);

	int Id = 0;
	std::string Name;
	std::string Address;

	std::list<int> RentedVideos;

	bool RentMovie(int movieId);

	bool ReturnMovie(int movieId);

	static Table<Customer, int, std::string, std::string> GetTable();
};
