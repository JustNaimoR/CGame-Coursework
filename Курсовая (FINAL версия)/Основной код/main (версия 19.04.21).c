#define _CRT_SECURE_NO_WARNINGS

#include "wincon_new.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
//  ========================================  Анимации и оформление ====================================================
void MainMenu_View_Animation(int sec, int option);
void MainMenu_Animation_Authors();
void MainMenu_Animation_Loading();
void MainMenu_Animation_TableOfRecords();
void OptionTableOfRecords_View();
void MainMenu_Animation_Close();
void MainMenu_OptionHelp_View(int option);
void OptionPlay_PrePlay_View(int option);
void OptionPlay_DifficultySelection_View(int current_option);
unsigned char* FirstGameWord_View();
void Game_EndOfGame_View(int num);

//  Основные данные о игре из Data.txt
struct Main_Data {
    char* Data;  //  Массив со всеми данными
    int Random;  //  Random число из файла данных
    int Animation;  //  Включенные (1) / Отключенные (0) анимации
    char* User_dictionary;  //  Путь к пользовательскому словарю
} Main_Data;

//  Структуры и переменные для работы trie ======================================================================  1  ==
struct block* Global_root;  //  Корень префиксного словаря в глобальной переменной
typedef struct element_of_block {  //  Символ (Элемент) блока
    int symbol_code;  //  Числовое значение символа
    struct block* next;  //  Адрес на последующие блоки
} Element;
typedef struct block {  //  Блок (Узел) дерева
    struct element_of_block* elements[50];  //  Массив адресов элементов в блоке
    int null_element;  //  NULL элемент для определения заглушки
} Block;
//  =============================================================================================================  1  ==

//  Структуры и переменные для сессии игры ======================================================================  2  ==
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
    unsigned char Notepad[9][15];  //  Записанные слова в блокноте
    unsigned char* Name;  //  Название игрока (Игрок 1 / Игрок 2)
} Player;

typedef struct Word_list {
    unsigned char word[30];  //  Записанное слово
    struct List *next;  //  Следущая часть списка
} List;
List* BeginOfList;  //  Адрес на начало списка использованных слов
//  =============================================================================================================  2  ==

//  Линейный список и прочее для записи рекордов ================================================================  3  ==
typedef struct Records_List {
    unsigned char name[4];  //  Имя игрока из списка
    int number_of_points;  //  Количество очков за игру
    struct Records_List* next;  //  Следующий элемент списка
} Records_list;

Records_list* First_Page;  //  Адрес на первый элмент в списке
//  =============================================================================================================  3  ==

//  ===============================================  Меню игры  ========================================================
void Menu();
//  =================================================  Игра  ===========================================================
void OptionPlay_DifficultySelection();
void Pre_Game(int difficulty);
void Game_COMP(int difficulty, unsigned char* first_word);
void TwoPlayersGame(unsigned char* first_word, Player* Player_1, Player* Player_2);
//  =============================================  Интерфейс игры  =====================================================
void COMPGame_Interface();
void TwoPlayersGame_Interface(unsigned char playing_field[5][5], int turn, Current_option option, int x, int y, struct Player* Player_1, struct Player* Player_2);
//  ============================================  Префиксное дерево  ===================================================
void Trie(const unsigned char* word_to_add);
int SearchWordInTrie(const unsigned char* word);
//  ===============================================  Опции меню  =======================================================
void OptionAboutProgram();
void OptionHelp();
void OptionPlay();
//  ==========================================  Опции справки меню  ====================================================
void OptionHelp_AnimationOperation();
void OptionHelp_WayOperation();
void OptionHelp_DeleteOperation();
//  =========================================  Обслуживающие функции  ==================================================
int Check_new_way(unsigned char* new_way);
void Choiced_option(int option, char* text_of_error);
void Choiced_help_option(int option);
int LKG(int seed);
int SeekNumberInMassive(const int* massive, int length, int number);
int GiveTheNumber(const char* start);
char* GiveTheString(const char* start);
void OptionTableOfRecords_SecondChance();
void OptionTableOfRecords_ThirdChance();
//  =======================================  Обслуживающие функции игры  ===============================================
void Game_FindWordInVocabulary();
int Game_SecondChance_SKIP();
void Game_Notepad(Player* Player);
int Game_SecondChance_DeleteFromNotepad();
void Game_ChangeCell(int KEY, int* current_X, int* current_Y, unsigned char playing_field[5][5], int selected_coordinates[][2]);
void Game_AddWordInList(unsigned char* word);
void Game_EndOfGame(Player* Player_1, Player* Player_2);
int Game_RepeatCheck(unsigned char* new_word);
int Game_SecondChance_ESC();
//  ==============================  Функции работы с файлом Data.txt и RECORDS.txt =====================================
int DataFile_CheckFile(const char* Data);
char* DataFile_OpenDataFile();
void DataFile_SaveDataInFile();
void DataFile_RecordTheDataInStruct(char* Data);
FILE* DataFile_CreateDefaultDataFile();
void RECORDSFile_Record();
FILE* RECORDSFile_CreateDefault();
void RECORDSFile_AddNewPage();
void RECORDSFile_SaveDataInFile();

//TODO - Реализовать сложность игры (****)

int main() {
    Main_Data.Data = DataFile_OpenDataFile();  //  Открытие главного файла и запись основных данных в структуру Main_Data
    RECORDSFile_Record();  //  Работа с файлом рекордов
    // Инициализируется консоль
    con_init(80, 25);
    //  Анимация начала работы программы
    if (Main_Data.Animation) {  //  Если значение показа анимаций == 1 (Есть разрешение)
        MainMenu_Animation_Authors();  //  Запуск анимация авторов
        MainMenu_Animation_Loading();  //  Запуск анимации загрузки
    }
    Menu();  //  Запуск меню и программы
    return 0;
}
//  ========================================  Анимации и оформление ====================================================
void MainMenu_View_Animation(int sec, int option) {  //  Анимация появления меню (sec - время анимации: 200 - default, 0 - без анимации); Номер подсвечиваемой опции (0 - default)
    ///  - Функция вывода главного меню

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон с белыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    //  Листинг таблицы-меню
    char menu_table[25][50] = { "========[   Игра \"Балда\"   ]========",
                                "/::\\                            /::\\",
                                "[[]]========[   Меню   ]========[[]]",
                                "\\::/                            \\::/",
                                " ||     [<]   "," Играть ","   [>]     || ",
                                " ||                              || ",
                                " ||     [*]  "," Рекорды ","   [*]     || ",
                                " TT                              TT ",
                                " ||     [I]"," О программе "," [I]     || ",
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

        Sleep(sec);  //  Усыпление компилятора
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
        if (string_of_menu == 5 + 4 * option) {  //  Условие для окраски определенной первой опции в меню
            con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Меняем на белый фон с черными буквами
            printf("%s", &menu_table[string_of_menu][0]);
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
        } else {
            printf("%s", &menu_table[string_of_menu][0]);
        }

    }

}

void MainMenu_Animation_Authors() {  //  Анимация запуска программы
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

void MainMenu_Animation_Loading() {  //  Анимация загрузки программы
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

void MainMenu_Animation_TableOfRecords() {  //  Анимация загрузки таблицы рекордов
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

void OptionTableOfRecords_View() {
    ///  Процедура рисунка окна меню с прошлыми рекордами
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    clrscr();
    show_cursor(0);

    //  Рисовка окна
    unsigned char table[][50] = { "         [*] Рекорды [*]            ",
                                  "+-------+---------------+----------+",
                                  "  Место       Игрок          Очки   ",
                                  "+-------+---------------+----------+",
                                  "|   1   |     ---            --     ",
                                  "|   2   |     ---            --     ",
                                  "|   3   |     ---            --     ",
                                  "+-------+---------------+----------+",
                                  "|   4   |     ---            --     ",
                                  "|   5   |     ---            --     ",
                                  "|   6   |     ---            --     ",
                                  "|   7   |     ---            --     ",
                                  "|   8   |     ---            --     ",
                                  "|   9   |     ---            --     ",
                                  "|  ...  |     ---            --     ",
                                  "+-------+---------------+----------+"};
    //  Размеры таблицы
    short width_of_table = strlen("+-------+---------------+----------+");
    short height_of_table = 16;
    //  Дополнительный адрес на список игроков + текущий записываемый игрок
    Records_list* dop_link = (Records_list*) First_Page;

    //  Рисовка таблицы
    for (int string = 0; string < height_of_table; string++) {
        //  Выделение исключительных строк
        if (string == 0) {  //  Вывод шапки меню
            gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("         ")),
                   (short) ((con_height_new() - height_of_table) / 2 + string));

            con_set_color(CON_CLR_YELLOW, CON_CLR_BLACK);
            printf("[*] Рекорды [*]");
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else if (string == 4) {  //  Первое место
            gotoxy((short) ((con_width_new() - width_of_table) / 2),
                   (short) ((con_height_new() - height_of_table) / 2 + string));

            //  Печатаем стандартным цветом
            printf("%s", table[string]);
            //  Выделяем цветом часть строки
            con_set_color(CON_CLR_CYAN_LIGHT, CON_CLR_BLACK);

            gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   ")),
                   (short) ((con_height_new() - height_of_table) / 2 + string));
            printf("1");  //  Цифра 1

            if (dop_link->next != NULL) {  //  Если первый элемент не является заглушкой в списке (то есть в нем есть данные)
                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   1   |     ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("%s", dop_link->name);  //  Имя игрока

                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   1   |     ---            ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("%d", dop_link->number_of_points);  //  Очки игрока

                //  Переходим к следующему игроку по списку
                dop_link = dop_link->next;
            } else {  //  >>  Данных первого игрока нет
                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   1   |     ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("---");  //  Пропуск имени игрока

                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   1   |     ---            ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("--");  //  Пропуск очков игрока
            }

            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else if (string == 5) {  //  Второе место
            gotoxy((short) ((con_width_new() - width_of_table) / 2),
                   (short) ((con_height_new() - height_of_table) / 2 + string));

            //  Печатаем стандартным цветом
            printf("%s", table[string]);
            //  Выделяем цветом часть строки
            con_set_color(CON_CLR_YELLOW, CON_CLR_BLACK);

            gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   ")),
                   (short) ((con_height_new() - height_of_table) / 2 + string));
            printf("2");  //  Цифра 2

            if (dop_link->next != NULL) {
                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   2   |     ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("%s", dop_link->name);  //  Имя игрока

                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   2   |     ---            ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("%d", dop_link->number_of_points);  //  Очки игрока

                //  Переходим к следующему игроку по списку
                dop_link = dop_link->next;
            } else {  //  >>  Данных второго игрока нет
                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   2   |     ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("---");  //  Пропуск имени игрока

                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   2   |     ---            ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("--");  //  Пропуск очков игрока
            }

            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else if (string == 6) {  //  Третье место
            gotoxy((short) ((con_width_new() - width_of_table) / 2),
                   (short) ((con_height_new() - height_of_table) / 2 + string));

            //  Печатаем стандартным цветом
            printf("%s", table[string]);
            //  Выделяем цветом часть строки
            con_set_color(CON_CLR_GRAY, CON_CLR_BLACK);

            gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   ")),
                   (short) ((con_height_new() - height_of_table) / 2 + string));
            printf("2");  //  Цифра 2

            if (dop_link->next != NULL) {
                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   3   |     ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("%s", dop_link->name);  //  Имя игрока

                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   3   |     ---            ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("%d", dop_link->number_of_points);  //  Очки игрока

                //  Переходим к следующему игроку по списку
                dop_link = dop_link->next;
            } else {  //  >>  Данных второго игрока нет
                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   3   |     ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("---");  //  Пропуск имени игрока

                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   3   |     ---            ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("--");  //  Пропуск очков игрока
            }

            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else {  //  Остальные строки таблицы
            gotoxy((short) ((con_width_new() - width_of_table) / 2),
                   (short) ((con_height_new() - height_of_table) / 2 + string));

            //  Печатаем стандартным цветом строку
            printf("%s", table[string]);

            //  Редактируем строки с именами игроков
            if (string >= 6 && string <= 14 && string != 7 && dop_link->next != NULL) {
                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   -   |     ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("%s", dop_link->name);  //  Имя игрока

                gotoxy((short) ((con_width_new() - width_of_table) / 2 + strlen("|   -   |     ---            ")),
                       (short) ((con_height_new() - height_of_table) / 2 + string));
                printf("%d", dop_link->number_of_points);  //  Очки игрока

                //  Переходим к следующему игроку по списку
                dop_link = dop_link->next;
            }
        }

    }

    //  Рисовка дальнейших действий
    gotoxy((short) ((con_width_new() - strlen("Press ESC to exit")) / 2 + 22),
           (short) ((con_height_new() - 1) / 2 + 11));
    printf("Press ESC for exit");
}

void MainMenu_Animation_Close() {
    ///  Анимация закрытия программы (удаление строк и столбцов в рандомном порядке)

    int width_of_console = con_width_new();  //  Ширина консоли
    int height_of_console = con_height_new();  //  Высота консоли
    //  Массив для хранения номеров удаленных строк

    //  Массив для хранения номеров удаленных столбцов
    int* deleted_columns = malloc(width_of_console * sizeof(int));
    if (deleted_columns == NULL) {  //  Проверка на выделение памяти
        Choiced_option(5, "Error with allocating memory to the array in the MainMenu_Animation_Close() function!");
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
        if (SeekNumberInMassive(deleted_columns, width_of_console, num % width_of_console)) {  //  Если найден
            //  Пока мы не найдем несовпадение увеличиваем num
            while (SeekNumberInMassive(deleted_columns, width_of_console, num % width_of_console)) {
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

void MainMenu_OptionHelp_View(int option) {
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

void OptionPlay_PrePlay_View(int option) {
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

void OptionPlay_DifficultySelection_View(int current_option) {
    ///  Вывод меню начала игры 2
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

    short width_of_console = con_width_new();  //  Ширина консоли
    short height_of_console = con_height_new();;  //  Высота консоли

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
    }
}

unsigned char* FirstGameWord_View() {
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

    unsigned char* start_word = (unsigned char*) malloc(sizeof(char) * 100);  //  Начальное слово (впиши больше 100 символов и посмотри чо будет. Поэтому их тут так много)
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
        show_cursor(0);  //  Убираем курсор

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
            if (!SearchWordInTrie(start_word)) {  //  В словаре такого слова нет
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

void Game_EndOfGame_View(int num) {
    ///  Функция вывода шаблонов части (1 из 4) окон конца игры [num - Номер конструкции]
    //  Меняем цвет всей корнсоли на белый с черным шрифтом - default цыета для это части игры
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    clrscr();
    show_cursor(0);

    //  Первая часть
    unsigned char first_picture[][75] = { "/ +--+  +------------+  +--+ \\",
                                          "\\                            /",
                                          "/    Игра окончена!  (\\__/)  \\",
                                          "\\                    (O.o )  /",
                                          "/                    (> < )  \\",
                                          "\\ +---------+    +---------+ /"};
    //  Вторая часть
    unsigned char second_picture[][75] = { "/ +--+  +----+  +----+  +--+ \\",
                                           "\\  ","+","     ","+","       ","+","           /",
                                           "/     ","Победитель -        ","   \\",
                                           "\\   (\\.../)     ","+","       ","+","    /",
                                           "/   ( ='.')  ","+","      ","+","        \\",
                                           "\\ +---+  +---+  +---+  +---+ /"};
    //  Третья часть картинки
    unsigned char third_picture[][75] = { "/ +--+  +------------+  +--+ \\",
                                           "\\                            /",
                                           "/     Игрок 1    Игрок 2     \\",
                                           "\\       __    :    __        /",
                                           "/                            \\",
                                           "\\ +---------+    +---------+ /"};
    //  Четвертая часть картинки
    unsigned char fourth_picture[][75] = { "/ +--+  ++  ++  ++  ++  +--+ \\",
                                           "\\                            /",
                                           "/   Введите имя победителя:  \\",
                                           "\\           _ _ _            /",
                                           "/                            \\",
                                           "\\ ++  +----+  ++  +----+  ++ /"};

    short width_of_picture = strlen("/ +--+  +------------+  +--+ \\");
    short height_of_picture = 6;

    if (num != 1) {
        int string = 0;  //  Текущая заполняемая строка консоли

        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        for (int mark = 0; mark < height_of_picture; mark++) {
            gotoxy((short) ((con_width_new() - width_of_picture) / 2), (short) ((con_height_new() - height_of_picture) / 2 + string++));
            //  Вывод опеределенной картинки
            if (num == 0) {
                printf("%s", first_picture[mark]);
            } else if (num == 2) {
                printf("%s", third_picture[mark]);
            } else if (num == 3) {
                printf("%s", fourth_picture[mark]);
            }
        }

    } else {  //  Исключительный вывод 1 картинки (Особоый раскрас)
        int string = 0;  //  Текущая заполняемая строка консоли
        //  Начальная точка вывода
        gotoxy((short) ((con_width_new() - width_of_picture) / 2), (short) ((con_height_new() - height_of_picture) / 2 + string++));
        for (int mark = 0; mark < 22; mark++) {
            //  Оговораиваемся насчет определенных частей выводного окна (строки "+" для различных цветов)
            if (mark == 2 || mark == 4 || mark == 6 || mark == 9 || mark == 12 || mark == 14 || mark == 17 || mark == 19) {
                //  Обновляем значение Random
                Main_Data.Random = LKG(Main_Data.Random);
                //  Установка цвета '+' в консоли
                if (Main_Data.Random % 7 == 0) {  //  Серый
                    con_set_color(CON_CLR_GRAY, CON_CLR_BLACK);
                } else if (Main_Data.Random % 7 == 1) {  //  Красный светлый
                    con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);
                } else if (Main_Data.Random % 7 == 2) {  //  Светлый зеленный
                    con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);
                } else if (Main_Data.Random % 7 == 3) {  //  Светлый синий
                    con_set_color(CON_CLR_BLUE_LIGHT, CON_CLR_BLACK);
                } else if (Main_Data.Random % 7 == 4) {  //  Светлый желтый
                    con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
                } else if (Main_Data.Random % 7 == 5) {  //  Светлый пурпурный
                    con_set_color(CON_CLR_MAGENTA_LIGHT, CON_CLR_BLACK);
                } else if (Main_Data.Random % 7 == 6) {  //  Светлый бирюзовый
                    con_set_color(CON_CLR_CYAN_LIGHT, CON_CLR_BLACK);
                }
            } else {  //  Другие строки - обычный белый цвет на черном фоне
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
            }
            //  Вывод '+'
            printf("%s", second_picture[mark]);
            //  Возвращаем дефолтный цвет
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

            if (mark == 0 || mark == 7 || mark == 10 || mark == 15 || mark == 20) {
                //  Новая строка вывода в консоли + Увеличиваем на 1 записываемую строку
                gotoxy((short) ((con_width_new() - width_of_picture) / 2), (short) ((con_height_new() - height_of_picture) / 2 + string++));
            }
        }
    }

}
//  ===============================================  Меню игры  ========================================================
void Menu() {  //  Главное меню игры
    ///  Функция главного меню игры, с которого идет выбор анимаций и опций

    MainMenu_View_Animation(200, 0);  //  Запуск анимации появления меню с задержкой анимации в 0.2 sec и активной первой опцией
    //  >>  Меню запущено

    int code = 0;  //  Нажатый символ с клавиатуры
    int current_option = 0;  //  Текущая выделяемая опция
    //  Схемы опций главного меню (Выделены отдельно для рисовки непосредственно в этой функции, не трогая  MainMenu_View_Animation())
    unsigned char option_schemes[5][25] = { " Играть "," Рекорды "," О программе  "," Справка "," Выход " };
    //  Размерность меню
    short width_of_table = strlen("[[]]========[   Меню   ]========[[]]");  //  (36 символов)
    short height_of_table = 15;

    while (1) {

        if (!key_is_pressed()) {
            code = key_pressed_code();  //  Считываем код символа

            if (code == KEY_UP) {  //  Если стрелка вверх
                if (current_option == 0) {  //  Если уже потолок опций
                    continue;
                } else {
                    //  Выводим снова текущую выделенную строку с дефолтным цветом (== Стираем выделение)
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen(" ||     [<]") + (14 - strlen((char*) option_schemes[current_option])) / 2),
                           (short)((con_height_new() - height_of_table) / 2 + current_option * 2 + 4));
                    printf("%s", option_schemes[current_option]);

                    //  Переходим к новой строке и выделяем ее
                    Sleep(40);
                    current_option--;
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen(" ||     [<]") + (14 - strlen((char*) option_schemes[current_option])) / 2),
                           (short)((con_height_new() - height_of_table) / 2 + current_option * 2 + 4));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Выделение (черные буквы и белый фон)
                    printf("%s", option_schemes[current_option]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем изначальные настройки цветов

                    continue;
                }
            }
            if (code == KEY_DOWN) {  //  Если стрелка вниз
                if (current_option == 4) {  //  Если ниже некуда
                    continue;
                } else {
                    //  Выводим снова текущую выделенную строку с дефолтным цветом (== Стираем выделение)
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen(" ||     [<]") + (14 - strlen((char*) option_schemes[current_option])) / 2),
                           (short)((con_height_new() - height_of_table) / 2 + current_option * 2 + 4));
                    printf("%s", option_schemes[current_option]);

                    //  Переходим к новой строке и выделяем ее
                    Sleep(40);
                    current_option++;
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen(" ||     [<]") + (14 - strlen((char*) option_schemes[current_option])) / 2),
                           (short)((con_height_new() - height_of_table) / 2 + current_option * 2 + 4));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Выделение (черные буквы и белый фон)
                    printf("%s", option_schemes[current_option]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем изначальные настройки цветов

                    continue;
                }
            }
            if (code == KEY_F1) {  //  Обновить окно
                current_option = 0;  //  Обнуляем выделяемую функцию
                MainMenu_View_Animation(0, current_option);
                continue;
            }
            if (code == KEY_ENTER && current_option == 4) {  //  Выбран выход из программы
                DataFile_SaveDataInFile();  //  Сохранение в файл данных
                RECORDSFile_SaveDataInFile();
                if (Main_Data.Animation) {  //  Если дано разрешение на анимацию
                    MainMenu_Animation_Close();
                }
                clrscr();  //  Очистка окна и окрас консоли в цвет
                ExitProcess(EXIT_SUCCESS);  //  На случай отказа показа анимаций
            }
            if (code == KEY_ENTER) {  //  Введен ввод
                pause(70);
                Choiced_option(current_option, "---");
                MainMenu_View_Animation(0, current_option);  //  Перерисовываем менюшку после отработки функции
                continue;
            }

            pause(40); // Небольная пауза (требуется для корректной работы с быстрым набором клавиш)
        }

    }

}
//  =================================================  Игра  ===========================================================
void OptionPlay_DifficultySelection() {
    ///  Меню начала игры 2 - выбор сложности игры с компьютером
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и белыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    const int width_of_table = 25;  //  Ширина меню
    const int height_of_table = 6;  //  Высота меню
    int current_string = 0;  //  Текущая заполняемая строка в консоли

    short width_of_console = con_width_new();  //  Ширина консоли
    short height_of_console = con_height_new();;  //  Высота консоли
    //  Опции данного интерфейса
    unsigned char option_schemes[3][20] = { "Высокая","Средняя","Низкая" };
    int current_option = 0;
    OptionPlay_DifficultySelection_View(current_option);

    int code = 0;  //  Нажатый символ с клавиатуры
    show_cursor(0);  //  Скрытие курсора
    while (1) {

        if (!key_is_pressed()) {
            code = key_pressed_code();  //  Считываем код символа

            if (code == KEY_UP) {  //  Если стрелка вверх
                if (current_option == 0) {  //  Если уже потолок опций
                    continue;
                } else {
                    //  Выводим снова текущую выделенную строку с дефолтным цветом (== Стираем выделение)
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("|    == С")),
                           (short)((con_height_new() - height_of_table) / 2 + current_option + 2));
                    printf("%s", option_schemes[current_option]);

                    //  Переходим к новой строке и выделяем ее
                    Sleep(40);
                    current_option--;
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("|    == С")),
                           (short)((con_height_new() - height_of_table) / 2 + current_option + 2));

                    //  Выделение (черный фон и определенный цвет букв)
                    if (current_option == 0) {  //  Высокая
                        con_set_color(CON_CLR_RED, CON_CLR_BLACK);
                    } else if (current_option == 1) {  //  Средняя
                        con_set_color(CON_CLR_YELLOW, CON_CLR_BLACK);
                    } else if (current_option == 2) {  //  Низкая
                        con_set_color(CON_CLR_GREEN, CON_CLR_BLACK);
                    }

                    //  Вывод строки
                    printf("%s", option_schemes[current_option]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем изначальные настройки цветов

                    continue;
                }
            }
            if (code == KEY_DOWN) {  //  Если стрелка вниз
                if (current_option == 2) {  //  Если ниже некуда
                    continue;
                } else {
                    //  Выводим снова текущую выделенную строку с дефолтным цветом (== Стираем выделение)
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("|    == С")),
                           (short)((con_height_new() - height_of_table) / 2 + current_option + 2));
                    printf("%s", option_schemes[current_option]);

                    //  Переходим к новой строке и выделяем ее
                    Sleep(40);
                    current_option++;
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("|    == С")),
                           (short)((con_height_new() - height_of_table) / 2 + current_option + 2));

                    //  Выделение (черный фон и определенный цвет букв)
                    if (current_option == 0) {  //  Высокая
                        con_set_color(CON_CLR_RED, CON_CLR_BLACK);
                    } else if (current_option == 1) {  //  Средняя
                        con_set_color(CON_CLR_YELLOW, CON_CLR_BLACK);
                    } else if (current_option == 2) {  //  Низкая
                        con_set_color(CON_CLR_GREEN, CON_CLR_BLACK);
                    }

                    //  Вывод строки
                    printf("%s", option_schemes[current_option]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем изначальные настройки цветов

                    continue;
                }
            }
            if (code == KEY_ESC) {  //  Выход
                return;
            }
            if (code == KEY_F1) {  //  Обновить окно
                current_option = 0;
                OptionPlay_DifficultySelection_View(current_option);
                continue;
            }
            if (code == KEY_ENTER) {  //  Выбор
                if (Main_Data.Animation == 1) {
                    Sleep(500);  //  Задержка
                    clrscr();
                    Sleep(1000);  //  Задержка
                    MainMenu_Animation_Loading();
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
                break;
            }
        }
        pause(40);
    }
}

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
        unsigned char *first_word = FirstGameWord_View();  //  Требование ввести первое слово и вывод меню

        if (difficulty == TWO_PLAYERS) {  //  Игра с человеком
            //  Создаем первого игрока и обнуляем его поля
            Player* Player_1 = (Player*) malloc((sizeof (Player)));
            Player_1 -> Name = (unsigned char*) "Игрок 1";  //  Имя
            Player_1 -> Skips = 0;                          //  Количество пропусков
            Player_1 -> Number_of_points = 0;               //  Количкство очков
            memset(Player_1 -> Notepad, 0, sizeof (Player_1 -> Notepad));
            //  Создаем второго игрока и обнуляем его поля
            Player* Player_2 = (Player*) malloc((sizeof (Player)));
            Player_2 -> Name = (unsigned char*) "Игрок 2";  //  Имя
            Player_2 -> Skips = 0;                          //  Количество пропусков
            Player_2 -> Number_of_points = 0;               //  Количкство очков
            memset(Player_2 -> Notepad, 0, sizeof (Player_2 -> Notepad));
            //  >>  Готовы два игрока

            TwoPlayersGame(first_word, Player_1, Player_2);  //  Начало игры
        } else {  //  Игра с компьютером
            Game_COMP(difficulty, first_word);
        }
    } else {  //  Если слов нет
        Choiced_option(5, "Vocabularies are empty or don't exist! Add some words in file \"VOC.txt\" of game");
    }
}

void Game_COMP(Difficulty_level difficulty, unsigned char* first_word) {
    ///  Основная функция игры с компьютером
    //TODO  Сделать игру с компьютером

}

void TwoPlayersGame(unsigned char* first_word, Player* Player_1, Player* Player_2) {
    ///  Основная функция игры двух людей
    //  Игровое поле
    unsigned char playing_field[5][5];
    memset(playing_field, 0, sizeof (playing_field));
    //  Заполнение игрового поля первым словом
    for (int mark = 0; mark < 5; mark++) {
        playing_field[mark][2] = first_word[mark];
    }
    first_word[5] = '\0';  //  Убираем '\n' для проверки с новыми выставленными словами

    //  Массив записанных ранее в игре слов (история так сказатб)
    unsigned char past_words[20][20];
    memset(past_words, 0, sizeof (past_words));

    //  Создание структуры-списка использованных в игре слов
    BeginOfList = (List*) malloc(sizeof(List));
    BeginOfList -> word[0] = 0;  //  Закрываем слово
    BeginOfList -> next = NULL;  //  Закрываем его

    //  Чертежи частей интерфейса игры для перерисовки
    unsigned char option_schemes[3][40] = { " Пропустить ход? "," Проверить слово "," Блокнот " };

    //  Координаты выставленной буквы
    int new_X = -1, new_Y = -1;
    //  Текущие x и y на игровом поле
    int current_X = 2, current_Y = 2;
    //  Текущая подсвечиваемая опция
    Current_option current_option = PLAY_FIELD;
    //  Текущая очередь
    int turn = 0;
    //  Режим поля (0 - обычный, 1 - выбрана ячейка и записана буква)
    int mode = 0;
    //  Координаты выделенных букв
    int selected_coordinates[20][2];
    memset(selected_coordinates, -1, sizeof (selected_coordinates));
    //  Дополнительная переменная для статуса выставленной буквы. (При значении 0 - Выставленная буква не используется в новом слове, 1 - используется)
    int second_use = 0;
    //  Количество открытых ячеек (Для оконачиния игры при нуле)
    int number_of_cells = 20;

    //  Вызываем интерфейс
    TwoPlayersGame_Interface(playing_field, turn, current_option, 2, 2, Player_1, Player_2);

    //  Основной цикл игры
    while (1) {

        if (!key_is_pressed()) {
            //  Код нажатой клавиши
            int code = key_pressed_code();

            if (code == KEY_UP) {  //  Передвижение вверх
                if (current_Y > 0 && current_option == PLAY_FIELD) {  //  Передвижение по игровому полю
                    Game_ChangeCell(code, &current_X, &current_Y, playing_field, selected_coordinates);
                    continue;
                } else if (current_option == CHECK_WORD) {  //  Переход на опцию "блокнот" с игрового поля
                    //  Убираем выделение с опции "Проверить слово"
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    Пропустить ход?                         (\\.../) (\\.../) (\\.../) (\\.../)                      |  ")),
                           (short)((con_height_new() - 30) / 2 + 26));  //  120x30 - размер интерфейса игры
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    printf("%s", option_schemes[1]);

                    //  Раскрашиваем новую опцию
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       |      ")),
                           (short)((con_height_new() - 30) / 2 + 7));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    printf("%s", option_schemes[2]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    current_option = NOTEPAD;
                    continue;
                } else if (second_use && current_Y == 0) {  //  В случае использования выставленной буквы - опция "Подтвердить слово"
                    //  Убираем подсветку ячейки
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + current_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + current_Y * 3));

                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    if (playing_field[current_X][current_Y] != 0) {
                        printf(" %c ", playing_field[current_X][current_Y]);  //  Ячейка с буквой
                    } else {
                        printf("   ");  //  Пустая ячейка
                    }

                    //  Подсвечиваем функцию
                    gotoxy((short )((con_width_new() - 120) / 2 + strlen("                                             +---+")),
                                  (short) ((con_height_new() - 30) / 2 + 4));
                    con_set_color(CON_CLR_BLACK, CON_CLR_GREEN_LIGHT);
                    printf(" Подтвердить слово ");
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    while (1) {

                        if (!key_is_pressed()) {
                            int code_2 = key_pressed_code();

                            if (code_2 == KEY_ENTER) {  //  Подтверждение слова
                                //  Собираем слово из записанных координат
                                unsigned char new_word[25];  //  Новое слово

                                int mark = 0;
                                while (selected_coordinates[mark][0] != -1) {
                                    new_word[mark] = playing_field[selected_coordinates[mark][0]][selected_coordinates[mark][1]];
                                    mark++;
                                }
                                new_word[mark] = 0;
                                //  >>  new_word - Новое записываемое слово

                                //  Проверка нового слова в словаре
                                if (!SearchWordInTrie(new_word)) {  //  Если слово не было найдено в словаре
                                    //  Удаляем из списка
                                    for (mark = 0; selected_coordinates[mark][0] != -1; mark++) {
                                        //  Стриаем с экрана записанные буквы
                                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             +") + selected_coordinates[mark][0] * 6),
                                               (short)((con_height_new() - 30) / 2 + 8 + 3 * selected_coordinates[mark][1]));

                                        printf(" %c ", playing_field[selected_coordinates[mark][0]][selected_coordinates[mark][1]]);
                                        selected_coordinates[mark][0] = -1;

                                    }
                                    //  Убираем надпись "Подтвердить слово"
                                    gotoxy((short )((con_width_new() - 120) / 2 + strlen("                                             +---+")),
                                           (short) ((con_height_new() - 30) / 2 + 4));
                                    con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);
                                    printf("                   ");
                                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                                    gotoxy((short )((con_width_new() - 120) / 2 + strlen("                                             ")),
                                           (short) ((con_height_new() - 30) / 2 + 4));
                                    con_set_color(CON_CLR_RED, CON_CLR_BLACK);
                                    printf("Такого слова нет в словаре!!!");
                                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                                    Sleep(2000);

                                    gotoxy((short )((con_width_new() - 120) / 2 + strlen("                                             ")),
                                           (short) ((con_height_new() - 30) / 2 + 4));
                                    printf("                             ");

                                    //  Возвращаем подсвет активной ячейки на игрвоом поле
                                    gotoxy((short )((con_width_new() - 120) / 2 + strlen("      |                                    2 |") + current_X * 6),
                                           (short) ((con_height_new() - 30) / 2 + 8));
                                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                                    if (playing_field[current_X][current_Y] != 0) {
                                        printf(" %c ", playing_field[current_X][current_Y]);  //  Ячейка с буквой
                                    } else {
                                        printf("   ");  //  Пустая ячейка
                                    }
                                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                                    second_use = 0;  //  Убираем состояние использования выставленной буквы
                                    break;
                                }
                                //  Проверка нового слова на повторение (был ли введен раньше)
                                if (Game_RepeatCheck(new_word) || !strcmp((char*) new_word, (char*) first_word)) {
                                    gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                      === +--+ ===         ")),
                                           (short) ((con_height_new() - 30) / 2 + 4));
                                    con_set_color(CON_CLR_RED, CON_CLR_BLACK);
                                    printf("Данное слово уже было использовано!");
                                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                                    Sleep(2000);
                                    //  Стираем надпись "Данное слово уже было использовано"
                                    gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                      === +--+ ===         ")),
                                           (short) ((con_height_new() - 30) / 2 + 4));
                                    printf("                                   ");

                                    //  Стираем все выделенные ячейки
                                    for (int mark_2 = 0; selected_coordinates[mark_2][0] != -1; mark_2++) {
                                        //  Снимаем выделение
                                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             |") + selected_coordinates[mark_2][0] * 6),
                                               (short)((con_height_new() - 30) / 2 + 8 + 3 * selected_coordinates[mark_2][1]));
                                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                                        printf(" %c ", playing_field[selected_coordinates[mark_2][0]][selected_coordinates[mark_2][1]]);
                                        //  Удаляем выделенное значение в массиве
                                        selected_coordinates[mark_2][0] = -1;
                                        selected_coordinates[mark_2][1] = -1;
                                    }

                                    second_use = 0;  //  Убираем выделение ячейки выставленной буквы

                                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             |") + current_X * 6),
                                           (short)((con_height_new() - 30) / 2 + 8 + 3 * current_Y));

                                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                                    printf(" %c ", playing_field[current_X][current_Y]);
                                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                                    break;
                                }

                                //  Начисляем очки за слово
                                if (turn == 0) {  //  Был ход первого игрока
                                    Player_1->Number_of_points += (int) strlen((char*) new_word);
                                } else if (turn == 1) {  //  Ход второго игрока
                                    Player_2->Number_of_points += (int) strlen((char*) new_word);
                                }
                                ///  Проверка на конец игры
                                if (number_of_cells == 0) {  //  Если закончились свободные ячейки
                                    //  Подведение итогов игры
                                    Sleep(2000);  //  Усыпляем компилятор
                                    Game_EndOfGame(Player_1, Player_2);  //  Заканичваем игру окнами и анимацией
                                    break;  //  Не играет никакой роли
                                }

                                //  Меняем ход
                                turn = (turn + 1) % 2;
                                mode = 0;  //  Восстанавливаем режим поля
                                second_use = 0;  //  Убираем состояние использования выставленной буквы
                                number_of_cells--;  //  Уменьшаем количество свободных ячеек
                                //  Восстанавлиаем координаты
                                current_X = 2;
                                current_Y = 2;
                                //  Выводим весь интерфейс игры заново
                                TwoPlayersGame_Interface(playing_field, turn, current_option, current_X, current_Y, Player_1, Player_2);

                                //  Рисуется таблица с записанными словами
                                gotoxy((short)((con_width_new() - 120) / 2 + strlen("                      ")),
                                       (short)((con_height_new() - 30) / 2 + 4));

                                con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
                                printf("=== +--+ ===");  //  Шапка таблиы записанных слов

                                List* dop_mark = (List*) BeginOfList;  //  Дополнительный адрес для передвижению по списку
                                mark = 0;

                                //  Добавление слова в список
                                Game_AddWordInList(new_word);

                                dop_mark = (List*) BeginOfList;
                                //  Выводим список последних 10 слов
                                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                                while (dop_mark->word[0] != 0 && mark <= 10) {  //  Выводим все записанные слова (Ровно 10 последних слов)
                                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                      ===")),
                                           (short)((con_height_new() - 30) / 2 + 5 + mark));

                                    printf("%s", dop_mark->word);

                                    dop_mark = (List *) dop_mark->next;
                                    mark++;
                                }

                                //  Обнуляем записанные координаты для последующего игрока
                                for (mark = 0; selected_coordinates[mark][0] != -1; mark++) {
                                    selected_coordinates[mark][0] = -1;
                                }

                                break;
                            } else if (code_2 == KEY_ESC) {  //  Отмена подтверждения слова
                                //  Снимаем подсветку
                                gotoxy((short )((con_width_new() - 120) / 2 + strlen("                                             +---+")),
                                       (short) ((con_height_new() - 30) / 2 + 4));
                                con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);
                                printf(" Подтвердить слово ");
                                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                                //  Обратно подсвечиваем ячейку поля
                                gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + current_X * 6),
                                       (short)((con_height_new() - 30) / 2 + 8 + current_Y * 3));

                                con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                                if (playing_field[current_X][current_Y] != 0) {
                                    printf(" %c ", playing_field[current_X][current_Y]);  //  Выделяем ячейку с буквой
                                } else {
                                    printf("   ");  //  Выделяем пустую ячейку
                                }
                                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                                break;
                            }
                        }
                    }
                    continue;
                }
            }
            if (code == KEY_DOWN) {  //  Передвижение вниз
                if (current_Y < 4 && current_option == PLAY_FIELD) {  //  Передвижение по игровому полю
                    Game_ChangeCell(code, &current_X, &current_Y, playing_field, selected_coordinates);
                    continue;
                } else if  (current_option == NOTEPAD) {  //  Переход с опции "блокнот" к опции "проверка слова"
                    //  Убираем выделение с опции "Блокнот"
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       |      ")),
                           (short)((con_height_new() - 30) / 2 + 7));  //  120x30 - размер интерфейса игры
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    printf("%s", option_schemes[2]);

                    //  Раскрашиваем новую опцию
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    Пропустить ход?                         (\\.../) (\\.../) (\\.../) (\\.../)                      |  ")),
                           (short)((con_height_new() - 30) / 2 + 26));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    printf("%s", option_schemes[1]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    current_option = CHECK_WORD;
                    continue;
                }
            }
            if (code == KEY_LEFT) {  //  Перемещение влево
                if (current_X > 0 && current_option == PLAY_FIELD) {  //  Передвижение по игровому полю
                    Game_ChangeCell(code, &current_X, &current_Y, playing_field, selected_coordinates);
                    continue;
                } else if (current_X == 0 && current_option == PLAY_FIELD) {  //  Переход с игрового поля на опцию "Skip"
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + current_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + current_Y * 3));
                    if (playing_field[current_X][current_Y] != 0) {
                        printf(" %c ", playing_field[current_X][current_Y]);  //  Стираем выделение ячейки с буквой
                    } else {
                        printf("   ");  //  Стираем выделение пустой ячейки
                    }

                    //  Раскрашиваем новую опцию
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("   ")),
                           (short)((con_height_new() - 30) / 2 + 26));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    printf("%s", option_schemes[0]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    current_option = SKIP;
                    continue;
                } else if (current_option == NOTEPAD) {  //  Переход к игровому полю с опции "блокнот"
                    //  Убираем выделение с опции "Блокнот"
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       |      ")),
                           (short)((con_height_new() - 30) / 2 + 7));  //  120x30 - размер интерфейса игры
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    printf("%s", option_schemes[2]);

                    //  Раскрашиваем ячейку поля
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + current_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + current_Y * 3));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    if (playing_field[current_X][current_Y] != 0) {
                        printf(" %c ", playing_field[current_X][current_Y]);  //  Выделяем ячейку с буквой
                    } else {
                        printf("   ");  //  Выделяем пустую ячейку
                    }
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    current_option = PLAY_FIELD;
                    continue;
                } else if (current_option == CHECK_WORD) {  //  Переход к игровому полю с опции "проверка слова"
                    //  Убираем выделение с опции "Проверить слово"
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    Пропустить ход?                         (\\.../) (\\.../) (\\.../) (\\.../)                      |  ")),
                           (short)((con_height_new() - 30) / 2 + 26));  //  120x30 - размер интерфейса игры
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    printf("%s", option_schemes[1]);

                    //  Раскрашиваем ячейку поля
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + current_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + current_Y * 3));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    if (playing_field[current_X][current_Y] != 0) {
                        printf(" %c ", playing_field[current_X][current_Y]);  //  Выделяем ячейку с буквой
                    } else {
                        printf("   ");  //  Выделяем пустую ячейку
                    }
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    current_option = PLAY_FIELD;
                    continue;
                }
            }
            if (code == KEY_RIGHT) {  //  Перемещение вправо
                if (current_X < 4 && current_option == PLAY_FIELD) {  //  Передвижение по игровому полю
                    Game_ChangeCell(code, &current_X, &current_Y, playing_field, selected_coordinates);
                    continue;
                } else if (current_option == SKIP) {  //  Переход к игровому полю с функции "Skip"
                    //  Убираем выделение с опции "пропустить ход"
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("   ")),
                           (short)((con_height_new() - 30) / 2 + 26));  //  120x30 - размер интерфейса игры
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    printf("%s", option_schemes[0]);

                    //  Раскрашиваем ячейку поля
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + current_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + current_Y * 3));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    if (playing_field[current_X][current_Y] != 0) {
                        printf(" %c ", playing_field[current_X][current_Y]);  //  Выделяем ячейку с буквой
                    } else {
                        printf("   ");  //  Выделяем пустую ячейку
                    }
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    current_option = PLAY_FIELD;
                    continue;
                } else if (current_option == PLAY_FIELD && current_X == 4 && current_Y <= 3) {  //  Переход с игрового поля на опцию "блокнот"
                    //  Перерисовываем прошлую ячейку поля (стираем выделение)
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + current_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + current_Y * 3));
                    if (playing_field[current_X][current_Y] != 0) {
                        printf(" %c ", playing_field[current_X][current_Y]);  //  Стираем выделение ячейки с буквой
                    } else {
                        printf("   ");  //  Стираем выделение пустой ячейки
                    }

                    //  Раскрашиваем новую опцию
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       |      ")),
                           (short)((con_height_new() - 30) / 2 + 7));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    printf("%s", option_schemes[2]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    current_option = NOTEPAD;
                    continue;
                } else if (current_option == PLAY_FIELD && current_X == 4 && current_Y == 4) {  //  Переход с игрового поля на опцию "проверить слово"
                    //  Перерисовываем прошлую ячейку поля (стираем выделение)
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + current_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + current_Y * 3));
                    if (playing_field[current_X][current_Y] != 0) {
                        printf(" %c ", playing_field[current_X][current_Y]);  //  Стираем выделение ячейки с буквой
                    } else {
                        printf("   ");  //  Стираем выделение пустой ячейки
                    }

                    //  Раскрашиваем новую опцию
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    Пропустить ход?                         (\\.../) (\\.../) (\\.../) (\\.../)                      |  ")),
                           (short)((con_height_new() - 30) / 2 + 26));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    printf("%s", option_schemes[1]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    current_option = CHECK_WORD;
                    continue;
                }
            }
            if (code == KEY_ENTER) {
                if (current_option == PLAY_FIELD) {  //  Выбор ячейки на игровом поле
                    if (playing_field[current_X][current_Y] != 0 && current_Y != new_Y && current_X != new_X && mode == 0) {  //  Если в ячейке уже находится буква в дефолтном моде
                        continue;  //  Пропуск
                    } else if (mode == 0 && playing_field[current_X][current_Y] == 0) {  //  Если мод дефолтный (Требуем выставить букву на место пропуска)
                        //  Прежде всего меняем значение мода -> выбрана буква
                        mode = 1;
                        //  Меняем окрас ячейки с новой буквой (бирюзовый цвет), убираем выделение поля ячейки и перемещаем курсор в точку ввода
                        con_set_color(CON_CLR_CYAN, CON_CLR_BLACK);  //  Окрас ячейки

                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + current_X * 6),
                               (short)((con_height_new() - 30) / 2 + 7 + 3 * current_Y));  //  Точка смены окраса ячейки (1)
                        printf("+---+");  //  Верхушка

                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + current_X * 6),
                               (short)((con_height_new() - 30) / 2 + 8 + 3 * current_Y));  //  Точка смены окраса ячейки (2)
                        printf("|   |");  //  Средняя часть

                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + current_X * 6),
                               (short)((con_height_new() - 30) / 2 + 9 + 3 * current_Y));  //  Точка смены окраса ячейки (3)
                        printf("+---+");  //  Низушка (???)
                        //  Меняем на норму
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                        //  Работаем с вводом буквы
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + current_X * 6 + 2),
                               (short)((con_height_new() - 30) / 2 + 8 + 3 * current_Y));  //  Точка ввода буквы
                        //  Требуем символ и полученный код записываем на поле
                        show_cursor(1);
                        playing_field[current_X][current_Y] = symbol_code(0);  //  (Русский символ)
                        show_cursor(0);

                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             |") + current_X * 6),
                               (short)((con_height_new() - 30) / 2 + 8 + 3 * current_Y));  //  Точка смены окраса ячейки (2)

                        con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                        printf(" %c ", playing_field[current_X][current_Y]);  //  Выделяем выставленную букву
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                        //  Записываем координаты выставленной буквы
                        new_X = current_X;
                        new_Y = current_Y;
                        continue;
                    }
                    //  Если выбрана непустая ячейка игрового поля при уже записанной букве - она включается в новое слово
                    if (mode == 1 && playing_field[current_X][current_Y] != 0 && !(current_X == new_X && current_Y == new_Y && second_use)) {

                        int koll = 0;  //  Количество элементов в строке выьранного слова
                        while (selected_coordinates[koll][0] != -1) {  //  Находим количество букв в выбранном слове и проверяем на сходство с записанными
                            if (selected_coordinates[koll][0] == current_X && selected_coordinates[koll][1] == current_Y) {  //  Продублировались
                                koll = -1;  //  Обнуляем
                                break;
                            }
                            koll++;
                        }

                        //  Если были найдены совпадения (Чтобы избежать добавления одной и той же ячейки несколько раз)
                        if (koll == -1) {
                            continue;
                        }

                        if (koll != 0) {
                            //  Вторая проверка - если новая буква не лажит рядом с последней - буква не будет подсвечиваться и использоваться в слове
                            if (!(abs(selected_coordinates[koll-1][0] - current_X) == 0 &&
                                  abs(selected_coordinates[koll-1][1] - current_Y) <= 1 ||
                                  abs(selected_coordinates[koll-1][0] - current_X) <= 1 &&
                                  abs(selected_coordinates[koll-1][1] - current_Y) == 0)) {
                                continue;
                            }
                        }
                        //  Если текущая добавляемая ячейка - последняя выставленная буква, то меняем ее режим + появляется возможность использовать слово
                        if (current_X == new_X && current_Y == new_Y) {
                            second_use = 1;
                            //  Рисуем функцию подтверждения слова
                            gotoxy((short )((con_width_new() - 120) / 2 + strlen("                                             +---+ ")),
                                   (short) ((con_height_new() - 30) / 2 + 4));

                            con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);
                            printf("Подтвердить слово");  //  Надпись
                            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                        }

                        //  Добавление координат символа в составную строку
                        selected_coordinates[koll][0] = current_X;
                        selected_coordinates[koll][1] = current_Y;

                        //  Разрисовываем добавленную букву
                        gotoxy((short) ((con_width_new() - 120) / 2 + strlen("+--+--+--+--+                              1 |") + current_X * 6),
                               (short) ((con_height_new() - 30) / 2 + 8 + 3 * current_Y));
                        con_set_color(CON_CLR_CYAN, CON_CLR_WHITE_LIGHT);
                        printf(" %c ", playing_field[current_X][current_Y]);
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                        continue;
                    }
                    //  Если выбрана выставленная только что буква, установлен требуемый мод и выставленная буква используется 2 раз (уже добавлена в слово) - удаляем после подтверждения Game_SecondChance_DeleteFromNotepad()
                    if (current_X == new_X && current_Y == new_Y && mode == 1 && Game_SecondChance_DeleteFromNotepad() && second_use) {
                        //  Стираем ее и убираем выделение ячейки
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Дефолтный цвет ячейки
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + current_X * 6),
                               (short)((con_height_new() - 30) / 2 + 7 + 3 * current_Y));  //  Точка смены окраса ячейки (1)
                        printf("+---+");  //  Верхушка

                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + current_X * 6),
                               (short)((con_height_new() - 30) / 2 + 8 + 3 * current_Y));  //  Точка смены окраса ячейки (2)
                        printf("|   |");  //  Средняя часть

                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + current_X * 6),
                               (short)((con_height_new() - 30) / 2 + 9 + 3 * current_Y));  //  Точка смены окраса ячейки (3)
                        printf("+---+");  //  Низушка (???)

                        //  Обнуляем координаты новой буквы и значение на поле
                        playing_field[new_X][new_Y] = 0;
                        //  Убираем второе использование
                        second_use = 0;

                        //  Стираем надпись "Подтвердить слово"
                        gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                                             +---+")),
                               (short) ((con_height_new() - 30) / 2 + 4));
                        printf("                   ");

                        int mark = 0;  //  Нахождение и удаление всех выделенных координат
                        while (selected_coordinates[mark][0] != -1) {
                            gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + selected_coordinates[mark][0] * 6),
                                   (short)((con_height_new() - 30) / 2 + 8 + selected_coordinates[mark][1] * 3));
                            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Стираем с поля ячейку
                            printf(" %c ", playing_field[selected_coordinates[mark][0]][selected_coordinates[mark][1]]);

                            selected_coordinates[mark][0] = -1;
                            selected_coordinates[mark][1] = -1;
                            mark++;
                        }

                        new_X = -1;
                        new_Y = -1;

                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             |") + current_X * 6),
                               (short)((con_height_new() - 30) / 2 + 8 + 3 * current_Y));  //  Точка смены окраса ячейки (2)

                        con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Выделение ячейки
                        printf("   ");  //  Выделяем данную ячейку
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Дефолтный цвет ячейки

                        //  Возвращаем изначальное положение мода поля
                        mode = 0;
                        continue;
                    }
                } else if (current_option == SKIP) {  //  Опция пропуска хода
                    if (mode == 1) {  //  Если выбрана буква - пропуск невозможен
                        //  Вывод сообщения о блокировке кнопки пропсука
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("       ")),
                               (short)((con_height_new() - 30) / 2 + 28));
                        con_set_color(CON_CLR_RED, CON_CLR_BLACK);
                        printf("BLOCKED");
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                        Sleep(750);
                        continue;
                    }
                    if (((turn == 0 && Player_1->Skips != 3) || turn == 1 && Player_2->Skips != 3) && Game_SecondChance_SKIP()) {  //  Подтвержден и доступен
                        if (turn == 0 && Player_1->Skips != 3) {  //  Пропуск первого игрока
                            Player_1 -> Skips++;
                            turn = (turn + 1) % 2;  //  Меняем очередь
                        } else if (turn == 1 && Player_2->Skips != 3) {  //  Пропуск второго игрока
                            Player_2 -> Skips++;
                            turn = (turn + 1) % 2;  //  Меняем очередь
                        }
                        mode = 0;
                        //  Обнуляем массив для последующей работы
                        memset(selected_coordinates, -1, sizeof (selected_coordinates));
                        //  Перерисовываем поле с данными игрока
                        TwoPlayersGame_Interface(playing_field, turn, current_option, current_X, current_Y, Player_1, Player_2);

                        //  Вывод Записанных ранее слов
                        List* dop_mark = (List*) BeginOfList;
                        //  Если ранее были записаны слова
                        if (dop_mark->word[0] != 0) {
                            //  Рисуется таблица с записанными словами
                            gotoxy((short)((con_width_new() - 120) / 2 + strlen("                      ")),
                                   (short)((con_height_new() - 30) / 2 + 4));

                            con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
                            printf("=== +--+ ===");  //  Шапка таблиы записанных слов
                            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                        }

                        int mark = 0;
                        //  Выводим список последних 10 слов
                        con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
                        while (dop_mark->word[0] != 0 && mark <= 10) {  //  Выводим все записанные слова (Ровно 10 последних слов)
                            gotoxy((short)((con_width_new() - 120) / 2 + strlen("                      ===")),
                                   (short)((con_height_new() - 30) / 2 + 5 + mark));

                            printf("%s", dop_mark->word);

                            dop_mark = (List *) dop_mark->next;
                            mark++;
                        }
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                        continue;
                    } else {  //  Отменен / Отказан
                        continue;
                    }
                } else if (current_option == NOTEPAD) {  //  Опция блокнота
                    if (turn == 0) {  //  Блокнот первого игрока
                        Game_Notepad(Player_1);
                        continue;
                    } else {  //  Блокнот второго игрока
                        Game_Notepad(Player_2);
                        continue;
                    }
                } else if (current_option == CHECK_WORD) {  //  Опция поиска слова по словарю
                    Game_FindWordInVocabulary();
                    continue;
                }
            }
            if (code == KEY_ESC) {  //  Отмена выбора ячейки или выход из игры
                if (mode == 1) {  //  Если стоит мод на выставление слова
                    //  Стираем все выделенные ячейки
                    for (int mark = 0; selected_coordinates[mark][0] != -1; mark++) {
                        //  Снимаем выделение
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             |") + selected_coordinates[mark][0] * 6),
                               (short)((con_height_new() - 30) / 2 + 8 + 3 * selected_coordinates[mark][1]));
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                        printf(" %c ", playing_field[selected_coordinates[mark][0]][selected_coordinates[mark][1]]);
                        //  Удаляем выделенное значение в массиве
                        selected_coordinates[mark][0] = -1;
                        selected_coordinates[mark][1] = -1;
                    }
                    //  Стираем надпись "Подтвердить слово" в случае ее присутсвия
                    if (second_use) {
                        gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                                             +---+")),
                               (short) ((con_height_new() - 30) / 2 + 4));
                        printf("                   ");
                    }

                    second_use = 0;  //  Убираем выделение ячейки выставленной буквы
                    //  Возвращаем выделение текущей ячейке (в случае когда во время стирания букв слова выделена была одна из этих ячеек)
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             |") + current_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + 3 * current_Y));

                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
                    printf(" %c ", playing_field[current_X][current_Y]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    continue;
                } else {
                    //  Подтверждение у пользователя о завершении игры и выход из игры
                    if (Game_SecondChance_ESC()) {
                        //  Игра продолжается (По сути вся функция нажатия клавиши F1)
                        current_X = 2;
                        current_Y = 2;
                        current_option = PLAY_FIELD;

                        TwoPlayersGame_Interface(playing_field, turn, current_option, current_X, current_Y, Player_1, Player_2);
                        if (second_use == 1) {  //  Если была выбрана выставленная буква - выводится надпись "подтверить слово"
                            gotoxy((short )((con_width_new() - 120) / 2 + strlen("                                             +---+ ")),
                                   (short) ((con_height_new() - 30) / 2 + 4));

                            con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);
                            printf("Подтвердить слово");  //  Надпись
                            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                        }

                        //  Если ранее были записаны слова
                        if (BeginOfList->word[0] != 0) {
                            List* dop_mark = (List*) BeginOfList;
                            //  Рисуется таблица с записанными словами
                            gotoxy((short)((con_width_new() - 120) / 2 + strlen("                      ")),
                                   (short)((con_height_new() - 30) / 2 + 4));

                            con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
                            printf("=== +--+ ===");  //  Шапка таблиы записанных слов
                            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                            int mark = 0;
                            //  Выводим список последних 10 слов
                            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                            while (dop_mark->word[0] != 0 && mark <= 10) {  //  Выводим все записанные слова (Ровно 10 последних слов)
                                gotoxy((short)((con_width_new() - 120) / 2 + strlen("                      ===")),
                                       (short)((con_height_new() - 30) / 2 + 5 + mark));

                                printf("%s", dop_mark->word);

                                dop_mark = (List *) dop_mark->next;
                                mark++;
                            }
                        }

                        if (mode == 1) {  //  Разрисовываем определенные части игрового поля в случае mode == 1
                            con_set_color(CON_CLR_CYAN, CON_CLR_BLACK);  //  Окрас ячейки поставленной буквы

                            gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + new_X * 6),
                                   (short)((con_height_new() - 30) / 2 + 7 + 3 * new_Y));  //  Точка смены окраса ячейки (1)
                            printf("+---+");  //  Верхушка

                            gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + new_X * 6),
                                   (short)((con_height_new() - 30) / 2 + 8 + 3 * new_Y));  //  Точка смены окраса ячейки (2)
                            printf("|   |");  //  Средняя часть

                            gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + new_X * 6),
                                   (short)((con_height_new() - 30) / 2 + 9 + 3 * new_Y));  //  Точка смены окраса ячейки (3)
                            printf("+---+");  //  Низушка (???)
                            //  Меняем на норму
                            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                            gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + new_X * 6 + 2),
                                   (short)((con_height_new() - 30) / 2 + 8 + 3 * new_Y));  //  Точка ввода буквы без выделения
                            printf("%c", playing_field[new_X][new_Y]);  //  Выставляем выставленную ранее букву

                            //  Разрисовка выделенных букв для слова
                            for (int mark = 0; selected_coordinates[mark][0] != -1; mark++) {
                                //  Раскраска символа на поле как использованного
                                gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             +-") + selected_coordinates[mark][0] * 6),
                                       (short)((con_height_new() - 30) / 2 + 8 + 3 * selected_coordinates[mark][1]));

                                //  Исключение для средней ячейки, поскольку она выделяется по своему
                                if (selected_coordinates[mark][0] == 2 && selected_coordinates[mark][1] == 2) {
                                    con_set_color(CON_CLR_CYAN, CON_CLR_WHITE_LIGHT);
                                } else {  //  Все остальные ячейки
                                    con_set_color(CON_CLR_CYAN, CON_CLR_BLACK);
                                }
                                //  Вывод подсвеченного символа
                                printf("%c", playing_field[selected_coordinates[mark][0]][selected_coordinates[mark][1]]);
                                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                            }

                        }
                        continue;
                    } else {
                        //  Выход из игры
                        Sleep(1500);  //  Задержка

                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                        clrscr();

                        Sleep(1500);

                        if (Main_Data.Animation) {
                            MainMenu_Animation_Loading();
                            Sleep(1000);
                        }

                        Menu();
                        break;  //  Не играет роли
                    }
                }
            }
            if (code == KEY_F1) {  //  Обновление окна
                current_X = 2;
                current_Y = 2;
                current_option = PLAY_FIELD;

                TwoPlayersGame_Interface(playing_field, turn, current_option, current_X, current_Y, Player_1, Player_2);
                if (second_use == 1) {  //  Если была выбрана выставленная буква - выводится надпись "подтверить слово"
                    gotoxy((short )((con_width_new() - 120) / 2 + strlen("                                             +---+ ")),
                           (short) ((con_height_new() - 30) / 2 + 4));

                    con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);
                    printf("Подтвердить слово");  //  Надпись
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                }

                //  Если ранее были записаны слова
                if (BeginOfList->word[0] != 0) {
                    List* dop_mark = (List*) BeginOfList;
                    //  Рисуется таблица с записанными словами
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                      ")),
                           (short)((con_height_new() - 30) / 2 + 4));

                    con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
                    printf("=== +--+ ===");  //  Шапка таблиы записанных слов
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    int mark = 0;
                    //  Выводим список последних 10 слов
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    while (dop_mark->word[0] != 0 && mark <= 10) {  //  Выводим все записанные слова (Ровно 10 последних слов)
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                      ===")),
                               (short)((con_height_new() - 30) / 2 + 5 + mark));

                        printf("%s", dop_mark->word);

                        dop_mark = (List *) dop_mark->next;
                        mark++;
                    }
                }

                if (mode == 1) {  //  Разрисовываем определенные части игрового поля в случае mode == 1
                    con_set_color(CON_CLR_CYAN, CON_CLR_BLACK);  //  Окрас ячейки поставленной буквы

                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + new_X * 6),
                           (short)((con_height_new() - 30) / 2 + 7 + 3 * new_Y));  //  Точка смены окраса ячейки (1)
                    printf("+---+");  //  Верхушка

                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + new_X * 6),
                           (short)((con_height_new() - 30) / 2 + 8 + 3 * new_Y));  //  Точка смены окраса ячейки (2)
                    printf("|   |");  //  Средняя часть

                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + new_X * 6),
                           (short)((con_height_new() - 30) / 2 + 9 + 3 * new_Y));  //  Точка смены окраса ячейки (3)
                    printf("+---+");  //  Низушка (???)
                    //  Меняем на норму
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             ") + new_X * 6 + 2),
                           (short)((con_height_new() - 30) / 2 + 8 + 3 * new_Y));  //  Точка ввода буквы без выделения
                    printf("%c", playing_field[new_X][new_Y]);  //  Выставляем выставленную ранее букву

                    //  Разрисовка выделенных букв для слова
                    for (int mark = 0; selected_coordinates[mark][0] != -1; mark++) {
                        //  Раскраска символа на поле как использованного
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("                                             +-") + selected_coordinates[mark][0] * 6),
                               (short)((con_height_new() - 30) / 2 + 8 + 3 * selected_coordinates[mark][1]));

                        //  Исключение для средней ячейки, поскольку она выделяется по своему
                        if (selected_coordinates[mark][0] == 2 && selected_coordinates[mark][1] == 2) {
                            con_set_color(CON_CLR_CYAN, CON_CLR_WHITE_LIGHT);
                        } else {  //  Все остальные ячейки
                            con_set_color(CON_CLR_CYAN, CON_CLR_BLACK);
                        }
                        //  Вывод подсвеченного символа
                        printf("%c", playing_field[selected_coordinates[mark][0]][selected_coordinates[mark][1]]);
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    }

                }
                continue;
            }
        }
        pause(40);  //  Пауза
    }

}
//  =============================================  Интерфейс игры  =====================================================
void COMPGame_Interface() {
    ///  Интерфейс игры с компьютером
    //TODO  Создать интерфейс игры с компьюетром
}

void TwoPlayersGame_Interface(unsigned char playing_field[5][5], int turn, Current_option option, int x, int y, struct Player* Player_1, struct Player* Player_2) {
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
                                   " 1  ---   --                                                                                     |                      ",
                                   " 2  ---   --                                                                                     +---------------------+",
                                   " 3  ---   --                                                                                                            ",
                                   " 4  ---   --                                   1     2     3     4     5                         +---------------------+",
                                   " 5  ---   --                                 +---+ +---+ +---+ +---+ +---+                       |       Блокнот        ",
                                   "+--+--+--+--+                              1 |   | |   | |   | |   | |   |                       +---------------------+",
                                   "   |  |  |                                   +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "   0  |                                      +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "      |                                    2 |   | |   | |   | |   | |   |                       |                      ",
                                   "      1                                      +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "  (\\(\\                                       +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "  ( =':')                                  3 |   | |   | |   | |   | |   |                       |                      ",
                                   "  (..(\")(\")                                  +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "+---+--+--+-+                                +---+ +---+ +---+ +---+ +---+                       |                      ",
                                   "    |  |  |                                4 |   | |   | |   | |   | |   |                       |                      ",
                                   "    |  0  |                                  +---+ +---+ +---+ +---+ +---+                       +-----+----+---+----+-+",
                                   "    |     1                                  +---+ +---+ +---+ +---+ +---+                             |    |   |    |  ",
                                   "    0                                      5 |   | |   | |   | |   | |   |                             |    |   0    |  ",
                                   " (\\__/)                                      +---+ +---+ +---+ +---+ +---+                             |    |        1  ",
                                   " (='.'=)         (\\__/)                                                                                1    |           ",
                                   " E[:]|||||[:]З   (O.o )                                                                                     0           ",
                                   " (\")_(\")         (> < )                                                                                               ",
                                   "+---------------------+                                                                          +---------------------+",
                                   "    Пропустить ход?                         (\\.../) (\\.../) (\\.../) (\\.../)                      |   Проверить слово    ",
                                   "                                            ( *.*) ('.'= ) ('.'= ) ('.'= )                       |     ___________      ",
                                   "                                            (\")_(\") (\")_(\") (\")_(\") (\")_(\")                      |                      ",
                                   "+------------------------------------------------------------------------------------------------+---------------------+" };
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
    for (int mark_y = 0, mark_x = 0; ; mark_x++) {
        if (mark_x == 5) {  //  Конец строки поля
            mark_x = 0;
            mark_y++;
        }
        if (mark_y == 5) {  //  Конец подсчета
            break;
        }
        if (playing_field[mark_x][mark_y] == 0) {  //  Пропуск в случае пустой ячейки
            continue;
        }
        //  Точка вывода
        gotoxy((short)((width_of_console - width_of_field) / 2 + strlen("                                             +-") + 6 * mark_x),
               (short)((height_of_console - height_of_field) / 2 + 8 + 3 * mark_y));
        printf("%c", playing_field[mark_x][mark_y]);
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
    printf("   %d         %d", Player_1 -> Number_of_points , Player_2 -> Number_of_points);
    //  =======================================================================================================  4  ====

    //  Вывод слов с блокнота  ================================================================================  5  ====
    if (turn == 0) {  //  Слова первого игрока
        for (int string = 0; string != 9; string++) {
            gotoxy((short)((width_of_console - width_of_field) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       | ")),
                   (short)((height_of_console - height_of_field) / 2 + 9 + string));
            printf(" %s", Player_1->Notepad[string]);
        }
    }
    else {  //  Слова второго игрока
        for (int string = 0; string != 9; string++) {
            gotoxy((short)((width_of_console - width_of_field) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       | ")),
                   (short)((height_of_console - height_of_field) / 2 + 9 + string));
            printf(" %s", Player_2->Notepad[string]);
        }
    }
    //  =======================================================================================================  5  ====

    //  Количество пропущенных ходов ==========================================================================  6  ====
    gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("        ")),
           (short) ((height_of_console - height_of_field) / 2 + 27));
    if (turn == 0) {  //  Первый игрок
        if (Player_1 -> Skips == 3) {  //  Расскраска в красный если количество пропусков максимально
            con_set_color(CON_CLR_RED, CON_CLR_BLACK);
            printf("(%d/3)", Player_1 -> Skips);
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else {
            printf("(%d/3)", Player_1->Skips);
        }
    } else {  //  Второй игрок
        if (Player_2 -> Skips == 3) {  //  Расскраска в красный если количество пропусков максимально
            con_set_color(CON_CLR_RED, CON_CLR_BLACK);
            printf("(%d/3)", Player_2 -> Skips);
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else {
            printf("(%d/3)", Player_2->Skips);
        }
    }
    //  Вывод таблицы рекордов в игре =========================================================================  6  ====
    Records_list* dop_link = (Records_list*) First_Page;  //  Доп переменнная
    //  Вывод таблицы
    for (int mark = 0; mark < 5; mark++) {
        //  Исключительные строки
        if (mark == 0 || mark == 1 || mark == 2) {  //  Первое место
            if (mark == 0) {
                con_set_color(CON_CLR_CYAN_LIGHT, CON_CLR_BLACK);
            } else if (mark == 1) {
                con_set_color(CON_CLR_YELLOW, CON_CLR_BLACK);
            } else {
                con_set_color(CON_CLR_GRAY, CON_CLR_BLACK);
            }

            gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen(" ")),
                   (short) ((height_of_console - height_of_field) / 2 + 3 + mark));
            printf("%d", mark + 1);

            gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen(" 1  ")),
                   (short) ((height_of_console - height_of_field) / 2 + 3 + mark));
            if (dop_link->next != NULL) {
                printf("%s", dop_link->name);
            } else {
                printf("---");
            }

            gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen(" 1  ---   ")),
                   (short) ((height_of_console - height_of_field) / 2 + 3 + mark));
            if (dop_link->next != NULL) {
                printf("%d", dop_link->number_of_points);
            } else {
                printf("---");
            }

            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
         }

        if (dop_link->next != NULL) {
            dop_link = dop_link->next;
        }
    }

    //  Подсветка опций  ======================================================================================  7  ====
    if (option == SKIP) {  //  Подсвечивать функцию пропуска
        gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("   ")),
               (short) ((height_of_console - height_of_field) / 2 + 26));
        con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
        printf(" Пропустить ход? ");
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    } else if (option == NOTEPAD) {  //  Подсвечивать функцию блокнота
        gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("                                             +---+ +---+ +---+ +---+ +---+                       |      ")),
               (short) ((height_of_console - height_of_field) / 2 + 7));
        con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
        printf(" Блокнот ");
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    } else if (option == CHECK_WORD) {  //  Подсвечивать функцию "Проверить слово"
        gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("    Пропустить ход?                         (\\.../) (\\.../) (\\.../) (\\.../)                      |  ")),
               (short) ((height_of_console - height_of_field) / 2 + 26));
        con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
        printf(" Проверить слово ");
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    } else if (option == PLAY_FIELD) {  //  Подсвечивать функцию "Игровое поле"
        con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Подсветка функции

        gotoxy((short) ((width_of_console - width_of_field) / 2 + strlen("                                             |") + 6 * x),
               (short) ((height_of_console - height_of_field) / 2 + 7 + 3 * y + 1));  //  Средняя часть ячейки
        if (playing_field[x][y] != 0) {
            printf(" %c ", playing_field[x][y]);
        } else {
            printf("   ");
        }

        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    }
    //  =======================================================================================================  7  ====
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

int SearchWordInTrie(const unsigned char* word) {
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
void OptionAboutProgram() {
    ///  Опция меню основной информации о программе
    //  Сообщения авторов
    const char Message_1[] = { "Создатели: Лещинский Р.Г., Комисаров Г.Н., Группа 4851003/00001" };
    const char Message_2[] = { "Год: 2021, Политехнический университет Петра Великого" };
    const char Message_3[] = { "Интитут Кибер безопасности и защиты информации (ИКиЗИ)" };
    //  Текст информации
    const char Message_4[] = { "Дата релиза: ___" };

    //  Информация для выхода
    const char Message_6[] = { "Press ESC to exit" };

    int width_of_console = con_width_new();  //  Ширина консоли
    int height_of_console = con_height_new();  //  Высота консоли

    clrscr();  //  Очистка окна и окрас консоли в цвет
    con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и красными буквами
    show_cursor(0);  //  Скрытие курсора

    gotoxy((short)((width_of_console - strlen(Message_1)) / 2), (short) ((height_of_console - 5) / 2));  //  Смещаемся для первого сообщения
    printf("%s", Message_1);  //  Вывод первого сообщения

    gotoxy((short)((width_of_console - strlen(Message_2)) / 2), (short) ((height_of_console - 5) / 2 + 1));  //  Смещаемся для второго сообщения
    printf("%s", Message_2); //  Вывод второго сообщения

    gotoxy((short)((width_of_console - strlen(Message_3)) / 2), (short) ((height_of_console - 5) / 2 + 2));  //  Смещаемся для третьего сообщения
    printf("%s", Message_3); //  Вывод третьего сообщения

    gotoxy((short)((width_of_console - strlen(Message_4)) / 2), (short) ((height_of_console - 5) / 2 + 4));  //  Смещаемся для четвертого сообщения
    printf("%s", Message_4); //  Вывод четвертого сообщения

    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и серыми буквами

    gotoxy((short)((width_of_console - strlen(Message_6)) / 2 + width_of_console / 3), 20);  //  Смещаемся для шестого сообщения
    printf("%s", Message_6); //  Вывод шестого сообщения

    int code;  //  Код символа
    while (!key_is_pressed()) {  //  Цикл для выхода
        code = key_pressed_code();  //  Считываем код символа

        if (code == KEY_ESC) {  //  Для выхода использовать ESC
            return;
        }
        pause(40);
    }
}

void OptionHelp() {
    ///  Опция меню 'Справка'
    //  Подготовка консоли к выводу меню
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Меняем на черный фон и серыми буквами
    show_cursor(0);  //  Скрытие курсора
    clrscr();  //  Очистка окна и окрас консоли в цвет

    MainMenu_OptionHelp_View(0);  //  Зарисовка меню

    int current_option = 0;  //  Текущая подсвечиваемая опция в Справке
    //  Схемы опций интерфейса
    unsigned char option_schemes[3][50] = { " Стереть рекорды "," Анимация в игре "," Словарь пользователя " };
    //  Рамеры меню
    short width_of_table = strlen("+------------------------------+-----------------------------+");
    short height_of_table = 34;
    //  Основной цикл оформления
    while (1) {

        //  Цикл клавиш
        if (!key_is_pressed()) {
            int code = key_pressed_code();  //  Запись кода нажатой клавиши

            if (code == KEY_ESC) {  //  Клавиша выхода
                return;
            }
            if (code == KEY_UP && current_option > 0) {  //  Клавиша вверх
                //  Выводим снова текущую выделенную строку с дефолтным цветом (== Стираем выделение)
                gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("                ")),
                       (short)((con_height_new() - height_of_table) / 2 + current_option * 2 + 24 + con_height_new() % 2));
                if (current_option != 2) {  //  Исключение для корректного вывода
                    printf("   ==== ");
                } else {
                    printf("==== ");
                }

                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                printf("%s", option_schemes[current_option]);

                //  Переходим к новой строке и выделяем ее
                Sleep(40);
                current_option--;
                gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("                ")),
                       (short)((con_height_new() - height_of_table) / 2 + current_option * 2 + 24 + con_height_new() % 2));
                if (current_option != 2) {  //  Исключение для корректного вывода
                    printf("   ==== ");
                } else {
                    printf("==== ");
                }

                con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Выделение (черные буквы и белый фон)
                printf("%s", option_schemes[current_option]);
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем изначальные настройки цветов

                continue;
            }
            if (code == KEY_DOWN && current_option < 2) {  //  Если клавиша вниз
                //  Выводим снова текущую выделенную строку с дефолтным цветом (== Стираем выделение)
                gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("                ")),
                       (short)((con_height_new() - height_of_table) / 2 + current_option * 2 + 24 + con_height_new() % 2));
                if (current_option != 2) {  //  Исключение для корректного вывода
                    printf("   ==== ");
                } else {
                    printf("==== ");
                }

                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                printf("%s", option_schemes[current_option]);

                //  Переходим к новой строке и выделяем ее
                Sleep(40);
                current_option++;
                gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("                ")),
                       (short)((con_height_new() - height_of_table) / 2 + current_option * 2 + 24 + con_height_new() % 2));
                if (current_option != 2) {  //  Исключение для корректного вывода
                    printf("   ==== ");
                } else {
                    printf("==== ");
                }

                con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Выделение (черные буквы и белый фон)
                printf("%s", option_schemes[current_option]);
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем изначальные настройки цветов

                continue;
            }
            if (code == KEY_ENTER) {  //  Если выбор
                Choiced_help_option(current_option);  //  Вызов обслуживающей функции
                MainMenu_OptionHelp_View(current_option);  //  Зарисовка меню
                continue;
            }
            if (code == KEY_F1) {  //  Обновить окно
                current_option = 0;
                MainMenu_OptionHelp_View(current_option);  //  Зарисовка меню
                continue;
            }
        }
        pause(40);
    }
}

void OptionPlay() {
    ///  Функция выбора опции в меню для игровых настроек
    int code = 0;  //  Нажатый символ с клавиатуры
    int current_option = 0;  //  Текущая выделяемая опция

    OptionPlay_PrePlay_View(current_option);  //  вывод первой части меню
    //  Схемы опций
    unsigned char option_schemes[2][40] = { " Против компьютера "," 2 игрока " };
    //  Размерность
    short width_of_table = strlen("+-----------------------+");  //  (36 символов)
    short height_of_table = 6;

    Sleep(300);  //  Небольшая задержка чтобы глаза не кололо
    while (1) {

        if (!key_is_pressed()) {
            code = key_pressed_code();  //  Считываем код символа

            if (code == KEY_UP) {
                if (current_option == 0) {  //  Если уже потолок опций
                    continue;
                } else {
                    //  Выводим снова текущую выделенную строку с дефолтным цветом (== Стираем выделение)
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("|      ")),
                           (short)((con_height_new() - height_of_table) / 2 + 4));

                    printf("%s", option_schemes[current_option]);

                    //  Переходим к новой строке и выделяем ее
                    Sleep(40);
                    current_option--;
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("|  ")),
                           (short)((con_height_new() - height_of_table) / 2 + 3));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Выделение (черные буквы и белый фон)
                    printf("%s", option_schemes[current_option]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем изначальные настройки цветов

                    continue;
                }
            }
            if (code == KEY_DOWN) {
                if (current_option == 1) {  //  Если ниже некуда
                    continue;
                } else {
                    //  Выводим снова текущую выделенную строку с дефолтным цветом (== Стираем выделение)
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("|  ")),
                           (short)((con_height_new() - height_of_table) / 2 + 3));

                    printf("%s", option_schemes[current_option]);

                    //  Переходим к новой строке и выделяем ее
                    Sleep(40);
                    current_option++;
                    gotoxy((short)((con_width_new() - width_of_table) / 2 + strlen("|      ")),
                           (short)((con_height_new() - height_of_table) / 2 + 4));
                    con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);  //  Выделение (черные буквы и белый фон)
                    printf("%s", option_schemes[current_option]);
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Возвращаем изначальные настройки цветов

                    continue;
                }
            }
            if (code == KEY_F1) {  //  Обновить окно
                current_option = 0;
                OptionPlay_PrePlay_View(current_option);
                continue;
            }
            if (code == KEY_ENTER) {  //  Выбранная опция
                if (current_option == 0) {  //  Против компьютера
                    OptionPlay_DifficultySelection();
                    OptionPlay_PrePlay_View(current_option);
                    continue;
                }
                if (current_option == 1) {  //  Против игрока
                    if (Main_Data.Animation == 1) {
                        Sleep(500);  //  Задержка
                        clrscr();
                        Sleep(1000);  //  Задержка
                        MainMenu_Animation_Loading();
                    }  //  Анимация загрузки
                    Pre_Game(TWO_PLAYERS);
                    break;
                }
            }
            if (code == KEY_ESC) {  //  Выход
                return;
            }
        }
        pause(40);
    }
}

void OptionTableOfRecords() {
    ///  Опция игрового меню показа рекордов
    if (Main_Data.Animation) {  //  Анимация показа
        MainMenu_Animation_TableOfRecords();
    }
    //  Высвечиваем таблицу
    OptionTableOfRecords_View();

    while (1) {

        if (!key_is_pressed()) {
            int code = key_pressed_code();

            if (code == KEY_F1) {  //  Обновление страницы
                OptionTableOfRecords_View();
                continue;
            }
            if (code == KEY_ESC) {  //  Выход из таблицы
                return;
            }
        }
        pause(40);
    }
}
//  ==========================================  Опции справки меню  ====================================================
void OptionHelp_AnimationOperation() {
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

void OptionHelp_WayOperation() {
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

    //  Требуемая строка
    unsigned char new_way[100];  //  Новый путь к словарю пользователя

    gotoxy((short)((width_of_console - width_of_picture) / 2 + 3), (short)((height_of_console - height_of_picture) / 2 + 6));
    show_cursor(1);  //  Показываем курсор
    scanf("%s", new_way);
    show_cursor(0);

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

void OptionHelp_DeleteOperation() {
    ///  Функция удаления рекордов
    //  Вывод окна предупредждения
    OptionTableOfRecords_SecondChance();
    while (1) {

        if (!key_is_pressed()) {
            int code = key_pressed_code();

            if (code == KEY_ENTER) {  //  Подтверждено удаление (==> 2 окошко подтверждения)
                //  Вывод окна
                OptionTableOfRecords_ThirdChance();
                while (1) {

                    if (!key_is_pressed()) {
                        int code_3 = key_pressed_code();

                        if (code_3 == KEY_ENTER) {  //  Окончательное удаление
                            //  Обнуляем список рекордов
                            First_Page->next = NULL;
                            return;
                        } else if (code_3 == KEY_ESC) {  //  Отмена
                            return;
                        } else if (code_3 == KEY_F1) {  //  Обновить окно
                            OptionTableOfRecords_ThirdChance();
                            continue;
                        }
                    }
                }

            } else if (code == KEY_F1) {  //  Обновить окно
                OptionTableOfRecords_SecondChance();
                continue;
            } else if (code == KEY_ESC) {  //  Отменено
                OptionTableOfRecords_SecondChance();
                break;
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
        OptionPlay();
    }
    else if (option == 1) {  //  Опция 'Рекорды'
        OptionTableOfRecords();
    }
    else if (option == 2) {  //  Опция 'О программе'
        OptionAboutProgram();  //  Вызов опции
    }
    else if (option == 3) {  //  Опция 'Справка'
        OptionHelp();
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
        OptionHelp_DeleteOperation();
    }
    else if (option == 1) {  //  Опция 'Анимации в игре'
        OptionHelp_AnimationOperation();
    } else if (option == 2) {  //  Опция 'Словарь пользователя'
        OptionHelp_WayOperation();
    }
}

int LKG(int seed) {
    ///  Линейно конгруентный генератор для генерации чисел от 0 до 1000
    return ((int)(48271 * seed + 1) % (int)pow(2, 31)) % 1000;
}

int SeekNumberInMassive(const int* massive, int length, int number) {
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

int GiveTheNumber(const char* start) {
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

char* GiveTheString(const char* start) {
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

void OptionTableOfRecords_SecondChance() {
    ///  Операция вывода окна первого подтверждения удаления
    show_cursor(0);
    //  Окно
    char table[][100] = { "+-------------------------------------+",
                          "     Вы уверены, что хотите удалить    ",
                          "               все рекорды?            ",
                          "+-------------------------------------+" };
    //  Размеры окна
    short width_of_table = strlen("+-------------------------------------+");
    short height_of_table = 4;

    // Вывод окна
    for (int string = 0; string < height_of_table; string++) {
        gotoxy((short)((con_width_new() - width_of_table) / 2) ,
               (short) ((con_height_new() - height_of_table) / 2 + string));
        //  Окрас букв
        if (string == 1 || string == 2) {
            con_set_color(CON_CLR_RED, CON_CLR_BLACK);
            printf("%s", table[string]);
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else {
            printf("%s", table[string]);
        }
    }
}

void OptionTableOfRecords_ThirdChance() {
    ///  Операция вывода окна второго подтверждения удаления
    show_cursor(0);
    //  Окно
    char table[][100] = { "+-------------------------------------+",
                          " ????    ????    ????    ????    ????  ",
                          "  Прям точно-точно?        * * *       ",
                          "       * * *        Прям точно-точно?  ",
                          "  ????    ????    ????    ????    ???? ",
                          "+-------------------------------------+" };
    //  Размеры окна
    short width_of_table = strlen("+-------------------------------------+");
    short height_of_table = 6;

    // Вывод окна
    for (int string = 0; string < height_of_table; string++) {
        gotoxy((short)((con_width_new() - width_of_table) / 2) ,
               (short) ((con_height_new() - height_of_table) / 2 + string));
        //  Окрас букв
        if (string == 2 || string == 3) {
            con_set_color(CON_CLR_RED, CON_CLR_BLACK);
            printf("%s", table[string]);
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else if (string == 1 || string == 4) {
            con_set_color(CON_CLR_GRAY, CON_CLR_BLACK);
            printf("%s", table[string]);
            con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
        } else {
            printf("%s", table[string]);
        }
    }
}
//  =======================================  Обслуживающие функции игры  ===============================================
void Game_FindWordInVocabulary() {
    ///  Функция для прорисовки итога поиска слова в дереве
    show_cursor(1);
    //  Надпись "Для отмены - "---" "
    gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                                            ( *.*) ('.'= ) ('.'= ) ('.'= )                       |  ")),
           (short) ((con_height_new() - 30) / 2 + 28));
    con_set_color(CON_CLR_CYAN_LIGHT, CON_CLR_BLACK);
    printf("Для отмены - \"---\" ");  //  Вывод бирюзовой надписи
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                                            ( *.*) ('.'= ) ('.'= ) ('.'= )                       |     ")),
           (short) ((con_height_new() - 30) / 2 + 27));

    unsigned char word[20];  //  Искомое слово
    scanf("%20s", word);  //  Вводим искомое слово
    show_cursor(0);  //  Отключаем курсор

    if (!strcmp((char*) word, "---")) {
        //  Стираем "Для отмены"
        gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                                            ( *.*) ('.'= ) ('.'= ) ('.'= )                       |  ")),
               (short) ((con_height_new() - 30) / 2 + 28));
        printf("                  ");
        return;
    }

    if (SearchWordInTrie(word)) {  //  Слово найдено
        gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                                            (\")_(\") (\")_(\") (\")_(\") (\")_(\")                      |")),
               (short) ((con_height_new() - 30) / 2 + 28));

        con_set_color(CON_CLR_GREEN, CON_CLR_BLACK);
        printf("Слово есть в словаре!");
        con_set_color(CON_CLR_WHITE_LIGHT,CON_CLR_BLACK);
    } else {  //  Слово не найдено
        gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                                            (\")_(\") (\")_(\") (\")_(\") (\")_(\")                      | ")),
               (short) ((con_height_new() - 30) / 2 + 28));

        con_set_color(CON_CLR_RED, CON_CLR_BLACK);
        printf("Слова нет в словаре!");
        con_set_color(CON_CLR_WHITE_LIGHT,CON_CLR_BLACK);
    }
    Sleep(2000);
    //  Стираем надпись о нахохождении или о ненахождения слова
    gotoxy((short) ((con_width_new() - 120) / 2 + strlen("                                            (\")_(\") (\")_(\") (\")_(\") (\")_(\")                      |")),
           (short) ((con_height_new() - 30) / 2 + 28));
    printf("                      ");
    //  Востанавливаем строку "  __________  "
    gotoxy((short) ((con_width_new() - 120) / 2 + strlen("        (0/3)                               ( *.*) ('.'= ) ('.'= ) ('.'= )                       |     ")),
           (short) ((con_height_new() - 30) / 2 + 27));
    printf("___________");
}

int Game_SecondChance_SKIP() {
    ///  Функция перевопроса пропустить ли ход (возвращает подтверждено ли)
    gotoxy((short) ((con_width_new() - 120) / 2 + strlen("  ")),
           (short) ((con_height_new() - 30) / 2 + 28));
    con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
    printf("Enter - подтвердить");
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    //  Запрсо клавиши
    while (1) {
        if (!key_is_pressed()) {

            int code = key_pressed_code();
            if (code == KEY_ESC) {  //  Отмена
                //  Стираем надпись
                gotoxy((short) ((con_width_new() - 120) / 2 + strlen("  ")),
                       (short) ((con_height_new() - 30) / 2 + 28));
                printf("                   ");

                return 0;
            } else if (code == KEY_ENTER) {  //  Подтверждение
                return 1;
            }

        }
    }
}

void Game_Notepad(Player* Player) {
    ///  Опция работы блокнота
    int current_word = 0;  //  Текущее подсвечиваемое слово в блокноте

    while (1) {
        //  Подсвет слова
        gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+                       | ")),
               (short)((con_height_new() - 30) / 2 + 9 + current_word));
        //  Ставим черный шрифт и белый фон
        con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
        if (Player->Notepad[current_word][0] != 0) {  //  Если в строке слово
            printf(" %s ", Player->Notepad[current_word]);  //  Подсвечиваемое слово
        } else {  //  Если в строке пустое поле
            printf("                    ");  //  Подсвечиваемое поле
        }
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

        //  Цикл действий пользователя в блокноте
        if (!key_is_pressed()) {
            int code = key_pressed_code();

            if (code == KEY_UP && current_word != 0) {  //  Движение вверх по списку
                //  Убираем окрас с предыдущей подсвечиваемой функции и меняем подсвет функции выше
                gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+                       | ")),
                       (short)((con_height_new() - 30) / 2 + 9 + current_word));
                if (Player->Notepad[current_word][0] != 0) {  //  Если в строке было слово
                    printf(" %s ", Player->Notepad[current_word]);  //  Убираем окрас слова
                } else {  //  Ксли было пустое поле
                    printf("                    ");  //  Убираем окрас поля
                }

                //  Уменьшаем значение текущей строки
                current_word--;
                continue;
            }
            if (code == KEY_DOWN && current_word < 8) {  //  Движение вниз по списку
                //  Убираем окрас с предыдущей подсвечиваемой функции и меняем подсвет функции ниже
                gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+                       | ")),
                       (short)((con_height_new() - 30) / 2 + 9 + current_word));
                if (Player->Notepad[current_word][0] != 0) {  //  Если в строке было слово
                    printf(" %s ", Player->Notepad[current_word]);  //  Убираем окрас слова
                } else {  //  Ксли было пустое поле
                    printf("                     ");  //  Убираем окрас поля
                }

                //  Увеличиваем значение текущей строки
                current_word++;
                continue;
            }
            if (code == KEY_ENTER) {  //  Выбор
                if (Player->Notepad[current_word][0] != 0) {  //  Выбрано слово

                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+     ")),
                           (short)((con_height_new() - 30) / 2 + 9 + current_word));
                    con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
                    printf("Удалить? (Enter)");  //  Сообщение об подтверждения удаления
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    //  Цикл для считывания выбора пользователя (удаление слова в строке)
                    while (1) {
                        if (!key_is_pressed()) {
                            int code_del = key_pressed_code();
                            if (code_del == KEY_ENTER) {  //  Подтвердить удаление
                                Player->Notepad[current_word][0] = 0;  //  Удаление
                                //  Стираем удаленное слово с таблицы
                                gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+")),
                                       (short)((con_height_new() - 30) / 2 + 9 + current_word));
                                printf("                       |                      ");
                                break;
                            }
                            if (code_del == KEY_ESC) {  //  Отмена удаления
                                //  Стирание надписи "Удалить"
                                gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+     ")),
                                       (short)((con_height_new() - 30) / 2 + 9 + current_word));
                                printf("                ");
                                break;
                            }
                        }
                    }
                    continue;
                } else {  //  Выбрано пустое поле (добавление слова в блокнот)
                    //  Стираем прошлое выделение пустой строки
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+                       | ")),
                           (short)((con_height_new() - 30) / 2 + 9 + current_word));
                    printf("                     ");

                    //  Вывод возможности отмены добавления
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+   ")),
                           (short)((con_height_new() - 30) / 2 + 9 + current_word));
                    con_set_color(CON_CLR_CYAN_LIGHT, CON_CLR_BLACK);
                    printf("Для отмены - \"---\" ");  //  Вывод бирюзовой надписи
                    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                    //  Ввод нового слова
                    gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+                       |  ")),
                           (short)((con_height_new() - 30) / 2 + 9 + current_word));
                    show_cursor(1);
                    unsigned char word[20];   //  Вводимое слово в блокнот
                    scanf("%19s", word);
                    show_cursor(0);

                    //  Проверка отмены
                    if (!strcmp((char*)word, "---")) {  //  Если написана строка отмены
                        //  Стираем надписи
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+")),
                               (short)((con_height_new() - 30) / 2 + 9 + current_word));
                        printf("                       |                      ");
                        continue;
                    }

                    //  Ввод слова
                    if (strlen((char*)word) <= 15) {  //  Запись корректного слова
                        //  Стираем прошлые записи
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+")),
                               (short)((con_height_new() - 30) / 2 + 9 + current_word));
                        printf("                       | ");
                        //  Запись нового слова в данные игрока
                        strcpy((char*)Player->Notepad[current_word], (char*)word);
                        continue;
                    } else {  //  Длина больше 15
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+   ")),
                               (short)((con_height_new() - 30) / 2 + 9 + current_word));
                        con_set_color(CON_CLR_RED, CON_CLR_BLACK);
                        printf("Слишком длинное    ");  //  Вывод ошибки (класный шрифт, черный фон)
                        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                        Sleep(2000);
                        //  Стираем и отменяем
                        gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+")),
                               (short)((con_height_new() - 30) / 2 + 9 + current_word));
                        printf("                       |                      ");
                        continue;
                    }
                }
            }
            if (code == KEY_ESC) {  //  Выход
                //  Стираем выделение стрроки
                gotoxy((short)((con_width_new() - 120) / 2 + strlen("   |  |  |                                   +---+ +---+ +---+ +---+ +---+                       | ")),
                       (short)((con_height_new() - 30) / 2 + 9 + current_word));

                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);  //  Стандартные цвета
                if (Player->Notepad[current_word][0] != 0) {  //  Если в строке было слово
                    printf(" %s ", Player->Notepad[current_word]);  //  Убираем подсвет
                } else {  //  Если в строке было пустое поле
                    printf("                    ");  //  Убираем подсвет
                }

                return;
            }
        }
    }
}

int Game_SecondChance_DeleteFromNotepad() {
    ///  Предупреждение об удалении выставленного символа
    //  Точка вывода сообщения
    gotoxy((short)((con_width_new() - 120) / 2 + strlen(" E[:]|||||[:]З   (O.o )                             ")),
           (short)((con_height_new() - 30) / 2 + 23));
    con_set_color(CON_CLR_YELLOW_LIGHT, CON_CLR_BLACK);
    printf("Удалить? (Enter)");
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    //  Запрсо клавиши
    while (1) {
        if (!key_is_pressed()) {

            int code = key_pressed_code();
            if (code == KEY_ESC) {  //  Отмена
                //  Стираем запись подтверждения
                gotoxy((short)((con_width_new() - 120) / 2 + strlen(" E[:]|||||[:]З   (O.o )                             ")),
                       (short)((con_height_new() - 30) / 2 + 23));
                printf("                ");

                return 0;
            } else if (code == KEY_ENTER) {  //  Подтверждение
                //  Стираем запись подтверждения
                gotoxy((short)((con_width_new() - 120) / 2 + strlen(" E[:]|||||[:]З   (O.o )                             ")),
                       (short)((con_height_new() - 30) / 2 + 23));
                printf("                ");
                return 1;
            }

        }
    }
}

void Game_ChangeCell(int KEY, int* current_X, int* current_Y, unsigned char playing_field[5][5], int selected_coordinates[][2]) {
    ///  Функция для раскраски новой ячейки на игровом поле
    //  Данные по игровому полю
    short width_of_field = 120;
    short height_of_field = 30;

    //  Перерисовываем прошлую ячейку поля (стираем выделение)
    //  Проверка на присутсвие такой координаты в записанных
    int check = 0;
    for (int mark = 0; selected_coordinates[mark][0] != -1; mark++) {
        if (selected_coordinates[mark][0] == *current_X && selected_coordinates[mark][1] == *current_Y) {
            check = 1;  //  Данная координата есть в записанных  >>  Выделяем ее другим шрифтом
            break;
        }
    }

    if (check) {  //  Раскраска записанной
        con_set_color(CON_CLR_CYAN, CON_CLR_BLACK);
    } else {
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    }

    gotoxy((short)((con_width_new() - width_of_field) / 2 + strlen("    0                                      5 |") + *current_X * 6),
           (short)((con_height_new() - height_of_field) / 2 + 8 + *current_Y * 3));
    if (playing_field[*current_X][*current_Y] != 0) {
        printf(" %c ", playing_field[*current_X][*current_Y]);  //  Стираем выделение ячейки с буквой
    } else {
        printf("   ");  //  Стираем выделение пустой ячейки
    }

    //  Меняем координату
    if (KEY == KEY_UP) {
        (*current_Y)--;
    } else if (KEY == KEY_DOWN) {
        (*current_Y)++;
    } else if (KEY == KEY_RIGHT) {
        (*current_X)++;
    } else if (KEY == KEY_LEFT) {
        (*current_X)--;
    }

    //  Переходим к новой ячейке
    Sleep(40);
    gotoxy((short)((con_width_new() - 120) / 2 + strlen("    0                                      5 |") + *current_X * 6),
           (short)((con_height_new() - 30) / 2 + 8 + *current_Y * 3));

    //  Проверка на присутсвие такой координаты в записанных
    check = 0;
    for (int mark = 0; selected_coordinates[mark][0] != -1; mark++) {
        if (selected_coordinates[mark][0] == *current_X && selected_coordinates[mark][1] == *current_Y) {
            check = 1;  //  Данная координата есть в записанных  >>  Выделяем ее другим шрифтом
            break;
        }
    }

    if (check) {  //  Раскраска
        con_set_color(CON_CLR_CYAN, CON_CLR_WHITE_LIGHT);
    } else {
        con_set_color(CON_CLR_BLACK, CON_CLR_WHITE_LIGHT);
    }

    if (playing_field[*current_X][*current_Y] != 0) {
        printf(" %c ", playing_field[*current_X][*current_Y]);  //  Выделяем ячейку с буквой
    } else {
        printf("   ");  //  Выделяем пустую ячейку
    }
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
}

void Game_AddWordInList(unsigned char* word) {
    ///  Функция добавления нового слова в список всех слов
    //  Создаем новую часть списка
    List* new_List = (List*) malloc(sizeof(List));
    new_List -> next = (struct List *) BeginOfList;  //  Добавляем прошлое начало списка в новую часть
    strcpy((char*) new_List -> word, (char*) word);  //  Записываем слово

    BeginOfList = (List*) new_List;  //  Записываем начало списка
}

void Game_EndOfGame(Player* Player_1, Player* Player_2) {
    ///  Функция подведение итогов игры
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    clrscr();
    show_cursor(0);

    //  Усыпляем компилятор перед выодом первой  картинки
    Sleep(2000);
    Game_EndOfGame_View(0);  //  Выводим "Игра окончена"

    //  Усыпляем компилятор для вывода второй картинки
    Sleep(3000);
    Game_EndOfGame_View(1);  //  Выводим шаблон картинки с победителем
    //  Дорисовываем ник победителя
    gotoxy((short) ((con_width_new() - strlen("/ +--+  +----+  +----+  +--+ \\")) / 2 + strlen("/     Победитель - ")),
            (short) ((con_height_new() - 6) / 2 + 2));
    //  Устанавливаем цвет и выводим ник
    if (Player_1->Number_of_points > Player_2->Number_of_points) {  //  Победил первый
        con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);
        printf("%s", Player_1->Name);
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    } else {  //  Победил второй
        con_set_color(CON_CLR_BLUE_LIGHT, CON_CLR_BLACK);
        printf("%s", Player_2->Name);
        con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    }

    //  Усыпляем консоль для вывода третьей картинки
    Sleep(3000);
    Game_EndOfGame_View(2);  //  Выводим итоговые очки
    //  Дорисовываем имена игроков (либо Игрок_1 и Игрок_2, либо Игрок_1 и COMP)
    gotoxy((short) ((con_width_new() - strlen("/ +--+  +------------+  +--+ \\")) / 2 + strlen("/     ")),
           (short) ((con_height_new() - 6) / 2 + 2));
    printf("%s", Player_1->Name);  //  Имя первого
    gotoxy((short) ((con_width_new() - strlen("/ +--+  +------------+  +--+ \\")) / 2 + strlen("/     _______    ")),
           (short) ((con_height_new() - 6) / 2 + 2));
    printf("%s", Player_2->Name);  //  Имя второго

    gotoxy((short) ((con_width_new() - strlen("/ +--+  +------------+  +--+ \\")) / 2 + strlen("\\       ")),
           (short) ((con_height_new() - 6) / 2 + 3));
    printf("%d", Player_1->Number_of_points);  //  Очки первого
    gotoxy((short) ((con_width_new() - strlen("/ +--+  +------------+  +--+ \\")) / 2 + strlen("\\       __    :    ")),
           (short) ((con_height_new() - 6) / 2 + 3));
    printf("%d", Player_2->Number_of_points);  //  Очки второго

    //  Усыпляем консоль для вывода четвертой картинки
    Sleep(3000);
    Game_EndOfGame_View(3);  //  Выводим окошко с требованием ввести имя победителя

    int symbol = 0;  //  Текущий записываемый символ
    unsigned char name[4];  //  Записаное имя (3 символа)
    name[3] = 0;  //  Закрываем строку

    show_cursor(1);
    while (1) {
        gotoxy((short) ((con_width_new() - strlen("/ +--+  ++  ++  ++  ++  +--+ \\")) / 2 + strlen("\\           ") + symbol*2),
               (short) ((con_height_new() - 6) / 2 + 3));  //  Исходная позиция ввода на консоли

        //  Требуем английский символ, выводим его
        int code = symbol_code(1);
        if (code != 0) {  //  Если введена буква
            name[symbol++] = code;  //  Записываем букву и увеличиваем номер записываемой ячейки
        } else if (code == 0 && symbol != 0){  //  Нажат Backspace
            gotoxy((short) ((con_width_new() - strlen("/ +--+  ++  ++  ++  ++  +--+ \\")) / 2 + strlen("\\           ") + symbol*2 - 2),
                   (short) ((con_height_new() - 6) / 2 + 3));
            printf("_");  //  Стираем прошлую букву

            name[symbol--] = 0;  //  Удаляем букву и уменьшаем номер записываемой ячейки
        }

        if (symbol == 3) {  //  Если записано имя
            show_cursor(0);
            break;
        }
    }
    //  >>  Получаем name - имя/ник игрока победителя

    //  Записываем данные победителя в таблицу рекордов
    //  Дополнительный адрес
    Records_list* dop_link = First_Page;
    //  Запись итоговых очков победителя
    int final_points;
    if (Player_1->Number_of_points > Player_2->Number_of_points) {
        final_points = Player_1->Number_of_points;
    } else {
        final_points = Player_2->Number_of_points;
    }
    //  Ищем место в списке для подстановки результатов
    while (dop_link-> next != NULL && dop_link->number_of_points > final_points) {
        dop_link = dop_link->next;
    }
    //  Запись
    if (dop_link->next == NULL) {  //  Если дошли до конца списка
        RECORDSFile_AddNewPage();  //  Новый адрес
        dop_link->number_of_points = final_points;  //  Запись очков
        strcpy((char*) dop_link->name, (char*) name);  //  Запись имени
    } else {  //  В середине списка
        //  Создаем новую страницу
        Records_list* new_page = (Records_list*) malloc(sizeof (Records_list));
        //  Заполняем данными
        new_page->number_of_points = final_points;
        strcpy((char*) new_page->name, (char*) name);
        //  Доп переменные
        int a;
        unsigned char b[4];
        //  Копируем в переменные
        a = dop_link->number_of_points;
        strcpy((char*) b, (char*) dop_link->name);
        //  Связываем с остальным списком
        strcpy((char*) dop_link->name, (char*) new_page->name);  //  Имя
        dop_link->number_of_points = new_page->number_of_points;  //  Очки
        //  Окончательно меняем местами данные в двух страницах рекордов (new_page и dop_link)
        strcpy((char*) new_page->name, (char*) b);  //  Имя
        new_page->number_of_points = a;  //  Очки

        new_page->next = dop_link->next;
        dop_link->next = new_page;
    }
    //  >>  Записали данные в таблицу рекордов

    Sleep(2000);
    clrscr();  //  Очищаем консоль
    Sleep(2000);
    if (Main_Data.Animation) {  //  Если есть разрешение
        MainMenu_Animation_Loading();  //  Запускаем анимацию загрузки (выход их игры)
        Sleep(1000);  //  Добавочная пауза
    }

    Menu();  //  Запускаем меню
}

int Game_RepeatCheck(unsigned char* new_word) {
    ///  Проверка нового слова на повторение (Записанные слова ранее хранятся в списке с корнем BeginOfList) (1 - найдено совпадение)
    //  Дополнительный адрес на начало списка
    List* dop_link = (List* ) BeginOfList;
    int mark = 0;
    //  Пробег по списку и сравнение нового слова с записанными
    while (dop_link->word[0] != 0) {
        //  Сравнение
        if (!strcmp((char*) dop_link->word, (char*) new_word)) {
            return 1;
        } else {
            //  Переход к следующему элементу списка
            dop_link = (List*) dop_link->next;
        }
    }

    return 0;
}

int Game_SecondChance_ESC() {
    ///  Функция проверки у пользователя о выходе из игры
    //  Основная картинка
    char picture[][75] = { "+---------------------------------+",
                           "   Вы уверены, что хотите выйти?   ",
                           "                                   ",
                           "  [+] YES [+]     [-] NO [-]     ",
                           "+---------------------------------+"};
    //  Размеры картинки
    short width_of_picture = strlen("+---------------------------------+");
    short height_of_picture = 5;
    //  Вывод чертежа
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
    for (int mark = 0; mark < height_of_picture; mark++) {
        gotoxy((short) ((con_width_new() - width_of_picture) / 2),
               (short) ((con_height_new() - height_of_picture) / 2 + mark));
        printf("%s", picture[mark]);

    }

    int current_option = 1;  //  Текущая активная опция
    //  Разрисовываем начальную опцию
    gotoxy((short) ((con_width_new() - width_of_picture) / 2 + strlen("  [+] YES [+]     ")),
           (short) ((con_height_new() - height_of_picture) / 2 + 3));
    con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);
    printf("[-] NO [-]");
    con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

    while (1) {

        if (!key_is_pressed()) {
            int code = key_pressed_code();

            if (code == KEY_LEFT && current_option != 0) {  //  Сдвиг влево
                //  Убираем подсветку с правой опции
                gotoxy((short) ((con_width_new() - width_of_picture) / 2 + strlen("  [+] YES [+]     ")),
                       (short) ((con_height_new() - height_of_picture) / 2 + 3));
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                printf("[-] NO [-]");

                //  Подсвечиваем левую опцию
                gotoxy((short) ((con_width_new() - width_of_picture) / 2 + strlen("  ")),
                       (short) ((con_height_new() - height_of_picture) / 2 + 3));
                con_set_color(CON_CLR_GREEN_LIGHT, CON_CLR_BLACK);
                printf("[+] YES [+]");
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                current_option--;
                continue;
            }
            if (code == KEY_RIGHT && current_option != 1) {  //  Сдвиг вправо
                //  Убираем подсветку с левой опции
                gotoxy((short) ((con_width_new() - width_of_picture) / 2 + strlen("  ")),
                       (short) ((con_height_new() - height_of_picture) / 2 + 3));
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);
                printf("[+] YES [+]");

                //  Подсвечиваем правую опцию
                gotoxy((short) ((con_width_new() - width_of_picture) / 2 + strlen("  [+] YES [+]     ")),
                       (short) ((con_height_new() - height_of_picture) / 2 + 3));
                con_set_color(CON_CLR_RED_LIGHT, CON_CLR_BLACK);
                printf("[-] NO [-]");
                con_set_color(CON_CLR_WHITE_LIGHT, CON_CLR_BLACK);

                current_option++;
                continue;
            }
            if (code == KEY_ENTER) {  //  Выбрано действие
                return current_option;
            }
            pause(40);
        }
    }
}
//  ==============================  Функции работы с файлом Data.txt и RECORDS.txt =====================================
int DataFile_CheckFile(const char* Data) {
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

char* DataFile_OpenDataFile() {
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
            file = DataFile_CreateDefaultDataFile();
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
    if (!DataFile_CheckFile(Data)) {  //  Если нарушена целостность
        Choiced_option(5, "File Data.txt damaged!");
    }

    DataFile_RecordTheDataInStruct(Data);  //  Запись данных в структуру

    fclose(file);  //  Закрываем файл
    return Data;
}

void DataFile_SaveDataInFile() {
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

void DataFile_RecordTheDataInStruct(char* Data) {
    ///  Запись информации в файле Data.txt в структуру Main_Data для удобства получения значений
    char* point;
    //  Запись значения Random в структурное поле
    point = strstr(Data, "Random");  //  Запись адреса на тип Random в файле
    Main_Data.Random = GiveTheNumber(point + 7);
    //  Запись значения Animation в структурное поле
    point = strstr(Data, "Animation");  //  Запись адреса на тип Animation в файле
    Main_Data.Animation = GiveTheNumber(point + 10);
    //  Запись значения User_dictionary в структурное поле
    point = strstr(Data, "User_dictionary");  //  Запись адреса на тип User_dictionary в файле
    Main_Data.User_dictionary = GiveTheString(point + strlen("User_dictionary "));
}

FILE* DataFile_CreateDefaultDataFile() {
    ///  Функция для создания default версии файла Data.txt в случае его отсутствия (возвращает ссылку на default файл)
    ///    default значения - Random 0; Animation 1; User_dictionary VOC.txt;
    FILE* default_file;  //  Новый default файл
    //  Создание файла и заполнение стандартных данных
    fopen_s(&default_file, "Data.txt", "w");

    fprintf_s(default_file, "Random 0\nAnimation 1\nUser_dictionary VOC.txt");

    return default_file;
}

void RECORDSFile_Record() {
    ///  Основная процедура для работы с файлом RECORDS.txt - открытие и все вытекающее
    //  Открытие требуемого файла (система создаетля)
    FILE* Record_file = fopen("C:\\Users\\Romario\\Desktop\\RECORDS.txt","r");

    if (Record_file == NULL) {  //  Если файла не было найдено (в системе создателя)
        Record_file = fopen("RECORDS.txt", "r");
        if (Record_file == NULL) {  //  Если не был найден файл рекордов по стандартному пути
            //  Вызов функции создания default версии
            Record_file = RECORDSFile_CreateDefault();
        }
    }
    //  Создаем первую страницу
    First_Page = (Records_list*) malloc(sizeof (Records_list));
    First_Page->next = NULL;  //  Делаем заглушку в адресе
    First_Page->name[0] = '\0';  //  Делаем заглушку в имени

    //  Дополнительный адрес для работы
    Records_list* dop_link = First_Page;
    unsigned char current_string[10];  //  Текущая записанная строка

    while (fgets((char*) current_string, 10, Record_file) != NULL) {
        memset(dop_link->name, 0, sizeof (dop_link->name));

        //  Считываем ник игрока
        for (int mark = 0; current_string[mark] != ' ' && current_string[mark] != '\n'; mark++) {
            if (mark == 3) {
                //  Имя может иметь лишь 3 буквы - в ином случае файл поврежден
                Choiced_option(5,"File RECORDS.txt is damaged!!!");
                break;  //  Не играет никакой роли
            }
            dop_link->name[mark] = current_string[mark];
        }
        //  Считываем его игровые очки
        dop_link->number_of_points = 0;
        for (int mark = 4; current_string[mark] != '\0' && current_string[mark] != '\n'; mark++) {
            if (!(current_string[mark] >= 48 && current_string[mark] <= 57)) {
                //  Если было найдено не число => файл поврежден
                Choiced_option(5,"File RECORDS.txt is damaged!!!");
            }
            dop_link->number_of_points = dop_link->number_of_points * 10 + (current_string[mark] - 48);
        }
        //  >>  Считали все данные игрока

        RECORDSFile_AddNewPage();  //  Добавляем новый элемент заглушку в список
        //  Переходим к следующей странице
        dop_link = dop_link->next;
        //  Обновляем строку
        memset(current_string, 0, sizeof (current_string));
    }

}

FILE* RECORDSFile_CreateDefault() {
    ///  Функция создания default версии файла RECORDS в случае его отсутсвия
    //  Создание файла
    FILE* new_file = fopen("RECORDS.txt", "w");

    return new_file;
}

void RECORDSFile_AddNewPage() {
    ///  Функция добавления страниц в список рекордов
    //  Дополнительный адрес для работы
    Records_list* dop_link = First_Page;

    //  Доходим до конца списка
    while (dop_link->next != NULL) {
        dop_link = dop_link->next;
    }

    //  Создаем новый элемент
    Records_list* new_page = (Records_list*) malloc(sizeof (Records_list));

    new_page->next = NULL;
    new_page->name[0] = '\0';

    //  Записываем в список новый элемент
    dop_link->next = new_page;
}

void RECORDSFile_SaveDataInFile() {
    ///  Процедура сохранения данных из списка рекордо в файл RECORDS,txt
    FILE* file;  //  Файл
    //  Проверка на систему
    if (fopen("C:\\Users\\Romario\\Desktop\\RECORDS.txt","r") != NULL) {  //  Автор
        file = fopen("C:\\Users\\Romario\\Desktop\\RECORDS.txt","w");
    } else {  //  Пользователь
        file = fopen("RECORDS.txt","w");
    }
    //  Доп адрес
    Records_list* dop_link = (Records_list*) First_Page;
    while (dop_link->next != NULL) {
        fprintf(file,"%s %d\n", dop_link->name, dop_link->number_of_points);

        dop_link = dop_link->next;
    }
}