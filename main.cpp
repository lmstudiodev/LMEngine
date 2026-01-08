#include "AppWindow.h"

int main()
{
	AppWindow app;

	if (app.Init())
	{
		while (app.IsRunnig())
		{
			app.Broadcast();
		}
	}

	return 0;
}