#include "input_output.h"
#include "actions.h"

int main(){
    setlocale(LC_ALL, ""); // Установка локали
    Text text; // Объявление структуры текста
    wprintf(L"Course work for option 5.3, created by Elena Guseva\n"); // Вывод информации о курсовой работе
    wchar_t command = ' '; // Объявление переменной для выбора функции
    command = getwchar(); // Получение символа от пользователя из stdin
    getwchar(); // Получение символа новой строки
    switch(command){ // Выбор действия в зависимости от введенного символа пользователем
        case '0':
            text = readText(); // Чтение текста
            removeEmptySentences(&text); // Удаление пустых предложений
            removeCopies(&text); // Удаление копий
            printText(text); // Вывод текста в stdout
            freeText(&text); // Освобождение памяти
            break;
        case '1':
            text = readText();
            removeEmptySentences(&text);
            removeCopies(&text);
            findMask(&text);
            //printText(text);
            freeText(&text);
            break;
        case '2':
            text = readText();
            removeEmptySentences(&text);
            removeCopies(&text);
            removeNotUpperSentence(&text);
            printText(text);
            freeText(&text);
            break;
        case '3':
            text = readText();
            removeEmptySentences(&text);
            removeCopies(&text);
            sort_by_latin_letters(text);
            printText(text);
            freeText(&text);
            break;
        case '4':
            text = readText();
            removeEmptySentences(&text);
            removeCopies(&text);
            povtor(text);
            printText(text);
            freeText(&text);
            break;
        case '5':
            wprintf(L"1)Для каждого предложения вывести строку образец, удовлетворяющую каждому слову в предложении. Строка условия содержит: символы, ? - 1 или больше любых символов, в начале и конце образца могут быть символы * - обозначающие 0 или больше символов. Например, для слов “Аристотель” и “Артишок”, строка образец будет иметь вид “Ар???о?*”.\n2)Удалить все предложения, в которых нет заглавных букв в начале слова.\n3)Отсортировать слова в предложении по количеству гласных букв в слове.\n4)Для каждого предложения вывести количество одинаковых слов в строке в формате “Количество одинаковых слов: <число>”, где <число> – кол-во одинаковых слов. Пример: “курсовая сдана, курсовая сдана сдана.” -> “Количество одинаковых слов: 5” (потому что 2 раза “курсовая” и 3 раза “сдана”). Слова сравнивать без учёта регистра\n");
            break;
        default:
            wprintf(L"Неверный выбор действия, повторите попытку...\n");
            return 0;
    }
    return 0;
}