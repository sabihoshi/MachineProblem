#include "Rental.h"

#include <fstream>
#include <iostream>

#include "Console.h"
#include "json.hpp"

void Box(Coordinates coord, const std::string& title, int length = 3)
{
	Clear();
	XY(coord);
	std::string strips = RGB("• • • • • • • • • • •", {255, 255, 0});
	WriteLine("╔════════════════════════════════════════════════════════════════╗");
	WriteLine("║ " + strips + Center(title, 20) + strips + " ║");
	WriteLine("╠═══════════════════════╦════════════════════════════════════════╣");

	for (int i = 0; i < length; ++i)
	{
                WriteLine("║                       ║                                        ║");
                if (i == length - 1) continue;
                WriteLine("╠═══════════════════════╬════════════════════════════════════════╣");
	}
	WriteLine("╚═══════════════════════╩════════════════════════════════════════╝");
	XY(coord.X + 2, coord.Y + 3);
}

void CleanBox(Coordinates coord, const std::string& title, int length = 1)
{
        Clear();
        XY(coord);
        std::string strips = RGB("• • • • • • • • • • •", {255, 255, 0});
        WriteLine("╔════════════════════════════════════════════════════════════════╗");
        WriteLine("║ " + strips + Center(title, 20) + strips + " ║");
        WriteLine("╠════════════════════════════════════════════════════════════════╣");

        for (int i = 0; i < length; ++i)
        {
          WriteLine("║                                                                ║");
        }
        WriteLine("╚════════════════════════════════════════════════════════════════╝");
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

bool Rental::InsertMovie(const Movie& movie)
{
	for (auto& v : _movies)
	{
		if (v == movie)
		{
			WriteLine("Movie already exists!");
			return false;
		}
	}

	_movies.push_back(movie);
	return true;
}

void Rental::RentMovie(Customer& customer, const int movieId)
{
	if (const auto movie = GetMovie(movieId))
	{
		if (movie->CanBeRented())
		{
			customer.RentMovie(movieId);
			movie->RentMovie();
			WriteLine("Movie rented!");

			auto table = Movie::GetTable();
			table.Add(*movie);
			table.Print();
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
		const auto movie = GetMovie(movieId);
		movie->ReturnMovie();
		WriteLine("Movie returned!");

		auto table = Movie::GetTable();
		table.Add(*movie);
		table.Print();
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
	Box({27, 1}, "INSERT A MOVIE", 6);
          Coordinates coord = {29, 4};
          WriteLine("Video ID", 2);
          WriteLine("Movie Title", 2);
          WriteLine("Production", 2);
          WriteLine("Genre", 2);
          WriteLine("Movie Image Filename", 2);
          WriteLine("Number of Copies");
          XY(coord); MoveCursor(CursorDirection::Right, 24);

	auto id = _movies.size() + 1;
	WriteLine(std::to_string(_movies.size() + 1), 2);
	auto title = Prompt<std::string>("", 2);
	auto production = Prompt<std::string>("", 2);
	auto genre = Prompt<std::string>("", 2);
	auto image = Prompt<std::string>("", 2);
	auto copies = Prompt<int>("", 2);
	MoveCursor(CursorDirection::Left, 26);

	Movie movie(id, title, production, genre, image, copies);
	if(InsertMovie(movie))
	{
		auto table = Movie::GetTable();
		table.Add(movie);
		table.Print();
	}
}

void Rental::RentMovieMenu()
{
	Box({27, 1}, "RENT A MOVIE", 2);
          Coordinates coord = {29, 4};
          XY(coord);
          WriteLine("Customer ID", 2);
          WriteLine("Movie ID", 2);
          XY(coord); MoveCursor(CursorDirection::Right, 24);

	auto customerId = Prompt<int>("", 2);
	auto movieId = Prompt<int>("", 2);
	MoveCursor(CursorDirection::Left, 26);

	auto customer = GetCustomer(customerId);
	auto movie = GetMovie(movieId);
	if (movie && customer)
		RentMovie(*customer, movieId);
	else
		WriteLine("Movie or customer not found!");
}

void Rental::ReturnMovieMenu()
{
	Box({27, 1}, "RETURN A MOVIE", 2);
          Coordinates coord = {29, 4};
          XY(coord);
          WriteLine("Customer ID", 2);
          WriteLine("Movie ID", 2);
          XY(coord); MoveCursor(CursorDirection::Right, 24);

	auto customerId = Prompt<int>("", 2);
	auto movieId = Prompt<int>("", 2);
	MoveCursor(CursorDirection::Left, 24);

	auto movie = GetMovie(movieId);
	auto customer = GetCustomer(customerId);
	if (movie && customer)
		ReturnMovie(*customer, movieId);
	else
		WriteLine("Movie or customer not found!");
}

void Rental::PrintMovieMenu()
{
      Clear();
        CleanBox({27, 1}, "SHOW MOVIE DETAILS");
        Coordinates coord = {29, 4};
        XY(coord);

        auto movieId = Prompt<int>("Enter movie code ", 2);
        MoveCursor(CursorDirection::Left, 2);

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
	XY(14, 2);
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
	CleanBox({27, 1}, "Check Movie Status");
	auto movieId = Prompt<int>("Enter movie ID", 2);
        MoveCursor(CursorDirection::Left, 2);

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
        XY(50, 11);
        constexpr Color lights = {255, 255, 0};
        constexpr Color movie = {0, 68, 116};
        constexpr Color rent = {45, 200, 255};
        constexpr Color number = {255, 255, 0};
        constexpr Color input = {146, 208, 80};
        const std::string& light = RGB("•", lights);

        XY(38, 0);
        WriteLine("╔═════════════════════════════════════════════╗");
        WriteLine("║ " + RGB("• • • • • • • • • • • • • • • • • • • • • •", lights) + " ║");
        WriteLine("║ " + light + RGB("╔═╗╔═╦═══╦╗──╔╦══╦═══╗", movie) + RGB("╔═══╦═══╦═╗─╔╦════╗", rent) + light + " ║");
        WriteLine("║ " + light + RGB("║║╚╝║║╔═╗║╚╗╔╝╠╣╠╣╔══╝", movie) + RGB("║╔═╗║╔══╣║╚╗║║╔╗╔╗║", rent) + light + " ║");
        WriteLine("║ " + light + RGB("║╔╗╔╗║║─║╠╗║║╔╝║║║╚══╗", movie) + RGB("║╚═╝║╚══╣╔╗╚╝╠╝║║╚╝", rent) + light + " ║");
        WriteLine("║ " + light + RGB("║║║║║║║─║║║╚╝║─║║║╔══╝", movie) + RGB("║╔╗╔╣╔══╣║╚╗║║─║║  ", rent) + light + " ║");
        WriteLine("║ " + light + RGB("║║║║║║╚═╝║╚╗╔╝╔╣╠╣╚══╗", movie) + RGB("║║║╚╣╚══╣║─║║║─║║  ", rent) + light + " ║");
        WriteLine("║ " + light + RGB("╚╝╚╝╚╩═══╝─╚╝─╚══╩═══╝", movie) + RGB("╚╝╚═╩═══╩╝─╚═╝─╚╝  ", rent) + light + " ║");
        WriteLine("║ " + RGB("• • • • • • • • • • • • • • • • • • • • • •", lights) + " ║");
        WriteLine("║                                             ║");
        WriteLine("║    [" + RGB("1", number) + "] Add New Customer                     ║");
        WriteLine("║    [" + RGB("2", number) + "] Show Customer Details                ║");
        WriteLine("║    [" + RGB("3", number) + "] Show Rented Movies of a Customer     ║");
        WriteLine("║                                             ║");
        WriteLine("║                                             ║");
        WriteLine("║                                             ║");
        WriteLine("║                                             ║");
        WriteLine("║                                             ║");
        WriteLine("║                                             ║");
        WriteLine("╠═════════════════════════════════════════════╣");
        WriteLine("║                                             ║");
        WriteLine("╚═════════════════════════════════════════════╝");
        XY(39, 21);

	auto choice = Prompt<int>(RGB("Enter choice", input));
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
	Box({27, 1}, "ADD A CUSTOMER");
        Coordinates coord = {29, 4};
        WriteLine("Customer ID", 2);
        WriteLine("Customer Name", 2);
        WriteLine("Customer Address", 2);
        XY( coord.X + 24, coord.Y);

	auto id = _customers.size() + 1;
	WriteLine(std::to_string(id),2 );
	auto name = Prompt<std::string>("", 2);
	auto address = Prompt<std::string>("", 2);
	MoveCursor(CursorDirection::Left, 26);

	Customer customer(id, name, address);
	_customers.push_back(customer);
	WriteLine("Customer added!");
}

void Rental::ShowCustomerDetailsMenu()
{
      Clear();
	CleanBox({27, 1}, "CUSTOMER DETAILS");
	auto id = Prompt<int>("Enter Customer ID", 2);
        MoveCursor(CursorDirection::Left, 2);
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
	CleanBox({14, 1}, "RENTED VIDEOS LIST");
	auto id = Prompt<int>("Enter customer code: ", 2);
        MoveCursor(CursorDirection::Left, 2);

	auto customer = GetCustomer(id);
	if (customer && !customer->RentedVideos.empty())
	{
		PrintMoviesRented(*customer);
	}
        else if (customer && customer->RentedVideos.empty())
        {
            WriteLine("Customer has no videos rented!");
        }
	else
	{
		WriteLine("Customer not found!");
	}
}
