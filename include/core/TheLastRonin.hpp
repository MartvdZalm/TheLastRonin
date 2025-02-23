#pragma once

class TheLastRonin
{

public:
	TheLastRonin();
	~TheLastRonin();

	void Run();
	bool IsRunning();

	static void Init();
	static TheLastRonin& Instance();
};
