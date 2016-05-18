/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/
#ifndef _SCAN_H_
#define _SCAN_H_

/* the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array store the lexeme of each token */
extern char tokenString[MAXTOKENLEN + 1];

/* returns the next token in source file*/
TokenType getToken(void);

#endif // !_SCAN_H_