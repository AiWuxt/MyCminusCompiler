/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include<iostream>
#include<string>


#define MAXRESERVED 6

typedef enum TokenType
{
	ENDFILE, ERROR,
	/* reserved words */
	IF, ELSE, INT, RETURN ,VOID ,WHILE,
	/* multi character tokens */
	ID, NUM,
	/* special symbols */
	PLUS, MINUS, TIMES, DIVIDE, LT, LE,
	GT, GE, EQ, UEQ, ASSIGN, SEMI, COMMA, 
	LPAREN, RPAREN, LBARCK, RBARCK, 
	LBRACE, RBRACE, LCOMME, RCOMME
};

extern FILE* source;	/* source code text file */
extern FILE* listing;	/* listing output text file */
extern FILE* code;		/* code text file for TM simulator */

extern int lineno;		/* source line number for listing */

/* Syntax tree for parsing */
typedef enum NodeKind {StmtK, ExpK};
typedef enum StmtKind
{
	IfK, RepeatK, AssignK, ReadK, WriteK
};
typedef enum ExpKind {OpK, ConstK, IdK};

/* ExpType is used for type checking */
typedef enum ExpType {Void, Integer, Boolean};

#define MAXCHILDREN 3

typedef struct treeNode
{
	struct treeNode* child[MAXCHILDREN];
	struct treeNode* sibling;
	int lineno;
	NodeKind nodekind;
	union 
	{
		StmtKind stmt; ExpKind exp;
	} kind;
	union 
	{
		TokenType op;
		int val;
		char * name;
	} attr;
	ExpType type;		// for type checking of exps

} TreeNode;

/* Flags for tracing */

// echo the listing file with line numbers during parsing
extern bool EchoSource;

// print the listing file as each token is recognized by the scanner
extern bool TraceScan;

// print the listing file in linearized form (using indents for children)
extern bool TraceParse;

// symbol table inserts and lookups to be reported to the listing file
extern bool TraceAnalyze;

// write the TM CODE FILE AS CODE IS GENERATED
extern bool TraceCode;

// prevents futher passes if an error occurs
extern bool Error;

#endif // !_GLOBALS_H_