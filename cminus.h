/*************************************************************************
> Author: aiwuxt
> Mail: aiwuxt@gmail.com
> Description:
************************************************************************/

#ifndef CMINUS_H_
#define CMINUS_H_


#include"globals.h"
#include"util.h"

#pragma warning(disable:4996)

/* set NO_PARSE to true to get a scanner-only compiler */
#define NO_PARSE true

/* set NO_ANALYZE to true to get a parser-only compiler */
#define NO_ANALYZE false

/* set NO_CODE to true to get a compiler that does not generate code*/
#define NO_CODE false

#if NO_PARSE
#include"scan.h"
#else
#include"parse.h"
#if !NO_ANALYZE
#include"analyze.h"
#if !NO_CODE
#include"cgen.h"
#endif
#endif
#endif

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
bool EchoSource = true;
bool TraceScan = true;
bool TraceParse = true;
bool TraceAnalyze = true;
bool TraceCode = true;

bool Error = false;

#endif // !CMINUS_H_

