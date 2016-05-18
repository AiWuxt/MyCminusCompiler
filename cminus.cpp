/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/
#include<iostream>
#include<fstream>
using namespace std;
#include "cminus.h"

int main(int argc, char * argv[])
{
	TreeNode * syntaxTree;
	char pgm[20];
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	strcpy(pgm, argv[1]);
	if (strchr(pgm, '.') == NULL)
		strcat(pgm, ".cm");
	source = fopen(pgm, "r");
	if (source == NULL)
	{
		fprintf(stderr, "File %s not found\n", pgm);
		exit(1);
	}
	listing = stdout;
	fprintf(listing, "\nCMINUS COMPILATION: %s\n", pgm);
#if NO_PARSE
	while (getToken() != ENDFILE);
#endif
	int end;
	cin >> end;
	return 0;
}


