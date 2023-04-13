#define WIN32_LEAN_AND_MEAN
#include <conio.h>
#include <windows.h>
#include <malloc.h>
#include <locale.h>

#include "wincon.h"

HANDLE g_shadow_buf = 0;
CHAR_INFO* g_shadow_buf_data = 0;
unsigned int g_shadow_buf_size = 0;

void con_init(short width, short height)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c;

	setlocale(LC_ALL, "Russian");

	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	c.X = width;
	c.Y = height;

	g_shadow_buf = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
			0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

short con_width()
{
	CONSOLE_SCREEN_BUFFER_INFO cbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbi);
	return (cbi.srWindow.Right - cbi.srWindow.Left) + 1;
}

short con_height()
{
	CONSOLE_SCREEN_BUFFER_INFO cbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbi);
	return (cbi.srWindow.Bottom - cbi.srWindow.Top) + 1;
}

void con_redraw()
{
	/**/
}


void con_draw_lock()
{
	HANDLE from = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE to = g_shadow_buf;
	CONSOLE_SCREEN_BUFFER_INFO cbi;
	SMALL_RECT rc;
	COORD zero_coord = { 0, 0 };
	CONSOLE_CURSOR_INFO ci;

	if (!g_shadow_buf)
		return;


	GetConsoleScreenBufferInfo(from, &cbi);
	if (g_shadow_buf_size < sizeof(CHAR_INFO)* cbi.dwSize.X * cbi.dwSize.Y)
	{
		if (g_shadow_buf_data)
			free(g_shadow_buf_data);
		g_shadow_buf_size = sizeof(CHAR_INFO)* cbi.dwSize.X * cbi.dwSize.Y;
		g_shadow_buf_data = malloc(g_shadow_buf_size);
		if (!g_shadow_buf_data)
			g_shadow_buf_size = 0;
	}

	rc.Left = 0;
	rc.Top = 0;
	rc.Right = cbi.dwSize.X - 1;
	rc.Bottom = cbi.dwSize.Y - 1;

	ReadConsoleOutput(from, g_shadow_buf_data, cbi.dwSize, zero_coord, &rc);
	WriteConsoleOutput(to, g_shadow_buf_data, cbi.dwSize, zero_coord, &rc);

	GetConsoleCursorInfo(from, &ci);
	SetConsoleCursorInfo(to, &ci);

	SetConsoleActiveScreenBuffer(g_shadow_buf);
}

void con_draw_release()
{
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
}

void con_set_color(short font, short background)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), font | (background << 4));
}

void clrscr()
{
	DWORD len;
	DWORD written;

	CONSOLE_SCREEN_BUFFER_INFO cbi;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD c;
	c.X = 0;
	c.Y = 0;

	GetConsoleScreenBufferInfo(handle, &cbi);
	len = cbi.dwSize.X * cbi.dwSize.Y;

	FillConsoleOutputCharacterA(handle, ' ', len, c, &written);
	FillConsoleOutputAttribute(handle, cbi.wAttributes, len, c, &written);
}


void gotoxy(short x, short y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void show_cursor(int show)
{
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = show;
	
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void pause(int ms)
{
	Sleep((DWORD)ms);
}

int key_is_pressed()
{
	return _kbhit();
}

int key_pressed_code()
{
	int ch = _getch();
	if (ch == 0 || ch == 224)
		ch = 256 + _getch();
	return ch;
}
