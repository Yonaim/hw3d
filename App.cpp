#include "App.h"
#include <sstream>

App::App() : wnd(800, 600, L"The Donkey Fart Box")
{
}

int App::Go()
{
	MSG  msg; // 내부에 hwnd 가짐
	BOOL gResult;

	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg); // 키보드 메세지일 경우 추가적으로 WM_CHAR 메세지 발생
		DispatchMessage(&msg);  // 해당되는 윈도우의 프로시저에 메세지 전달

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
