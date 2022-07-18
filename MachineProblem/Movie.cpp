#include "Movie.h"

#include "Table.h"

Movie::Movie() = default;

Movie::Movie(const int id, std::string title, std::string production, std::string genre, std::string image, int copies):
	Id(id), Copies(copies), Title(std::move(title)),
	Production(std::move(production)), Genre(std::move(genre)),
	Image(std::move(image)) { }

void Movie::RentMovie()
{
	Rents++;
}

void Movie::ReturnMovie()
{
	Rents--;
}

bool Movie::operator==(const Movie& other) const
{
	return Id == other.Id;
}

bool Movie::CanBeRented() const
{
	return Rents < Copies;
}

Table<Movie, int, std::string, std::string, std::string, int, int, std::string> Movie::GetTable()
{
	return Table<Movie, int, std::string, std::string, std::string, int, int, std::string>(
		{"Id", "Title", "Production", "Genre", "Copies", "Rented", "Image"},
		[&](const Movie& v) { return std::make_tuple(v.Id, v.Title, v.Production, v.Genre, v.Copies, v.Rents, v.Image); });
}
