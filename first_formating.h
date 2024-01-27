#include "libraries.h"
#include "structs.h"

int findCopies(Word *word1, Word *word2);
int areSentencesEqual(const Sentence* s1, const Sentence* s2);
void removeDuplicateSentences(Text* text) ;
void freeText(Text* text);
void freeSentence(Sentence sent);
void removeSentence (int i, Text* text);
void removeNewlineAtSentenceStart(Text* text);