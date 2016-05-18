/*************************************************************************
> Author: aiwuxt
> Mail: aiwuxt@gmail.com
> Description:
************************************************************************/

#ifndef CMINUS_H_
#define CMINUS_H_


#include"globals.h"
#include"util.h"
#include"scan.h"
#include"parse.h"
#include"symtab.h"
#include"analyze.h"
#include"code.h"
#include"cgen.h"

#pragma warning(disable:4996)

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE true

int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

bool EchoSource = true;
bool TraceScan = true;
bool TraceParse = true;
bool TraceAnalyze = true;
bool TraceCode = true;

bool Error = false;

#endif // !CMINUS_H_

