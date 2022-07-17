#include "Rental.h"

#include <fstream>
#include <iostream>

#include "Console.h"
#include "json.hpp"

void Box(Coordinates coord, const std::string& title, int length = 10)
{
	Clear();
	XY(coord);
	std::string strips = RGB("• • • • • • • • • • • • • •", {255, 255, 0});
	WriteLine("╔════════════════════════════════════════════════════════════════════════════╗");
	WriteLine("║ " + strips + Center(title, 20) + strips + " ║");
	WriteLine("╠════════════════════════════════════════════════════════════════════════════╣");

	for (int i = 0; i < length; ++i)
	{
		WriteLine("║                                                                            ║");
	}
	WriteLine("╚════════════════════════════════════════════════════════════════════════════╝");
	XY(coord.X + 2, coord.Y + 3);
}

void Rental::LoadCustomers(const std::string& path)
{
	nlohmann::json json;

	std::ifstream file(path);
	file >> json;

	_customers = json.get<std::deque<Customer>>();
}

void Rental::LoadMovies(const std::string& path)
{
	nlohmann::json json;

	std::ifstream file(path);
	file >> json;

	_movies = json.get<std::list<Movie>>();
}

void Rental::SaveCustomers(const std::string& path)
{
	const nlohmann::json json = _customers;
	std::ofstream file(path);
	file << json.dump(4);
}

void Rental::SaveMovies(const std::string& path)
{
	const nlohmann::json json = _movies;
	std::ofstream file(path);
	file << json.dump(4);
}

void Rental::InsertMovie(const Movie& movie)
{
	for (auto& v : _movies)
	{
		if (v == movie)
		{
			WriteLine("Movie already exists!");
			return;
		}
	}

	_movies.push_back(movie);
}

void Rental::RentMovie(Customer& customer, const int movieId)
{
	if (const auto video = GetMovie(movieId))
	{
		if (video->CanBeRented())
		{
			customer.RentMovie(movieId);
			video->RentVideo();
			WriteLine("Movie rented!");
		}
		else
		{
			WriteLine("Movie is not available!");
		}
	}
	else
	{
		WriteLine("Movie not found!");
	}
}

void Rental::ReturnMovie(Customer& customer, int movieId)
{
	if (customer.ReturnMovie(movieId))
	{
		const auto video = GetMovie(movieId);
		video->ReturnVideo();
		WriteLine("Movie returned!");
	}
	else
	{
		WriteLine("Movie not found!");
	}
}

void Rental::PrintMoviesRented(Customer& customer)
{
	auto table = Movie::GetTable();
	for (auto& id : customer.RentedVideos)
	{
		auto video = GetMovie(id);
		table.Add(*video);
	}

	table.Print();
}

Movie* Rental::GetMovie(int videoId)
{
	for (auto& v : _movies)
	{
		if (v.Id == videoId)
		{
			return &v;
		}
	}

	return nullptr;
}

Customer* Rental::GetCustomer(int customerId)
{
	for (auto& v : _customers)
	{
		if (v.Id == customerId)
		{
			return &v;
		}
	}

	return nullptr;
}

void Rental::InsertMovieMenu()
{
	Box({7, 1}, "INSERT A MOVIE");

	auto id = _movies.size() + 1;
	auto title = Prompt<std::string>("Enter the movie title");
	auto production = Prompt<std::string>("Enter the production company");
	auto genre = Prompt<std::string>("Enter the movie genre");
	auto image = Prompt<std::string>("Enter the movie image");
	auto copies = Prompt<int>("Enter the number of copies");

	Movie movie(id, title, production, genre, image, copies);
	InsertMovie(movie);
}

void Rental::RentMovieMenu()
{
	Box({7, 1}, "RENT A MOVIE");
	auto movieId = Prompt<int>("Enter movie code: ");
	auto customerId = Prompt<int>("Enter customer code: ");
	auto movie = GetMovie(movieId);
	auto customer = GetCustomer(customerId);
	if (movie && customer)
		RentMovie(*customer, movieId);
	else
		std::cout << "Movie or customer not found!" << std::endl;
}

void Rental::ReturnMovieMenu()
{
	Box({7, 1}, "RETURN A MOVIE");
	auto movieId = Prompt<int>("Enter movie code: ");
	auto customerId = Prompt<int>("Enter customer code: ");
	auto movie = GetMovie(movieId);
	auto customer = GetCustomer(customerId);
	if (movie && customer)
		ReturnMovie(*customer, movieId);
	else
		WriteLine("Movie or customer not found!");
}

void Rental::PrintMoviesRentedMenu() {}

void Rental::PrintMovieMenu()
{
	Clear();
	Box({7, 1}, "SHOW MOVIE DETAILS");
	auto movieId = Prompt<int>("Enter movie code: ");
	XY(35, 2);
	auto table = Movie::GetTable();
	auto movie = GetMovie(movieId);
	if (movie)
	{
		table.Add(*movie);
		table.Print();
	}
	else
	{
		WriteLine("Movie not found!");
	}
}

void Rental::PrintAllMoviesMenu()
{
	Clear();
	XY(35, 2);
	auto table = Movie::GetTable();
	for (auto& v : _movies)
	{
		table.Add(v);
	}
	table.Print();
}

void Rental::CheckMovieAvailabilityMenu()
{
	Clear();
	Box({7, 1}, "Check Movie Availability");
	auto movieId = Prompt<int>("Enter movie code");
	auto movie = GetMovie(movieId);
	if (movie && movie->CanBeRented())
	{
		WriteLine("Movie is available!");
	}
	else
	{
		WriteLine("Movie is not available!");
	}
}

void Rental::CustomerMaintenanceMenu()
{
	Clear();
	Box({7, 1}, "CUSTOMER MAINTENANCE");
	constexpr Color number = {255, 255, 0};
	WriteLine("[" + RGB("1", number) + "] Add New Customer");
	WriteLine("[" + RGB("2", number) + "] Show Customer Details");
	WriteLine("[" + RGB("3", number) + "] List of Videos Rented");
	auto choice = Prompt<int>("Enter choice");
	switch (choice)
	{
		case 1:
			AddCustomerMenu();
			break;
		case 2:
			ShowCustomerDetailsMenu();
			break;
		case 3:
			ListVideosRentedMenu();
			break;
		default:
			WriteLine("Invalid choice");
			break;
	}
}

void Rental::AddCustomerMenu()
{
	Clear();
	Box({7, 1}, "ADD A CUSTOMER");
	auto id = _customers.size() + 1;
	auto name = Prompt<std::string>("Enter customer name");
	auto address = Prompt<std::string>("Enter customer address");
	Customer customer(id, name, address);
	_customers.push_back(customer);
	WriteLine("Customer added!");
}

void Rental::ShowCustomerDetailsMenu()
{
	Clear();
	Box({7, 1}, "SHOW CUSTOMER DETAILS");
	auto id = Prompt<int>("Enter customer code: ");
	auto customer = GetCustomer(id);
	if (customer)
	{
		auto table = Customer::GetTable();
		table.Add(*customer);
		table.Print();
	}
	else
	{
		WriteLine("Customer not found!");
	}
}

void Rental::ListVideosRentedMenu()
{
	Clear();
	Box({7, 1}, "LIST OF VIDEOS RENTED");
	auto id = Prompt<int>("Enter customer code: ");
	auto customer = GetCustomer(id);
	if (customer)
	{
		PrintMoviesRented(*customer);
	}
	else
	{
		WriteLine("Customer not found!");
	}
}
