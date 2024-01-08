
int comparation(const void* a, const void* b) {
    const Word* first = (const Word*)a;
    const Word* second = (const Word*)b;

    if (first->len > second->len)
        return 1;
    else if (first->len < second->len)
        return -1;

    return wcscmp(first->chars, second->chars);
}

void findMask(Text* text) {
    for (size_t l = 0; l < text->len; ++l) {
        Sentence sentence = text->sents[l];
        wchar_t* print_text = calloc(sentence.len, sizeof(wchar_t));
        wcscpy(print_text, sentence.words[0].chars);

        qsort(sentence.words, sentence.len, sizeof(Word), comparation);

        size_t charCount = 0;
        for (size_t i = 0; i < sentence.len; ++i)
            charCount += sentence.words[i].len;

        wchar_t* mask = calloc(charCount * 3, sizeof(wchar_t));
        wchar_t stateChar;
        int inAllWords;
        int inCurrentWord;
        size_t prevIndex;
        size_t maskLen = 0;
        int end = 0;
        int once;
        int onceCheck = 1;
        size_t* indexArray = calloc(sentence.len, sizeof(size_t));
        size_t* prevIndexArray = calloc(sentence.len, sizeof(size_t));

        for (size_t j = 0; j < sentence.words[0].len; ++j) {
            stateChar = sentence.words[0].chars[j];
            inAllWords = 1;
            int singleStep = 0;
            int manySteps = 0;
            for (size_t k = 1; k < sentence.len; ++k) {
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
                        for (size_t k = 1; k < sentence.len; ++k) {
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
                for (size_t k = 1; k < sentence.len; ++k) {
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
        mask[maskLen] = '\0';

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
        wprintf(L"%ls\n", print_text);

        free(mask);
        free(newMask);
        free(indexArray);
        free(prevIndexArray);
        free(print_text);
    }
    freeText(text);
}

