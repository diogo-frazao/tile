#include <SDL.h>
#include "core/app.h"
#include <windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	App app;
	app.runApp();
	return 0;
}

/*
int main(int argc, char* args[])
{
	App app;
	app.runApp();
	return 0;
}
*/