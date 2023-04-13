#define _CRT_SECURE_NO_WARNINGS

#include "wincon.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <math.h>

void Menu_Animation(int sec, int option);
void Menu_Animation_Authors();
void Menu_Animation_Loading();
void Menu_Animation_Table_of_records();
void Menu_Close();
void Menu();

void Choiced_option(int option, char* text_of_error);
int LKG(int seed);
int Seek(int* massive, int length, int number);
char* Open_the_data();
int Translate_the_number(char* start);
int Integrity_check(char* Data);
void Save_data(char* Data);

char* Main_Data;  //  Основные данные по игре

//TODO  - Главные задачи лежат в функции Choiced_option()
//TODO  - В таблице рекордов использовать структуры (как идея - в доболнительных сведениях об игроке: количество ходов, очков, времени игры...)
//TODO  - Найти применение enum, union; Списки или деревья
//TODO  - Реализовать сложность игры
//TODO  - во вкладке справка реализовать возможность отключения анимаций загрузки и т.п.
//TODO  - Дабавление возможности считывания файлов с определенных папок по дефолту (Изначально стоит "C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt" как пример)

//TODO  - Проверка файла Data.txt на повреждения (все оформлено в определенном виде)

int main() {
    Main_Data = Open_the_data();  //  Открытие файла и запись основных данных

    Menu();  //  Запуск меню и программы
    return 0;
}
//  ================================================  Анимации  ========================================================
void Menu_Animation(int sec, int option) {  //  Анимация появления меню (sec - время анимации: 200 - default, 0 - без анимации); Номер подсвечиваемой опции (0 - default)
    ///  - Функция вывода меню  (!!!Сильно зависит от изначальной картинки. В случае заменый меню ломается логика!!!)

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    //  Листинг таблицы-меню
    char menu_table[25][50] = { "========[   Игра \"Балда\"   ]========",
                                "/::\\                            /::\\",
                                "[[]]========[   Меню   ]========[[]]",
                                "\\::/                            \\::/",
                                " ||     [<]   "," Играть ","   [>]     || ",
                                " ||                              || ",
                                " ||     [*]   "," Рекорды ","  [*]     || ",
                                " TT                              TT ",
                                " ||     [I] "," О программе ","[I]     || ",
                                " ||                              || ",
                                " ||     [?]  "," Справка ","   [?]     || ",
                                " ||                              || ",
                                "/__\\    [x]   "," Выход ","    [x]    /__\\",
                                "|::|                            |::|",
                                "IIII============================IIII" };

    int width_of_table = 36;  //  Ширина меню
    int height_of_table = 15;  //  Высота меню

    int mark_X = 0;  //  Марка для вывода знаков '='

    short width_of_console;  //  Ширина консоли
    short height_of_console;  //  Высота консоли

    for (int step_of_output = 0; step_of_output < (con_width_new() - width_of_table) / 2; step_of_output += 3) {  //  Цикл анимации вывода '='

        width_of_console = con_width_new();
        height_of_console = con_height_new();

        gotoxy((short)mark_X, (height_of_console - height_of_table) / 2);  //  Вверх + лево
        printf("===");

        gotoxy(width_of_console - mark_X - 3, (height_of_console - height_of_table) / 2);  //  Вверх + право
        printf("===");

        gotoxy(width_of_console - mark_X - 3, (height_of_console - height_of_table) / 2 + height_of_table - 1);  //  Низ + право
        printf("===");

        gotoxy((short)mark_X, (height_of_console - height_of_table) / 2 + height_of_table - 1);  //  Низ + лево
        printf("===");

        mark_X += 3;  //  Двигаем марку x для вывода следующих символов

        Sleep(sec);  //  Усыплени компилятора
    }

    int string_of_console = 0;  //  Строка, которуб сейчас использует вывод меню
    for (int string_of_menu = 0; string_of_menu < height_of_table + 10; string_of_menu++) {  //  Вывод меню
        //  ====================  Опасный участок (очень сильно зависит от нарисованной картинки)  ========================================
        //  Учет элементов меню для не перехода курсора
        if (string_of_menu != 5 && string_of_menu != 6) {  //  Не переход 1 опции
            if (string_of_menu != 9 && string_of_menu != 10) { //  //  Не переход 2 опции
                if (string_of_menu != 13 && string_of_menu != 14) {  //  Не переход 3 опции
                    if (string_of_menu != 17 && string_of_menu != 18) {  //  Не переход 4 опции
                        if (string_of_menu != 21 && string_of_menu != 22) {  //  Не переход 5 опции
                            gotoxy((width_of_console - width_of_table) / 2, (height_of_console - height_of_table) / 2 + string_of_console++);
                        }
                    }
                }
            }
        }
        //  ===============================================================================================================================
        if (string_of_menu == 5 + 4 * option) {  //  Условие для окраски определенной опции в меню

            con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Меняем на белый фон с черными буквами
            printf("%s", &menu_table[string_of_menu][0]);
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами

        }
        else {
            printf("%s", &menu_table[string_of_menu][0]);
        }

    }

}

void Menu_Animation_Authors() {  //  Анимация запуска программы
    ///  - Создано 2-е анимации перехода буквы в букву (из 'R' в 'D' и наоборот)
    ///  - Как основа - выбрана первая анимация (picture_1). Вторая анимация отображается в зависимости от Random переменной
    ///  - Функция лишь для вывода анимации букв авторов в консоли перед работой меню

    con_set_color(CON_CLR_BLACK, CON_CLR_GRAY);  //  Устанавливаем серый фон и черный шрифт
    show_cursor(0);  //  Скрытие курсора

    //  Трехмерный массив первой картинки, ее вариации ( picture_1[кол. вариаций в анимации][строчек символа][символов в строчке] )
    const char picture_1[5][21][26] = { {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                         "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                         "    /:::/__\\:::\\    \\    ","   /::::\\   \\:::\\    \\   ","  /::::::\\   \\:::\\    \\  ",
                                         " /:::/\\:::\\   \\:::\\____\\ ","/:::/  \\:::\\   \\:::|    |","\\::/   |::::\\  /:::|____|",
                                         " \\/____|:::::\\/:::/    / ","       |:::::::::/    /  ","       |::|\\::::/    /   ",
                                         "       |::| \\::/____/    ","       |::|  ~|          ","       |::|   |          ",
                                         "       \\::|   |          ","        \\:|   |          ","         \\|___|          "},
                /* 2 вариация */        {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                         "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                         "    /:::/__\\:::\\    \\    ","   /::::\\   \\:::\\    \\   ","  /::::::\\   \\:::\\    \\  ",
                                         " /:::/\\:::\\   \\:::\\____\\ "," /:::/    /   \\:::\\ ___\\ ","\\::/   |::::\\  /:::|____|",
                                        " \\/____|:::::\\/:::/    / ","       |:::::::::/    /  ","       |::|\\::::/    /   ",
                                        "       |::| \\::/____/    ","       |::|  ~|          ","       |::|   |          ",
                                        "       \\::|   |          ","        \\:|   |          ","         \\|___|          "},
                /* 3 вариация */        {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                         "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                         "    /:::/__\\:::\\    \\    ","   /::::\\   \\:::\\    \\   ","   /:::/    \\:::\\    \\   ",
                                         "  /:::/    / \\:::\\    \\  "," /:::/    /   \\:::\\ ___\\ ","/:::/____/  ___\\:::|    |",
                                         "\\:::\\    \\ /\\  /:::|____|","       |:::::::::/    /  ","       |::|\\::::/    /   ",
                                         "       |::| \\::/____/    ","       |::|  ~|          ","       |::|   |          ",
                                         "       \\::|   |          ","        \\:|   |          ","         \\|___|          "},
                /* 4 вариация */        {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                         "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                         "     /:::/\\:::\\    \\     ","    /:::/  \\:::\\    \\    ","   /:::/    \\:::\\    \\   ",
                                         "  /:::/    / \\:::\\    \\  "," /:::/    /   \\:::\\ ___\\ ","/:::/____/  ___\\:::|    |",
                                         "\\:::\\    \\ /\\  /:::|____|"," \\:::\\    /::\\ \\::/    / ","  \\:::\\   \\:::\\ \\/____/  ",
                                         "       |::| \\::/____/    ",
                                         "       |::|  ~|          ","       |::|   |          ","       \\::|   |          ",
                                         "        \\:|   |          ","         \\|___|          "},
               /* 5 вариация */         {"                         ","          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                         "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                         "    /:::/  \\:::\\    \\    ","   /:::/    \\:::\\    \\   ","  /:::/    / \\:::\\    \\  ",
                                         " /:::/    /   \\:::\\ ___\\ ","/:::/____/  ___\\:::|    |","\\:::\\    \\ /\\  /:::|____|",
                                         " \\:::\\    /::\\ \\::/    / ","  \\:::\\   \\:::\\ \\/____/  ","   \\:::\\   \\:::\\____\\    ",
                                         "    \\:::\\  /:::/    /    ","     \\:::\\/:::/    /     ","      \\::::::/    /      ",
                                         "       \\::::/    /       ","        \\::/____/        "} };
    //  Трехмерный массив второй картинки, ее вариации
    const char picture_2[5][21][26] = { {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                         "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                         "    /:::/  \\:::\\    \\    ","   /:::/    \\:::\\    \\   ","  /:::/    / \\:::\\    \\  ",
                                         " /:::/    /   \\:::\\ ___\\ ","/:::/____/  ___\\:::|    |","\\:::\\    \\ /\\  /:::|____|",
                                         " \\:::\\    /::\\ \\::/    / ","  \\:::\\   \\:::\\ \\/____/  ","   \\:::\\   \\:::\\____\\    ",
                                         "    \\:::\\  /:::/    /    ","     \\:::\\/:::/    /     ","      \\::::::/    /      ",
                                         "       \\::::/    /       ","        \\::/____/        ","                         "},
                /* 2 вариация */        {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                         "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                         "    /:::/  \\:::\\    \\    ","   /:::/    \\:::\\    \\   ","  /:::/    / \\:::\\    \\  ",
                                         "/:::/  \\:::\\   \\:::|    |","/:::/____/  ___\\:::|    |","\\:::\\    \\ /\\  /:::|____|",
                                         " \\:::\\    /::\\ \\::/    / ","  \\:::\\   \\:::\\ \\/____/  ","   \\:::\\   \\:::\\____\\    ",
                                         "    \\:::\\  /:::/    /    ","     \\:::\\/:::/    /     ","      \\::::::/    /      ",
                                         "       \\::::/    /       ","        \\::/____/        ","                         "},
                /* 3 вариация */        {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                         "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                         "    /:::/  \\:::\\    \\    ","  /::::::\\   \\:::\\    \\  "," /:::/\\:::\\   \\:::\\____\\ ",
                                         "/:::/  \\:::\\   \\:::|    |","\\::/   |::::\\  /:::|____|"," \\/____|:::::\\/:::/    / ",
                                         " \\:::\\    /::\\ \\::/    / ","  \\:::\\   \\:::\\ \\/____/  ","   \\:::\\   \\:::\\____\\    ",
                                         "    \\:::\\  /:::/    /    ","     \\:::\\/:::/    /     ","      \\::::::/    /      ",
                                         "       \\::::/    /       ","        \\::/____/        ","                         "},
               /* 4 вариация */        {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                        "       /::::\\    \\       ","     /:::/\\:::\\    \\     ","    /:::/__\\:::\\    \\    ",
                                        "   /::::\\   \\:::\\    \\   ","  /::::::\\   \\:::\\    \\  "," /:::/\\:::\\   \\:::\\____\\ ",
                                        "/:::/  \\:::\\   \\:::|    |","\\::/   |::::\\  /:::|____|"," \\/____|:::::\\/:::/    / ",
                                        "       |:::::::::/    /  ","       |::|\\::::/    /   ","       |::| \\::/____/    ",
                                        "   \\:::\\  /:::/    /     ","    \\:::\\/:::/    /      ","     \\::::::/    /       ",
                                        "      \\::::/    /        ","       \\::/____/         ","                         "},
               /* 5 вариация */        {"          _____          ","         /\\    \\         ","        /::\\    \\        ",
                                        "       /::::\\    \\       ","      /::::::\\    \\      ","     /:::/\\:::\\    \\     ",
                                        "    /:::/__\\:::\\    \\    ","   /::::\\   \\:::\\    \\   ","  /::::::\\   \\:::\\    \\  ",
                                        " /:::/\\:::\\   \\:::\\____\\ ","/:::/  \\:::\\   \\:::|    |","\\::/   |::::\\  /:::|____|",
                                        " \\/____|:::::\\/:::/    / ","       |:::::::::/    /  ","       |::|\\::::/    /   ",
                                        "       |::| \\::/____/    ","       |::|  ~|          ","       |::|   |          ",
                                        "       \\::|   |          ","        \\:|   |          ","         \\|___|          "} };

    const int width_of_picture = 25;  //  Ширина картинок
    const int height_of_picture = 21;  //  Высота картинок

    Sleep(1000);

    for (int current_picture = 0; current_picture < 5; current_picture++) {  //  Цикл вывода картинок (5 различных версия картинки)

        Sleep(500);

        clrscr();  //  Очистка окна и окрас консоли в цвет

        int width_of_console = con_width_new();  //  Ширина консоли
        int height_of_console = con_height_new();  //  Высота консоли

        short current_X = (width_of_console - width_of_picture) / 2;
        short current_Y = (height_of_console - height_of_picture) / 2;

        gotoxy(current_X, current_Y);  //  Перемещаем курсор на начальную позицию вывода

        for (int string_picture = 0; string_picture < height_of_picture; string_picture++) {  //  Цикл вывода строк для картинок

            if (Translate_the_number(Main_Data + 7) % 2 == 1) {  //  Учет Random переменной для вывода разных анимаций
                printf("%s", &picture_1[current_picture][string_picture][0]);  //  Вывод первой картинки
            }
            else {
                printf("%s", &picture_2[current_picture][string_picture][0]);  //  Вывод второй картинки
            }

            gotoxy(current_X, ++current_Y);  //  Перемещение курсора на следующую строку (стразу на изначальную позицию вывода)
        }

    }

    Sleep(1000);
    clrscr();  //  Очистка окна и окрас консоли в цвет
}

void Menu_Animation_Loading() {  //  Анимация загрузки программы
    ///  Функция лишь для вывода анимации загрузки во время работы программы
    ///     (Не отображает степень загрузки системы)

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и серыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    const char animation_loading[] = { "/-\\|" };  //  Анимация

    gotoxy(con_width_new() - 20, con_height_new() - 4);
    printf("Loading...");

    for (int step = 0; step < 2; step++) {

        short required_width_of_console = con_width_new() - 20;  //  Ширина консоли
        short required_height_of_console = con_height_new() - 4;  //  Высота консоли

        gotoxy(required_width_of_console, required_height_of_console);

        for (int step_of_animation = 0; step_of_animation < 4; step_of_animation++) {

            gotoxy(required_width_of_console + strlen("Loading..."), required_height_of_console);
            printf("%c", animation_loading[step_of_animation]);
            Sleep(250);

        }

    }

    clrscr();  //  Очистка окна и окрас консоли в цвет
    Sleep(1000);
}

void Menu_Animation_Table_of_records() {  //  Анимация загрузки таблицы рекордов
    ///  - Функция лишь анимация загрузки таблицы рекордов (не отображает загруженность системы)

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и серыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    //  Фигурки человечков
    char figures[3][46] = { "   ( ._.)                               LLL", "  (___)                                 (  (>",
                            "  /  |                                  /  \\" };
    //  Позиции загрузки
    char positions[5][45] = { "  ( 9 )9  [====                    ]  \\(*_*)","  ( 9 )9  [=========               ]  \\(*_*)",
                              "  ( 9 )9  [===============         ]  \\(*_*)","  ( 9 )9  [====================    ]  \\(*_*)",
                              "  ( 9 )9  [========================]  \\(*_*)" };

    const int width_of_picture = 44;  //  Ширина картинок
    const int height_of_picture = 4;  //  Высота картинок

    for (int step = 0; step < 5; step++) {  //  Цикл расположения прогресс-бара

        int width_of_console = con_width_new();  //  Ширина консоли
        int height_of_console = con_height_new();  //  Высота консоли

        gotoxy((width_of_console - width_of_picture) / 2, (height_of_console - height_of_picture) / 2 - 1);
        printf("%s", &figures[0][0]);  //  Первая фиксированная строка

        gotoxy((width_of_console - width_of_picture) / 2, (height_of_console - height_of_picture) / 2);  //  Центр консоли
        printf("%s", &positions[step][0]);  //  Прогресс-бар

        gotoxy((width_of_console - width_of_picture) / 2, (height_of_console - height_of_picture) / 2 + 1);
        printf("%s", &figures[1][0]);  //  Вторая фиксированная строка

        gotoxy((width_of_console - width_of_picture) / 2, (height_of_console - height_of_picture) / 2 + 2);
        printf("%s", &figures[2][0]);  //  Третья фиксированная строка

        if (step == 0) {  //  Условие выделения времени загрузки на загрузку
            Sleep(400);
        }
        else if (step == 1) {
            Sleep(450);
        }
        else if (step == 2) {
            Sleep(900);
        }
        else if (step == 3) {
            Sleep(200);
        }
        else if (step == 4) {
            Sleep(250);
        }

    }

    Sleep(150);
    clrscr();  //  Очистка окна и окрас консоли в цвет
}

void Menu_Close() {
    ///  Анимация закрытия программы (удаление строк и столбцов в рандомном порядке)

    int width_of_console = con_width_new();  //  Ширина консоли
    int height_of_console = con_height_new();  //  Высота консоли
    //  Массив для хранения номеров удаленных строк

    //  Массив для хранения номеров удаленных столбцов
    int* deleted_columns = malloc(width_of_console * sizeof(int));
    if (deleted_columns == NULL) {  //  Проверка на выделение памяти
        Choiced_option(5, "Error with allocating memory to the array in the Menu_Close() function!");
    }
    memset(deleted_columns, -1, width_of_console * sizeof(int));

    //  Используемое сгенерированное число (первоначальное значение - это число Random из файла Data)
    int num = Translate_the_number(Main_Data + 7);

    //  Цикл удаления (версия удаления стобцов)
    for (int step = 0; step < width_of_console; step++) {
        //  Генерация случайного числа
        num = LKG(num);
        //  Марка пробега по консоли и массиву
        int mark = 0;
        //  Поиск числа в массиве
        if (Seek(deleted_columns, width_of_console, num % width_of_console)) {  //  Если найден
            //  Пока мы не найдем несовпадение увеличиваем num
            while (Seek(deleted_columns, width_of_console, num % width_of_console)) {
                num = (num + 1) % width_of_console;
            }
        }
        //  Записываем новое число
        while (deleted_columns[mark] != -1 && mark++ != width_of_console) {}  //  Смещаемся до конца массива
        deleted_columns[mark] = num % width_of_console;

        mark = 0;  //  Обнуляем для работы в консоли
        gotoxy(num % width_of_console, 0);
        while (mark < height_of_console) {  //  Само удаление
            printf(" ");  //  Удаляем знак
            gotoxy(num % width_of_console, mark++);  //  Смещаемся вниз
        }

        Sleep(10);  //  Небольшая задержка
    }

    ExitProcess(EXIT_SUCCESS);
}
//  ===============================================  Меню игры  ========================================================
void Menu() {  //  Главное меню игры
    ///  Функция главного меню игры, с которого идет выбор анимаций и опций

    // Инициализируется консоль
    con_init(80, 25);

    Menu_Animation_Authors();  //  Запуск анимация авторов
    Menu_Animation_Loading();  //  Запуск анимации загрузки

    Menu_Animation(200, 0);  //  Запуск анимации появления меню
    //  >>  Меню запущено

    int code = 0;  //  Нажатый символ с клавиатуры
    int current_option = 0;  //  Текущая выделяемая опция
    while (1) {
        Menu_Animation(0, current_option);

        while (!key_is_pressed()) {
            code = key_pressed_code();  //  Считываем код символа

            if (code == KEY_UP) {  //  Если стрелка вверх
                if (current_option == 0) {  //  Если уже потолок опций
                    continue;
                }
                else {
                    current_option--;
                    break;
                }
            }
            if (code == KEY_DOWN) {  //  Если стрелка вниз
                if (current_option == 4) {  //  Если ниже некуда
                    continue;
                }
                else {
                    current_option++;
                    break;
                }
            }

            if (code == KEY_ENTER && current_option == 4) {  //  Выбран выход из программы
                Save_data(Main_Data);  //  Сохранение в файл данных
                Menu_Close();
                ExitProcess(EXIT_SUCCESS);  //  Чтобы компилятор не ругался на бесконечный цикл (программа закрывается еще в функции Menu_Close() )
            }
            if (code == KEY_ENTER) {  //  Введен ввод
                Choiced_option(current_option, "---");
            }
            //pause(40); // Небольная пауза (чтобы не загружать процессор)
        }

    }

}
//  =========================================  Обслуживающие функции  ==================================================
void Choiced_option(int option, char* text_of_error) {  //  Выбранная опция
    ///  Обслуживающая функция главного меню, однако используется и для вывода экрана с ошибкой работы (text_of_error - текст ошибки (только английский!) )
    ///     (В передаваемых зпараметрах - 5 (или больше) для вызова экрана ошибки) (Если вызвана не ошибка, то передавать text_of_error = "---")

    if (option == 0) {  //  Опция 'Играть'
        //TODO  Реализовать опцию игры
    }
    else if (option == 1) {  //  Опция 'Рекорды'
        //TODO  Реализовать опцию таблицы рекордов
    }
    else if (option == 2) {  //  Опция 'О программе'
        //TODO  Реализовать опцию 'о программе'
    }
    else if (option == 3) {  //  Опция 'Справка'
        //TODO  Реализовать справку
    }
    else {  //  Неверное число (либо ошибка работы программы)
        con_set_color(CON_CLR_BLACK, CON_CLR_RED);  //  Меняем на красный фон и черными буквами
        show_cursor(0);  //  Скрытие курсора
        clrscr();  //  Очистка окна и окрас консоли в цвет

        char Error[] = { "!!! ERROR !!!" };

        gotoxy((con_width_new() - strlen(Error)) / 2, con_height_new() / 2);
        printf("%s", Error);
        //  Вывод текста ошибки
        gotoxy((con_width_new() - strlen(text_of_error)) / 2, con_height_new() / 2 + 2);
        if (!strcmp(text_of_error, "---")) {  //  Другие ошибки
            printf("%s", text_of_error);
        }
        else {  //  Если ошибка из главного меню при неверной опции
            printf("Incorrect option!");
        }
        Sleep(500);

        getchar();
        ExitProcess(EXIT_FAILURE);
    }
}

int LKG(int seed) {
    ///  Линейно конгруентный генератор для генерации чисел от 0 до 1000
    return ((int) (48271 * seed + 1) % (int)pow(2, 31)) % 1000;
}

int Seek(int* massive, int length, int number) {
    ///  Функция поиска в массиве massive длины length числа number (0 - не найден, 1 - найден) (massive[i] == -1 - ячейка пуста)
    //  Марка пробега по массиву
    int mark = 0;
    //  Проверка на нахождение элемента (0 - не найден, 1 - найден)
    int check = 0;
    while (massive[mark] != -1 && mark != length) {
        if (massive[mark] == number) {
            check = 1;
            break;
        }
        mark++;
    }
    return check;
}

int Integrity_check(char* Data) {
    ///  Функция-проверка целостности файла Data.txt (1 - цело, 0 - повреждения)

    //  Марка пробега по массиву информации
    int mark = 0;
    //  Текущая записанная строка
    char current_string[100];
    //  Марка для текущей строки
    int mark_for_string = 0;
    memset(current_string, 0, sizeof (current_string));

    while (Data[mark] != '\n' && Data[mark] != '\0') {  //  Первая строка (Проверка на наличие "Random число\n" или "Random число\0")
        if (Data[mark] == ' ') {  //  Нахождение первого пробела
            if (strcmp(current_string, "Random") != 0) {
                return 0;
            } else {  //  Если совпадние было
                current_string[mark_for_string++] = Data[mark++];  //  Запись пробела
                while (Data[mark] >= '0' && Data[mark] <= '9') {
                    current_string[mark_for_string++] = Data[mark++];  //  Запись числа
                }
                if (!(current_string[mark_for_string - 1] >= '0' && current_string[mark_for_string - 1] <= '9')) {  //  Если не записано число
                    return 0;
                }
                if (Data[mark] != '\n' && Data[mark] != '\0') {  //  Если не встречен конец файла
                    return 0;
                }
                break;
            }
        }
        current_string[mark_for_string++] = Data[mark++];  //  Запись
    }

    return 1;
}

char* Open_the_data() {
    ///  - Функция открытия файла Data с основной информацией
    ///  - Возвращает адрес на массив с записанными данными (пустые элементы = 0)

    //  Открываем файл Data
    FILE* file = fopen("Data.txt", "r");
    //  Проверка открытия файла (1)
    if (file == NULL) {  //  Если файл не находится в ближайших файлах
        //  Открытие файла по пути создателя (лишь для создателя)
        file = fopen("C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt", "r");
        if (file == NULL) {  //  Если и в таком случае не был открыт файл
            //  Конец работы программы
            Choiced_option(5, "File Data doesn't exist!");
        }
    }

    fseek(file, 0, SEEK_END);  //  Перемещаемся в конец файла для получения последнего байта
    char* Data = malloc(sizeof(char) * ftell(file) + 100);  //  Выделяем требуемое количество памяти для массива + 100 элементов для возможности замены
    memset(Data, 0, sizeof(char) * ftell(file) + 1);  //  Обнуляем

    fclose(file);  ///  Поскольку не понятна работа функции freopen() используется метод закрытия и открытия файла
    file = fopen("Data.txt", "r");  //  Переоткрытие для считывания данных
    //  Проверка переоткрытия файла (2)
    if (file == NULL) {  //  Если файл не находится в ближайших файлах
        //  Открытие файла по пути создателя (лишь для создателя)
        file = fopen("C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt", "r");
        if (file == NULL) {  //  Если и в таком случае не был открыт файл
            //  Конец работы программы
            Choiced_option(5, "File Data doesn't exist!");
        }
    }
    int i = 0;  //  Переменная для записи элементов в Data
    while (fscanf(file, "%c", &Data[i++]) != EOF);  //  Запись файла

    //  Проверка целостности
    if (!Integrity_check(Data)) {  //  Если нарушена целостность
        Choiced_option(5, "File Data.txt damaged!");
    }

    fclose(file);  //  Закрываем файл
    return Data;
}

int Translate_the_number(char* start) {
    ///  - Функция перевода строчных цифр (из массива char) в int число
    ///  - start - адрес начала переводимого числа в массиве
    int number = 0;  //  Переводимое число
    int mark = 0;  //  Марка для пробега по массиву
    while (start[mark] <= '9' && start[mark] >= '0') {  //  Пока символы в пределе кодов цифр
        number = number * 10 + start[mark] - '0';  //  '0' = 48 - для перевода в число
        mark++;  //  Смещаемся по массиву
    }
    return number;
}

void Save_data(char* Data) {
    ///  Функция записи/обновления данных в файл Data.txt
    //  Открываем файл Data
    FILE* file = fopen("Data.txt", "w");
    //  Проверка открытия файла
    if (file == NULL) {  //  Если файл не находится в ближайших файлах
        //  Открытие файла по пути создателя (лишь для создателя)
        file = fopen("C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt", "w");
        if (file == NULL) {  //  Если и в таком случае не был открыт файл
            //  Конец работы программы
            Choiced_option(5, "File Data doesn't exist!");
        }
    }
    //  Марка для пробега по массиву данных
    int mark = 0;
    while (Data[mark] != '\0') {  //  Запись данных
        fprintf(file, "%c", Data[mark++]);
    }

    fclose(file);  //  Закрытие файла
}

void Shift(int Start, int How_many, int Where, char* text) {
    ///  Массив сдвига элементов
    if (Where == 0) {  //  Сдвиг влево
        while (text[Start] != '\0') {
            text[Start - How_many] = text[Start];
            Start++;
        }
        text[Start - How_many] = '\0';  //  Закрытие текста
    }
    else {  //  Сдвиг вправо
        unsigned int length = strlen(text);  //  Фиксация длины текста программы

        while (length != Start) {
            text[(length - 1) + How_many] = text[length - 1];
            text[length - 1] = ' ';

            if (length == strlen(text)) {
                text[length + How_many] = '\0';
            }  //  Закрытие строки
            length--;
        }
    }
}