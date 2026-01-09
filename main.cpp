#include "AppWindow.h"
#include "InputSystem.h"
#include <iostream>

int main()
{
	try
	{
		GraphicEngine::Create();
		InputSystem::Create();
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

	InputSystem::Release();
	GraphicEngine::Release();

	return 0;
}