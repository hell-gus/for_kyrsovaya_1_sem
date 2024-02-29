#include "libraries.h"

#ifndef TEXT_STRUCT // Если не определена TEXT_STRUCT
#define TEXT_STRUCT // Определение TEXT_STRUCT

typedef struct Word{ // Определение структуры слова
    wchar_t * chars; // массив символов
    wchar_t sep; // символ-разделитель
    size_t len; // длина слова
    //size_t count;  //кол-во вхождений этого слова
    unsigned count_of_glasn_letters;
} Word;
typedef struct Sentence{ // Определение структуры предложения
    Word* words; // массив слов в предложении
    size_t len; // длина предложения
    size_t count;
} Sentence;
typedef struct Text{ // Определение структуры текста
    Sentence* sents; // массив предложений в тексте
    size_t len; // длина текста НИT. ЭТО КОЛИЧЕСТВО symbols
} Text;
#endif
