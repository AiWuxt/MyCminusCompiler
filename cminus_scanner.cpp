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
		{
			while (!(line[i] == '/' && line[i - 1] == '*') && (line[i] != '\0'))
			{
				i++;
			}
			if (line[i] == '/' && line[i - 1] == '*')
			{
				i--;
				isCommentLine = false;
			}
		}
		if (line[i] >= 'a'&&line[i] <= 'z' || line[i] >= 'A'&&line[i] <= 'Z')
		{
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] >= 'a'&&line[i] <= 'z' || line[i] >= 'A'&&line[i] <= 'Z');
			bool iskey = false;
			for (string keyword : keywords)
			{
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
		{
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] >= '0'&&line[i] <= '9');
			cout << "NUM, val= " << tokens[count] << endl;
			i--; count++;
		}
		else if (line[i] == ' ' || line[i] == '\t' || line[i] == '\0')
		{
			do
			{
				i++;
			} while (line[i] == ' ' || line[i] == '\t' || line[i] == '\0');
			i--;
		}
		else if (line[i] == '*')
		{
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] == '/');
			cout << "special symbol: " << tokens[count] << endl;
			i--; count++;
		}
		else if (line[i] == '/')
		{
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] == '*');
			cout << "special symbol: " << tokens[count] << endl;
			if (tokens[count] == "/*")
			{
				while (!(line[i] == '/' && line[i - 1] == '*') && (line[i] != '\0'))
				{
					i++;
				}
				if (line[i] == '/' && line[i - 1] == '*') i--;
				else
				{
					isCommentLine = true;
				}
			}
			i--; count++;
		}
		else if (line[i] == '<' || line[i] == '>'
			|| line[i] == '=' || line[i] == '!')
		{
			do
			{
				tokens[count] += line[i];
				i++;
			} while (line[i] == '=');
			cout << "special symbol: " << tokens[count] << endl;
			i--; count++;
		}
		else
		{
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