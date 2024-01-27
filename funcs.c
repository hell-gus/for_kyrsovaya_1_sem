#include "funcs.h"
#include "first_formating.h"

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
        wchar_t* printText = (wchar_t*)malloc((wcslen(sentence.words[0].chars)+1)*sizeof(wchar_t));
        wcscpy(printText, sentence.words[0].chars);
        //GPTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT|
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
        free(printText);
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
+1

//func3
int compare_by_glasn_letters(const void* arg1, const void* arg2){
    Word word1 = *((Word*) arg1);
    Word word2 = *((Word*) arg2);
    if (word1.count_of_glasn_letters > word2.count_of_glasn_letters) return 1;
    if (word1.count_of_glasn_letters < word2.count_of_glasn_letters) return -1;
    return 0;}
void sort_by_glasn_letters(Text text){
    for (unsigned i = 0; i < text.len; i++){
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
    for (int i = 0; i < t.len; i++) {
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
