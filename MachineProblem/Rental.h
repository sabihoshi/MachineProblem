#pragma once
#include <list>
#include <queue>

#include "Movie.h"
#include "Customer.h"

class Rental
{
private:
	std::list<Movie> _movies;
	std::deque<Customer> _customers;

	bool InsertMovie(const Movie& movie);

	void RentMovie(Customer& customer, int movieId);

	void ReturnMovie(Customer& customer, int movieId);

	void PrintMoviesRented(Customer& customer);

	Customer* GetCustomer(int customerId);

	Movie* GetMovie(int videoId);

	void AddCustomerMenu();

	void ShowCustomerDetailsMenu();

	void ListVideosRentedMenu();

public:
	void InsertMovieMenu();

	void RentMovieMenu();

	void ReturnMovieMenu();

	void PrintMoviesRentedMenu();

	void PrintMovieMenu();

	void PrintAllMoviesMenu();

	void CheckMovieAvailabilityMenu();

	void CustomerMaintenanceMenu();

	void LoadMovies(const std::string& path);

	void LoadCustomers(const std::string& path);

	void SaveMovies(const std::string& path);

	void SaveCustomers(const std::string& path);
};
