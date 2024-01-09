#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

char* pastevents(char* input,Directories* directory);
// char* parsedfully(char* input);
int checkforpasteventsexecute(char* tokenized_input);
int checkforpastevents(char* tokenized_input);
int parsedfully(char* input);
void writeinfile(char* input,Directories* directory);

#endif