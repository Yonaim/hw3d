#include "App.h"
#include <sstream>

App::App() : wnd(800, 600, L"The Donkey Fart Box")
{
}

int App::Go()
{
	MSG  msg; // 내부에 hwnd 가짐
	BOOL gResult;

	while (1)
	{
		// ecode -> std::optional<int>
		// if the value of ecode is not optional, then the value is exit code
		if (const auto ecode = Window::ProcessMessages())
			return *ecode;
		else
			DoFrame();
	}

	if (gResult == -1)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// the value passed to PostQuitMessage
	return msg.wParam;
}

void App::DoFrame()
{
	const float        t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());
}
