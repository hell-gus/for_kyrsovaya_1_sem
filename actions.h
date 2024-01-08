#pragma once // Директива препроцессора, которая гарантирует, что заголовочный файл будет включен только один раз

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<wchar.h>
#include<time.h>
#include<wctype.h>
#include<stdbool.h>

#define MEMORY_STEP 5 // Определение константы для шага выделения памяти
#define SENTENCE_SEP L".\n" // Определение константы для разделителя предложений
#define WORD_SEP L" ," // Определение константы для разделителей слов

#ifndef TEXT_STRUCT // Если не определена TEXT_STRUCT
#define TEXT_STRUCT // Определение TEXT_STRUCT

typedef struct Word{ // Определение структуры слова
    wchar_t * chars; // массив символов
    wchar_t sep; // символ-разделитель
    size_t len; // длина слова
    size_t count;  //кол-во вхождений этого слова
    unsigned count_of_latin_letters;
} Word;
typedef struct Sentence{ // Определение структуры предложения
    Word* words; // массив слов в предложении
    size_t len; // длина предложения
    size_t count;
} Sentence;
typedef struct Text{ // Определение структуры текста
    Sentence* sents; // массив предложений в тексте
    size_t len; // длина текста НИХУЯ. ЭТО КОЛИЧЕСТВО
} Text;
#endif

void removeEmptySentences(Text* text); // Объявление функции удаления пустых предложений
void removeCopies(Text* text); // Объявление функции удаления копий
void removeSentence(int i, Text* text); // Объявление функции удаления предложения
void freeSentence(Sentence sent); // Объявление функции освобождения памяти предложения
void freeText(Text* text); // Объявление функции освобождения памяти текста
void changeOneWord(Word* word, wchar_t* toWord);
int cmp(const void *a, const void *b);
wchar_t* input_line();
int comparation(const void* a, const void* b);
void findMask(Text* text);
void removeNotUpperSentence(Text* text);//func2
int compare_by_latin_letters(const void* arg1, const void* arg2);
void sort_by_latin_letters(Text text);
void povtor(struct Text t);