/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/
#include<iostream>
#include<fstream>
using namespace std;
#include "cminus.h"

int main()
{
	char line[ArSize] = {};
	ifstream fin;
	fin.open("test.cm");
	int count = 1;
	bool isCommentLine = false;
	while (fin.getline(line, ArSize))
	{
		cout << "--------------------------" << endl;
		cout << "The " << count << " line: " << endl;
		cout << line << endl;
		cout << "--------------------------" << endl;
		Scanner(line, isCommentLine);
		count++;
	}
	int a;
	cin >> a;
	return 0;
}


