#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct element_of_block {  //  Символ (Элемент) блока
    int symbol_code;  //  Числовое значение символа
    struct block* next;  //  Адрес на последующие блоки
} Element;

typedef struct block {  //  Блок (Узел) дерева
    struct element_of_block* elements[50];  //  Массив адресов элементов в блоке
    int null_element;  //  NULL элемент для определения заглушки
} Block;

struct block* Global_root;  //  Корень в глобальной переменной

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

void Delete_word(const unsigned char* word) {
    ///  Функция поиска слова из дерева (Не работает в Clion, лишь в VS из-за разного представления кириллицы)
    if (!Search_word(word)) {  //  Если слова нет в дереве
        printf("No comparing!!!\n");
    }

    //TODO  Доделать удаление из дерева

}

int main() {

    FILE* file;  //  Файл со словарем
    fopen_s(&file, "C:\\Users\\Romario\\Desktop\\VOC_1.txt", "r");

    unsigned char current_string[100];  //  Текущая строка из файла
//  ====================================================================================================================
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

    struct block Root;  //  Корень
    Global_root = (struct block *) &Root;

    Root.null_element = 1;  //  Поскольку он является корневым, он не может быть заглушкой
    memset (Root.elements, 0, sizeof (Root.elements));  //  Обнуляем массив элементов в корне

    struct block* dop_mark;  //  Дополнительный адрес корня для работы
    int symbol_key;  //  Номер ключа слова (номера каждого его элемента на определенном этапе: 0 .. strlen(word))

    //  Основной цикл для считывания каждого слова из словаря и создания префиксного дерева (CREATING)
    while (fgets(current_string, 100, file) != 0) {  //  Блок цикла для считывания слова
        dop_mark = &Root;  //  Возвращаемся к корню

        for (symbol_key = 0; current_string[symbol_key] != '\n' && current_string[symbol_key] != '\0'; symbol_key++) {
            int check_mark = 0;  //  Отметка проверки нахождения элемента в блоке (0 - не найден)


            //  Цикл проверки на нахождение в блоке символа (Случай нахождения символа в блоке)
            for (int element_check = 1; dop_mark -> elements[element_check] != NULL; element_check++) {
                //  Если числовое значение символа равно значению уже записанного
                if (dop_mark -> elements[element_check] -> symbol_code == (int) current_string[symbol_key]) {
                    dop_mark = dop_mark -> elements[element_check] -> next;  //  Переходим к новому блоку

                    check_mark = 1;  //  Найден схожий элемент
                    break;
                }
            }  //  >>  check_mark - Найден ли схожий элемент (Если да, dop_mark - адрес на последующий блок)

            if (!check_mark) {  //  Если не найден элемент (Случай ненахождения символа в блоке)

                Element* new_element = (Element*) malloc(sizeof (Element));  //  Новая структура под новый элемент в узле
                new_element -> symbol_code = (int) current_string[symbol_key];  //  Записываем текущий символ

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

                if (symbol_key + 1 == strlen((char*) current_string) - 1) {  //  Если достигнут конец записываемого слова (Учет случая записи "аб", а после "абордаж")
                    Element* new_element = (Element*) malloc(sizeof (Element));  //  Новая структура для элемента-заглушки в заглушку-блок
                    new_element -> symbol_code = 'E';  //  Значение символа-заглушки

                    dop_mark -> elements[0] = new_element;
                }
            }


        }  //  >>  Записанное слово current_string в дерево-словарь

        if (dop_mark -> null_element == 1) {  //  Случай "aб" при записанном "абордаж" (Слово входит в записанное ранее) => нет заглушки
            Element* new_element = (Element*) malloc(sizeof (Element));  //  Новая структура для элемента-заглушки
            new_element -> symbol_code = 'E';  //  Значение символа-заглушки

            dop_mark -> elements[0] = new_element;  //  Запись в отдельную ячейку под заглушку
        }

    }  //  >>  Созданное дерево
//  ====================================================================================================================


    if (Search_word("аб")) {
        printf("Comparing!!!\n");
    } else {
        printf("No comparing!!!\n");
    }



    return 0;
}