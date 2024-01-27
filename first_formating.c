#include "first_formating.h"
#include "input_and_output.h"

int findCopies(Word *word1, Word *word2) {
    int flag = 1;
    if (word1->len == word2->len) {
        for (size_t a = 0; a < word1->len; a++) {
            if (towlower(word1->chars[a]) != towlower(word2->chars[a])) {
                flag = 0;
                break;
            }
        }
    } else {
        flag = 0;
    }
    if (flag) {
        return 0;
    }
    return 1;
}

int areSentencesEqual(const Sentence* s1, const Sentence* s2) {
    if (s1->len != s2->len) {
        return 0;  // Разные длины предложений, они не могут быть равными
    }

    for (size_t i = 0; i < s1->len; ++i) {
        if (findCopies(&(s1->words[i]), &(s2->words[i])) != 0) {
            return 0;  // Найдено несовпадение слов, предложения не равны
        }
    }

    return 1;  // Предложения идентичны
}

void removeDuplicateSentences(Text* text) {
    // Поиск и удаление повторяющихся предложений в тексте
    //removeNewlineAtSentenceStart(&text);
    for (size_t i = 0; i < text->len; ++i) {
        for (size_t j = i + 1; j < text->len; ++j) {
            if (areSentencesEqual(&(text->sents[i]), &(text->sents[j]))) {
                // Найдено повторяющееся предложение
                removeSentence(j, text);
                j--; // Уменьшаем j, чтобы проверить следующее предложение на совпадение
            }
        }
    }
}

void freeText(Text* text){
    for (size_t i = 0; i < text->len; i++)
        freeSentence(text->sents[i]);
    free(text->sents);
    text->len = 0;
}

void freeSentence(Sentence sent){ // функция освобождения памяти для одного предложения
    for(Word* word = sent.words; word < sent.words + sent.len; word++) // проходимся через каждое слово в предложении
        free(word->chars); // освобождение памяти для одного слова

    free(sent.words); // освобождение памяти, выделенной под массив слов в этом предложении
}

void removeSentence (int i, Text* text){
    freeSentence(text->sents[i]);
    memmove(text->sents+i, text->sents+i+1,(text->len-i-1)*sizeof(Sentence));
    text->len--;
}

void removeNewlineAtSentenceStart(Text* text) {
    for (size_t i = 0; i < text->len; ++i) {
        if (text->sents[i].len > 0 && text->sents[i].words[0].chars[0] == L'\n') {
            // Если первый символ первого слова в предложении - символ новой строки,
            // то удаляем его
            for (size_t j = 0; j < text->sents[i].words[0].len; ++j) {
                text->sents[i].words[0].chars[j] = text->sents[i].words[0].chars[j + 1];
            }
            text->sents[i].words[0].len--; // Уменьшаем длину слова
        }
    }
}
