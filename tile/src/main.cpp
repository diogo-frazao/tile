#include "core/app.h"

#ifdef _WIN32
#include <windows.h>
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	App app;
	app.runApp();
	return 0;
}
#else
int main(int argc, char* argv[])
{
	App app;
	app.runApp();
	return 0;
}
#endif