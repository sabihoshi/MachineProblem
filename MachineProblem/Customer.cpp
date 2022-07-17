#include "Customer.h"

#include "Rental.h"

Customer::Customer() = default;

Customer::Customer(int id, std::string name, std::string address)
	: Id(id), Name(std::move(name)), Address(std::move(address)) {}

bool Customer::RentMovie(int movieId)
{
	if (std::find(RentedVideos.begin(), RentedVideos.end(), movieId) != RentedVideos.end())
	{
		return false;
	}

	RentedVideos.push_back(movieId);
	return true;
}

bool Customer::ReturnMovie(int movieId)
{
	// Check if the movie has been rented and remove it
	const auto it = std::find(RentedVideos.begin(), RentedVideos.end(), movieId);
	if (it == RentedVideos.end())
	{
		return false;
	}

	RentedVideos.erase(it);
	return true;
}

Table<Customer, int, std::string, std::string> Customer::GetTable()
{
	return Table<Customer, int, std::string, std::string>(
		{"Id", "Name", "Address"},
		[&](Customer& c) { return std::make_tuple(c.Id, c.Name, c.Address); }
	);
}
