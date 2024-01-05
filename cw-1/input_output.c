#include "input_output.h"

Word readWord(){ // функция считывает слово из stdin и возвращает его в виде структуры Word
    int size = MEMORY_STEP; // инициализация первоначального размера слова, устанавливаем размер равный MEMORY_STEP
    int len = 0; // инициализация длины слова
    wchar_t * chars = malloc(size*sizeof(wchar_t)); //выдялем память под массив широких символов
    wchar_t sep, symbol = L' '; //объявляем две переменные sep и symbol и инициализируем их пробелом

    while(symbol == L' ') // цикл пока symbol равен пробелу
        symbol = getwchar(); // считываем следующий символ из stdin

    while(!wcschr(WORD_SEP, symbol) && !wcschr(SENTENCE_SEP, symbol)){ // цикл продолжается, пока symbol не является разделителем слов и не является разделителем предложений
        chars[len++] = symbol; //добавляем символ в массив и увеличиваем длину массива на 1 соответственно
        if(len == size){ // если длина слова стала равна первоначально выделенному размеру
            size += MEMORY_STEP; // увеличиваем размер на MEMORY_STEP
            chars = realloc(chars, size*sizeof(wchar_t)); // перевыделяем память для массива символов с новым размером
        }
        symbol = getwchar(); //вводим следующий символ с помощью getwchar
    }
// после цикла
    chars[len] = L'\0'; //добавляем нулевой символ в конец строки
    sep = symbol; //устанавливаем sep равным symbol, который в этот момент является либо разделителем слов, либо разделителем предложений

    return (Word){chars, sep, len}; // возвращаем структуру Word с нашим массивом слов, символом-разделителем и длиной слова
}

Sentence readSentence(){ // функция считывает предложение из stdin, состоящее из слов и возвращает его в виде структуры Sentence
    int size = MEMORY_STEP; // инициализируем первоначальный размер предложения
    int len = 0; // инициализируем длину предложения
    Word* words = malloc(size*sizeof(Word)); // выделяем память для массива слов
    Word word; // объявляем переменную слова

    do{
        word = readWord(); // записываем первое прочтенное слово в переменную word
        words[len++] = word; //записываем слово word в массив слов words и увеличиваем длину предложения
        if(len == size){ // если длина предложения стала равна размеру предложения
            size += MEMORY_STEP; // увеличиваем размер предложения на MEMORY_STEP
            words = realloc(words, size*sizeof(Word)); // перевыделяем память для массива слов
        }
    }while(!wcschr(SENTENCE_SEP, word.sep)); // цикл продолжается пока разделитель слова word.sep не станет SENTENCE_SEP

    return (Sentence){words, len}; // Возвращаем структуру Sentence с массивом слов и длиной предложения
}



Text readText(){ // функция считывает текст из stdin и возвращает его в виде структуры Text
    Text text;
    text.len = 0;
    text.sents = (Sentence*) malloc(sizeof(Sentence));
    Sentence new_sentence = readSentence();
    while(true){
        bool main_flag = true;
        for (unsigned i = 0; i < text.len; i++){
            bool second_flag = true;
            Sentence old_sentence = text.sents[i];
            if (new_sentence.len == old_sentence.len){
                for (unsigned j = 0; j < old_sentence.len; j++){
                    if((wcscasecmp(new_sentence.words[j].chars, old_sentence.words[j].chars) != 0) || (new_sentence.words[j].sep != old_sentence.words[j].sep)){
                        second_flag = false;
                        break;
                    }
                }
                if (second_flag){
                    main_flag = false;
                    break;
                }
            }
        }
        if (main_flag){
            text.sents[text.len++] = new_sentence;
            text.sents = (Sentence*) realloc(text.sents, sizeof(Sentence)*(text.len+1));
            if ((new_sentence.words[new_sentence.len -1].sep == L'\n') || (new_sentence.words[new_sentence.len - 1].sep == WEOF)) break;
        }
        new_sentence = readSentence();
    }

    return text;
}

void printText(Text text){ // функция выводит каждое слово в каждом предложении текста
    for(Sentence* sent = text.sents; sent < text.sents + text.len; sent++) { // для каждого предложения в тексте
        for (Word *word = sent->words; word < sent->words + sent->len; word++) { // для каждого слова в предложении
            if (word->len) { // если массив символов не пустой
                wprintf(L"%ls%lc", word->chars, word->sep); // выводим слово вместе с разделителем
                if (word->sep != L' ') // если разделитель это не пробел
                    wprintf(L" "); // выводим дополнительно пробел
            }
        }
        wprintf(L"\n"); // выводим символ переноса строки
    }
}

void findAndPrint(Sentence* sent, wchar_t* prevWord){ // функция ищет prevWord в предложении и если находит, выводит все слова предложения, подчеркивая и выделяя красным цветом prevWord
    int flag = 0; // инициализируем флаг для отслеживания prevWord
    for(Word *word = sent->words; word < sent->words + sent->len && !flag; word++){ // проходимся через каждое слово в предложении, пока не найдем prevWord (т.е. пока flag равен 0)
        if(!wcscasecmp(word->chars, prevWord)) // Сравниваем текущее слово с prevWord c помощью функции wcscasecmp()
            flag = 1; // если слова совпадают, выставляем флаг равный 1
    }
    if(!flag) return; // если после перехода по всем слова flag все еще равен нулю, функция завершает работу
    for(Word *word = sent->words; word < sent->words + sent->len; word++){ // если prevWord найден, проходимся еще раз словам предложения
        if(!wcscasecmp(word->chars, prevWord)) // если слово совпадает с prevWord
            wprintf(L"\033[4;31m%ls\033[0m%lc", word->chars, word->sep); // выводим слово с подчеркиванием и красным цветом
        else // иначе
            wprintf(L"%ls%lc", word->chars, word->sep); // выводим слово без обработки
        if (word->sep != L' ') // если разделитель слова не является пробелом
            wprintf(L" "); // дополнительно выводим пробел
    }
    wprintf(L"\n"); // выводим символ конца строки
}

void printSentences(Text* text){ // функция проходит через каждое предложение в тексте, и если в предложении есть последнее слово предыдущего предложения, с помощью доп. функции findAndPrint() выводит все слова предложения, подчеркивая и выделяя красным цветом это слово
    Sentence* sent = text->sents; // инициализируем переменную sent и записываем туда первое предложение текста
    wchar_t* prevWord = text->sents[text->len -1].words[text->sents[text->len -1].len-1].chars; // инициализируем переменную prevWord и записываем туда последнее слово последнего предложения в тексте
    findAndPrint(sent, prevWord); // вызываем функцию для проверки на последнее слово предложения
    for(prevWord = sent->words[sent->len-1].chars, sent = text->sents+1; sent < text->sents + text->len; prevWord = sent->words[sent->len-1].chars, sent++) // проходимся через каждое предложение, начиная со второго
        findAndPrint(sent, prevWord); // для каждого предложения проверяем на последнее слово
}
