#ifndef __wincon_h
#define __wincon_h

/* wincon - библиотека для работы с консолью windows. Простой аналог ncurses.
   Для использования необходимо также добавить в проект файл wincon.c.
   Пример использования приведен в файле demo.c

   Если в консоли не поддерживается кодировка русских букв, то следует
   поменять шрифт на любой Юникодный (например Lucida Console).
 */

/* Инициализирует консоль: устанавливает кодировку win1251. 
   Параметр double_buf указывает - использовать ли двойную буфферизацию */
void con_init(short width, short height);

/* Функция возвращает текущую ширину и высоту консоли */
short con_width();
short con_height();

/* Функция устанавливает цвет шрифта и фона. Используйте одну из констант CON_CLR_ */
void con_set_color(short font, short background);

/* Функция очищает консоль текущим цветом фона. */
void clrscr();

void con_draw_lock();
void con_draw_release();

/* Функция перемещает курсор консоли в указанную позицию. Вывод с помощью printf, putchar будет происходить с этой позиции */
void gotoxy(short x, short y);

/* Функция показывает (show=1) или скрывает (show=0) мигающий курсор ввода консоли. */
void show_cursor(int show);

/* Функция ожидает указанное количество миллисекунд и возвращет управление вызывающему коду. */
void pause(int ms);

/* Функция проверяет - нажал ли пользователь клавишу в консоли. Возвращает 1 или 0. */
int key_is_pressed();

/* Функция возвращает код нажатой клавиши. 
  Если пользователь ничего не нажимал - функция не вернет управление до тех пор, пока пользователь не нажмет клавишу.
  Функцией key_is_pressed() можно проверить - есть ли код нажатой клавиши в буфере */
int key_pressed_code();

/* Коды некоторых специальных клавиш, возвращаемых key_pressed_code() */
#define KEY_UP     (256 + 72)
#define KEY_DOWN   (256 + 80)
#define KEY_LEFT   (256 + 75)
#define KEY_RIGHT  (256 + 77)
#define KEY_ENTER  (13)
#define KEY_ESC	   (27)
#define KEY_BACK   (8)


/* Коды некоторых цветов для con_set_color */

#define BIT_BLUE	(0x01)
#define BIT_GREEN	(0x02)
#define BIT_RED		(0x04)
#define BIT_LIGHT	(0x08)

/* 16 цветов, поддерживаемых консолью. Можно передавать в con_set_colors */
#define CON_CLR_BLACK		(0)
#define CON_CLR_RED			(BIT_RED)
#define CON_CLR_GREEN		(BIT_GREEN)
#define CON_CLR_BLUE		(BIT_BLUE)
#define CON_CLR_YELLOW		(BIT_RED | BIT_GREEN)
#define CON_CLR_MAGENTA		(BIT_RED | BIT_BLUE)
#define CON_CLR_CYAN		(BIT_GREEN | BIT_BLUE)
#define CON_CLR_WHITE		(BIT_RED | BIT_GREEN | BIT_BLUE)

#define CON_CLR_GRAY		(BIT_LIGHT)
#define CON_CLR_RED_LIGHT		(BIT_RED | BIT_LIGHT)
#define CON_CLR_GREEN_LIGHT		(BIT_GREEN | BIT_LIGHT)
#define CON_CLR_BLUE_LIGHT		(BIT_BLUE | BIT_LIGHT)
#define CON_CLR_YELLOW_LIGHT		(BIT_RED | BIT_GREEN | BIT_LIGHT)
#define CON_CLR_MAGENTA_LIGHT		(BIT_RED | BIT_BLUE | BIT_LIGHT)
#define CON_CLR_CYAN_LIGHT		(BIT_GREEN | BIT_BLUE | BIT_LIGHT)
#define CON_CLR_WHITE_LIGHT		(BIT_BLUE | BIT_RED | BIT_GREEN | BIT_LIGHT)

#endif // __wincon_h
