#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"

class Window
{
  public:
	class Exception : public ChiliException
	{
	  public:
		Exception(int line, const char *file, HRESULT hr) noexcept;
		const char         *what() const noexcept override;
		virtual const char *GetType() const noexcept;
		static std::string  TranslateErrorCode(HRESULT hr);
		HRESULT             GetErrorCode() const noexcept;
		std::string         GetErrorString() const noexcept;

	  private:
		HRESULT hr;
	};

  private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	  public:
		static const wchar_t *GetName() noexcept;
		static HINSTANCE      GetInstance() noexcept;

	  private:
		WindowClass() noexcept; // 싱글톤이므로 생성자는 private
		~WindowClass();
		WindowClass(const WindowClass &)            = delete; // Do not use copy
		WindowClass &operator=(const WindowClass &) = delete; // Do not use copy
		static constexpr const wchar_t *wndClassName
			= L"Chili Direct3D Engine Window"; // wide-character string

		static WindowClass wndClass; // 단 하나의 객체
		HINSTANCE          hInst;    // 객체의 멤버변수
	};

  public:
	Window(int width, int height, const wchar_t *name);
	~Window();
	Window(const Window &)            = delete;
	Window &operator=(const Window &) = delete;

  private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam,
										   LPARAM lParam);
	static LRESULT CALLBACK
	HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam,
				   LPARAM lParam); // c 전역 함수만 받음으로 인해 중간 다리
								   // 역할을 해주는 thunk
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int  width;
	int  height;
	HWND hWnd;
};

// error exception helper macro
#define CHWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
