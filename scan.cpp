/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/

#include"globals.h"
#include"util.h"
#include"scan.h"

typedef enum
{
	START, INLE, INGE, INEQ, INUEQ, INCOMMSTART, INCOMMEND1,
	INCOMMEND2, INNUM, INID, DONE
} StateType;

char tokenString[MAXTOKENLEN + 1];

#define BUFLEN 256

static char lineBuf[BUFLEN];
static int linepos = 0;
static int bufsize = 0;

static char getNextChar(void)
{
	if (!(linepos < bufsize))
	{
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source))
		{
			if (EchoSource)
				fprintf(listing, "%4d: %s", lineno, lineBuf);
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else
			return EOF;
	}
	else
		return lineBuf[linepos++];
}

static void ungetNextChar(void)
{
	linepos--;
}

static struct 
{
	char * str;
	TokenType tok;
} reservedWords[MAXRESERVED] = 
{{"if", IF}, {"else", ELSE}, {"int", INT},
{"return", RETURN}, {"void", VOID}, {"while", WHILE}
};

static TokenType reservedLookup(char *s)
{
	for (int i = 0; i < MAXRESERVED; i++)
	{
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	}
	return ID;
}

TokenType getToken(void)
{
	int tokenStringIndex = 0;
	TokenType currentToken;
	StateType state = START;
	bool save;
	while (state != DONE)
	{
		char c = getNextChar();
		save = true;
		switch (state)
		{
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c))
				state = INID;
			else if (c == ' ' || c == '\t' || c == '\n')
				save = false;
			else if (c == '/')
			{
				save = false;
				state = INCOMMSTART;
			}
			else if (c == '<')
				state = INLE;
			else if (c == '>')
			{
				save = false;
				state = INGE;
			}
			else if (c == '=')
				state = INEQ;
			else if (c == '!')
				state = INUEQ;
			else
			{
				state = DONE;
				switch (c)
				{
				case EOF:
					save = false;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case ';':
					currentToken = SEMI;
					break;
				case ',':
					currentToken = COMMA;
					break;
				case '(':
					currentToken = LPAREN;
					break;
				case ')':
					currentToken = RPAREN;
					break;
				case '[':
					currentToken = LBARCK;
					break;
				case ']':
					currentToken = RBARCK;
					break;
				case '{':
					currentToken = LBRACE;
					break;
				case '}':
					currentToken = RBRACE;
					break;
				default:
					break;
				}
			}
			break;
		case INCOMMSTART:
			if (c == '*')
			{
				save = false;
				state = INCOMMEND1;
			}
			else
			{
				state = DONE;
				ungetNextChar();
				currentToken = DIVIDE;
			}
			break;
		case INCOMMEND1:
			save = false;
			if (c == '*')
				state = INCOMMEND2;
			break;
		case INCOMMEND2:
			save = false;
			if (c == '/')
				state = START;
			else
				state = INCOMMEND2;
			break;
		case INLE:
			state = DONE;
			if (c == '=')
				currentToken = LE;
			else
			{
				ungetNextChar();
				currentToken = LT;
			}
			break;
		case INGE:
			state = DONE;
			if (c == '=')
				currentToken = GE;
			else
			{
				ungetNextChar();
				currentToken = GT;
			}
			break;
		case INEQ:
			state = DONE;
			if (c == '=')
				currentToken = EQ;
			else
			{
				ungetNextChar();
				currentToken = ASSIGN;
			}
			break;
		case INUEQ:
			state = DONE;
			if (c == '=')
				currentToken = UEQ;
			else
			{
				ungetNextChar();
				save = false;
				currentToken = ERROR;
			}
			break;
		case INNUM:
			if (!isdigit(c))
			{
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c))
			{
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = ID;
			}
			break;
		case DONE:
			break;
		default:
			fprintf(listing, "Scanner Bug: state= %d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;
			break;
		}
		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			tokenString[tokenStringIndex++] = c;
		if (state == DONE)
		{
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)
				currentToken = reservedLookup(tokenString);
		}
	}
	if (TraceScan)
	{
		fprintf(listing, "\t%d: ", lineno);
		printToken(currentToken, tokenString);
	}
	return currentToken;
}