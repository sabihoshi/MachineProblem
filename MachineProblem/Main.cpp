#include <iostream>
#include <Windows.h>
#pragma push_macro("max", "RGB")
#undef max
#undef RGB
#include "Console.h"
#include "Rental.h"
#pragma pop_macro("max", "RGB")

#define MoviePath "Movies.json"
#define CustomerPath "Customers.json"

int Menu();

int main()
{
	SetConsoleOutputCP(65001);

	Rental rental;
	rental.LoadMovies(MoviePath);
	rental.LoadCustomers(CustomerPath);

	int choice;
	do
	{
		choice = Menu();
		switch (choice)
		{
			case 1:
				rental.InsertMovieMenu();
				break;
			case 2:
				rental.RentMovieMenu();
				break;
			case 3:
				rental.ReturnMovieMenu();
				break;
			case 4:
				rental.PrintMovieMenu();
				break;
			case 5:
				rental.PrintAllMoviesMenu();
				break;
			case 6:
				rental.CheckMovieAvailabilityMenu();
				break;
			case 7:
				rental.CustomerMaintenanceMenu();
				break;
			case 8:
				break;
			default: std::cout << "Invalid choice. Please try again." << std::endl;
				break;
		}
		std::cout << "Press enter to continue...";
		std::string pause;
		std::getline(std::cin, pause);
	}
	while (choice != 8);

	rental.SaveMovies(MoviePath);
	rental.SaveCustomers(CustomerPath);
}

int Menu()
{
	Clear();
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
	WriteLine("║           [" + RGB("1", number) + "] Insert a Movie                ║");
	WriteLine("║           [" + RGB("2", number) + "] Rent a Movie                  ║");
	WriteLine("║           [" + RGB("3", number) + "] Return a Movie                ║");
	WriteLine("║           [" + RGB("4", number) + "] Show Movie Details            ║");
	WriteLine("║           [" + RGB("5", number) + "] Movie List                    ║");
	WriteLine("║           [" + RGB("6", number) + "] Check Movie Availability      ║");
	WriteLine("║           [" + RGB("7", number) + "] Customer Maintenance          ║");
	WriteLine("║           [" + RGB("8", number) + "] Exit Program                  ║");
	WriteLine("║                                             ║");
	WriteLine("╠═════════════════════════════════════════════╣");
	WriteLine("║                                             ║");
	WriteLine("╚═════════════════════════════════════════════╝");

	XY(39, 21);
	return Prompt<int>(RGB("Enter choice", input));
}
