#include "funcs.h"
#include "first_formating.h"

//func1
/*
Word* splitSentence(Sentence sentence, size_t* count) {
    int limit = SIZE_STEP;
    Word* words = malloc(sizeof(Word) * SIZE_STEP);
    wchar_t* buffer;
    wchar_t* word = wcstok(sentence.words, L",  \t", &buffer);

    while (word != NULL) {
        if (wcslen(word) > 0) {
            *count = *count + 1;
            if (*count >= limit - 1) {
                limit += SIZE_STEP;
                words = realloc(words, sizeof(Word) * limit);
            }

            // Инициализируем новое слово
            words[*count - 1].chars = (wchar_t*)malloc((wcslen(word) + 1) * sizeof(wchar_t));
            wcscpy(words[*count - 1].chars, word);
            words[*count - 1].sep = L' ';  // Вы можете установить значение символа-разделителя по вашему выбору
            words[*count - 1].len = wcslen(word);
            words[*count - 1].count_of_glasn_letters = 0;  // Инициализируем значение для гласных букв

            // Здесь вы можете выполнить дополнительные действия с каждым словом, если необходимо
        }
        word = wcstok(NULL, L",  \t", &buffer);
    }

    return words;
}
*/

int comparation(const void* a, const void* b) {
    const wchar_t* first = ((Word*)a)->chars;
    const wchar_t* second = ((Word*)b)->chars;

    if (wcslen(first) > wcslen(second))
        return 1;

    else if (wcslen(first) < wcslen(second))
        return -1;

    if (wcscmp(first,second) > 0)
        return 1;
    else if (wcscmp(first,second) < 0)
        return -1;
    return 0;
}
/*
wchar_t* findCrudeMask(wchar_t* w1, wchar_t* w2){
    wchar_t* mask = calloc(wcslen(w1)+1,sizeof(wchar_t));
    for (int i = 0; i< wcslen(w1); i++){
        if(w1[i] == w2[i]){
            mask[i] = w1[i];
        }else{
            mask[i] = L'?';
        }
    }
    return mask;
}

void findMask(Text text){
    for (int l = 0; l < text.len - 1; ++l) {
        Sentence sentence = text.sents[l];
        Word* words = sentence.words;
        wchar_t* mask = findCrudeMask(words[0].chars, words[1].chars);
        wprintf(L"%ls\n", mask);
    }
}
*/

void findMask(Text text) {
    for (int l = 0; l < text.len - 1; ++l) {
        Sentence sentence = text.sents[l];
        Word* words = calloc(sentence.count, sizeof(Word));
        memcpy(words,sentence.words, sentence.count * sizeof(Word));
        //qsort(words,wordCount,sizeof(wchar_t*),comparation);
        //wchar_t* printText = calloc(text.sents[l].len,sizeof(wchar_t)); 
        //wchar_t* printText = (wchar_t*)malloc((wcslen(text.sents[l].words[0].chars)+1)*sizeof(wchar_t)); 
        int wordCount = sentence.len;
        int charCount = 0;
        qsort(words,wordCount,sizeof(Word),comparation);
        //wchar_t** words = splitSentence(sentence,&wordCount);
        //words[wordCount-1][wcslen(words[wordCount-1])-1] = '\0';
        
        //wprintf(L"%d\n", wordCount);
        //return;

        for (int i = 0; i < wordCount; ++i)
            charCount += wcslen(words[i].chars);

        wchar_t* mask = calloc(charCount*3,sizeof(wchar_t));
        wchar_t stateChar;
        int inAllWords = 0;
        int inCurrentWord = 0;
        int prevIndex = 0;
        int maskLen = 0;
        int end = 0;
        int once = 0;
        int onceCheck = 1;
        int* indexArray = calloc(wordCount, sizeof(int));
        int* prevIndexArray = calloc(wordCount, sizeof(int));
        /*
        for (int j = 0; j < wordCount; j++){
            indexArray[j] = 0;
            prevIndexArray[j] = 0;
        }
        */

        for (int j = 0; j < wcslen(words[0].chars); ++j) {
            stateChar = words[0].chars[j];
            inAllWords = 1;
            int singleStep = 0;
            int manySteps = 0;
            for (int k = 1; k < wordCount; ++k) {
                inCurrentWord = 0;
                once = 1;
                for (int q = indexArray[k]; q < wcslen(words[k].chars); ++q) {
                    if (indexArray[k] != 0 && once) {
                        ++q;
                        once = 0;
                    }

                    if (stateChar == words[k].chars[q]) {
                        if (j == 0 && q != 0) {
                            wcscat(mask,L"?");
                            ++maskLen;
                        }

                        if (j != 0) {
                            int diff = q - indexArray[k];
                            if (diff == 1)
                                singleStep = 1;
                            if (diff >= 2)
                                manySteps = 1;
                            if (indexArray[k] != 0)
                                prevIndexArray[k] = indexArray[k];
                        }

                        indexArray[k] = q;
                        inCurrentWord = 1;
                        if (j == wcslen(words[0].chars)-1 && q != wcslen(words[k].chars)-1) {
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
                        wcscat(mask,L"?");
                        ++maskLen;
                        for (int k = 1; k < wordCount; ++k) {
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
            }
            else {
                for (int k = 1; k < wordCount; ++k) {
                    indexArray[k] = prevIndexArray[k];
                }                
                wcscat(mask,L"?");
                ++maskLen;
            }
        }

        if (end) {
            wcscat(mask,L"?");
            ++maskLen;
        }

        if (mask[0] == L'?')
            mask[0] = L'*';
        if (mask[maskLen-1] == L'?')
                    mask[maskLen-1] = L'*';
                mask[maskLen] = '\0';

                wchar_t* newMask = malloc(sizeof(wchar_t)*(maskLen+1));
                int newLen = 0;
                int questionCheck = 1;
                for (int i = 0; i < maskLen; ++i) {
                    if (mask[i] != L'?') {
                        newMask[newLen++] = mask[i];
                        questionCheck = 1;
                    }
                    else if (mask[i] == L'?' && questionCheck) {
                        newMask[newLen++] = mask[i];
                        questionCheck = 0;
                    }
                }
                if (mask[maskLen-1] == L'*')
                    newMask[newLen-1] = L'*';

                if (newLen >= 2 && newMask[newLen-2] == L'?' && newMask[newLen-1] == L'*') {
                    --newLen;
                    newMask[newLen-1] = L'*';
                }

                if (newLen == 2 && newMask[0] == L'*' && newMask[1] == L'*') {
                    --newLen;
                }
                newMask[newLen] = L'\0';

                if (newLen >= 2 && newMask[0] == L'*' && newMask[1] == L'?') {
                    wchar_t* veryNewMask = calloc(newLen+1,sizeof(wchar_t));
                    if (veryNewMask == NULL){
                        return;
                    }
                    int cnt = 0;
                    for (int r = 0; r < newLen; ++r) {
                        if (r != 1)
                            veryNewMask[cnt++] = newMask[r];
                    }
                    veryNewMask[newLen] = L'\0';
                    wprintf(L"%ls\n", veryNewMask);
                    free(veryNewMask);
                }
                else 
                    wprintf(L"%ls\n", newMask);
                //wprintf(L"%ls\n", printText);
                free(words);
                free(mask);
                free(newMask);
                free(indexArray);
                free(prevIndexArray);
            }
}



//func2
void removeNotUpperSentence(Text* text){
    for (size_t i = 0; i < text->len - 1; ++i) {
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
int compare_by_glasn_letters(const void* arg1, const void* arg2){
    Word word1 = *((Word*) arg1);
    Word word2 = *((Word*) arg2);
    if (word1.count_of_glasn_letters > word2.count_of_glasn_letters) return 1;
    if (word1.count_of_glasn_letters < word2.count_of_glasn_letters) return -1;
    return 0;}
void sort_by_glasn_letters(Text text){
    for (unsigned i = 0; i < text.len - 1; i++){
        wchar_t seps[text.sents[i].len];
        for (unsigned j = 0; j < text.sents[i].len; j++){
            text.sents[i].words[j].count_of_glasn_letters = 0;
            seps[j] = text.sents[i].words[j].sep;
            for (unsigned k = 0; k < text.sents[i].words[j].len; k++){
                wchar_t symbol = text.sents[i].words[j].chars[k];                                if (wcschr(L"aeiouyAEIOUYауоиэыяюеёАУОИЭЫЯЮЕЁ", symbol) != NULL)
                    text.sents[i].words[j].count_of_glasn_letters++;
            }
        }
        qsort(text.sents[i].words, text.sents[i].len, sizeof(Word), compare_by_glasn_letters);
        for (unsigned j = 0; j < text.sents[i].len; j++){
            text.sents[i].words[j].sep = seps[j];
        }
    }
}

//func4
void povtor(Text t) {
    for (int i = 0; i < t.len - 1; i++) {
        int count = 0;
        int* proces = (int*)calloc(t.sents[i].len, sizeof(int));

        for (int k = 0; k < t.sents[i].len - 1; k++) {
            if (proces[k] == 1) continue;
            int word_count = 1;
            //registrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
            for (int j = k + 1; j < t.sents[i].len; j++) {
                if (wcscmp(t.sents[i].words[k].chars, t.sents[i].words[j].chars) == 0) {
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