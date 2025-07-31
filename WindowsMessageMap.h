#pragma once
#include <string>
#include <unordered_map>
#include <Windows.h>

class WindowsMessageMap
{
public:
	WindowsMessageMap () noexcept;
	std::string operator()(DWORD msg , LPARAM lp , WPARAM wp) const;
private:
	std::unordered_map<DWORD , std::string> map;
};
