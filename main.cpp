#include <stdafx.h>
#include <Game.h>

int main()
{

	try
	{
		Game app;

		app.run();
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}