#include "stdafx.h"
#include "PostProcessingDemo.h"
#include "InputSystem.h"

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
		try
		{
			PostProcessingDemo app;

			while (app.IsRunnig());
		}
		catch (...)
		{
			std::cout << "[LMEngine Error] AppWindow initialization failed." << "\n";
			
			InputSystem::Release();
			GraphicEngine::Release();

			return -1;
		}
	}

	InputSystem::Release();
	GraphicEngine::Release();

	return 0;
}