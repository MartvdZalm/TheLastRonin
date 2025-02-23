#include "core/TheLastRonin.hpp"

int main()
{
	TheLastRonin::Init();

	while (TheLastRonin::Instance().IsRunning())
	{
		TheLastRonin::Instance().Run();
	}

	return 0;
}
