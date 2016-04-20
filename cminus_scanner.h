#pragma once
/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/
 
#ifndef CMINUS_KEYS_H_
#define CMINUS_KEYS_H_

#include<string>
const int ArSize = 80;
void Scanner(const char * line, bool &isCommentLine);
const string keywords[6] = { "else", "if", "int", "return", "void", "while" };

#endif // !CMINUS_KEYS_H_