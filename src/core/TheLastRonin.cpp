#include "core/TheLastRonin.hpp"

#include "io/Parser.hpp"

TheLastRonin::TheLastRonin()
{

}

TheLastRonin::~TheLastRonin() {}

void TheLastRonin::Init()
{
	Parser::Parse();
}

void TheLastRonin::Run()
{

}

bool TheLastRonin::IsRunning()
{
	return true;
}

TheLastRonin& TheLastRonin::Instance()
{
	static TheLastRonin theLastRonin;
	return theLastRonin;
}
