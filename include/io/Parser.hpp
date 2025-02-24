#include <fstream>

namespace Parser
{
	void Parse();
	void GetInt(std::ifstream& file, int* value);
	void GetFloat(std::ifstream& file, float* value);
};
