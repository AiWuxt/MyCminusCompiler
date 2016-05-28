/*************************************************************************
    > Author: aiwuxt
    > Mail: aiwuxt@gmail.com 
    > Description: 
 ************************************************************************/
#include"globals.h"
#include"util.h"
#include"scan.h"
#include"parse.h"

// holds current token
static TokenType token;
static TokenType lastToken;

/* function prototypes for recursive calls */
static TreeNode * program(void);
static TreeNode * declaration_list(void);
static TreeNode * declaration(void);
static TreeNode * var_declaration(void);
//static TreeNode * type_specifier(void);
static TreeNode * fun_declaration(void);
static TreeNode * params(void);
static TreeNode * param_list(void);
static TreeNode * param(void);
static TreeNode * compound_stmt(void);
static TreeNode * local_declarations(void);
static TreeNode * statement_list(void);
static TreeNode * statement(void);
static TreeNode * expession_stmt(void);
static TreeNode * selection_stmt(void);
static TreeNode * iteration_stmt(void);
static TreeNode * return_stmt(void);
static TreeNode * expression(void);
static TreeNode * var(void);
static TreeNode * simple_expression(void);
//static TreeNode * relop(void);
static TreeNode * additive_expression(void);
//static TreeNode * addop(void);
static TreeNode * term(void);
//static TreeNode * mulop(void);
static TreeNode * factor(void);
//static TreeNode * call(void);
static TreeNode * args(void);
static TreeNode * arg_list(void);


static void syntaxError(char * message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = true;
}

static void match(TokenType expected)
{
	if (token == expected)
	{
		lastToken = token;
		token = getToken();
	}
	else
	{
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "      ");
	}
}


TreeNode * program(void)
{
	return declaration_list();
}

TreeNode * declaration_list(void)
{
	TreeNode * t = declaration();
	TreeNode * p = t;
	while ((token!=INT) && (token!=VOID) && (token!=ENDFILE))
	{
		token = getToken();
		if (token == ENDFILE)
			break;
	}
	while ((token==INT) || (token==VOID))
	{
		TreeNode * q;
		q = declaration();
		if (q != NULL)
		{
			if (t == NULL)
				t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

TreeNode * declaration(void)
{
	TreeNode * t = newStmtNode(DecK);
	if ((token == INT) || (token == VOID))
	{
		if (token == INT)
			t->type = Integer;
		else
			t->type = Void;
		match(token);
		
		TreeNode * p = newExpNode(IdK);
		if ((p != NULL) && (token == ID))
		{
			p->attr.name = copyString(tokenString);
			t->child[0] = p;
		}
		
		match(ID);
		if (token == LPAREN)
			t->child[1] = fun_declaration();
		else
			t->child[1] = var_declaration();
	}
	return t;
}

TreeNode * var_declaration(void)
{
	TreeNode * t = NULL;
	/*if (lastToken = ID)
	{
		TreeNode * p = newExpNode(IdK);
		if (p != NULL)
		{
			p->attr.name = copyString(tokenString);
			t->child[0] = p;
		}
	}*/
	if ((token == LBARCK))
	{
		match(LBARCK);
		TreeNode * p = newExpNode(ConstK);
		if ((p != NULL) && (token == NUM))
			p->attr.val = atoi(tokenString);
		t = p;
		match(NUM);
		match(RBARCK);
	}
	match(SEMI);
	return t;
}

TreeNode * fun_declaration(void)
{
	TreeNode * t = newStmtNode(FunDecK);
	/*if (t != NULL)
		t->attr.name = copyString(tokenString);*/
	/*
	if (lastToken = ID)
	{
		TreeNode * p = newExpNode(IdK);
		if (p != NULL)
		{
			p->attr.name = copyString(tokenString);
			t->child[0] = p;
		}
	}
	*/
	match(LPAREN);
	t->child[0] = params();
	match(RPAREN);
	t->child[1] = compound_stmt();
	return t;
}

TreeNode * params(void)
{
	TreeNode * t = newStmtNode(ParamsK);
	if (token == VOID)
	{
		t->type = Void;
		match(VOID);
		return t;
	}
	else
		t = param_list();
	return t;
}

TreeNode * param_list(void)
{
	TreeNode * t = newStmtNode(ParamsK);
	int i = 0;
	t->child[i] = param();
	while (token==COMMA)
	{
		match(COMMA);
		t->child[++i] = param();
	}
	return t;
}

TreeNode * param(void)
{
	TreeNode * t = newStmtNode(ParamK);
	if ((token == INT) || (token == VOID))
	{
		if (token == INT)
			t->type = Integer;
		else
			t->type = Void;
		match(token);
		TreeNode * p = newExpNode(IdK);
		if ((p != NULL) && (token == ID))
		{
			p->attr.name = copyString(tokenString);
			t->child[0] = p;
		}
		match(ID);
		if (token == LBARCK)
		{
			match(LBARCK);
			p = newExpNode(ConstK);
			if (p != NULL)
			{
				p->attr.val = NULL;
				t->child[1] = p;
			}
			match(RBARCK);
		}
	}
	return t;
}

TreeNode * compound_stmt(void)
{
	TreeNode * t = newStmtNode(CompoundK);
	match(LBRACE);
	if (t != NULL)
	{
		t->child[0] = local_declarations();
		t->child[1] = statement_list();
	}
	match(RBRACE);
	return t;
}

TreeNode * local_declarations(void)
{
	TreeNode * t = newStmtNode(LocDecK);
	int i = 0;
	while ((token == INT) || (token == VOID))
	{
		TreeNode * p = newStmtNode(VarDecK);
		if (token == INT)
			p->type = Integer;
		else
			p->type = Void;
		match(token);
		TreeNode * q = newExpNode(IdK);
		if (q!=NULL && token == ID)
		{
			q->attr.name = copyString(tokenString);
			p->child[0] = q;
			match(ID);
		}
		else
		{
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			token = getToken();
		}
		if ((token == LBARCK))
		{
			match(LBARCK);
			TreeNode * r = newExpNode(ConstK);
			if ((r != NULL) && (token == NUM))
				r->attr.val = atoi(tokenString);
			p->child[1] = r;
			match(NUM);
			match(RBARCK);
		}
		match(SEMI);
		//p->child[1] = var_declaration();
		t->child[i++] = p;
	}
	return t;
}

TreeNode * statement_list(void)
{
	TreeNode * t = newStmtNode(StatemsK);
	int i = 0;
	while (token != RBRACE)
		t->child[i++] = statement();
	return t;
}

TreeNode * statement(void)
{
	TreeNode * t = NULL;
	switch (token)
	{
	case IF:
		t = selection_stmt();
		break;
	case LBRACE:
		t = compound_stmt();
		break;
	case WHILE:
		t = iteration_stmt();
		break;
	case RETURN:
		t = return_stmt();
		break;
	default:
		t = expession_stmt();
		break;
	}
	return t;
}

TreeNode * expession_stmt(void)
{
	TreeNode * t = newStmtNode(ExpessionK);
	if ((token != SEMI) && (t!=NULL))
			t->child[0] = expression();
	match(SEMI);
	return t;
}

TreeNode * selection_stmt(void)
{
	TreeNode * t = newStmtNode(SelectionK);
	match(IF);
	match(LPAREN);
	if (t != NULL)
		t->child[0] = expression();
	match(RPAREN);
	if (t != NULL)
		t->child[1] = statement();
	if (token == ELSE)
	{
		match(ELSE);
		if (t != NULL)
			t->child[2] = statement();
	}
	return t;
}

TreeNode * iteration_stmt(void)
{
	TreeNode * t = newStmtNode(IterationK);
	match(WHILE);
	match(LPAREN);
	if (t != NULL)
		t->child[0] = expression();
	match(RPAREN);
	if (t != NULL)
		t->child[1] = statement();
	return t;
}

TreeNode * return_stmt(void)
{
	TreeNode * t = newStmtNode(ReturnK);
	match(RETURN);
	if (token != SEMI)
	{
		if (t != NULL)
			t->child[0] = expression();
	}
	match(SEMI);
	return t;
}

TreeNode * expression(void)
{
	TreeNode * t = simple_expression();
	TreeNode * p = newExpNode(OpK);
	if (token == ASSIGN)
	{
		p->attr.op = token;
		match(ASSIGN);
		p->child[0] = t;
		p->child[1] = expression();
		return p;
	}
	return t;
}

TreeNode * var(void)
{
	TreeNode * t = NULL;
	if (token == ID)
	{
		t = newExpNode(IdK);
		if ((t != NULL) && (token == ID))
			t->attr.name = copyString(tokenString);
		match(ID);
		if (token == LBARCK)
		{
			match(LBARCK);
			t->child[0] = expression();
			match(RBARCK);
		}
	}
	return t;
}

TreeNode * simple_expression(void)
{
	TreeNode * t = additive_expression();
	if ((token == LE) || (token == LT) || (token == GE)
		|| (token == GT) || (token == EQ) || (token == UEQ))
	{
		TreeNode * p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t != NULL)
			t->child[1] = additive_expression();
	}
	return t;
}

TreeNode * additive_expression(void)
{
	TreeNode * t = term();
	while ((token == PLUS) || (token == MINUS))
	{
		TreeNode * p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			match(token);
			t->child[1] = term();
		}
	}
	return t;
}

TreeNode * term(void)
{
	TreeNode * t = factor();
	while ((token==TIMES)||(token==DIVIDE))
	{
		TreeNode * p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
			match(token);
			p->child[1] = factor();
		}
	}
	return t;
}

TreeNode * factor(void)
{
	TreeNode * t = NULL;
	switch (token)
	{
	case NUM:
		t = newExpNode(ConstK);
		if ((t != NULL) && (token == NUM))
			t->attr.val = atoi(tokenString);
		match(NUM);
		break;
	case LPAREN:
		match(LPAREN);
		t = expression();
		match(RPAREN);
		break;
	case ID:
		t = newExpNode(IdK);
		if ((t != NULL) && (token == ID))
			t->attr.name = copyString(tokenString);
		match(ID);
		if (token == LBARCK)
		{
			match(LBARCK);
			t->child[0] = expression();
			match(RBARCK);
		}
		else if (token == LPAREN)
		{
			match(LPAREN);
			t->child[0] = args();
			match(RPAREN);
		}
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode * args(void)
{
	TreeNode * t = NULL;
	if (token!=RPAREN)
		t = arg_list();
	return t;
}

TreeNode * arg_list(void)
{
	TreeNode * t = newStmtNode(ArgsK);
	int i = 0;
	t->child[i] = expression();
	while (token == COMMA)
	{
		match(COMMA);
		t->child[++i] = expression();
	}
	return t;
}


TreeNode * parse(void)
{
	TreeNode * t;
	token = getToken();
	t = program();
	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}