#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keybuffer.size() > 0u)
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else
		return Keyboard::Event(); // Invalid Event (default)
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keybuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	// simply create empty queue
	keybuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	return 0;
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charbuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
    autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    keystates[keycode] = true;
    keybuffer.push(Keyboard::Event(Event::Type::Press, keycode));
    TrimBuffer(keybuffer); // just in case of the buffer goes oversize
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;
}

void Keyboard::OnChar(char character) noexcept
{
    charbuffer.push(character);
	TrimBuffer(charbuffer); // just in case of the buffer goes oversize
}

void Keyboard::ClearState() noexcept
{
    keystates.reset();
}

template <typename T> void Keyboard::TrimBuffer(std::queue<T> &buffer) noexcept
{
    while (buffer.size() > bufferSize)
        buffer.pop();
}
