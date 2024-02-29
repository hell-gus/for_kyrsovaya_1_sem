#include "extra_funcs.h"

//void ignoreSymb(Sentence* sent){
//    for(unsigned i=0;i<)
//};

int isPalindrome(wchar_t* str, size_t len){
    wchar_t sepsi[]=L"\"'+/\\~[]&`|{}=_<>-();:#$^%@*!??";
    wchar_t *newstr=malloc(sizeof(wchar_t)*(len+1));
    int n=0;
    
    for (unsigned i=0;i<wcslen(str);i++){
        if (wcschr(sepsi, str[i])==NULL){
            newstr[n]=str[i];
            
            n++;
            
        }
    }
    newstr[n]=L'\0';
    for(unsigned i=0;i<wcslen(newstr); i++){
        if(towlower(newstr[i])!=towlower(newstr[wcslen(newstr)-1-i])){
            return 0;
        }
    }

    return 1;
}

void findPalindrome(Text text){
    for(unsigned i=0; i<text.len - 1;i++){
        size_t len = 0;
        for(size_t j = 0; j < text.sents[i].len; j++){
            len += text.sents[i].words[j].len;
        }
        wchar_t* str = calloc((len+1), sizeof(wchar_t));
                
        
        for(size_t j = 0; j < text.sents[i].len; j++){
            wcscat(str, text.sents[i].words[j].chars);
            
        }
        
        if(isPalindrome(str, len)==1){
            for (int j = 0; j < text.sents[i].len; j++){
                    wprintf(L"%ls%lc", text.sents[i].words[j].chars, text.sents[i].words[j].sep);
                }
                wprintf(L"\n");
            }
        str=NULL;
        free(str);
        
    }


}