#include "actions.h"

void removeSentence (int i, Text* text){ // функция удаления 1-го предложения из текста и сдвига всех последующих предложений на одну позицию назад
    freeSentence(text->sents[i]); // освобождение памяти і-го предложения
    memmove(text->sents+i, text->sents+i+1,(text->len-i-1)*sizeof(Sentence)); //сдвиг всех предложений после удаленного предложения на одну позицию назад
    text->len--; //уменьшение длины текста
}

void removeEmptySentences(Text* text){ // Функция удаления пустых предложений
    int i = 0; // Инициализация счетчика
    for(Sentence* sent = text->sents; sent < text->sents + text->len; sent++, i++) { // Цикл по предложениям текста
        if(!sent->words[0].len){ // Если длина первого слова равна нулю
            removeSentence(i, text); // Удаление предложения
            i--; // Уменьшение счетчика
            sent--; // Уменьшение указателя на предложение
        }
    }
}

void removeCopies(Text* text){ // Функция удаления копий
    int c1, c2; // Инициализация переменных для сравнения
    Word *w1, *w2; // Инициализация указателей на слова

    for(int r = text->len-1; r > 0;r--) // Цикл по предложениям текста с конца
        for(int l = r-1; l >= 0; l--){ // Цикл по предложениям текста до текущего предложения
            if(text->sents[l].len != text->sents[r].len) // Если длины предложений не равны
                continue;
            w1 = text->sents[l].words; // Установка указателя на слова первого предложения
            w2 = text->sents[r].words; // Установка указателя на слова второго преложения
            c1 = 0; // Обнуление первой переменной для сравнения
            c2 = 0; // Обнуление второй переменной для сравнения
            for(int i = 0; i < text->sents[l].len; i++){ // Цикл по словам предложения
                c1 = wcscasecmp(w1[i].chars, w2[i].chars); // Сравнение слова без учета регистра
                c2 = w1[i].sep != w2[i].sep; // Сравнение слов без учета регистра
                if(c1 || c2) // Если слова или разделители не равны
                    break;
            }
            if(!c1 && !c2){ // Если слова и разделители равны
                removeSentence(r, text); // Удаление предложения
                break;
            }
        }
}

void freeSentence(Sentence sent){ // функция освобождения памяти для одного предложения
    for(Word* word = sent.words; word < sent.words + sent.len; word++) // проходимся через каждое слово в предложении
        free(word->chars); // освобождение памяти для одного слова

    free(sent.words); // освобождение памяти, выделенной под массив слов в этом предложении
}

void freeText(Text* text){ // функция освобождения памяти, выделенной для текста, то есть для выделенной для всех предложений в тексте.
    for(int i = 0;i < text->len;i++) // проходимся через каждое предложение в тексте
        freeSentence(text->sents[i]); // освобождение памяти для i-го предложения
    free(text->sents); // освобождения памяти, выделенной под массив предложений в тексте
    text->len = 0; // устанавливаем длину текста равной нулю, так как предложения удалены
}

void changeOneWord(Word* word, wchar_t* toWord) { // функция замены символов в слове на новые символы с перевыделением памяти
    free(word->chars); // освобождаем память, выделенную под символы слова
    word->len = wcslen(toWord); //устанавливаем длину слвоа равной длине нового слова с помощью функции wcslen
    wchar_t* newChars = malloc((wcslen(toWord) + 1)* sizeof(wchar_t)); //выделяем новую память под символы нового слова. размер выделенной памяти равен длине нового слова + 1 (для \0) умноженной на размер широкого символа
    wcscpy(newChars, toWord); //копируем символы из towords в newchars с помощью функции wcspy
    word->chars = newChars; //записываем новые символы в слово
}

int cmp(const void *a, const void *b){ //функция сравнения двух предложений по их длине для последующей сортировки массива предложений в порядке убывания длины
    Sentence *s1 = (Sentence*)a; // приводим указатели a и b к типу Sentence*
    Sentence *s2 = (Sentence*)b;
    return s2->len - s1->len; // возвращаем разницу
}

wchar_t* input_line(){
    int size = MEMORY_STEP;
    int len = 0;
    wchar_t * chars = malloc(size*sizeof(wchar_t));
    wchar_t symbol = L' ';
    wprintf(L"Введите строку:\n");
    while(symbol == L' ')
        symbol = getwchar();

    while(!wcschr(L"\n", symbol)){
        chars[len++] = symbol;
        if(len == size){
            size += MEMORY_STEP;
            chars = realloc(chars, size*sizeof(wchar_t));
        }
        symbol = getwchar();
    }

    chars[len] = L'\0';
    return chars;
}


//func1
int comparation(const void* a, const void* b) {
    const Sentence* first = (const Sentence*)a;
    const Sentence* second = (const Sentence*)b;

    if (first->len > second->len)
        return 1;
    else if (first->len < second->len)
        return -1;

    return wcscmp(first->words[0].chars, second->words[0].chars);
}

void findMask(Text* text) {
    for (size_t l = 0; l < text->len; ++l) {
        Sentence sentence = text->sents[l];
        wchar_t* printText = calloc(sentence.len, sizeof(wchar_t));
        printText = wcsdup(sentence.words[0].chars);
        size_t wordCount = sentence.count;
        size_t charCount = 0;

        qsort(sentence.words, wordCount, sizeof(Word), comparation);

        for (size_t i = 0; i < wordCount; ++i)
            charCount += sentence.words[i].len;

        wchar_t* mask = calloc(charCount * 3, sizeof(wchar_t));
        wchar_t stateChar;
        int inAllWords;
        size_t inCurrentWord;
        size_t prevIndex;
        size_t maskLen = 0;
        int end = 0;
        int once;
        int onceCheck = 1;
        size_t* indexArray = calloc(wordCount, sizeof(size_t));
        size_t* prevIndexArray = calloc(wordCount, sizeof(size_t));

        for (size_t j = 0; j < sentence.words[0].len; ++j) {
            stateChar = sentence.words[0].chars[j];
            inAllWords = 1;
            int singleStep = 0;
            int manySteps = 0;
            for (size_t k = 1; k < wordCount; ++k) {
                inCurrentWord = 0;
                once = 1;
                for (size_t q = indexArray[k]; q < sentence.words[k].len; ++q) {
                    if (indexArray[k] != 0 && once) {
                        ++q;
                        once = 0;
                    }

                    if (stateChar == sentence.words[k].chars[q]) {
                        if (j == 0 && q != 0) {
                            wcscat(mask, L"?");
                            ++maskLen;
                        }

                        if (j != 0) {
                            size_t diff = q - indexArray[k];
                            if (diff == 1)
                                singleStep = 1;
                            if (diff >= 2)
                                manySteps = 1;
                            if (indexArray[k] != 0)
                                prevIndexArray[k] = indexArray[k];
                        }

                        indexArray[k] = q;
                        inCurrentWord = 1;
                        if (j == sentence.words[0].len - 1 && q != sentence.words[k].len - 1) {
                            end = 1;
                        }
                        break;
                    }
                }
                if (!inCurrentWord)
                    inAllWords = 0;
            }

            if (inAllWords) {
                if (j != 0) {
                    if (j - prevIndex == 1)
                        singleStep = 1;
                    if (j - prevIndex >= 2)
                        manySteps = 1;
                    if (singleStep && manySteps) {
                        wcscat(mask, L"?");
                        ++maskLen;
                        for (size_t k = 1; k < wordCount; ++k) {
                            if (onceCheck)
                                continue;
                            indexArray[k] = prevIndexArray[k];
                        }
                        onceCheck = 0;
                        continue;
                    }
                }
                prevIndex = j;
                mask[maskLen] = stateChar;
                ++maskLen;
            } else {
                for (size_t k = 1; k < wordCount; ++k) {
                    indexArray[k] = prevIndexArray[k];
                }
                wcscat(mask, L"?");
                ++maskLen;
            }
        }

        if (end) {
            wcscat(mask, L"?");
            ++maskLen;
        }

        if (mask[0] == L'?')
            mask[0] = L'*';

        if (mask[maskLen - 1] == L'?')
            mask[maskLen - 1] = L'*';
        mask[maskLen] = L'\0';

        wchar_t* newMask = malloc(sizeof(wchar_t) * (maskLen + 1));
        size_t newLen = 0;
        int questionCheck = 1;
        for (size_t i = 0; i < maskLen; ++i) {
            if (mask[i] != L'?') {
                newMask[newLen++] = mask[i];
                questionCheck = 1;
            } else if (mask[i] == L'?' && questionCheck) {
                newMask[newLen++] = mask[i];
                questionCheck = 0;
            }
        }
        if (mask[maskLen - 1] == L'*')
            newMask[newLen - 1] = L'*';

        if (newLen >= 2 && newMask[newLen - 2] == L'?' && newMask[newLen - 1] == L'*') {
            --newLen;
            newMask[newLen - 1] = L'*';
        }

        if (newLen == 2 && newMask[0] == L'*' && newMask[1] == L'*') {
            --newLen;
        }
        newMask[newLen] = L'\0';

        if (newLen >= 2 && newMask[0] == L'*' && newMask[1] == L'?') {
            wchar_t* veryNewMask = calloc(newLen + 1, sizeof(wchar_t));
            size_t cnt = 0;
            for (size_t r = 0; r < newLen; ++r) {
                if (r != 1)
                    veryNewMask[cnt++] = newMask[r];
            }
            veryNewMask[newLen] = L'\0';
            wprintf(L"%ls\n", veryNewMask);
        } else
            wprintf(L"%ls\n", newMask);
        //wprintf(L"%ls\n", printText);

        free(mask);
        free(newMask);
        free(indexArray);
        free(prevIndexArray);
    }
    //freeText(text); // Assuming you have a function to free the allocated memory
}


//func2
void removeNotUpperSentence(Text* text){
    for (size_t i = 0; i < text->len; ++i) {
        Sentence* sentence = &text->sents[i];
        bool flag = false;

        for (size_t j = 0; j < sentence->len; ++j) {
            Word* word = &sentence->words[j];

            if (!(word->len > 0 && iswupper(word->chars[0]))) {
                flag = true;
                break;
            }
        }
        if (flag) {
            removeSentence(i, text);
        }
    
    }
    
}


//func3
int compare_by_latin_letters(const void* arg1, const void* arg2){    
    Word word1 = *((Word*) arg1);    
    Word word2 = *((Word*) arg2);    
    if (word1.count_of_latin_letters > word2.count_of_latin_letters) return 1;
    if (word1.count_of_latin_letters < word2.count_of_latin_letters) return -1;    
    return 0;}
void sort_by_latin_letters(Text text){    
    for (unsigned i = 0; i < text.len; i++){        
        wchar_t seps[text.sents[i].len];
        for (unsigned j = 0; j < text.sents[i].len; j++){            
            text.sents[i].words[j].count_of_latin_letters = 0;            
            seps[j] = text.sents[i].words[j].sep;            
            for (unsigned k = 0; k < text.sents[i].words[j].len; k++){
                wchar_t symbol = text.sents[i].words[j].chars[k];                                if (wcschr(L"aeiouyAEIOUYауоиэыяюеёАУОИЭЫЯЮЕЁ", symbol) != NULL)
                    text.sents[i].words[j].count_of_latin_letters++;            
                }        
            }        
        qsort(text.sents[i].words, text.sents[i].len, sizeof(Word), compare_by_latin_letters);
        for (unsigned j = 0; j < text.sents[i].len; j++){            
            text.sents[i].words[j].sep = seps[j];        
            }    
        }
}

//func4
void povtor(Text t) {
    for (int i = 0; i < t.len; i++) {
        int count = 0;
        int* proces = (int*)calloc(t.sents[i].len, sizeof(int));

        for (int k = 0; k < t.sents[i].len - 1; k++) {
            if (proces[k] == 1) continue;
            int word_count = 1;
            for (int j = k + 1; j < t.sents[i].len; j++) {
                if (wcscasecmp(t.sents[i].words[k].chars, t.sents[i].words[j].chars) == 0) {
                    proces[j] = 1;
                    word_count++;
                }
            }
            if (word_count > 1) {
                count += word_count;
            }
        }
        wprintf(L"Количество одинаковых слов: %d\n", count);
        free(proces);
    }
}


