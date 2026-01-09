#include "AppWindow.h"
#include <iostream>

int main()
{
	try
	{
		GraphicEngine::Create();
	}
	catch(...)
	{
		std::cout << "[LMEngine Error] GraphicEngine initialization failed." << "\n";
		return -1;
	}
	
	{
		AppWindow app;

		if (app.Init())
		{
			while (app.IsRunnig())
			{
				app.Broadcast();
			}
		}
	}

	GraphicEngine::Release();

	return 0;
}