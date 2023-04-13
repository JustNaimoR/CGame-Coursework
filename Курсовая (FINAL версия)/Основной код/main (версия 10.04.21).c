#define _CRT_SECURE_NO_WARNINGS

#include "wincon_new.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
//  ========================================  Анимации и оформление ====================================================
void Menu_Animation(int sec, int option);
void Menu_Animation_Authors();
void Menu_Animation_Loading();
void Menu_Animation_Table_of_records();
void Menu_Close();
void Help_Output(int option);
void Way_operation();
void Menu_PrePlay_Output_1(int option);
void Menu_PrePlay_Output_2();
unsigned char* First_word_Output();
//  ===============================================  Меню игры  ========================================================
void Menu();
//  =================================================  Игра  ===========================================================
void Pre_Game(int difficulty);
void Game_COMP(int difficulty, unsigned char* first_word);
void Game_players(unsigned char* first_word);
//  =============================================  Интерфейс игры  =====================================================
void Game_Interface_COMP();
/*  void Game_Interface_players(unsigned char playing_field[5][5], int turn, Current_option option);  (84 строка)*/
//  ============================================  Префиксное дерево  ===================================================
void Trie(const unsigned char* word_to_add);
int Search_word(const unsigned char* word);
//  ===============================================  Опции меню  =======================================================
void Option_About_program();
void Option_Help();
//  ==========================================  Опции справки меню  ====================================================
void Animation_operation();
//  =========================================  Обслуживающие функции  ==================================================
int Check_new_way(unsigned char* new_way);
void Choiced_option(int option, char* text_of_error);
void Choiced_help_option(int option);
void Choiced_option_Preplay();
int LKG(int seed);
int Seek(const int* massive, int length, int number);
int Translate_the_number(const char* start);
char* Translate_the_string(const char* start);
//  ====================================  Функции работы с файлом Data.txt  ============================================
int File_data_check(const char* Data);
char* Open_the_data();
void Save_data_in_file();
void Record_the_data_in_struct(char* Data);
FILE* Create_default_file();

//  Основные данные о игре из Data.txt
struct Main_Data {
    char* Data;  //  Массив со всеми данными
    int Random;  //  Random число из файла данных
    int Animation;  //  Включенные (1) / Отключенные (0) анимации
    char* User_dictionary;  //  Путь к пользовательскому словарю
} Main_Data;

//  Структуры и переменные для сессии игры ======================================================================  1  ==
typedef enum Difficulty_level {  //  Перечисляемый тип для уровня сложности игры
    TWO_PLAYERS,  //  Два игрока
    EASY,  //  Простой уровень
    NORMAL,  //  Нормальный уровень
    HARD  //  Сложный уровень
} Difficulty_level;
//  Подсвечиваемая ячейка на игровом поле
typedef enum Current_option {
    SKIP,  //  Опция "Пропустить ход?"
    NOTEPAD,  //  Опиця "Блокнот"
    CHECK_WORD,  //  Опиця "Проверить слово"
    PLAY_FIELD  //  Игровое поле
} Current_option;

typedef struct Player {  //  Игрок
    int Skips;  //  Количество пропусков ходов ( X <= 3 )
    int Number_of_points;  //  Количество очков на поле
    unsigned char** Notepad;  //  Записанные слова в блокноте ( X <= 9 )
    unsigned char* Name;  //  Название игрока (Игрок 1 / Игрок 2)
} Player;
//  =============================================================================================================  1  ==

///  *** Лежит здесь, поскольку выше не видит enum Current_option ***
void Game_Interface_players(unsigned char playing_field[5][5], int turn, Current_option option, int x, int y, struct Player* Player_1, struct Player* Player_2);

//  Структуры и переменные для работы trie ======================================================================  2  ==
struct block* Global_root;  //  Корень префиксного словаря в глобальной переменной
typedef struct element_of_block {  //  Символ (Элемент) блока
    int symbol_code;  //  Числовое значение символа
    struct block* next;  //  Адрес на последующие блоки
} Element;
typedef struct block {  //  Блок (Узел) дерева
    struct element_of_block* elements[50];  //  Массив адресов элементов в блоке
    int null_element;  //  NULL элемент для определения заглушки
} Block;
//  =============================================================================================================  2  ==

//TODO - Реализовать сложность игры
//TODO - Дабавление возможности считывания файлов с определенных папок по дефолту (Изначально стоит "C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt" как пример)
//TODO - Добавить управление путем к словарю

int main() {
    con_init(80, 25);

    unsigned char mass[][5] = { {224, 225, 226, 227, 228}, {229, 230, 231, 232, 233}, {234, 235, 236, 237, 238}, {239, 240, 241, 242, 243}, {244, 245, 246, 247, 248} };

    Player* Player_1 = (Player*)malloc((sizeof(Player)));
    Player_1->Name = (unsigned char*)"Игрок 1";  //  Имя
    Player_1->Skips = 0;                          //  Количество пропусков
    Player_1->Number_of_points = 0;               //  Количкство очков
    //  Notepad (выделяем память и обнуляем ее)
    Player_1->Notepad = (unsigned char**)malloc(sizeof(unsigned char*) * 9);
    for (int string_mark = 0; string_mark < 9; string_mark++) {
        Player_1->Notepad[string_mark] = (unsigned char*)malloc(sizeof(unsigned char) * 15);
    }
    //  Создаем второго игрока
    Player* Player_2 = (Player*)malloc((sizeof(Player)));
    Player_2->Name = (unsigned char*)"Игрок 2";  //  Имя
    Player_2->Skips = 0;                          //  Количество пропусков
    Player_2->Number_of_points = 0;               //  Количкство очков
    //  Notepad (выделяем память и обнуляем ее)
    Player_2->Notepad = (unsigned char**)malloc(sizeof(unsigned char*) * 9);
    for (int string_mark = 0; string_mark < 9; string_mark++) {
        Player_2->Notepad[string_mark] = (unsigned char*)malloc(sizeof(unsigned char) * 15);
    }
    memset(Player_1->Notepad, NULL, sizeof(unsigned char) * 15);
    memset(Player_2->Notepad, NULL, sizeof(unsigned char) * 15);


    Player_1->Notepad[0] = "ПРивет";
    Player_1->Notepad[1] = "Город";
    Player_1->Notepad[2] = "Дорога";

    Player_2->Notepad[0] = "Варштейн";
    Player_2->Notepad[1] = "Верую";


    Game_Interface_players(mass, 0, 0, 0, 0, Player_1, Player_2);
    Sleep(3000);
    Game_Interface_players(mass, 1, 0, 0, 0, Player_1, Player_2);
    return 0;

    Main_Data.Data = Open_the_data();  //  Открытие файла и запись основных данных в структуру Main_Data

    Menu();  //  Запуск меню и программы
    return 0;
}
//  ========================================  Анимации и оформление ====================================================
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

    const int width_of_table = 36;  //  Ширина меню
    const int height_of_table = 15;  //  Высота меню

    int mark_X = 0;  //  Марка для вывода знаков '='

    short width_of_console;  //  Ширина консоли
    short height_of_console;  //  Высота консоли

    if (!Main_Data.Animation) {  //  Если значение показа анимаций == 0
        sec = 0;  //  Время рисовки меню обнуляется
    }

    for (int step_of_output = 0; step_of_output < (con_width_new() - width_of_table) / 2; step_of_output += 3) {  //  Цикл анимации вывода '='

        width_of_console = con_width_new();
        height_of_console = con_height_new();

        gotoxy((short)mark_X, (short)((height_of_console - height_of_table) / 2));  //  Вверх + лево
        printf("===");

        gotoxy((short)(width_of_console - mark_X - 3), (short)((height_of_console - height_of_table) / 2));  //  Вверх + право
        printf("===");

        gotoxy((short)(width_of_console - mark_X - 3), (short)((height_of_console - height_of_table) / 2 + height_of_table - 1));  //  Низ + право
        printf("===");

        gotoxy((short)mark_X, (short)((height_of_console - height_of_table) / 2 + height_of_table - 1));  //  Низ + лево
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
                            gotoxy((short)((width_of_console - width_of_table) / 2), (short)((height_of_console - height_of_table) / 2 + string_of_console++));
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

        short current_X = (short)((width_of_console - width_of_picture) / 2);
        short current_Y = (short)((height_of_console - height_of_picture) / 2);

        gotoxy(current_X, current_Y);  //  Перемещаем курсор на начальную позицию вывода

        for (int string_picture = 0; string_picture < height_of_picture; string_picture++) {  //  Цикл вывода строк для картинок

            if (Main_Data.Random % 2 == 1) {  //  Учет Random переменной для вывода разных анимаций
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

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    const char animation_loading[] = { "/-\\|" };  //  Анимация

    //  Подсказки игры
    const char tips_word[] = { "### Подсказка ###" };
    const char tips[][250] = { "Будьте внимательны: может найдется ход и получше...", "Важное условие успеха - ваш словарный запас",
                               "Разнообразные ходы на \"перспективу\" - не самый плохой вариант", "Читайте книжки и не занимайтесь подобной фигней",
                               "Не спорьте с компьютером", "Поставьте зачет pls", "Mmmmmmm... monke", "Нашел бы занятие получше, чем в балду играть...",
                               "В конце концов, противник может вычислить вашу комбинацию и перехватить её", "Чем длиннее слово, тем больше за него дается очков",
                               "Цель игры – набрать максимально возможное количество очков до тех пор, \n         пока игровое поле не закончилось" };
    const int tips_koll = 11;  //  Количество tips

    gotoxy((short)(con_width_new() - 20), (short)(con_height_new() - 4));
    printf("Loading...");  //  Вывод loading... в требуемомо месте

    con_set_color(CON_CLR_CYAN, CON_CLR_BLACK);  //  Установка требуемых цветов и вывод подсказки
    gotoxy(6, (short)(con_height_new() - 5));
    printf("%s", tips_word);
    gotoxy(3, (short)(con_height_new() - 4));
    printf("%s", tips[Main_Data.Random % tips_koll]);
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

    Main_Data.Random = LKG(Main_Data.Random);  //  Обновляем значение Random

    for (int step = 0; step < 2; step++) {

        short required_width_of_console = (short)(con_width_new() - 20);  //  Ширина консоли
        short required_height_of_console = (short)(con_height_new() - 4);  //  Высота консоли

        gotoxy(required_width_of_console, required_height_of_console);

        for (int step_of_animation = 0; step_of_animation < 4; step_of_animation++) {

            gotoxy((short)(required_width_of_console + strlen("Loading...")), required_height_of_console);
            printf("%c", animation_loading[step_of_animation]);
            Sleep(250);

        }

    }

    clrscr();  //  Очистка окна и окрас консоли в цвет
    Sleep(1000);
}

void Menu_Animation_Table_of_records() {  //  Анимация загрузки таблицы рекордов
    ///  - Функция лишь анимация загрузки таблицы рекордов (не отображает загруженность системы)

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    //  Фигурки человечков
    const char figures[3][46] = { "   ( ._.)                               LLL", "  (___)                                 (  (>",
                                  "  /  |                                  /  \\" };
    //  Позиции загрузки
    const char positions[5][45] = { "  ( 9 )9  [====                    ]  \\(*_*)","  ( 9 )9  [=========               ]  \\(*_*)",
                                    "  ( 9 )9  [===============         ]  \\(*_*)","  ( 9 )9  [====================    ]  \\(*_*)",
                                    "  ( 9 )9  [========================]  \\(*_*)" };

    const int width_of_picture = 44;  //  Ширина картинок
    const int height_of_picture = 4;  //  Высота картинок

    for (int step = 0; step < 5; step++) {  //  Цикл расположения прогресс-бара

        int width_of_console = con_width_new();  //  Ширина консоли
        int height_of_console = con_height_new();  //  Высота консоли

        gotoxy((short)((width_of_console - width_of_picture) / 2), (short)((height_of_console - height_of_picture) / 2 - 1));
        printf("%s", &figures[0][0]);  //  Первая фиксированная строка

        gotoxy((short)((width_of_console - width_of_picture) / 2), (short)((height_of_console - height_of_picture) / 2));  //  Центр консоли
        printf("%s", &positions[step][0]);  //  Прогресс-бар

        gotoxy((short)((width_of_console - width_of_picture) / 2), (short)((height_of_console - height_of_picture) / 2 + 1));
        printf("%s", &figures[1][0]);  //  Вторая фиксированная строка

        gotoxy((short)((width_of_console - width_of_picture) / 2), (short)((height_of_console - height_of_picture) / 2 + 2));
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
    int num = Main_Data.Random;

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
        gotoxy((short)(num % width_of_console), 0);
        while (mark < height_of_console) {  //  Само удаление
            printf(" ");  //  Удаляем знак
            gotoxy((short)(num % width_of_console), (short)mark++);  //  Смещаемся вниз
        }

        Sleep(10);  //  Небольшая задержка
    }

    ExitProcess(EXIT_SUCCESS);
}

void Help_Output(int option) {
    ///  Функция вывода меню справки и её окраса (option - текущая подсвечиваемая функция)
    ///  - Ширина строк не будет совпадать с выделенным на них места, поскольку кириллица занимает вдвое больще места
    //  Очистка консоли
    show_cursor(0);
    clrscr();  //  Очистка окна и окрас консоли в цвет
    //  Массив с оформлением Справки
    char Strings_1[22][120] = { "                        [?] Справка [?]                       ",
                                "+------------------------------+-----------------------------+",
                                "             Действие                       Клавиша           ",
                                "+------------------------------+-----------------------------+",
                                "                                                              ",
                                "            Ход вверх         ==>        \"стрелка вверх\"       ",
                                "                                                              ",
                                "            Ход вниз          ==>        \"стрелка вниз\"        ",
                                "                                                              ",
                                "            Ход вправо        ==>       \"стрелка вправо\"      ",
                                "                                                              ",
                                "            Ход влево         ==>       \"Стрелка влево\"       ",
                                "                                                              ",
                                "              Выбор           ==>            Enter            ",
                                "                                                              ",
                                "              Выход           ==>             Esc             ",
                                "                                                              ",
                                "          Обновить окно       ==>             F1              ",
                                "+------------------------------------------------------------+",
                                "                           Настройки                          ",
                                "+------------------------------------------------------------+",
                                "                                                              " };
    char String_2[12][120] = { "                   ==== "," Стереть рекорды "," ====                ",
                              "                                                              ",
                              "                   ==== "," Анимация в игре "," ====               ",
                              "                                                              ",
                              "                ==== "," Словарь пользователя "," ====             ",
                              "+------------------------------------------------------------+" };
    const int width_of_picture = 62;  //  Ширина всей картинки меню
    const int height_of_picture_1 = 22;  //  Высота первой части меню
    const int height_of_picture_2 = 12;  //  высота второй части меню (с учетом разделения на 3 части - смотри массив)

    int width_of_console = con_width_new();  //  Определение ширины консоли
    int height_of_console = con_height_new();  //  Определение высоты косноли
    int current_string = (height_of_console - height_of_picture_2 + 5 - height_of_picture_1) / 2 + 1;  //  Номер текущей заполняемой строки (1 - поскольку нулевую строку выводим отедельно)

    //  Часть для окраски нулевой строки в красный цвет
    con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);  //  Окраска на черный фон с красными буквами
    gotoxy((short)((width_of_console - width_of_picture) / 2), (short)(current_string - 1));
    printf("%s", &Strings_1[0][0]);  //  Вывод нулевой строки
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем прежние цвета

    //  Цикл вывода картинки с 1-ого элемента до места окраски функций
    for (int string_1 = 1; string_1 < height_of_picture_1; string_1++) {
        gotoxy((short)((width_of_console - width_of_picture) / 2), (short)current_string++);  //  Перенос в требуемую точку
        printf("%s", &Strings_1[string_1][0]);  //  Вывод строки
    }
    //  Цикл вывода второй части меню справки
    for (int string_2 = 0; string_2 < height_of_picture_2; string_2++) {
        //  Условия для переноса курсора
        if (string_2 != 1 && string_2 != 2) {
            if (string_2 != 5 && string_2 != 6) {
                if (string_2 != 9 && string_2 != 10) {
                    gotoxy((short) ((width_of_console - width_of_picture) / 2), (short) (current_string++));  //  Перенос в требуемую точку
                }
            }
        }
        if (string_2 == 1 && option == 0 || string_2 == 5 && option == 1 || string_2 == 9 && option == 2) {  //  Услоиве окраса
            con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Меняем на белый фон с черными буквами
            printf("%s", &String_2[string_2][0]);  //  Вывод части с окрасом
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
        }
        else {
            printf("%s", &String_2[string_2][0]);  //  Вывод части
        }
    }
    //  >>  Выведенно меню справки с окраской определенной функции
}

void Way_operation() {
    ///  Функция-опция справки меню. Настривание пути к словарю пользователя
    con_set_color(CON_CLR_BLACK, CON_CLR_BLUE);  //  Меняем на синий фон и белыми буквами
    show_cursor(0);  //  Скрытие курсора
    //  Части сообщения
    char message[][100] = { "                                                  ","        Текущий путь к словарю пользователя:      ",
                            "                                                  ","                                                  ",
                            "(*)===                                      ===(*)","         Новый: (Для отмены введите \"---\")        ",
                            "                                                  ","(*)===                                      ===(*)",
                            "                                                  "};

    const int width_of_picture = 50;  //  Ширина
    const int height_of_picture = 9;  //  Высота

    int width_of_console = con_width_new();  //  Ширина консоли
    int height_of_console = con_height_new();  //  Высота консоли
    //  Вывод всего меню
    for (int current_string = 0; current_string < height_of_picture; current_string++) {
        gotoxy((short)((width_of_console - width_of_picture) / 2), (short)((height_of_console - height_of_picture) / 2 + current_string));
        printf("%s", message[current_string]);
    }
    //  Вывод текущего пути
    gotoxy((short)((width_of_console - strlen(Main_Data.User_dictionary)) / 2), (short)((height_of_console - height_of_picture) / 2 + 2));
    printf("%s", Main_Data.User_dictionary);

    show_cursor(1);  //  Показ курсора
    //  Требуемая строка
    unsigned char new_way[100];  //  Новый путь к словарб пользователя

    gotoxy((short)((width_of_console - width_of_picture) / 2 + 3), (short)((height_of_console - height_of_picture) / 2 + 6));
    scanf("%s", new_way);

    if (new_way[0] == '-' && new_way[1] == '-' && new_way[2] == '-' && new_way[3] == '\0') {
        //  Возвращаем изначальный вид
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        show_cursor(0);
        return;
    } else {  //  Если введен путь
        //  Проверка нового элемента
        if (!Check_new_way(new_way)) {  //  Случай ненахождения
            char error_message[][15] = { "              ","   NO  FILE   ","              " };

            con_set_color(CON_CLR_BLACK, CON_CLR_RED);
            for (int mark = 0; mark < 3; mark++) {
                gotoxy((short)((width_of_console - strlen("              ")) / 2),(short) ((height_of_console - 3) / 2 + mark));
                printf("%s", error_message[mark]);
            }
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

            Sleep(1000);
            return;
        }
        //  Копирование
        for (int mark = 0; new_way[mark - 1] != 0; mark++) {
            Main_Data.User_dictionary[mark] = (char) new_way[mark];
        }
        //  Возвращаем изначальный вид
        show_cursor(0);
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

        return;
    }
}

void Menu_PrePlay_Output_1(int option) {
    ///  Меню начала игры 1 - выбор игры (option - подсвечиваемая опция)
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет
    //  Вывод типа игры
    char play_gameplay[][50] = { "+--                   --+",
                                 "|   "," [P]  Играть  [P] ","  |",
                                 "+-----------------------+",
                                 "|  "," Против компьютера ","  |",
                                 "|      "," 2 игрока ","       |",
                                 "+--                   --+" };

    const int width_of_table = 25;  //  Ширина меню
    const int height_of_table = 6;  //  Высота меню
    int current_string = 0;  //  Текущая заполняемая строка

    short width_of_console = con_width_new();  //  Ширина консоли
    short height_of_console = con_height_new();  //  Высота консоли

    for (int mark = 0; mark < height_of_table + 6; mark++) {
        //  Требуемое место для курсора
        gotoxy((short)((width_of_console - width_of_table) / 2), (short)((height_of_console - height_of_table) / 2 + current_string++));
        //  Для составных строк
        if (mark == 1 || mark == 5 || mark == 8) {
            if (mark == 1) {  //  Строка "|    [P]  Играть  [P]   |"
                printf("%s", &play_gameplay[mark++][0]);  //  "|   "
                con_set_color(CON_CLR_CYAN, CON_CLR_BLACK);  //  Меняем на черный фон и бирюзовыми буквами
                printf("%s", &play_gameplay[mark++][0]);  //  " [P]  Играть  [P] "
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
                printf("%s", &play_gameplay[mark][0]);  //  "  |"
            }
            else {
                printf("%s", &play_gameplay[mark++][0]);
                if (mark == 6 && option == 0 || mark == 9 && option == 1) {  //  Подсвечивается
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Меняем на белый фон и черными буквами
                    printf("%s", &play_gameplay[mark++][0]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
                }
                else {  //  не подсвечивается
                    printf("%s", &play_gameplay[mark++][0]);
                }
                printf("%s", &play_gameplay[mark][0]);
            }
        }
        else {
            printf("%s", &play_gameplay[mark][0]);
        }
    }
}

void Menu_PrePlay_Output_2() {
    ///  Меню начала игры 2 - выбор сложности игры с компьютером (И меню, и вывод)
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет
    //  Вывод сложности игры
    char play_difficulty[][50] = { "+--                   --+",
                                   "|    == Сложность ==    |",
                                   "        "," Высокая ","        ",
                                   "        "," Средняя ","        ",
                                   "|       "," Низкая ","        |",
                                   "+--                   --+" };

    const int width_of_table = 25;  //  Ширина меню
    const int height_of_table = 6;  //  Высота меню
    int current_string = 0;  //  Текущая заполняемая строка в консоли

    short width_of_console;  //  Ширина консоли
    short height_of_console;  //  Высота консоли

    int code = 0;  //  Нажатый символ с клавиатуры
    int current_option = 0;  //  Текущая выделяемая опция
    while (1) {
        //  Рисовка 2 части
        show_cursor(0);  //  Скрытие курсора
        clrscr();  //  Очистка окна и окрас консоли в цвет

        width_of_console = con_width_new();
        height_of_console = con_height_new();

        for (int mark = 0; mark < height_of_table + 6; mark++) {
            gotoxy((short)((width_of_console - width_of_table) / 2), (short)((height_of_console - height_of_table) / 2 + current_string++));
            //  Составные строки
            if (mark == 2 || mark == 5 || mark == 8) {
                printf("%s", &play_difficulty[mark++][0]);  //  Первая часть составной строки
                if (mark == 3 && current_option == 0) {
                    con_set_color(CON_CLR_RED, CON_CLR_BLACK);  //  Меняем на черный фон и красными буквами
                    printf("%s", &play_difficulty[mark++][0]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
                }
                else if (mark == 6 && current_option == 1) {
                    con_set_color(CON_CLR_YELLOW, CON_CLR_BLACK);  //  Меняем на черный фон и желтыми буквами буквами
                    printf("%s", &play_difficulty[mark++][0]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
                }
                else if (mark == 9 && current_option == 2) {
                    con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и зелеными буквами
                    printf("%s", &play_difficulty[mark++][0]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
                }
                else {
                    printf("%s", &play_difficulty[mark++][0]);
                }
                printf("%s", &play_difficulty[mark][0]);  //  Третья часть составной строки
            }
            else {
                printf("%s", &play_difficulty[mark][0]);
            }
        }  //  >>  Выведена менюшка
        current_string = 0;

        if (!key_is_pressed()) {
            code = key_pressed_code();  //  Считываем код символа

            if (code == KEY_UP) {  //  Если стрелка вверх
                if (current_option == 0) {  //  Если уже потолок опций
                    continue;
                }
                else {
                    current_option--;
                    continue;
                }
            }
            if (code == KEY_DOWN) {  //  Если стрелка вниз
                if (current_option == 2) {  //  Если ниже некуда
                    continue;
                }
                else {
                    current_option++;
                    continue;
                }
            }
            if (code == KEY_ESC) {  //  Выход
                return;
            }
            if (code == KEY_F1) {  //  Обновить окно
                continue;
            }
            if (code == KEY_ENTER) {  //  Выбор
                if (Main_Data.Animation == 1) {
                    Sleep(500);  //  Задержка
                    clrscr();
                    Sleep(1000);  //  Задержка
                    Menu_Animation_Loading();
                }  //  Анимация загрузки
                if (current_option == 0) {  //  Высокая сложность
                    Pre_Game(HARD);
                }
                else if (current_option == 1) {  //  Средняя сложность
                    Pre_Game(NORMAL);
                }
                else if (current_option == 3) {  //  Низкая сложность
                    Pre_Game(EASY);
                }
                Menu();  //  Вызов меню (по завершению игры)
            }
        }
    }
}

unsigned char* First_word_Output() {
    //TODO  Проверить работу после установки словаря
    ///  Вывод таблички с просьбой ввести первое слово для начала игры и получение слова
    //  Окно ввода стартового слова
    char table[][100] = { "+------------------------------+",
                          "     Введите стартовое слово:   ",
                          "       ===  (5 букв)  ===       ",
                          "             _____              ",
                          "+------------------------------+" };
    //  Определяем размеры консоли
    short width_of_console = con_width_new();
    short height_of_console = con_height_new();

    int check_word;  //  Проверка слова на правильность

    const short length_of_table = (short) strlen(table[0]);

    unsigned char start_word[100];  //  Начальное слово (впиши больше 100 символов и посмотри чо будет. Поэтому их тут так много)
    while (1) {
        //  Очистка консоли для вывода
        clrscr();
        //  Вывод таблички
        for (int current_string = 0; current_string < 5; current_string++) {
            gotoxy((short)((width_of_console - length_of_table) / 2), (short)((height_of_console - 4) / 2 + current_string));
            printf("%s", table[current_string]);
        }

        gotoxy((short)((width_of_console - length_of_table) / 2 + 13), (short)((height_of_console - 4) / 2 + 3));
        show_cursor(1);  //  Показываем курсор (сейчас в точке ввод начального слова)

        con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
        scanf("%s", start_word);  //  Вводим слово
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

        int mark = 0;
        check_word = 0;
        while (start_word[mark] != 0) {
            //  Установленные допустимые символы - русские
            if (!(start_word[mark] >= 224 && start_word[mark] <= 255) || mark > 4) {
                break;
            }
            //  Установленная длина - 5 символов
            if (start_word[mark + 1] == '\0' && mark == 4) {
                check_word = 1;
                break;
            }
            mark++;
        }

        if (!check_word) {  //  Если слово не той длины
            show_cursor(0);
            //  Выводим красным цветом "INCORRECT" в месте ввода слова
            gotoxy((short)((width_of_console - length_of_table) / 2 + 11), (short)((height_of_console - 4) / 2 + 3));
            con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);
            printf("INCORRECT            ");
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
            Sleep(1000);

            gotoxy((short)((width_of_console - length_of_table) / 2), (short)((height_of_console - 4) / 2 + 3));
            printf("%s", table[3]);  //  Возвращаем изначальную строку (стираем слово INCORRECT)
        } else {
            //  >>  Данное слово состоит из русских букв, имеет длину 5 (Нужно проверить из словаря что слово существует)
            if (!Search_word(start_word)) {  //  В словаре такого слова нет
                show_cursor(0);
                //  Выводим красным цветом "Такого слова нет в словаре" в месте ввода слова
                gotoxy((short)((width_of_console - length_of_table) / 2 + 3), (short)((height_of_console - 4) / 2 + 3));
                con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);
                printf("Такого слова нет в словаре");
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                Sleep(1000);

                gotoxy((short)((width_of_console - length_of_table) / 2), (short)((height_of_console - 4) / 2 + 3));
                printf("%s", table[3]);  //  Возвращаем изначальную строку

                continue;  //  Возвращаемся к изначальному меню функции
            }

            show_cursor(0);
            start_word[5] = '\n';  //  Обязательный символ для добавление в дерево
            start_word[6] = '\0';  //  Закрываем строку
            Sleep(800);
            return start_word;
        }

    }
}
//  ===============================================  Меню игры  ========================================================
void Menu() {  //  Главное меню игры
    ///  Функция главного меню игры, с которого идет выбор анимаций и опций

    // Инициализируется консоль
    con_init(80, 25);

    if (Main_Data.Animation) {  //  Если значение показа анимаций == 1 (Есть разрешение)
        Menu_Animation_Authors();  //  Запуск анимация авторов
        Menu_Animation_Loading();  //  Запуск анимации загрузки
    }

    Menu_Animation(200, 0);  //  Запуск анимации появления меню
    //  >>  Меню запущено

    int code = 0;  //  Нажатый символ с клавиатуры
    int current_option = 0;  //  Текущая выделяемая опция
    while (1) {
        Menu_Animation(0, current_option);

        if (!key_is_pressed()) {
            code = key_pressed_code();  //  Считываем код символа

            if (code == KEY_UP) {  //  Если стрелка вверх
                if (current_option == 0) {  //  Если уже потолок опций
                    continue;
                }
                else {
                    current_option--;
                    continue;
                }
            }

            if (code == KEY_DOWN) {  //  Если стрелка вниз
                if (current_option == 4) {  //  Если ниже некуда
                    continue;
                }
                else {
                    current_option++;
                    continue;
                }
            }

            if (code == KEY_F1) {  //  Обновить окно
                continue;
            }
            if (code == KEY_ENTER && current_option == 4) {  //  Выбран выход из программы
                Save_data_in_file();  //  Сохранение в файл данных
                if (Main_Data.Animation) { Menu_Close(); }  //  Если дано разрешение на анимацию
                clrscr();  //  Очистка окна и окрас консоли в цвет
                ExitProcess(EXIT_SUCCESS);  //  На случай отказа показа анимаций
            }
            if (code == KEY_ENTER) {  //  Введен ввод
                Choiced_option(current_option, "---");
                continue;
            }
            //pause(40); // Небольная пауза (чтобы не загружать процессор)
        }

    }

}
//  =================================================  Игра  ===========================================================
void Pre_Game(Difficulty_level difficulty) {
    ///  Функция игры для преигровых действий (difficulty - сложность игры с роботом)

    //  Инициализация (создание) дерева-словаря
    struct block Root;  //  Создание корня префиксного дерева
    Global_root = (struct block *) &Root;  //  Запись адреса корня в глобальную переменную
    Root.null_element = 1;  //  Поскольку он является корневым, он не может быть заглушкой
    memset(Root.elements, 0, sizeof (Root.elements));  //  Обнуляем массив адресов элементов в корне
    //  >>  Создан корень дерева
    //  Открываем словари
    FILE* Main_dictionary = fopen("VOC.txt", "r");  //  Стандартный словарь игры
    FILE* User_dictionary = fopen(Main_Data.User_dictionary, "r");  //  Пользовательский словарь

    unsigned char string[50];  //  Записываемая строка
    if (Main_dictionary != NULL) {  //  Если найден начальный словарь
        //  Пока не найдется конец файла
        while(fgets(string, 50, Main_dictionary) != NULL) {
            Trie(string);  //  Добавление слова
        }
    }
    if (User_dictionary != NULL) {  //  Если найден пользовательский словарь
        while(fgets(string, 50, User_dictionary) != NULL) {
            Trie(string);  //  Добавление слова
        }
    }
    //  >>  Записаны все слова из двух библиотек
    ///  В случае ненахождения словарей - дерево будет пустым

    if (Root.elements[1] != NULL) {  //  Если добавлены слова
        unsigned char *first_word = First_word_Output();  //  Требование ввести первое слово и вывод меню

        if (difficulty == TWO_PLAYERS) {  //  Игра с человеком
            //  Создаем первого игрока и обнуляем его поля
            Player* Player_1 = (Player*) malloc((sizeof (Player)));
            Player_1 -> Name = (unsigned char*) "Игрок 1";  //  Имя
            Player_1 -> Skips = 0;                          //  Количество пропусков
            Player_1 -> Number_of_points = 0;               //  Количкство очков
            //  Notepad (выделяем память и обнуляем ее)
            Player_1 -> Notepad = (unsigned char**) malloc(sizeof (unsigned char*) * 9);
            for (int string_mark = 0; string_mark < 9; string_mark++) {
                Player_1 -> Notepad[string_mark] = (unsigned char* ) malloc(sizeof (unsigned char) * 15);
            }
            memset(Player_1->Notepad, (int) NULL, sizeof(unsigned char) * 15);
            //  Создаем второго игрока
            Player* Player_2 = (Player*) malloc((sizeof (Player)));
            Player_2 -> Name = (unsigned char*) "Игрок 2";  //  Имя
            Player_2 -> Skips = 0;                          //  Количество пропусков
            Player_2 -> Number_of_points = 0;               //  Количкство очков
            //  Notepad (выделяем память и обнуляем ее)
            Player_2 -> Notepad = (unsigned char**) malloc(sizeof (unsigned char*) * 9);
            for (int string_mark = 0; string_mark < 9; string_mark++) {
                Player_2 -> Notepad[string_mark] = (unsigned char* ) malloc(sizeof (unsigned char) * 15);
            }
            memset(Player_2->Notepad, (int) NULL, sizeof(unsigned char) * 15);
            //  >>  Готовы два игрока

            Game_players(first_word);  //  Начало игры
        } else {  //  Игра с компьютером
            //TODO  разобраться с игрой с компьютером
            Game_COMP(difficulty, first_word);
        }
    } else {  //  Если слов нет
        Choiced_option(5, "Vocabularies are empty or don't exist! Add some words in file \"VOC.txt\" of game");
    }
}

void Game_players(unsigned char* first_word) {
    ///  Основная функция игры двух людей
    //  Игровое поле
    unsigned char playing_field[5][5];
    memset(playing_field, 0, sizeof (playing_field));



}

void Game_COMP(Difficulty_level difficulty, unsigned char* first_word) {
    ///  Основная функция игры с компьютером


}
//  =============================================  Интерфейс игры  =====================================================
void Game_Interface_players(unsigned char playing_field[5][5], int turn, Current_option option, int x, int y, struct Player* Player_1, struct Player* Player_2) {
    ///  Игровое поле игры "TWO_PLAYERS"
    ///    playing_field - игровое поле и буквы на нем;
    ///    turn - чей сейчас ход (turn == 0 - ход первого игрока, 1 - второго)
    ///    option - текущая подсвечиваемая ячейка
    ///    x, y - Подсвечиваемые координаты на x,y (при option != PLAY_FIELD - никакой роли не играют)

    //  Ход игроков (для вывода)
    unsigned char players_turn[2][50] = { " ==  Ход Игрока 1  == "," ==  Ход Игрока 2  == " };

    clrscr();  //  Очищаем консоль
    show_cursor(0);  //  Убираем курсор
    //  Чертеж игрового поля
    unsigned char FIELD[][200] = { "+-----------+------------------------------------+--------------------+--------------------------+---------------------+",
                                   "   Рейтинг                                                                                       |                      ",
                                   "+-----------+                                    +--------------------+                          |                      ",
                                   "                                                                                                 |                      ",
                                   "                                                                                                 +---------------------+",
                                   "                                                                                                                        ",
                                   "                                               1     2     3     4     5                         +---------------------+",
                                   "                                             +---+ +---+ +---+ +---+ +---+                       |       Блокнот        ",
                                   "+-----------+                              1 |   | |   | |   | |   | |   |                       +---------------------+",
                                   "                                             +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "                                             +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "                                           2 |   | |   | |   | |   | |   |                       |                      ",
                                   "                                             +---+ +---+ +---+ +---+ +---+                       |                      ",
                                 "  (\\(\\                                       +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "  ( =':')                                  3 |   | |   | |   | |   | |   |                       |                      ",
                                 "  (..(\")(\")                                  +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "+-----------+                                +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "                                           4 |   | |   | |   | |   | |   |                       |                      ",
                                   "                                             +---+ +---+ +---+ +---+ +---+                       +---------------------+",
                                   "                                             +---+ +---+ +---+ +---+ +---+                                              ",
                                   "                                           5 |   | |   | |   | |   | |   |                                              ",
                                  " (\\__/)                                      +---+ +---+ +---+ +---+ +---+                                              ",
                                   " (='.'=)         (\\__/)                                                                                                ",
                                   " E[:]|||||[:]З   (O.o )                                                                                                 ",
                                   " (\")_(\")         (> < )                                                                                               ",
                                   "+---------------------+                                                                          +---------------------+",
                               "    Пропустить ход?                         (\\.../) (\\.../) (\\.../) (\\.../)                      |   Проверить слово    ",
                                   "                                            ( *.*) ('.'= ) ('.'= ) ('.'= )                       |     ___________      ",
                           "                                            (\")_(\") (\")_(\") (\")_(\") (\")_(\")                      |                      ",
                                   "+------------------------------------------------------------------------------------------------+---------------------+"};
    //  Размеры поля
    short width_of_field = 120;
    short height_of_field = 30;
    //  Размер консоли
    short width_of_console = con_width_new();
    short height_of_console = con_height_new();

    //  Цикл вывода шаблона ===================================================================================  1  ====
    for (int string = 0; string < height_of_field; string++) {
        //  Перемещаемся на точку вывода лчередной строки игрового поля
        gotoxy((short) ((width_of_console - width_of_field) / 2), (short) ((height_of_console - height_of_field) / 2 + string));
        printf("%s", FIELD[string]);
    }
    //  =======================================================================================================  1  ====

    //  Расскраска Некоторых участков в зависимости от очереди хода  ==========================================  2  ====
    if (turn == 0) {  //  Ход первого игрока
        con_set_color( CON_CLR_RED_LIGHT, CON_CLR_BLACK);  //  Красный шрифт и черный фон
    } else {  //  Ход второго игрока
        con_set_color( CON_CLR_BLUE_LIGHT, CON_CLR_BLACK);  //  Синий шрифт и черный фон
    }
    //  Строка 0
    gotoxy((short) ((width_of_console - width_of_field) / 2), (short) ((height_of_console - height_of_field) / 2));
    printf("%s", FIELD[0]);
    //  Строка 1
    gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("   Рейтинг                                       ")),
           (short) ((height_of_console - height_of_field) / 2 + 1));
    printf("%s", players_turn[turn]);
    //  Строка 2
    gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("+-----------+                                    ")),
           (short) ((height_of_console - height_of_field) / 2 + 2));
    printf("+--------------------+");
    //  Строка 29
    gotoxy((short) ((width_of_console - width_of_field) / 2),
           (short) ((height_of_console - height_of_field) / 2 + 29));
    printf("%s", FIELD[29]);

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем белые буквы и черный фон
    //  =======================================================================================================  2  ====

    //  Выставление значений на игровое поле  =================================================================  3  ====
    for (int mark_y = 0, mark_x = 0; mark_y < 5; ) {
        if (playing_field[mark_x][mark_y] == 0) {  //  Пропуск в случае пустой ячейки
            continue;
        }
        //  Точка вывода
        gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("                                             +-") + 6 * mark_x),
               (short) ((height_of_console - height_of_field) / 2 + 8 + 3 * mark_y));
        printf("%c", playing_field[mark_x][mark_y]);

        mark_x++;
        if (mark_x == 5) {  //  Конец строки поля
            mark_x = 0;
            mark_y++;
        }
    }
    //  =======================================================================================================  3  ====

    //  Вывод счета игроков  ==================================================================================  4  ====
    //  Вывод имен
    gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("   Рейтинг                                                                                       |  ")),
           (short) ((height_of_console - height_of_field) / 2 + 1));
    printf("%s    %s", Player_1 -> Name, Player_2 -> Name);
    //  Вывод счета
    gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("   Рейтинг                                                                                       |  ")),
           (short) ((height_of_console - height_of_field) / 2 + 3));
    printf("   %d          %d", Player_1 -> Number_of_points , Player_2 -> Number_of_points);
    //  =======================================================================================================  4  ====

    //  Вывод слов с блокнота  ================================================================================  5  ====
    if (turn == 0) {  //  Слова первого игрока
        for (int string = 0; Player_1 -> Notepad[string] != NULL; string++) {
            gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       | ")),
                   (short) ((height_of_console - height_of_field) / 2 + 9 + string));
            printf("%s", Player_1 -> Notepad[string]);
        }
    } else {  //  Слова второго игрока
        for (int string = 0; Player_2 -> Notepad[string] != NULL; string++) {
            gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       | ")),
                   (short) ((height_of_console - height_of_field) / 2 + 9 + string));
            printf("%s", Player_2 -> Notepad[string]);
        }
    }
    //  =======================================================================================================  5  ====
}

void Game_Interface_COMP() {
    ///  Интерфейс игры с компьютером

}
//  ============================================  Префиксное дерево  ===================================================
void Trie(const unsigned char* word_to_add) {
    ///  word_to_add'\n' - добавляемое слово в дерево (Обязательно наличие '\n' на конце) (Не работает в Clion, лишь в VS из-за разного представления кириллицы)
    ///  Данные по дереву сохраняются в глобально переменной Global_root
    struct block Root;

    struct block* dop_mark = Global_root;  //  Дополнительный адрес корня для работы
    int symbol_key;  //  Номер ключа слова (номера каждого его элемента на определенном этапе: 0 .. strlen(word))

    //  Основной цикл для слова word_to_add и добавление его в префиксное дерево
    for (symbol_key = 0; word_to_add[symbol_key] != '\n' && word_to_add[symbol_key] != '\0'; symbol_key++) {
        int check_mark = 0;  //  Отметка проверки нахождения элемента в блоке (0 - не найден)

        //  Цикл проверки на нахождение в блоке символа (Случай нахождения символа в блоке)
        for (int element_check = 1; dop_mark -> elements[element_check] != NULL; element_check++) {
            //  Если числовое значение символа равно значению уже записанного
            if (dop_mark -> elements[element_check] -> symbol_code == (int) word_to_add[symbol_key]) {
                dop_mark = dop_mark -> elements[element_check] -> next;  //  Переходим к новому блоку

                check_mark = 1;  //  Найден схожий элемент
                break;
            }
        }  //  >>  check_mark - Найден ли схожий элемент (Если да, dop_mark - адрес на последующий блок)

        if (!check_mark) {  //  Если не найден элемент (Случай ненахождения символа в блоке)

            Element* new_element = (Element*) malloc(sizeof (Element));  //  Новая структура под новый элемент в узле
            new_element -> symbol_code = (int) word_to_add[symbol_key];  //  Записываем текущий символ

            Block* new_block = (Block*) malloc(sizeof (Block));  //  Создание нового блока под новый символ
            new_block -> null_element = 0;  //  Заглушка - поскольку в нем ничего нет
            memset(new_block -> elements, 0, sizeof (new_block -> elements));  //  Обнуление новосозданного блока

            new_element -> next = new_block;  //  Записываем в блок адрес блока заглушки
            //  >>  Готовая новая ветка нового символа и заглушки к нему
            int mark_of_end = 1;  //  Марка для пробега в конец массива элементов
            while (dop_mark -> elements[mark_of_end] != NULL) {
                mark_of_end++;
            }  //  >>  elements[mark_of_end] - заполняемая ячейка новым символом
            dop_mark -> elements[mark_of_end] = new_element;  //  Записываем адрес нового готового элемента
            dop_mark -> null_element = 1;  //  Пскольку мы в него перешли в новый блок, предыдущий не является заглушкой

            dop_mark = dop_mark -> elements[mark_of_end] -> next;  //  Переходим в новую созданную ячейку

            if (symbol_key + 1 == strlen((char*) word_to_add) - 1) {  //  Если достигнут конец записываемого слова (Учет случая записи "аб", а после "абордаж")
                Element* new_element = (Element*) malloc(sizeof (Element));  //  Новая структура для элемента-заглушки в заглушку-блок
                new_element -> symbol_code = 'E';  //  Значение символа-заглушки

                dop_mark -> elements[0] = new_element;
            }
        }


    }  //  >>  Записанное слово word_to_add в дерево-словарь

    if (dop_mark -> null_element == 1) {  //  Случай "aб" при записанном "абордаж" (Слово входит в записанное ранее) => нет заглушки
        Element* new_element = (Element*) malloc(sizeof (Element));  //  Новая структура для элемента-заглушки
        new_element -> symbol_code = 'E';  //  Значение символа-заглушки

        dop_mark -> elements[0] = new_element;  //  Запись в отдельную ячейку под заглушку
    }

    /*  Trie - префиксное дерево. Лучше всего подходит для хранения слов из словаря
     *      В каждом узле дерева (структура Block) находятся записанные символы (структуры Elements)
     *      В стурктуре Block 2 поля - elements* (Адреса на структуры элементов в данном блоке), null_element - показатель заглушки-узла
     *          - Заглушка-узел - блок со значением null_element равным 0 (в других ситуациях 1), который является концом записанного слова
     *              (как у слова '\0' в конце, так и здесь).
     *          - elements[0] - специальная ячейка для хранения заглушки-символа (когда записываемое слово входит полностью в ранее записанное)
     *              ("аб" и ранее записанное "абордаж"). Если таких случаев не было - element[0] == NULL
     *      В структуре Element также 2 поля - symbol_code (числовое значение символа/элемента), next* - адрес на последующий блок,
     *          свюзующийся через этот символ
     *  Каждый блок - просто группа встречающихся символов на определенном этапе. Они связаны через предыдущие по пути элементы
     *      Совокупность блоков по определенному пути + заглушка - записанное ранее слово.
     *  Заглушка - узел (null_element == 0 у узла) или элемент (element[0] != NULL у элемента), показывающие конец слова в дереве.
     *      Узел в себе имеет заглушку-элемент (для учета случая записи "аб" и после "абордаж")
     *
     *           ===  Для более точной картины смотри "Структурное дерево для хранения словаря (1)" и "(2)" ===
     * */
}

int Search_word(const unsigned char* word) {
    ///  Функция поиска слова из дерева (Не работает в Clion, лишь в VS из-за разного представления кириллицы)
    int symbol_key = 0;  //  Номер текущего ключа слова word
    struct block* dop_mark = Global_root;  //  Адрес на текущий блок

    while (word[symbol_key] != '\0') {  //  Цикл поиска по дереву
        int check = 0;  //  Переменная-проверка найден ли в блоке очередной ключ слова
        //  Цикл проверки существования ключа в данном блоке
        for (int mark = 1; dop_mark -> elements[mark] != NULL; mark++) {
            //  Условие проверки
            if (dop_mark -> elements[mark] -> symbol_code == word[symbol_key]) {
                dop_mark = dop_mark -> elements[mark] -> next;  //  переходим в новый блок
                check = 1;
                break;
            }
        }  //  >>  check - нашелся ли элемент в блоке

        if (!check) {  //  Если не найден ключ
            return 0;
        }

        symbol_key++;
    }  //  >>  (+) - dop_mark - адрес на возможную заглушку; (-) - Слово не найдено и функция завершщится до конца while

    if (dop_mark -> null_element == 0 || dop_mark -> elements[0] != NULL) {  //  Если заглушка-блок или найдена заглушка-символ (Также и при elements[0] != NULL - есть заглушка-символ)
        return 1;
    } else {
        return 0;
    }
}
//  ==============================================  Опции меню  ========================================================
void Option_About_program() {
    ///  Опция меню основной информации о программе
    //  Сообщения авторов
    const char Message_1[] = { "Создатели: Лещинский Р.Г., Комисаров Г.Н., Группа 4851003/00001" };
    const char Message_2[] = { "Год: 2021, Политехнический университет Петра Великого" };
    const char Message_3[] = { "Интитут Кибер безопасности и защиты информации (ИКиЗИ)" };
    //  Текст информации
    const char Message_4[] = { "Дата релиза: ___" };
    const char Message_5[10][200] = { " Роман: Сильно понравилась идея курсовой. С 12.03.21 уже сидел и взахлеб",
                                      "программировал её, несмотря на другие занятия.",
                                      "Более всего понравились анимации, что породило штуки 5.",
                                      "Были мысли насчет пасхалки, однако все-же времени маловато было.",
                                      "Сама по себе игра не очень интересная (Ну \"Балда\", камон. Кто в такое играет?),",
                                      "но оформление дало возможность полностью погрузиться в работу и ",
                                      "писать понятный код с огромным количеством комментариев и понятными функциями." };
    //  Информация для выхода
    const char Message_6[] = { "Press ESC for exit" };

    int width_of_console = con_width_new();  //  Ширина консоли
    int height_of_console = con_height_new();  //  Высота консоли

    clrscr();  //  Очистка окна и окрас консоли в цвет
    con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и красными буквами
    show_cursor(0);  //  Скрытие курсора

    gotoxy((short)((width_of_console - strlen(Message_1)) / 2), 2);  //  Смещаемся для первого сообщения
    printf("%s", Message_1);  //  Вывод первого сообщения

    gotoxy((short)((width_of_console - strlen(Message_2)) / 2), 3);  //  Смещаемся для второго сообщения
    printf("%s", Message_2); //  Вывод второго сообщения

    gotoxy((short)((width_of_console - strlen(Message_3)) / 2), 4);  //  Смещаемся для третьего сообщения
    printf("%s", Message_3); //  Вывод третьего сообщения

    gotoxy((short)((width_of_console - strlen(Message_4)) / 2), 6);  //  Смещаемся для четвертого сообщения
    printf("%s", Message_4); //  Вывод четвертого сообщения

    con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и зелеными буквами

    for (int string = 8, mark = 0; string < 15; string++, mark++) {
        gotoxy((short)((width_of_console - strlen(&Message_5[mark][0])) / 2), (short)string);
        printf("%s", &Message_5[mark][0]); //  Вывод части пятого сообщения
    }

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и серыми буквами

    gotoxy((short)((width_of_console - strlen(Message_6)) / 2 + width_of_console / 3), 17);  //  Смещаемся для шестого сообщения
    printf("%s", Message_6); //  Вывод шестого сообщения

    int code;  //  Код символа
    while (!key_is_pressed()) {  //  Цикл для выхода
        code = key_pressed_code();  //  Считываем код символа

        if (code == KEY_ESC) {  //  Для выхода использовать ESC
            return;
        }
    }
}

void Option_Help() {
    ///  Опция меню 'Справка'
    //  Подготовка консоли к выводу меню
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и серыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    int code;  //  Код нажатой клавиши

    int current_option = 0;  //  Текущая подсвечиваемая опция в Справке

    //  Основной цикл оформления
    while (1) {

        Help_Output(current_option);  //  Зарисовка меню
        //  Цикл клавиш
        if (!key_is_pressed()) {
            code = key_pressed_code();  //  Запись кода нажатой клавиши

            if (code == KEY_ESC) {  //  Если клавиша выхода
                return;
            }
            if (code == KEY_UP && current_option > 0) {  //  Если клавиша вверх
                current_option--;
                continue;
            }
            if (code == KEY_DOWN && current_option < 2) {  //  Если клавиша вниз
                current_option++;
                continue;
            }
            if (code == KEY_ENTER) {  //  Если выбор
                Choiced_help_option(current_option);  //  Вызов обслуживающей функции
                continue;
            }
            if (code == KEY_F1) {  //  Обновить окно
                continue;
            }
        }
    }
}
//  ==========================================  Опции справки меню  ====================================================
void Animation_operation() {
    ///  Функция-опция справки меню. Отключает/включает показываемую анимацию
    con_set_color(CON_CLR_BLACK, CON_CLR_BLUE);  //  Меняем на синий фон и белыми буквами
    show_cursor(0);  //  Скрытие курсора
    //  Части сообщения
    char message[][25] = { "                     ","    Анимация - ", "                     ", "ON    ","OFF   ", };

    const int width_of_picture = 21;  //  Ширина
    const int height_of_picture = 3;  //  Высота

    int width_of_console = con_width_new();  //  Ширина консоли
    int height_of_console = con_height_new();  //  Высота консоли
    //  Переход на точку вывода и вывод первой части
    gotoxy((short)((width_of_console - width_of_picture) / 2), (short)((height_of_console - height_of_picture) / 2));
    printf("%s", &message[0][0]);
    //  Цикл для действующей кнопки смены
    while (1) {
        //  Запись остального сообщения
        gotoxy((short)((width_of_console - width_of_picture) / 2), (short)((height_of_console - height_of_picture) / 2 + 1));
        printf("%s", &message[1][0]);  //  "Анимация - "
        if (Main_Data.Animation == 1) {  //  Если включена
            con_set_color(CON_CLR_GREEN, CON_CLR_BLUE);  //  Устанавливаем черный фон и зеленый шрифт
            printf("%s", &message[3][0]);  //  Вывод "ON"
            con_set_color(CON_CLR_BLACK, CON_CLR_BLUE);  //  Меняем на черный фон и белыми буквами
        }
        else {  //  Если выключена
            con_set_color(CON_CLR_RED, CON_CLR_BLUE);
            printf("%s", &message[4][0]);  //  Вывод "OFF"
            con_set_color(CON_CLR_BLACK, CON_CLR_BLUE);
        }
        gotoxy((short)((width_of_console - width_of_picture) / 2), (short)((height_of_console - height_of_picture) / 2 + 2));
        printf("%s", &message[2][0]);
        //  Нажатая клавиша
        int code = key_pressed_code();
        while (!key_is_pressed()) {
            if (code == KEY_ENTER || code == KEY_UP || code == KEY_DOWN) {  //  Смена
                Main_Data.Animation = (Main_Data.Animation + 1) % 2;  //  Смена 0 на 1, 1 на 0
                break;
            }
            if (code == KEY_ESC) {  //  Выход
                //  Возвращаем изначальный вид
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                return;
            }
            if (code == KEY_F1) {  //  Обновление окна
                continue;
            }
        }
    }
}
//  =========================================  Обслуживающие функции  ==================================================
int Check_new_way(unsigned char* new_way) {
    ///  Проверка нового введенного пути к пользовательскому словарю
    FILE* new_file = fopen(new_way, "r");

    if (new_file == NULL) {
        return 0;
    } else {
        return 1;
    }
}

void Choiced_option(int option, char* text_of_error) {  //  Выбранная опция
    ///  Обслуживающая функция главного меню, однако используется и для вывода экрана с ошибкой работы (text_of_error - текст ошибки (только английский!) )
    ///     (В передаваемых зпараметрах - 5 (или больше) для вызова экрана ошибки) (Если вызвана не ошибка, то передавать text_of_error = "---")

    if (option == 0) {  //  Опция 'Играть'
        Choiced_option_Preplay();
    }
    else if (option == 1) {  //  Опция 'Рекорды'
        //TODO  Реализовать опцию таблицы рекордов
        Choiced_option(5, "NOT DONE YET!!!");
    }
    else if (option == 2) {  //  Опция 'О программе'
        Option_About_program();  //  Вызов опции
    }
    else if (option == 3) {  //  Опция 'Справка'
        Option_Help();
    }
    else {  //  Неверное число (либо ошибка работы программы)
        con_set_color(CON_CLR_BLACK, CON_CLR_RED);  //  Меняем на красный фон и черными буквами
        show_cursor(0);  //  Скрытие курсора
        clrscr();  //  Очистка окна и окрас консоли в цвет

        char Error[] = { "!!! ERROR !!!" };

        gotoxy((short)((con_width_new() - strlen(Error)) / 2), (short)(con_height_new() / 2));
        printf("%s", Error);
        //  Вывод текста ошибки
        gotoxy((short)((con_width_new() - strlen(text_of_error)) / 2), (short)(con_height_new() / 2 + 2));
        if (strcmp(text_of_error, "---") != 0) {  //  Другие ошибки
            printf("%s", text_of_error);
        }
        else {  //  Если ошибка из главного меню при неверной опции
            printf("Incorrect option!");
        }
        Sleep(2000);

        //getchar();
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и серыми буквами
        show_cursor(0);  //  Скрытие курсора
        clrscr();  //  Очистка окна и окрас консоли в цвет
        gotoxy(0, 1);

        ExitProcess(EXIT_FAILURE);
    }
}

void Choiced_help_option(int option) {
    ///  Функция справки для выбора соответствующей настройки
    if (option == 0) {  //  Опция 'Стереть рекорды'
        //TODO  Реализовать функцию стирания рекордов (в случае несуществования таблицы - создать default file)
        Choiced_option(5, "NOT DONE YET!!!");
    }
    else if (option == 1) {  //  Опция 'Анимации в игре'
        Animation_operation();
    } else if (option == 2) {  //  Опция 'Словарь пользователя'
        Way_operation();
    }
}

void Choiced_option_Preplay() {
    ///  Функция выбора опции в меню для игровых настроек
    int code = 0;  //  Нажатый символ с клавиатуры
    int current_option = 0;  //  Текущая выделяемая опция

    Sleep(200);  //  Небольшая задержка чтобы глаза не кололо
    while (1) {
        Menu_PrePlay_Output_1(current_option);  //  вывод первой части меню

        if (!key_is_pressed()) {
            code = key_pressed_code();  //  Считываем код символа

            if (code == KEY_UP) {
                if (current_option == 0) {  //  Если уже потолок опций
                    continue;
                }
                else {
                    current_option--;
                    continue;
                }
            }
            if (code == KEY_DOWN) {
                if (current_option == 1) {  //  Если ниже некуда
                    continue;
                }
                else {
                    current_option++;
                    continue;
                }
            }
            if (code == KEY_F1) {  //  Обновить окно
                continue;
            }
            if (code == KEY_ENTER) {  //  Выбранная опция
                if (current_option == 0) {  //  Против компьютера
                    Menu_PrePlay_Output_2();
                    continue;
                }
                if (current_option == 1) {  //  Против игрока
                    if (Main_Data.Animation == 1) {
                        Sleep(500);  //  Задержка
                        clrscr();
                        Sleep(1000);  //  Задержка
                        Menu_Animation_Loading();
                    }  //  Анимация загрузки
                    Pre_Game(TWO_PLAYERS);
                    continue;
                }
            }
            if (code == KEY_ESC) {  //  Выход
                return;
            }
        }
    }
}

int LKG(int seed) {
    ///  Линейно конгруентный генератор для генерации чисел от 0 до 1000
    return ((int)(48271 * seed + 1) % (int)pow(2, 31)) % 1000;
}

int Seek(const int* massive, int length, int number) {
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

int Translate_the_number(const char* start) {
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

char* Translate_the_string(const char* start) {
    ///  Функция, возвращающая строку до символа '\n' или '\0'
    ///  start - адрес начала строки
    int mark = 0;
    char* string = malloc(sizeof (char) * 200 );  //  Финальная строка (200 символов - макс.)
    while (start[mark] != '\0' && start[mark] != '\n') {
        string[mark] = start[mark];
        mark++;
    }
    //  Закрываем строку
    string[mark] = '\0';

    return string;
}
//  ====================================  Функции работы с файлом Data.txt  ============================================
int File_data_check(const char* Data) {
    ///  Функция-проверка целостности файла Data.txt (1 - цело, 0 - повреждения)

    //  Марка пробега по массиву информации
    int mark = 0;
    //  Текущая записанная строка
    char current_string[100];
    //  Марка для текущей строки
    int mark_for_string = 0;
    memset(current_string, 0, sizeof(current_string));

    //  Проверка #1 ("Random число\n") - первая строка
    while (Data[mark] != '\n' && Data[mark] != '\0') {  //  Первая строка
        if (Data[mark] == ' ') {  //  Нахождение первого пробела
            if (strcmp(current_string, "Random") != 0) {
                return 0;
            }
            else {  //  Если совпадние было
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

    //  Проверка #2 ("Animation число\n") - вторая строка
    mark = 0;  //  Обнуляем для повторной работы
    mark_for_string = 0;  //  Обнуляем для повторной работы
    memset(current_string, 0, sizeof(current_string));  //  Обнуляем для повторной работы

    char* point = strstr(Data, "Animation");  //  Получение адреса первого вхождения (Метки Animation)
    if (point == NULL) {  //  Если не бы найден
        return 0;
    }
    while (*(point + mark - 1) != ' ' && *(point + mark) != '\0') {  //  Запись в current_string метки (для наглядности. Не является необходимым)
        current_string[mark_for_string++] = *(point + mark++);
    }
    //  >>  "Animation "
    while (*(point + mark) != ' ' && *(point + mark) != '\0' && *(point + mark) != '\n') {  //  Запись числа
        current_string[mark_for_string++] = *(point + mark++);
    }
    if (mark_for_string != 11) {  //  Проверка на запись лишь одного символа после пробела
        return 0;
    }
    if (current_string[mark_for_string - 1] != '0' && current_string[mark_for_string - 1] != '1') {  //  Если было записано не число 0/1
        return 0;
    }

    //  Проверка #3 ("User_dictionary путь\n") - третья строка
    mark = 0;  //  Обнуляем для повторной работы
    mark_for_string = 0;  //  Обнуляем для повторной работы
    memset(current_string, 0, sizeof(current_string));  //  Обнуляем для повторной работы

    point = strstr(Data,"User_dictionary");  //  Получение адреса первого вхождения (Метки User_dictionary)
    if (point == NULL) {  //  Если не был найден
        return 0;
    }
    while (*(point + mark - 1) != ' ' && *(point + mark) != '\0') {  //  Запись в current_string метки (для наглядности. Не является необходимым)
        current_string[mark_for_string++] = *(point + mark++);
    }
    //  >>  "User_dictionary "
    if (*(point + mark) == ' ') {  //  Проверка на запись лишь одного пробела
        return 0;
    }

    return 1;
}

char* Open_the_data() {
    ///  - Функция открытия файла Data.txt с основной информацией
    ///  - Возвращает адрес на массив с записанными данными (пустые элементы = 0)

    //  Открываем файл Data
    FILE* file = fopen("C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt", "r");
    //  Проверка открытия файла (1)
    if (file == NULL) {  //  Если файл не находится в ближайших файлах
        //  Открытие файла по пути создателя (лишь для создателя)
        file = fopen("Data.txt", "r");
        if (file == NULL) {  //  Если и в таком случае не был открыт файл
            //  Создание default версии
            file = Create_default_file();
            //  Choiced_option(5, "File Data doesn't exist!");  // - в случае несоздания файла а просто вывода ошибки
        }
    }

    fseek(file, 0, SEEK_END);  //  Перемещаемся в конец файла для получения последнего байта
    char* Data = malloc(sizeof(char) * ftell(file) + 100);  //  Выделяем требуемое количество памяти для массива + 100 элементов для возможности замены
    memset(Data, 0, sizeof(char) * ftell(file) + 100);  //  Обнуляем

    fclose(file);  ///  Поскольку не понятна работа функции freopen() используется метод закрытия и открытия файла
    //  Открытие файла по пути создателя (лишь для создателя)
    file = fopen("C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt", "r"); //  Переоткрытие для считывания данных
    //  Проверка переоткрытия файла (2)
    if (file == NULL) {  //  Если файл не найден в папке создателя
        file = fopen("Data.txt", "r");
    }
    int i = 0;  //  Переменная для записи элементов в Data
    while (fscanf(file, "%c", &Data[i++]) != EOF);  //  Запись файла

    //  Проверка целостности
    if (!File_data_check(Data)) {  //  Если нарушена целостность
        Choiced_option(5, "File Data.txt damaged!");
    }

    Record_the_data_in_struct(Data);  //  Запись данных в структуру

    fclose(file);  //  Закрываем файл
    return Data;
}

void Save_data_in_file() {
    ///  Функция записи/обновления данных в файл Data.txt из массива Data[]
    //  Обновление значения Random
    Main_Data.Random = LKG(Main_Data.Random);
    //  Для проверки с какой системы открывается файла (Если из создателя - файл корректируется в папке проекта)
    FILE* file = fopen("C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt", "r");
    if (file == NULL) {  //  В случае другой системы
        //  Открываем файл Data в ближайшей папке
        file = fopen("Data.txt", "w");
    }
    else {  //  В случае системы создателя
        //  Открываем(создаем) для записи
        file = fopen("C:\\Users\\Romario\\CLionProjects\\Course_university\\Data.txt", "w");
    }
    //  Проверка открытия файла
    if (file == NULL) {  //  Если в таком случае не был открыт файл
        //  Конец работы программы
        Choiced_option(5, "File Data can't be opened!");
    }
    //  Запись значений структуры
    fprintf(file, "Random %d\nAnimation %d\nUser_dictionary %s\n", Main_Data.Random, Main_Data.Animation, Main_Data.User_dictionary);

    fclose(file);  //  Закрытие файла
}

void Record_the_data_in_struct(char* Data) {
    ///  Запись информации в файле Data.txt в структуру Main_Data для удобства получения значений
    char* point;
    //  Запись значения Random в структурное поле
    point = strstr(Data, "Random");  //  Запись адреса на тип Random в файле
    Main_Data.Random = Translate_the_number(point + 7);
    //  Запись значения Animation в структурное поле
    point = strstr(Data, "Animation");  //  Запись адреса на тип Animation в файле
    Main_Data.Animation = Translate_the_number(point + 10);
    //  Запись значения User_dictionary в структурное поле
    point = strstr(Data, "User_dictionary");  //  Запись адреса на тип User_dictionary в файле
    Main_Data.User_dictionary = Translate_the_string(point + strlen("User_dictionary "));
}

FILE* Create_default_file() {
    ///  Функция для создания default версии файла Data.txt в случае его отсутствия (возвращает ссылку на default файл)
    ///    default значения - Random 0; Animation 1; User_dictionary VOC.txt;
    FILE* default_file;  //  Новый default файл
    //  Создание файла и заполнение стандартных данных
    fopen_s(&default_file, "Data.txt", "w");

    fprintf_s(default_file, "Random 0\nAnimation 1\nUser_dictionary VOC.txt");

    return default_file;
}