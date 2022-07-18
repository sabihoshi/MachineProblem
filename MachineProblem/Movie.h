#pragma once
#include <string>

#include "json.hpp"
#include "Table.h"
#include "VariadicTable.h"

class Movie
{
private:
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Movie, Id, Copies, Rents, Title, Production, Genre, Image)

public:
	int Id = 0;
	int Copies = 0;
	int Rents = 0;
	std::string Title;
	std::string Production;
	std::string Genre;
	std::string Image;

	Movie();

	Movie(int id, std::string title, std::string production, std::string genre, std::string image, int copies);

	void RentMovie();

	void ReturnMovie();

	bool operator==(const Movie& other) const;

	bool CanBeRented() const;

	static Table<Movie, int, std::string, std::string, std::string, int, int, std::string> GetTable();
};
