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
		if (token == ENDFILE)
		{
			return NULL;
			break;
		}	
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
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
	TreeNode * t = fun_declaration();
	return t;
}

TreeNode * fun_declaration(void)
{
	TreeNode * t = NULL;
	if (token==INT || token == VOID)
	{
		t = newStmtNode(VarDecK);
		if (token == INT)
			t->type = Integer;
		else
			t->type = Void;
		match(token);
		TreeNode * p = newExpNode(IdK);
		if (p != NULL && token == ID)
		{
			p->attr.name = copyString(tokenString);
			t->child[0] = p;
			match(ID);
		}
		else
		{
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			token = getToken();
		}
		if (token == LPAREN)
		{
			TreeNode * func = newStmtNode(FunDecK);
			func->type = t->type;
			func->child[0] = t->child[0];
			if (token == LPAREN)
			{
				match(LPAREN);
				func->child[1] = params();
				match(RPAREN);
				func->child[2] = compound_stmt();
			}
			return func;
		}
		else
		{
			if ((token == LBARCK))
			{
				match(LBARCK);
				TreeNode * q = newExpNode(ConstK);
				if ((q != NULL) && (token == NUM))
					q->attr.val = atoi(tokenString);
				p->child[1] = q;
				match(NUM);
				match(RBARCK);
			}
			match(SEMI);
		}
	}
	return t;
}

TreeNode * params(void)
{
	TreeNode * t = param_list();
	return t;
}

TreeNode * param_list(void)
{
	TreeNode * t = param();
	if (t == NULL)
	{
		t = newStmtNode(ParamK);
		t->type = Void;
		return t;
	}
	TreeNode * p = t;
	while (token==COMMA)
	{
		TreeNode * q;
		match(COMMA);
		q = param();
		if (q != NULL)
		{
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

TreeNode * param(void)
{
	TreeNode * t = NULL;
	if ((token == INT) || (token == VOID))
	{
		t = newStmtNode(ParamK);
		if (token == INT)
			t->type = Integer;
		else
			t->type = Void;
		if (lastToken == LPAREN && token == VOID)
		{
			match(token);
			if (lastToken == VOID && token == RPAREN)
				return NULL;
		}
		else
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
	else
	{
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
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
TreeNode * var_declaration(void)
{
	TreeNode * t = NULL;
	if (token==INT || token == VOID)
	{
		t = newStmtNode(VarDecK);
		if (token == INT)
			t->type = Integer;
		else
			t->type = Void;
		match(token);
		TreeNode * p = newExpNode(IdK);
		if (p != NULL && token == ID)
		{
			p->attr.name = copyString(tokenString);
			t->child[0] = p;
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
			TreeNode * q = newExpNode(ConstK);
			if ((q != NULL) && (token == NUM))
				q->attr.val = atoi(tokenString);
			p->child[1] = q;
			match(NUM);
			match(RBARCK);
		}
		match(SEMI);
	}
	return t;
}
TreeNode * local_declarations(void)
{
	TreeNode * t = var_declaration();
	TreeNode * p = t;
	while ((token == INT) || (token == VOID))
	{
		TreeNode * q = var_declaration();
		if (q != NULL)
		{
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

TreeNode * statement_list(void)
{
	TreeNode * t = statement();
	TreeNode * p = t;
	while (token != RBRACE)	
	{
		if (token == ENDFILE)
		{
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			break;
		}
		TreeNode *  q;
		q = statement();
		if (q != NULL)
		{
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
	}	
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
	case ID:
	case LPAREN:
	case NUM:
		t = expession_stmt();
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
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
	TreeNode * t = newStmtNode(ArgsK);
	if (token!=RPAREN)
		t = arg_list();
	return t;
}

TreeNode * arg_list(void)
{
	TreeNode * t = newStmtNode(ArgsK);
	TreeNode * p = t;
	TreeNode * q = expression();
	while (token == COMMA)
	{
		match(COMMA);
		if (q != NULL)
		{
			if (t == NULL) t = p = q;
			else
			{
				p->sibling = q;
				p = q;
			}
		}
		q = expression();
	}
	if (q != NULL)
	{
		if (t == NULL) t = p = q;
		else
		{
			p->sibling = q;
			p = q;
		}
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