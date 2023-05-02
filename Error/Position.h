#pragma once
#include <string>

class Position {
public:
	unsigned int character;
	unsigned int line;

public:
	Position()
		:character(1), line(1) {}
	Position(unsigned int index, unsigned int line)
		:character(index), line(line) {}


	friend std::ostream& operator<<(std::ostream& os, const Position& head);

};