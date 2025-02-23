#include "io/Parser.hpp"

#include "Settings.hpp"
#include <fstream>
#include <iostream>
#include <string>

float CHARACTER::X;
float CHARACTER::Y;
float CHARACTER::Z;

void Parser::GetInt(std::ifstream& file, float* value)
{
	std::string word;
	while (file >> word) {
		if ((word[0] >= '0' && word[0] <= '9') || word[0] == '-') {
			*value = std::stoi(word);
			return;
		}
	}
}

void Parser::GetFloat(std::ifstream& file, float* value)
{
	std::string word;
	while (file >> word) {
		if ((word[0] >= '0' && word[0] <= '9') || word[0] == '-' || word[0] == '.') {
			*value = std::stof(word);
			return;
		}
	}
}

void Parser::Parse()
{
	std::ifstream config;
	config.open("../config.txt")

	if (!config.is_open()) {
		
		return;
	}
}
