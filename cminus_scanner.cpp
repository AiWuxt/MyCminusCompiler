/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/
 
#include<iostream>
using namespace std;
#include<string>
#include"cminus_scanner.h"

void Scanner(const char *line, bool &isCommentLine)
{
	string *tokens = new string[ArSize];
	int count = 0;
	for (int i = 0; i < strlen(line); i++)
	{	
		if (isCommentLine)
		{	// previous line is comment line and not end with */,
			// so this line skip until get the */
			while (!(line[i] == '/' && line[i - 1] == '*') && (line[i] != '\0'))
			{
				i++;
			}
			if (line[i] == '/' && line[i - 1] == '*')
			{	// get the '*/'
				i--;
				isCommentLine = false;
			}
		}
		if (line[i] >= 'a'&&line[i] <= 'z' || line[i] >= 'A'&&line[i] <= 'Z')
		{	// get ID or reserved words
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] >= 'a'&&line[i] <= 'z' || line[i] >= 'A'&&line[i] <= 'Z');
			bool iskey = false;
			for (string keyword : keywords)
			{	// filter reserved words
				if (tokens[count] == keyword)
				{
					iskey = true;
					break;
				}
			}
			if (iskey)
				cout << "reserved word: " << tokens[count] << endl;
			else
				cout << "ID, name= " << tokens[count] << endl;

			i--; count++;
		}
		else if (line[i] >= '0'&&line[i] <= '9')
		{	// get num 
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] >= '0'&&line[i] <= '9');
			cout << "NUM, val= " << tokens[count] << endl;
			i--; count++;
		}
		else if (line[i] == ' ' || line[i] == '\t' || line[i] == '\0')
		{	// space or tab or end skip
			do
			{
				i++;
			} while (line[i] == ' ' || line[i] == '\t' || line[i] == '\0');
			i--;
		}
		else if (line[i] == '*')
		{	// get * or */
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] == '/');
			cout << "special symbol: " << tokens[count] << endl;
			i--; count++;
		}
		else if (line[i] == '/')
		{	// get / or /*
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] == '*');
			cout << "special symbol: " << tokens[count] << endl;
			if (tokens[count] == "/*")
			{	// if token is '/*', then skip until '*/' or '\0'
				while (!(line[i] == '/' && line[i - 1] == '*') && (line[i] != '\0'))
				{
					i++;
				}
				if (line[i] == '/' && line[i - 1] == '*') i--;
				else
				{	// this line end with '\0'
					isCommentLine = true;
				}
			}
			i--; count++;
		}
		else if (line[i] == '<' || line[i] == '>'
			|| line[i] == '=' || line[i] == '!')
		{	// get special symbols with '=' or without '='
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] == '=');
			cout << "special symbol: " << tokens[count] << endl;
			i--; count++;
		}
		else
		{	// get only single special symbol or undefined symbol
			tokens[count] = line[i];
			cout << "special symbol: " << tokens[count] << endl;
			count++;
		}
		/*
		if (line[i] == '+' || line[i] == '-' || line[i] == ';'
		|| line[i] == ',' || line[i] == '(' || line[i] == ')'
		|| line[i] == '[' || line[i] == ']' || line[i] == '{'
		|| line[i] == '}')
		{
		tokens[count] = line[i];
		}
		*/
	}
	delete[] tokens;
}