#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>

struct Coordinates
{
	int X;
	int Y;
};

enum class CursorDirection
{
	Up,
	Down,
	Right,
	Left
};

enum class GroundType
{
	Fore,
	Back
};

struct Color
{
	int R;
	int G;
	int B;
};

const std::string COLOR_RESET = "\033[0m";

/**
 * \brief Returns the encoded string that allows you to change the console color.
 * \param color The Color to change the output to.
 * \param type Specify whether to change the foreground or the background color.
 * \return The encoded string with color.
 */
inline std::string ChangeColor(const Color color, const GroundType type = GroundType::Fore)
{
	if (color.R == -1 && color.G == -1 && color.B == -1)
		return COLOR_RESET;

	const auto r = std::to_string(color.R);
	const auto g = std::to_string(color.G);
	const auto b = std::to_string(color.B);

	switch (type)
	{
		case GroundType::Fore: return "\033[38;2;" + r + ";" + g + ";" + b + "m";
		case GroundType::Back: return "\033[48;2;" + r + ";" + g + ";" + b + "m";
	}
}

/**
 * \brief Returns an encoded string in which the specific given text has a specific color.
 * \param string The text to change the color.
 * \param color The Color to change the text to.
 * \param type Specify whether to change the foreground or the background color.
 * \return The text you provided with the coloring you provided.
 */
inline std::string RGB(const std::string& string, const Color color, const GroundType type = GroundType::Fore)
{
	return ChangeColor(color, type) + string + COLOR_RESET;
}

/**
 * \brief Swaps the colors of the foreground and background around.
 */
inline void SwapColors()
{
	std::cout << "\033[7m";
}

/**
 * \brief Change the colors of the console so that the next outputs will be the specific color.
 * \param color The Color to use.
 * \param type Specify whether to change the foreground or the background color.
 */
inline void SetColor(const Color color, const GroundType type = GroundType::Fore)
{
	std::cout << ChangeColor(color, type);
}

/**
 * \brief Resets the color back to the console defaults.
 */
inline void ResetColor()
{
	std::cout << COLOR_RESET;
}

/**
 * \brief Sets the cursor position based on the coordinates.
 * \param c The coordinate you want to set the cursor to.
 */
inline void XY(const Coordinates c)
{
	std::cout << "\033[" << c.Y << ";" << c.X << "f";
}

inline void Clear()
{
	std::cout << "\033[2J\033[1;1H";
}

inline std::string Repeat(const std::string& string, const int repeats)
{
	std::ostringstream repeated;
	const auto iterator = std::ostream_iterator<std::string>(repeated);
	fill_n(iterator, repeats, string);
	return repeated.str();
}

inline std::string Repeat(const char character, const int repeats)
{
	return Repeat(std::to_string(character), repeats);
}

inline std::string Center(const std::string& string, const int length)
{
	const int left = length / 2 - string.length() / 2;
	const int right = length - left - string.length();
	return Repeat(" ", left) + string + Repeat(" ", right);
}

inline std::string Right(const std::string& string, const int length)
{
	const int repeat = length - string.length();
	return string + Repeat(" ", repeat);
}

inline std::string Left(const std::string& string, const int length)
{
	const int repeat = length - string.length();
	return Repeat(" ", repeat) + string;
}

inline void XY(const int x, const int y)
{
	return XY({x, y});
}

inline void SaveXY()
{
	std::cout << "\033[s";
}

inline void LoadXY()
{
	std::cout << "\033[u";
}

inline void MoveCursor(const CursorDirection direction, const short amount = 1)
{
	switch (direction)
	{
		case CursorDirection::Up: std::cout << "\033[" + std::to_string(amount) + "A";
			break;
		case CursorDirection::Down: std::cout << "\033[" + std::to_string(amount) + "B";
			break;
		case CursorDirection::Right: std::cout << "\033[" + std::to_string(amount) + "C";
			break;
		case CursorDirection::Left: std::cout << "\033[" + std::to_string(amount) + "D";
			break;
	}
}

inline void WriteLine(const std::string& string = "")
{
	SaveXY();
	std::cout << string;
	LoadXY();
	MoveCursor(CursorDirection::Down);
}

inline void CinReset()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <typename T>
T Prompt(const std::string& prompt)
{
	while (true)
	{
		SaveXY();
		std::cout << prompt << "~> ";
		T input;
		std::cin >> input;

		ResetColor();
		if (!std::cin)
		{
			LoadXY();
			std::cout << Repeat(" ", 15 + prompt.length() + 3);
			LoadXY();
			CinReset();
			continue;
		}

		CinReset();
		LoadXY();
		MoveCursor(CursorDirection::Down);
		return input;
	}
}

template <>
inline std::string Prompt<std::string>(const std::string& prompt)
{
	while (true)
	{
		SaveXY();

		std::cout << prompt << "~> ";
		std::string input;
		std::getline(std::cin, input);

		ResetColor();
		if (!std::cin)
		{
			LoadXY();
			std::cout << Repeat(" ", 15 + prompt.length() + 3);
			LoadXY();
			CinReset();
			continue;
		}

		LoadXY();
		MoveCursor(CursorDirection::Down);
		return input;
	}
}
