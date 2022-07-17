#pragma once
#include <functional>

#include "VariadicTable.h"

template <class T, class ... Ts>
class Table
{
private:
	VariadicTable<Ts...> _table;
	std::function<std::tuple<Ts...>(T&)> _getters;

public:
	Table(const std::vector<std::string>& headers,
		std::function<std::tuple<Ts...>(T&)> getters,
		unsigned int static_column_size = 0,
		unsigned int cell_padding = 1) :
		_table(headers, static_column_size, cell_padding),
		_getters(std::move(getters)) { }

	void Add(T& add)
	{
		std::tuple<Ts...> params = _getters(add);
		std::apply(&VariadicTable<Ts...>::addRow, std::tuple_cat(std::make_tuple(&_table), params));
	}
	
	void Print()
	{
		_table.print(std::cout);
	}
};
