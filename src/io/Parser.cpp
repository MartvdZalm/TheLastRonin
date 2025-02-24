#include "io/Parser.hpp"

#include "Settings.hpp"
#include "utils/Logger.hpp"
#include <fstream>
#include <iostream>
#include <string>

float CHARACTER::X;
float CHARACTER::Y;
float CHARACTER::Z;

void Parser::GetInt(std::ifstream& file, int* value)
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
	config.open("../Config.txt");

	if (!config.is_open()) {
		Logger::Error("Couldn't open the Config.txt file");

		return;
	} 

	Logger::Info("Successfully opened Config.txt file");
}
