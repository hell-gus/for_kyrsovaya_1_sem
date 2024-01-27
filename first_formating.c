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
void format(Text *text) {
    for (size_t i = 0; i < text->len - 1; i++) {
        for (size_t j = i + 1; j < text->len; j++) {
            bool match_flag = true;

            for (size_t i1 = 0; i1 < text->len - 1; i1++) {
                    if (wcscmp(text->sents[i].words[i1].chars, text->sents[j].words[i1].chars) != 0){
                        match_flag = false;
                        break;
                    }
            }
            if (match_flag==true){
                removeSentence(j, text);
                j--;
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
