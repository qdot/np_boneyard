#define WIN32_MEAN_AND_LEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include "qTestEngine.h"

WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	boost::shared_ptr<qTestEngine> TestEngine(new qTestEngine());
	if(TestEngine->init())
	{
		TestEngine->gameLoop();
	}
	TestEngine.reset();
	return 0;
}