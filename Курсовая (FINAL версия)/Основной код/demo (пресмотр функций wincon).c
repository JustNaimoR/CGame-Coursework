#define _CRT_SECURE_NO_WARNINGS
#include "wincon.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main_menu();
void demo_animation();
void demo_colors();
void demo_input();
void about();

int main()
{
	// Инициализируется консоль, скрывается курсор
	con_init(80, 25);
	show_cursor(0);

	// Запуск главного меню
	main_menu();

	return 0;
}




// Поддержка главного меню
void main_menu()
{
	const char* menu_items[] = { "Демо: анимация", "Демо: ввод", "Демо: цвета", "О программе", "Выход" };
	int menu_active_idx = 0;
	int menu_items_count = sizeof(menu_items) / sizeof(menu_items[0]);

	short clr_bg = CON_CLR_BLUE;
	short clr_bg_active = CON_CLR_GRAY;
	short clr_font = CON_CLR_WHITE_LIGHT;

	while (1)
	{
		int left = 30;
		int top = 2;
		int b;

		// Заблокировать отрисовку
		con_draw_lock();

		// Очистка экрана
		con_set_color(clr_font, clr_bg);
		clrscr();

		// Цикл отрисовывает кнопки
		for (b = 0; b < menu_items_count; b++)
		{
			short btn_bg = clr_bg; // По умолчанию фон кнопки - как фон экрана
			if (b == menu_active_idx)
				btn_bg = clr_bg_active; // Если кнопка активна - то рисуется другим цветом

			gotoxy(left, top);
			con_set_color(clr_font, btn_bg);

			printf("********************");
			top++;
			gotoxy(left, top);
			printf("*                   ");
			
			gotoxy(left + 10 - strlen(menu_items[b]) / 2, top);
			printf("%s", menu_items[b]);

			con_set_color(clr_font, btn_bg);
			gotoxy(left + 19, top);
			printf("*");
			top++;
			gotoxy(left, top);
			printf("********************");
			top += 2;
		}
		
		// Данные подготовлены, вывести на экран
		con_draw_release();
		

		while (!key_is_pressed()) // Если пользователь нажимает кнопку
		{
			int code = key_pressed_code();
			if (code == KEY_UP) // Если это стрелка вверх
			{
				// То переход к верхнему пункту (если это возможно)
				if (menu_active_idx > 0) 
				{
					menu_active_idx--;
					break;
				}
			}
			else if (code == KEY_DOWN) // Если стрелка вниз
			{
				// То переход к нижнему пункту (если это возможно)
				if (menu_active_idx + 1 < menu_items_count)
				{
					menu_active_idx++;
					break;
				}
			}
			else if (code == KEY_ESC || code == 'q' || code == 'Q' || 
				     code == (unsigned char)'й' || code == (unsigned char)'Й') // ESC или 'q' - выход
			{
				return;
			}
			else if (code == KEY_ENTER) // Нажата кнопка Enter
			{
				if (menu_active_idx == menu_items_count - 1) // Выбран последний пункт - это выход
					return;

				if (menu_active_idx == 0)
					demo_animation();

				if (menu_active_idx == 1)
					demo_input();

				if (menu_active_idx == 2)
					demo_colors();

				if (menu_active_idx == menu_items_count - 2)
					about();

				break;
			}
		
		
			pause(40); // Небольная пауза (чтобы не загружать процессор)
		} // while (!key_is_pressed())


		// "Съедается" оставшийся ввод
		while (key_is_pressed())
			key_pressed_code();

	} // while(1)
}


void about()
{
	con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
	clrscr();

	gotoxy(8, 2);
	printf("О программе:");

	con_set_color(CON_CLR_GRAY, CON_CLR_BLACK);
	gotoxy(8, 3);
	printf("Данная программа является примером использования библиотеки wincon.\n\n");

	gotoxy(8, 4);
	printf("Для продолжения нажмите любую клавишу.");
	
	key_pressed_code();
	return;
}

void demo_colors()
{
	int x = 0, y = 0;
	gotoxy(0, 0);
	con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
	clrscr();

	gotoxy(x, y++);
	con_set_color(CON_CLR_GRAY, CON_CLR_BLACK);
	printf("Черный / серый");

	gotoxy(x, y++);
	con_set_color(CON_CLR_BLUE_LIGHT, CON_CLR_BLUE);
	printf("Синий");

	gotoxy(x, y++);
	con_set_color(CON_CLR_RED_LIGHT, CON_CLR_RED);
	printf("Красный");

	gotoxy(x, y++);
	con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_GREEN);
	printf("Зеленый");

	gotoxy(x, y++);
	con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_YELLOW);
	printf("Желтый");

	gotoxy(x, y++);
	con_set_color(CON_CLR_MAGENTA_LIGHT, CON_CLR_MAGENTA);
	printf("Пурпурный");

	gotoxy(x, y++);
	con_set_color(CON_CLR_CYAN_LIGHT, CON_CLR_CYAN);
	printf("Голубой");

	gotoxy(x, y++);
	con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_WHITE);
	printf("Белый");


	con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
	gotoxy(30, 20);
	printf("Пример демонстрирует все поддерживаемые цвета.");

	gotoxy(30, 21);
	printf("Для продолжения нажмите любую клавишу.");

	key_pressed_code();
	return;
}


void demo_animation()
{
	int x = 0, y = 1;
	int m = 1;
	int speed = 40;

	con_set_color(CON_CLR_BLACK, CON_CLR_GREEN);
	clrscr();

	gotoxy(40, 20);
	printf("Нажмите q для выхода.");
	gotoxy(40, 21);
	printf("Перемещение: стрелки вверх/вниз.");
	gotoxy(40, 22);
	printf("Скорость: кнопки +/-");

	while (1)
	{
		int i = 0;
		int new_y = y;

		gotoxy(x, y);
		printf("*");

		// Звездочка перемещается раз в 200 мс, но кнопку могут нажать в это время. 
		// Поэтому каждые 40 мс проверяется нажатие 
		for (i = 0; i < 5; i ++)
		{
			if (key_is_pressed())
			{
				int c = key_pressed_code();
				if (c == 'q' || c == 'Q' || c == KEY_ESC)
					return;
				if (c == KEY_UP && new_y > 1)
				{
					new_y--;
					break;
				}

				if (c == KEY_DOWN && new_y < 19)
				{
					new_y++;
					break;
				}

				if (c == '+' && speed > 0)
					speed--;
				if (c == '-')
					speed++;
			}

			pause(speed);
		}

		
		// Распечатывается пробел на старой позиции (чтобы не перерисовывать весь экран)
		gotoxy(x, y);
		printf(" "); 

		x += m;
		if (x >= 80 || x < 0)
		{
			m *= -1;
			x += m;
		}

		y = new_y;
	}
}

void demo_input()
{
	con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_GRAY);
	clrscr();

	while (1)
	{
		char buf[1024] = { 0 };
		int digit = -1;

		gotoxy(0, 1);
		printf("Введите число ('q' - выход):");
		
		gotoxy(0, 2);
		show_cursor(1);
		scanf_s("%s", buf, _countof(buf));
		show_cursor(0);

		// Весь остальной пользовательский ввод съедается
		while (key_is_pressed())
			key_pressed_code();

		if (!strcmp(buf, "q"))
			return;

		digit = atoi(buf);

		clrscr();
		gotoxy(0, 0);
		printf("Вы ввели: как число: %d (0x%x), как строка: '%s'\n", digit, digit, buf);
	}
}


