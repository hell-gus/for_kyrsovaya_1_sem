#include "input_and_output.h"
#include "first_formating.h"
unsigned newlineCounter = 0;
Word readWord(){
    int size = MEMORY_STEP;
    int len = 0;//длина слова
    wchar_t *chars = malloc(size * sizeof(wchar_t));// массив символов без разделителей
    wchar_t sep, symbol = L' ';// символ разделитель 

    while(symbol == L' ')
        symbol = getwchar();

    while((!wcschr(WORD_SEP, symbol)) && (!wcschr(SENTENCE_SEP, symbol))){

        if (symbol==L'\n') newlineCounter++;
        else newlineCounter=0;
        if (newlineCounter==2) break;

        chars[len++] = symbol;

        if(len == size){
            size += MEMORY_STEP;
            chars = (wchar_t*)realloc(chars, size * sizeof(wchar_t));
        }
        symbol = getwchar();
    }

    chars[len] = L'\0';
    sep = symbol;

    return (Word){chars, sep, len};
}

Sentence readSentence(){
    int size = MEMORY_STEP;
    int len = 0; // длина предложения
    int count=0;
    Word* words = (Word*)malloc(size*sizeof(Word)); // массив слов
    Word word; //  переменная слово

    do {
        word = readWord();
        memcpy(&words[len++], &word, sizeof(Word));//копирование слова в массив слов
        count++;
        if (len == size) {
            size += MEMORY_STEP;
            words = realloc(words, size * sizeof(Word));
        }

        if (newlineCounter==2){
            break;
        }

    } while (!wcschr(SENTENCE_SEP, word.sep));


    return (Sentence){words, len, count};
}

Text readText(){
    Text text;
    text.len=0;
    text.sents=NULL;
    Sentence new_sentence = readSentence();
    while (true) {
        bool main_flag = true;
        for (unsigned i = 0; i<text.len; i++){
            bool second_flag = true;
            Sentence old_sentence = text.sents[i];
            if (new_sentence.len == old_sentence.len){
                for (unsigned j=0; j<old_sentence.len; j++){
                    if ((wcscmp(new_sentence.words[j].chars, old_sentence.words[j].chars) !=0 ||
                    (new_sentence.words[j].sep!=old_sentence.words[j].sep))){
                        second_flag=false;
                        break;
                    }
                }
            if (second_flag){
                main_flag=false;
                break;
            }
            }
        }
    if (main_flag){
        text.sents=(Sentence*)realloc(text.sents, sizeof (Sentence)*(text.len+1));
        text.sents[text.len++]= new_sentence;

        if ((new_sentence.words[new_sentence.len-1].sep ==L'\n') ||
                (new_sentence.words[new_sentence.len-1].sep==WEOF)) {
            break;
        }
    }
    new_sentence=readSentence();
    }
    return text;
}


void printText(Text text){
    //removeNewlineAtSentenceStart(&text);
    for (int i = 0; i < text.len; i++){
        for (int j = 0; j < text.sents[i].len; j++){
            wprintf(L"%ls%lc", text.sents[i].words[j].chars, text.sents[i].words[j].sep);
            if (text.sents[i].words[j].sep != L' ') {
                wprintf(L" ");
            }
        }
        wprintf(L"\n");
    }

}