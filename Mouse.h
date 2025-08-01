﻿#pragma once
#include <queue>
#include <bitset>

/**
 * @brief
 * Mouse Class: 마우스의 실시간 상태,
 * Mouse::Event: 마우스에서 발생한 이벤트, 특정 시점의 스냅샷
 */
class Mouse
{
	// Mouse class is the extension of Window class
	friend class Window;

  public:
	class Event
	{
	  public:
		enum class Type // C++11 grammer
		{
			LPress,
			RPress,
			LRelease,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};

	  private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int  x;
		int  y;

	  public:
		Event() noexcept : type(Type::Invalid),
						   leftIsPressed(false),
						   rightIsPressed(false),
						   x(0),
						   y(0)
		{
		}
		Event(Type type, const Mouse &parent) noexcept
			: type(type),
			  leftIsPressed(parent.leftIsPressed),
			  rightIsPressed(parent.rightIsPressed),
			  x(parent.x),
			  y(parent.y)
		{
		}
		bool IsInvalid() const noexcept
		{
			return type == Type::Invalid;
		}
		Type GetType() const noexcept
		{
			return type;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return {x, y};
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		bool LeftIsPressed() const noexcept
		{
			return leftIsPressed;
		}
		bool ReftIsPressed() const noexcept
		{
			return rightIsPressed;
		}
	};

  public:
	Mouse()                         = default;
	Mouse(const Mouse &)            = delete;
	Mouse &operator=(const Mouse &) = delete;

	std::pair<int, int> GetPos() const noexcept;
	int                 GetPosX() const noexcept;
	int                 GetPosY() const noexcept;
	bool                IsInWindow() const noexcept;
	bool                LeftIsPressed() const noexcept;
	bool                RightIsPressed() const noexcept;
	Mouse::Event        Read() noexcept;
	bool                IsEmpty() const noexcept;
	void                Flush() noexcept;

  private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept; // just trim buffer if oversize
	void OnWheelDelta(int x, int y, int delta);

  private:
	static constexpr unsigned int bufferSize = 16u;
	int                           x;
	int                           y;
	bool                          leftIsPressed  = false;
	bool                          rightIsPressed = false;
	bool                          isInWindow     = false;
	int                           wheelDeltaCarry = 0;
	std::queue<Event>             buffer;
};
