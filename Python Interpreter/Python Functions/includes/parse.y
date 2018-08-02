%{
	#include "includes/ast.h"
	#include <iostream>
	#include <string>
	#include <stack>


	int yylex (void);
	extern char *yytext;

	extern int yylineno;
	void yyerror (const char *);

	struct function {
		int firstLine;
		int firstColumn;
		int complexity = 1;
		std::string functionName;
	}func;

	PoolOfNodes& pool = PoolOfNodes::getInstance();

	bool isfinal = false;
	int temp = 0;
	int isinner = 0;
	std::stack<function> functionStack;

    void reverseStack(std::stack<function> functionStack);
    void yyerror (const char *s);
%}

%union 
{
    Node* node;
    int intNumber;
    float floatNumber;
    char *str;
    char op;
}
// 83 tokens, in alphabetical order:

%token AMPEREQUAL AMPERSAND AND AS ASSERT AT BACKQUOTE BAR BREAK CIRCUMFLEX
%token CIRCUMFLEXEQUAL CLASS COLON COMMA CONTINUE DEDENT DEF DEL DOT DOUBLESLASH
%token DOUBLESLASHEQUAL DOUBLESTAR DOUBLESTAREQUAL ELIF ELSE ENDMARKER EQEQUAL
%token EQUAL EXCEPT EXEC FINALLY FOR FROM GLOBAL GREATER GREATEREQUAL GRLT
%token IF IMPORT IN INDENT IS LAMBDA LBRACE LEFTSHIFT LEFTSHIFTEQUAL LESS
%token LESSEQUAL LPAR LSQB MINEQUAL MINUS NAME NEWLINE NOT NOTEQUAL NUMBER
%token OR PASS PERCENT PERCENTEQUAL PLUS PLUSEQUAL PRINT RAISE RBRACE RETURN
%token RIGHTSHIFT RIGHTSHIFTEQUAL RPAR RSQB SEMI SLASH SLASHEQUAL STAR STAREQUAL
%token STRING TILDE TRY VBAREQUAL WHILE WITH YIELD
%token INT FLOAT

%type <str> NAME
%type <intNumber> INT
%type <floatNumber> FLOAT
%type <op>	pick_PLUS_MINUS pick_multop pick_unop augassign comp_op
%type <node> atom power factor term arith_expr testlist pick_yield_expr_testlist
star_EQUAL shift_expr and_expr xor_expr expr comparison not_test and_test or_test opt_IF_ELSE test star_COMMA_test expr_stmt parameters star_trailer trailer opt_arglist 
arglist argument pick_argument pick_yield_expr_testlist_comp opt_yield_test testlist_comp print_stmt opt_test funcdef stmt suite plus_stmt pick_NEWLINE_stmt star_NEWLINE_stmt decorated exec_stmt assert_stmt if_stmt simple_stmt small_stmt flow_stmt return_stmt star_ELIF compound_stmt 

%start start

%locations

%%

start
	: file_input
	;
file_input 
	: star_NEWLINE_stmt ENDMARKER
	{
	}
	;
pick_NEWLINE_stmt 
	: NEWLINE
	{
		$$ = new PrintNode(0); 
		pool.add($$);
	}
	| stmt
	{	
		if($1) {
			$1->eval();
		}
		else{
		}
	}
	;
star_NEWLINE_stmt 
	: star_NEWLINE_stmt pick_NEWLINE_stmt
	| %empty 	{ $$ = nullptr;}
	;
decorator 
	: AT dotted_name LPAR opt_arglist RPAR NEWLINE
	| AT dotted_name NEWLINE
	;
opt_arglist 
	: arglist 	{ $$ = $1;}
	| %empty 	{ $$ = nullptr;}
	;
decorators 
	: decorators decorator
	| decorator
	;
decorated 
	: decorators classdef 	{$$ = nullptr;}
	| decorators funcdef  	{$$ = nullptr;}
	;
funcdef 
	: DEF NAME parameters COLON suite	
        {
        	$$ = new FunctionNode($2,$5);
        	pool.add($$);
        	delete[] $2;
        }
	;
parameters 
	: LPAR varargslist RPAR 	{$$ = 0;}
	| LPAR RPAR 	{$$ = 0;}
	;
varargslist 
	: star_fpdef_COMMA pick_STAR_DOUBLESTAR
	| star_fpdef_COMMA fpdef opt_EQUAL_test opt_COMMA
	;
opt_EQUAL_test 
	: EQUAL test
	| %empty
	;
star_fpdef_COMMA 
	: star_fpdef_COMMA fpdef opt_EQUAL_test COMMA
	| %empty
	;
opt_DOUBLESTAR_NAME 
	: COMMA DOUBLESTAR NAME     {delete[] $3;}
	;
pick_STAR_DOUBLESTAR 
	: STAR NAME opt_DOUBLESTAR_NAME  {delete[] $2;}
	| DOUBLESTAR NAME   {delete[] $2;}
	;
opt_COMMA 
	: COMMA
	| %empty
	;
fpdef 
	: NAME  {delete[] $1;}
	| LPAR fplist RPAR
	;
fplist 
	: fpdef star_fpdef_notest COMMA
	| fpdef star_fpdef_notest
	;
star_fpdef_notest
	: star_fpdef_notest COMMA fpdef
	| %empty
	;
stmt 
	: simple_stmt    { $$ = $1;}      
	| compound_stmt  { $$ = $1;}   
	;
simple_stmt 
	: small_stmt star_SEMI_small_stmt SEMI NEWLINE  { $$ = $1;}
	| small_stmt star_SEMI_small_stmt NEWLINE 		{ $$ = $1;}
	;
star_SEMI_small_stmt 
	: star_SEMI_small_stmt SEMI small_stmt
	| %empty
	;
small_stmt 
	: expr_stmt 	{ $$ = $1;}
	| print_stmt 	{ $$ = $1;}
	| del_stmt 		{ $$ = nullptr;}
	| pass_stmt 	{ $$ = nullptr;}
	| flow_stmt 	{ $$ = $1;}
	| import_stmt 	{ $$ = nullptr;}
	| global_stmt 	{ $$ = nullptr;}
	| exec_stmt 	{ $$ = nullptr;}
	| assert_stmt	{ $$ = nullptr;}
	;
expr_stmt
	: testlist augassign pick_yield_expr_testlist 
	{ 
		if($1 && $3) {
			if($2 == '0')
			{
				Node* temp = new AddBinaryNode($1,$3);
				$1 = new AsgBinaryNode($1,temp);
				pool.add($1);
				delete temp;
							
			}
			else if($2 == '1')
			{
				Node* temp = new SubBinaryNode($1,$3);
				$1 = new AsgBinaryNode($1,temp);
				pool.add($1);
				delete temp;						
							
			}
			else if($2 == '2')
			{
				Node* temp = new MulBinaryNode($1,$3);
				$1 = new AsgBinaryNode($1,temp);
				pool.add($1);
				delete temp;
				
			}
			else if($2 == '3')
			{
				Node* temp = new DivBinaryNode($1,$3);
				$1 = new AsgBinaryNode($1,temp);
				pool.add($1);
				delete temp;
				
			}
			else if($2 == '4')
			{
				Node* temp = new PctBinaryNode($1,$3);
				$1 = new AsgBinaryNode($1,temp);
				pool.add($1);
				delete temp;
				
			}
			else if($2 == '5')
			{
				Node* temp = new DbStarBinaryNode($1,$3);
			    $1 = new AsgBinaryNode($1, temp);
				pool.add($1);
				delete temp;

			}
			else if($2 == '6')  
			{
				Node* temp = new DbSlashBinaryNode($1,$3);
				$1 = new AsgBinaryNode($1, temp);
				pool.add($1);
				delete temp;
			}
		}	
	}
	| testlist star_EQUAL
		{
			if($2)
			{
				$$ = new AsgBinaryNode($1,$2);
				pool.add($$);
			}
			else 
			{
				$$ = $1;
			}
		}
	;
pick_yield_expr_testlist 
	: yield_expr {$$ = 0;}
	| testlist 	{$$ = $1;}
	;
star_EQUAL 
	: star_EQUAL EQUAL pick_yield_expr_testlist
		{
			$$ = $3;
		}
	| %empty 	{$$ = 0;}
	;
augassign 
	: PLUSEQUAL		{ $$ = '0';}
	| MINEQUAL 		{ $$ = '1';}
	| STAREQUAL 	{ $$ = '2';}
	| SLASHEQUAL 	{ $$ = '3';}
	| PERCENTEQUAL 	{ $$ = '4';}
	| AMPEREQUAL 	{ $$ = 0;}
	| VBAREQUAL 	{ $$ = 0;}
	| CIRCUMFLEXEQUAL 	{ $$ = 0;}
	| LEFTSHIFTEQUAL 	{ $$ = 0;}
	| RIGHTSHIFTEQUAL 	{ $$ = 0;}
	| DOUBLESTAREQUAL 	{ $$ = '5';}
	| DOUBLESLASHEQUAL 	{ $$ = '6';}
	;
print_stmt 
	: PRINT opt_test 	
		{
			if ($2)
			{
				$$ = new PrintNode($2);
				pool.add($$);
			}
			else 
				$$ = nullptr;

		}
	| PRINT RIGHTSHIFT test opt_test_2 	{$$ = 0;}
	;
star_COMMA_test 
	: star_COMMA_test COMMA test 	{$$ = $3;}
	| %empty 	{$$ = 0;}
	;
opt_test 
	: test star_COMMA_test opt_COMMA {$$ = $1;}
	| %empty {$$ = nullptr;}
	;
plus_COMMA_test 
	: plus_COMMA_test COMMA test
	| COMMA test
	;
opt_test_2 
	: plus_COMMA_test opt_COMMA
	| %empty
	;
del_stmt 
	: DEL exprlist
	;
pass_stmt 
	: PASS
	;
flow_stmt
	: break_stmt 	{$$ = 0;}
	| continue_stmt {$$ = 0;}
	| return_stmt 	{$$ = $1;}
	| raise_stmt 	{$$ = 0;}
	| yield_stmt 	{$$ = 0;}
	;
break_stmt 
	: BREAK
	;
continue_stmt 
	: CONTINUE
	;
return_stmt 
	: RETURN testlist 	
	{ 
		$$ = new ReturnNode($2);
		pool.add($$);
	}
	| RETURN 	
	{ 
		$$ = new ReturnNode(nullptr);
		pool.add($$);
	}
	;
yield_stmt
	: yield_expr
	;
raise_stmt 
	: RAISE test opt_test_3
	| RAISE
	;
opt_COMMA_test 
	: COMMA test
	| %empty
	;
opt_test_3 
	: COMMA test opt_COMMA_test
	| %empty
	;
import_stmt 
	: import_name
	| import_from
	;
import_name 
	: IMPORT dotted_as_names
	;
import_from 
	: FROM pick_dotted_name IMPORT pick_STAR_import
	;
pick_dotted_name 
	: star_DOT dotted_name
	| star_DOT DOT
	;
pick_STAR_import 
	: STAR
	| LPAR import_as_names RPAR
	| import_as_names
	;
import_as_name 
	: NAME AS NAME  {delete [] $1; delete[] $3;}
	| NAME          {delete[] $1;}
	;
dotted_as_name 
	: dotted_name AS NAME   {delete[] $3;}
	| dotted_name
	;
import_as_names 
	: import_as_name star_COMMA_import_as_name COMMA
	| import_as_name star_COMMA_import_as_name
	;
star_COMMA_import_as_name 
	: star_COMMA_import_as_name COMMA import_as_name
	| %empty
	;
dotted_as_names 
	: dotted_as_name
	| dotted_as_names COMMA dotted_as_name
	;
dotted_name 
	: NAME  {delete[] $1;}
	| dotted_name DOT NAME  {delete[] $3;}
	;
global_stmt 
	: GLOBAL NAME star_COMMA_NAME   {delete[] $2;}
	;
star_COMMA_NAME 
	: star_COMMA_NAME COMMA NAME    {delete[] $3;}
	| %empty
	;
exec_stmt 
	: EXEC expr IN test opt_COMMA_test 	{$$ = nullptr;}
	| EXEC expr 	{ $$ = nullptr;}
	;
assert_stmt 
	: ASSERT test COMMA test 	{ $$ = nullptr;}
	| ASSERT test 				{ $$ = nullptr;}
	;
compound_stmt 
	: if_stmt       { $$ = $1;}    
	| while_stmt    { $$ = nullptr;}    
	| for_stmt      { $$ = nullptr;}    
	| try_stmt   	{ $$ = nullptr;}
	| with_stmt 	{ $$ = nullptr;}
	| funcdef 		{ $$ = $1;}
	| classdef 		{ $$ = nullptr;}
	| decorated 	{ $$ = $1;}
	;
if_stmt 
	: IF test COLON suite star_ELIF ELSE COLON suite    
	{
		if ($2){
			$$ = new IfNode($2,$4,$8);
			pool.add($$);
		}
		else
			$$ = nullptr;
	}
	| IF test COLON suite star_ELIF   
	{
		if ($2){
			$$ = new IfNode($2,$4,nullptr);
			pool.add($$);
		}
		else 
			$$ = nullptr;
	}  
	;
star_ELIF 
	: star_ELIF ELIF test COLON suite  
		{
			$$ = nullptr;
		}
	| %empty 	{ $$ = nullptr;}
	;
while_stmt 
	: WHILE test COLON suite ELSE COLON suite       
	| WHILE test COLON suite                        
	;
for_stmt 
	: FOR exprlist IN testlist COLON suite ELSE COLON suite     
	| FOR exprlist IN testlist COLON suite                      
	;
try_stmt 
	: TRY COLON suite plus_except opt_ELSE opt_FINALLY  
	| TRY COLON suite FINALLY COLON suite       
	;
plus_except 
	: plus_except except_clause COLON suite
	| except_clause COLON suite
	;
opt_ELSE 
	: ELSE COLON suite
	| %empty
	;
opt_FINALLY 
	: FINALLY COLON suite 
	| %empty
	;
with_stmt 
	: WITH with_item star_COMMA_with_item COLON suite
	;
star_COMMA_with_item 
	: star_COMMA_with_item COMMA with_item
	| %empty
	;
with_item 
	: test AS expr
	| test
	;
except_clause
	: EXCEPT test opt_AS_COMMA	
	| EXCEPT 	
	;
pick_AS_COMMA
	: AS
	| COMMA
	;
opt_AS_COMMA
	: pick_AS_COMMA test
	| %empty
	;
suite 
	: simple_stmt     { $$ = $1;}        
	| NEWLINE INDENT plus_stmt DEDENT   
		{
			$$ = $3;
		}
	;
plus_stmt 
	: plus_stmt stmt        
		{
			$$ = $1;
			dynamic_cast<SuiteNode*>($$)->insert($2);
		}
	| stmt                  
		{
			$$ = new SuiteNode();
			dynamic_cast<SuiteNode*>($$)->insert($1);
			pool.add($$);
		}
	;
testlist_safe 
	: old_test plus_COMMA_old_test opt_COMMA
	| old_test
	;
plus_COMMA_old_test 
	: plus_COMMA_old_test COMMA old_test
	| COMMA old_test
	;
old_test 
	: or_test
	| old_lambdef
	;
old_lambdef 
	: LAMBDA varargslist COLON old_test
	| LAMBDA COLON old_test
	;
test
	: or_test opt_IF_ELSE	{$$ = $1;}
	| lambdef 	{$$ = 0;}
	;
opt_IF_ELSE 
	: IF or_test ELSE test 	{$$ = 0;}
	| %empty 	{$$ = 0;}
	;
or_test 
	: and_test 	{$$ = $1;}
	| or_test OR and_test 	{$$ = 0;}
	;
and_test 
	: not_test 	{$$ = $1;}
	| and_test AND not_test 	{$$ = 0;}
	;
not_test 
	: NOT not_test 	{$$ = 0;}
	| comparison 	{$$ = $1;}
	;
comparison 
	: expr 		{$$ = $1;}
	| comparison comp_op expr 
	{
		if($2 == '1')
		{
			$$ = new LessBinaryNode($1,$3);
			pool.add($$);
		} else if($2 == '2') {
			$$ = new GreaterBinaryNode($1,$3);
			pool.add($$);
		} else if($2 == '3') {
			$$ = new EqualEqualBinaryNode($1,$3);
			pool.add($$);
		} else if($2 == '4') {
			$$ = new GreaterEqualBinaryNode($1,$3);
			pool.add($$);
		} else if($2 == '5') {
			$$ = new LessEqualBinaryNode($1,$3);
			pool.add($$);
		} else if($2 == '6') {
			$$ = new NotEqualBinaryNode($1,$3);
			pool.add($$);
		}
	}
	;
comp_op 
	: LESS 			{ $$ = '1';}
	| GREATER 		{ $$ = '2';}
	| EQEQUAL 		{ $$ = '3';}
	| GREATEREQUAL 	{ $$ = '4';}
	| LESSEQUAL 	{ $$ = '5';}
	| GRLT 			{ $$ = 0;}
	| NOTEQUAL 		{ $$ = 6;}
	| IN 			{ $$ = 0;}
	| NOT IN 		{ $$ = 0;}
	| IS 			{ $$ = 0;}
	| IS NOT 		{ $$ = 0;}
	;
expr 
	: xor_expr	{$$ = $1;}
	| expr BAR xor_expr {$$ = 0;}
	;
xor_expr 
	: and_expr	{$$ = $1;}
	| xor_expr CIRCUMFLEX and_expr {$$ = 0;}
	;
and_expr
	: shift_expr {$$ = $1;}
	| and_expr AMPERSAND shift_expr {$$ = 0;}
	;
shift_expr
	: arith_expr	
	{
		$$ = $1;
	}
	| shift_expr pick_LEFTSHIFT_RIGHTSHIFT arith_expr  {$$ = 0;}
	;
pick_LEFTSHIFT_RIGHTSHIFT
	: LEFTSHIFT
	| RIGHTSHIFT
	;
arith_expr 
	: term	
	{ 
		$$ = $1; 
	}
	| arith_expr pick_PLUS_MINUS term
	{
		if($2 == '+')
		{
			$$ = new AddBinaryNode($1,$3);
			pool.add($$);
		}
		else if($2 == '-') 
		{
			$$ = new SubBinaryNode($1,$3);
			pool.add($$);
		}
	}
	;
pick_PLUS_MINUS
	: PLUS 	{ $$ = '+';}
	| MINUS { $$ = '-';}
	;
term 
	: factor 	{ $$ = $1;}
	| term pick_multop factor 
	{
		if($2 == '*')
		{
			$$ = new MulBinaryNode($1,$3);
			pool.add($$);
		}
		else if($2 == '/')
		{
			$$ = new DivBinaryNode($1,$3);
			pool.add($$);
		}
		else if($2 == '%')
		{
			$$ = new PctBinaryNode($1,$3);
			pool.add($$);
		}
		else if($2 == '7')
		{
			$$ = new DbSlashBinaryNode($1,$3);
			pool.add($$);
		}
		else
		{
		}
	}
	;
pick_multop
	: STAR 	{ $$ = '*';}
	| SLASH { $$ = '/';}
	| PERCENT 		{$$ = '%';}
	| DOUBLESLASH 	{ $$ = '7';}
	;
factor
	: pick_unop factor	
	{
	    if($2) {
 			if($1 == '+') {
 				$$ = $2;
			
 			}
 		    else if($1 == '-') {
				$$ = new UnaryNode($2);
				pool.add($$);
 			} else {
 				
 			}

	    }
	}
	| power		{$$ = $1;}
	;
pick_unop
	: PLUS 	{ $$ = '+';}
	| MINUS { $$ = '-';}
	| TILDE { $$ = '~';}
	;
power
	: atom star_trailer DOUBLESTAR factor	
	{
		if ($1 && $4)
		{
			$$ = new DbStarBinaryNode($1,$4);
			pool.add($$);
		}
	}
	| atom star_trailer		
	{
		if ($2) 
		{
			std::string funcname = reinterpret_cast<IdentNode*>($1)->getIdent();
			$$ = new CallNode(funcname);
			pool.add($$);
		}
		else 
			$$ = $1;
	}
	;
star_trailer
	: star_trailer trailer 	{ $$ = $2;}
	| %empty 	{$$ = 0;}
	;
atom
	: LPAR opt_yield_test RPAR {$$ = $2;}
	| LSQB opt_listmaker RSQB	{$$ = 0;}
	| LBRACE opt_dictorsetmaker RBRACE {$$ = 0;}
	| BACKQUOTE testlist1 BACKQUOTE	{$$ = 0;}
	| NAME  
			{
				$$ = new IdentNode($1);
				delete[] $1;
				pool.add($$);
			}
	| INT 	
			{
				$$ = new IntLiteral($1);
				pool.add($$);
			}
	| FLOAT
			{
				$$ = new FloatLiteral($1);
				pool.add($$);
			}
	| plus_STRING	{$$ = 0;}
	;
pick_yield_expr_testlist_comp
	: yield_expr 	{$$ = 0;}
	| testlist_comp {$$ = $1;}
	;
opt_yield_test
	: pick_yield_expr_testlist_comp {$$ = $1;}
	| %empty {$$ = 0;}
	;
opt_listmaker
	: listmaker
	| %empty
	;
opt_dictorsetmaker
	: dictorsetmaker
	| %empty
	;
plus_STRING
	: plus_STRING STRING
	| STRING
	;
listmaker
	: test list_for
	| test star_COMMA_test opt_COMMA
	;
testlist_comp
	: test comp_for 	{$$ = 0;}
	| test star_COMMA_test opt_COMMA 	{$$ = $1;}
	;
lambdef
	: LAMBDA varargslist COLON test
	| LAMBDA COLON test
	;
trailer
	: LPAR opt_arglist RPAR  {}	
	| LSQB subscriptlist RSQB 	{ $$ = 0;}
	| DOT NAME  { delete[] $2;}
	;
subscriptlist
	: subscript star_COMMA_subscript COMMA
	| subscript star_COMMA_subscript
	;
star_COMMA_subscript
	: star_COMMA_subscript COMMA subscript
	| %empty
	;
subscript
	: DOT DOT DOT
	| test
	| opt_test_only COLON opt_test_only opt_sliceop
	;
opt_test_only
	: test
	| %empty
	;
opt_sliceop
	: sliceop
	| %empty
	;
sliceop
	: COLON test
	| COLON
	;
exprlist
	: expr star_COMMA_expr COMMA
	| expr star_COMMA_expr
	;
star_COMMA_expr
	: star_COMMA_expr COMMA expr
	| %empty
	;
testlist
	: test star_COMMA_test COMMA 	
	| test star_COMMA_test 	{$$ = $1;}
	;
dictorsetmaker
	: test COLON test pick_for_test_test
	| test pick_for_test
	;
star_test_COLON_test
	: star_test_COLON_test COMMA test COLON test
	| %empty
	;
pick_for_test_test
	: comp_for
	| star_test_COLON_test opt_COMMA
	;
pick_for_test
	: comp_for
	| star_COMMA_test opt_COMMA
	;
classdef
	: CLASS NAME LPAR opt_testlist RPAR COLON suite  {delete[] $2;}
	| CLASS NAME COLON suite    {delete[] $2;}
	;
opt_testlist
	: testlist
	| %empty
	;
arglist
	: star_argument_COMMA pick_argument {$$ = $2;}
	;
star_argument_COMMA
	: star_argument_COMMA argument COMMA
	| %empty
	;
star_COMMA_argument
	: star_COMMA_argument COMMA argument
	| %empty
	;
opt_DOUBLESTAR_test
	: COMMA DOUBLESTAR test
	| %empty
	;
pick_argument
	: argument opt_COMMA 	{$$ = $1;}
	| STAR test star_COMMA_argument opt_DOUBLESTAR_test 	{$$ = 0;}
	| DOUBLESTAR test 	{$$ = 0;}
	;
argument
	: test opt_comp_for 	{$$ = $1;}
	| test EQUAL test
	;
opt_comp_for
	: comp_for
	| %empty
	;
list_iter
	: list_for
	| list_if
	;
list_for
	: FOR exprlist IN testlist_safe list_iter
	| FOR exprlist IN testlist_safe
	;
list_if
	: IF old_test list_iter
	| IF old_test
	;
comp_iter
	: comp_for
	| comp_if
	;
comp_for
	: FOR exprlist IN or_test comp_iter
	| FOR exprlist IN or_test
	;
comp_if
	: IF old_test comp_iter
	| IF old_test
	;
testlist1
	: test
	| testlist1 COMMA test
	;
yield_expr
	: YIELD testlist
	| YIELD
	;
star_DOT
	: star_DOT DOT
	| %empty
	;

%%

#include <stdio.h>

void reverseStack(std::stack<function> functionStack) 
{
	while(!functionStack.empty()) {
		std::cout << "(\"" << functionStack.top().firstLine << ":" << 
		functionStack.top().firstColumn << ": '" << functionStack.top().functionName 
		<< "'\", " << functionStack.top().complexity << ")" << std::endl;
		functionStack.pop();
	}
}



void yyerror (const char *s)
{
    if(yylloc.first_line > 0)	{
        fprintf (stderr, "%d.%d-%d.%d:", yylloc.first_line, yylloc.first_column,
	                                     yylloc.last_line,  yylloc.last_column);
    }
    fprintf(stderr, " %s with [%s]\n", s, yytext);
}

