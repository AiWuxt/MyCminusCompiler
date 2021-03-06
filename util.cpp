/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/

#include"globals.h"
#include"util.h"

/* Procedure printToken prints a token
	and its lexeme to the listing file */
void printToken(TokenType token, const char * tokenString)
{
	switch (token)
	{
	case ENDFILE:
		fprintf(listing, "EOF\n");
		break;
	case ERROR:
		fprintf(listing, "LEXICAL ERROR: %s\n", tokenString);
		break;
	case IF:
	case ELSE:
	case INT:
	case RETURN:
	case VOID:
	case WHILE:
		fprintf(listing, "reserved word: %s\n", tokenString);
		break;
	case ID:
		fprintf(listing, "ID, name= %s\n", tokenString);
		break;
	case NUM:
		fprintf(listing, "NUM, val= %s\n", tokenString);
		break;
	case PLUS:
		fprintf(listing, "+\n");
		break;
	case MINUS:
		fprintf(listing, "-\n");
		break;
	case TIMES:
		fprintf(listing, "*\n");
		break;
	case DIVIDE:
		fprintf(listing, "/\n");
		break;
	case LT:
		fprintf(listing, "<\n");
		break;
	case LE:
		fprintf(listing, "<=\n");
		break;
	case GT:
		fprintf(listing, ">\n");
		break;
	case GE:
		fprintf(listing, ">=\n");
		break;
	case EQ:
		fprintf(listing, "==\n");
		break;
	case UEQ:
		fprintf(listing, "!=\n");
		break;
	case ASSIGN:
		fprintf(listing, "=\n");
		break;
	case SEMI:
		fprintf(listing, ";\n");
		break;
	case COMMA:
		fprintf(listing, ",\n");
		break;
	case LPAREN:
		fprintf(listing, "(\n");
		break;
	case RPAREN:
		fprintf(listing, ")\n");
		break;
	case LBARCK:
		fprintf(listing, "[\n");
		break;
	case RBARCK:
		fprintf(listing, "]\n");
		break;
	case LBRACE:
		fprintf(listing, "{\n");
		break;
	case RBRACE:
		fprintf(listing, "}\n");
		break;
	case LCOMME:
		fprintf(listing, "/*\n");
		break;
	case RCOMME:
		fprintf(listing, "\\*\n");
		break;
	default:
		fprintf(listing, "Unknown token: %d\n", token);
		break;
	}
}

/* creates a new statement node for syntax tree construction*/
TreeNode * newStmtNode(StmtKind kind)
{
	TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else
	{
		for (int i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
		t->type = UNDEFINED;
	}
	return t;
}

/* creates a new expression node for syntax tree construction*/
TreeNode * newExpNode(ExpKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else
	{
		for (int i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}
	return t;
}

/* allocates and makes a new copy of an existing string*/
char * copyString(char *s)
{
	if (s == NULL)
		return NULL;
	int n = strlen(s) + 1;
	char *t = (char*)malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else
		strcpy(t, s);
	return t;
}

/* Variable indentno is used by printTree to
	store current number of spaces to indent*/
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
	for (int i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
	listing file using indentation to indicate subtrees*/
void printTree(TreeNode *tree)
{
	INDENT;
	while (tree != NULL)
	{
		printSpaces();
		if (tree->nodekind == StmtK)
		{
			switch (tree->kind.stmt)
			{
			case SelectionK:
				fprintf(listing, "Selection\n");
				break;
			case CompoundK:
				fprintf(listing, "Compound\n");
				break;
			case ExpessionK:
				fprintf(listing, "Expession\n");
				break;
			case IterationK:
				fprintf(listing, "While\n");
				break;
			case ReturnK:
				fprintf(listing, "Return\n");
				break;
			case DecK:
				fprintf(listing, "Declaration: %s\n", EXPTYPE[tree->type]);
				break;
			case CallK:
				fprintf(listing, "Call\n");
				break;
			case VarDecK:
				fprintf(listing, "Var_Declaration: %s\n", EXPTYPE[tree->type]);
				break;
			case FunDecK:
				fprintf(listing, "Fun_Declaration: %s\n", EXPTYPE[tree->type]);
				break;
			case ParamK:
				fprintf(listing, "Param: %s\n", EXPTYPE[tree->type]);
				break;
			case ParamsK:
				fprintf(listing, "Params\n");
				break;
			case ArgsK:
				fprintf(listing, "Args\n");
				break;
			case ArgK:
				fprintf(listing, "Arg\n");
				break;
			case LocDecK:
				fprintf(listing, "Local_declarations\n");
				break;
			case StatemsK:
				fprintf(listing, "Statemtnt_list\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK)
		{
			switch (tree->kind.exp)
			{
			case OpK:
				fprintf(listing, "Op: ");
				printToken(tree->attr.op, "\0");
				break;
			case ConstK:
				fprintf(listing, "const: %d\n", tree->attr.val);
				break;
			case IdK:
				fprintf(listing, "Id: %s\n", tree->attr.name);
				break;
			default:
				fprintf(listing, "Unknown node kind\n");
				break;
			}
		}
		else
		{
			fprintf(listing, "Unknown node kind\n");
		}
		for (int i = 0; i < MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}