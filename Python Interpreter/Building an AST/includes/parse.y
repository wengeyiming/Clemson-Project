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
    
%}

%union 
{
    Node* node;
    int intNumber;
    float fltNumber;
    char *str;
    char op;
}

%type <str> NAME

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

%type <intNumber> INT
%type <fltNumber> FLOAT
%type <node> atom power factor term arith_expr testlist pick_yield_expr_testlist
star_EQUAL star_EQUAL_R shift_expr and_expr xor_expr expr comparison not_test and_test or_test opt_IF_ELSE test star_COMMA_test expr_stmt parameters star_trailer trailer opt_arglist 
arglist argument pick_argument pick_yield_expr_testlist_comp opt_yield_test testlist_comp print_stmt opt_test
%type <op>	pick_PLUS_MINUS pick_multop pick_unop augassign

%start start

%locations

%%

start
	: file_input
	;
file_input // Used in: start
	: star_NEWLINE_stmt ENDMARKER 
	  {
	     reverseStack(functionStack);
	  }
	;
pick_NEWLINE_stmt // Used in: star_NEWLINE_stmt
	: NEWLINE
	| stmt
	;
star_NEWLINE_stmt // Used in: file_input, star_NEWLINE_stmt
	: star_NEWLINE_stmt pick_NEWLINE_stmt
	| %empty
	;
decorator // Used in: decorators
	: AT dotted_name LPAR opt_arglist RPAR NEWLINE
	| AT dotted_name NEWLINE
	;
opt_arglist // Used in: decorator, trailer
	: arglist {$$ = $1;}
	| %empty {$$ = 0;}
	;
decorators // Used in: decorators, decorated
	: decorators decorator 
	| decorator 
	;
decorated // Used in: compound_stmt
	: decorators classdef
	| decorators funcdef
	;
funcdef // Used in: decorated, compound_stmt
	: DEF 
	{
		if(@1.first_column == 1) {
			func.firstLine = @1.first_line; 
			func.firstColumn = @1.first_column - 1;
			isinner = 0;
		} else {
			isinner++;
		}		
	}	
	NAME parameters COLON suite
	{
		if(isinner > 0) {
			func.complexity++;
			isinner--;
		} else {
			func.functionName = $3;
			functionStack.push(func);
			func.complexity = 1;
		}
		delete[] $3;
	}

	;
parameters // Used in: funcdef
	: LPAR varargslist RPAR {$$ = 0;}
	| LPAR RPAR {$$ = 0;}
	;
varargslist // Used in: parameters, old_lambdef, lambdef
	: star_fpdef_COMMA pick_STAR_DOUBLESTAR
	| star_fpdef_COMMA fpdef opt_EQUAL_test opt_COMMA
	;
opt_EQUAL_test // Used in: varargslist, star_fpdef_COMMA
	: EQUAL test
	| %empty
	;
star_fpdef_COMMA // Used in: varargslist, star_fpdef_COMMA
	: star_fpdef_COMMA fpdef opt_EQUAL_test COMMA
	| %empty
	;
opt_DOUBLESTAR_NAME // Used in: pick_STAR_DOUBLESTAR
	: COMMA DOUBLESTAR NAME { delete[] $3;}
	| %empty
	;
pick_STAR_DOUBLESTAR // Used in: varargslist
	: STAR NAME opt_DOUBLESTAR_NAME { delete[] $2;}
	| DOUBLESTAR NAME { delete[] $2;}
	;
opt_COMMA // Used in: varargslist, opt_test, opt_test_2, testlist_safe, listmaker, testlist_comp, pick_for_test_test, pick_for_test, pick_argument
	: COMMA
	| %empty
	;
fpdef // Used in: varargslist, star_fpdef_COMMA, fplist, star_fpdef_notest
	: NAME { delete[] $1;}
	| LPAR fplist RPAR
	;
fplist // Used in: fpdef
	: fpdef star_fpdef_notest COMMA
	| fpdef star_fpdef_notest
	;
star_fpdef_notest // Used in: fplist, star_fpdef_notest
	: star_fpdef_notest COMMA fpdef
	| %empty
	;
stmt // Used in: pick_NEWLINE_stmt, plus_stmt
	: simple_stmt
	| compound_stmt
	;
simple_stmt // Used in: stmt, suite
	: small_stmt star_SEMI_small_stmt SEMI NEWLINE
	| small_stmt star_SEMI_small_stmt NEWLINE
	;
star_SEMI_small_stmt // Used in: simple_stmt, star_SEMI_small_stmt
	: star_SEMI_small_stmt SEMI small_stmt
	| %empty
	;
small_stmt // Used in: simple_stmt, star_SEMI_small_stmt
	: expr_stmt
	| print_stmt
	| del_stmt
	| pass_stmt
	| flow_stmt
	| import_stmt
	| global_stmt
	| exec_stmt
	| assert_stmt
	;
expr_stmt // Used in: small_stmt
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
			if($2 != 0)
			{
				$$ = new AsgBinaryNode($1,$2);
				pool.add($$);
			}
		}

	;
pick_yield_expr_testlist // Used in: expr_stmt, star_EQUAL
	: yield_expr {$$ = 0;}
	| testlist {$$ = $1;}
	;
star_EQUAL_R
	: star_EQUAL 	{$$ = $1;}
	| %empty 	{$$ = 0;}
	;	
star_EQUAL // Used in: expr_stmt, star_EQUAL
	: EQUAL pick_yield_expr_testlist star_EQUAL_R
		{
			if ($3 == 0)
			{
				$$ = $2;
			}
			else
			{
				$$ = new AsgBinaryNode($2,$3);
				pool.add($$);
			}
		}
	| %empty 	{$$ = 0;}
	;
augassign // Used in: expr_stmt
	: PLUSEQUAL { $$ = '0';}
	| MINEQUAL { $$ = '1';}
	| STAREQUAL { $$ = '2';}
	| SLASHEQUAL { $$ = '3';}
	| PERCENTEQUAL { $$ = '4';}
	| AMPEREQUAL
	| VBAREQUAL
	| CIRCUMFLEXEQUAL
	| LEFTSHIFTEQUAL
	| RIGHTSHIFTEQUAL
	| DOUBLESTAREQUAL  { $$ = '5';}
	| DOUBLESLASHEQUAL { $$ = '6';}
	;
print_stmt // Used in: small_stmt
	: PRINT opt_test
	{
		$$ = $2;
		$2->eval()->print();
	}	
	| PRINT RIGHTSHIFT test opt_test_2
	;
star_COMMA_test // Used in: star_COMMA_test, opt_test, listmaker, testlist_comp, testlist, pick_for_test
	: star_COMMA_test COMMA test {$$ = $3;}
	| %empty {$$ = 0;}
	;
opt_test // Used in: print_stmt
	: test star_COMMA_test opt_COMMA
	| %empty {$$ = 0;}
	;
plus_COMMA_test // Used in: plus_COMMA_test, opt_test_2
	: plus_COMMA_test COMMA test
	| COMMA test
	;
opt_test_2 // Used in: print_stmt
	: plus_COMMA_test opt_COMMA
	| %empty
	;
del_stmt // Used in: small_stmt
	: DEL exprlist
	;
pass_stmt // Used in: small_stmt
	: PASS
	;
flow_stmt // Used in: small_stmt
	: break_stmt
	| continue_stmt
	| return_stmt
	| raise_stmt
	| yield_stmt
	;
break_stmt // Used in: flow_stmt
	: BREAK
	;
continue_stmt // Used in: flow_stmt
	: CONTINUE
	;
return_stmt // Used in: flow_stmt
	: RETURN testlist
	| RETURN
	;
yield_stmt // Used in: flow_stmt
	: yield_expr
	;
raise_stmt // Used in: flow_stmt
	: RAISE test opt_test_3 {func.complexity++;}
	| RAISE {func.complexity++;}
	;
opt_COMMA_test // Used in: opt_test_3, exec_stmt
	: COMMA test
	| %empty
	;
opt_test_3 // Used in: raise_stmt
	: COMMA test opt_COMMA_test
	| %empty
	;
import_stmt // Used in: small_stmt
	: import_name
	| import_from
	;
import_name // Used in: import_stmt
	: IMPORT dotted_as_names
	;
import_from // Used in: import_stmt
	: FROM pick_dotted_name IMPORT pick_STAR_import
	;
pick_dotted_name // Used in: import_from
	: star_DOT dotted_name
	| star_DOT DOT
	;
pick_STAR_import // Used in: import_from
	: STAR
	| LPAR import_as_names RPAR
	| import_as_names
	;
import_as_name // Used in: import_as_names, star_COMMA_import_as_name
	: NAME AS NAME { delete[] $1; delete[] $3;}
	| NAME { delete[] $1;}
	;
dotted_as_name // Used in: dotted_as_names
	: dotted_name AS NAME { delete[] $3;}
	| dotted_name
	;
import_as_names // Used in: pick_STAR_import
	: import_as_name star_COMMA_import_as_name COMMA
	| import_as_name star_COMMA_import_as_name
	;
star_COMMA_import_as_name // Used in: import_as_names, star_COMMA_import_as_name
	: star_COMMA_import_as_name COMMA import_as_name
	| %empty
	;
dotted_as_names // Used in: import_name, dotted_as_names
	: dotted_as_name
	| dotted_as_names COMMA dotted_as_name
	;
dotted_name // Used in: decorator, pick_dotted_name, dotted_as_name, dotted_name
	: NAME { delete[] $1;}
	| dotted_name DOT NAME { delete[] $3;}
	;
global_stmt // Used in: small_stmt
	: GLOBAL NAME star_COMMA_NAME { delete[] $2;}
	;
star_COMMA_NAME // Used in: global_stmt, star_COMMA_NAME
	: star_COMMA_NAME COMMA NAME { delete[] $3;}
	| %empty
	;
exec_stmt // Used in: small_stmt
	: EXEC expr IN test opt_COMMA_test
	| EXEC expr
	;
assert_stmt // Used in: small_stmt
	: ASSERT test COMMA test {func.complexity++;}
	| ASSERT test {func.complexity++;}
	;
compound_stmt // Used in: stmt
	: if_stmt
	| while_stmt  {func.complexity++;}
	| for_stmt   {func.complexity++;}
	| {temp = func.complexity;} try_stmt
	| with_stmt
	| funcdef
	| classdef
	| decorated
	;
if_stmt // Used in: compound_stmt
	: IF test COLON suite star_ELIF ELSE COLON suite {func.complexity++;}
	| IF test COLON suite star_ELIF {func.complexity++;}
	;
star_ELIF // Used in: if_stmt, star_ELIF
	: star_ELIF ELIF test COLON suite {func.complexity++;}
	| %empty
	;
while_stmt // Used in: compound_stmt
	: WHILE test COLON suite ELSE COLON suite
	| WHILE test COLON suite
	;
for_stmt // Used in: compound_stmt
	: FOR exprlist IN testlist COLON suite ELSE COLON suite
	| FOR exprlist IN testlist COLON suite
	;
try_stmt // Used in: compound_stmt
	: TRY COLON suite plus_except opt_ELSE opt_FINALLY 
	{ 
		if(!isfinal) 
			func.complexity++;  
		else 
			func.complexity = temp;  		
	}
	| TRY COLON suite FINALLY COLON suite 
	{
		func.complexity = temp;  
	}
	;
plus_except // Used in: try_stmt, plus_except
	: plus_except except_clause COLON suite
	| except_clause COLON suite
	;
opt_ELSE // Used in: try_stmt
	: ELSE COLON suite
	| %empty
	;
opt_FINALLY // Used in: try_stmt
	: FINALLY COLON suite 
	{
		isfinal = true;
	}
	| %empty { isfinal = false; }
	;
with_stmt // Used in: compound_stmt
	: WITH with_item star_COMMA_with_item COLON suite
	;
star_COMMA_with_item // Used in: with_stmt, star_COMMA_with_item
	: star_COMMA_with_item COMMA with_item
	| %empty
	;
with_item // Used in: with_stmt, star_COMMA_with_item
	: test AS expr
	| test
	;
except_clause // Used in: plus_except
	: EXCEPT test opt_AS_COMMA { func.complexity++;  }
	| EXCEPT { func.complexity++;  }
	;
pick_AS_COMMA // Used in: opt_AS_COMMA
	: AS
	| COMMA
	;
opt_AS_COMMA // Used in: except_clause
	: pick_AS_COMMA test
	| %empty
	;
suite // Used in: funcdef, if_stmt, star_ELIF, while_stmt, for_stmt, try_stmt, plus_except, opt_ELSE, opt_FINALLY, with_stmt, classdef
	: simple_stmt
	| NEWLINE INDENT plus_stmt DEDENT
	;
plus_stmt // Used in: suite, plus_stmt
	: plus_stmt stmt
	| stmt
	;
testlist_safe // Used in: list_for
	: old_test plus_COMMA_old_test opt_COMMA
	| old_test
	;
plus_COMMA_old_test // Used in: testlist_safe, plus_COMMA_old_test
	: plus_COMMA_old_test COMMA old_test
	| COMMA old_test
	;
old_test // Used in: testlist_safe, plus_COMMA_old_test, old_lambdef, list_if, comp_if
	: or_test
	| old_lambdef
	;
old_lambdef // Used in: old_test
	: LAMBDA varargslist COLON old_test
	| LAMBDA COLON old_test
	;
test // Used in: opt_EQUAL_test, print_stmt, star_COMMA_test, opt_test, plus_COMMA_test, raise_stmt, opt_COMMA_test, opt_test_3, exec_stmt, assert_stmt, if_stmt, star_ELIF, while_stmt, with_item, except_clause, opt_AS_COMMA, opt_IF_ELSE, listmaker, testlist_comp, lambdef, subscript, opt_test_only, sliceop, testlist, dictorsetmaker, star_test_COLON_test, opt_DOUBLESTAR_test, pick_argument, argument, testlist1
	: or_test opt_IF_ELSE {$$ = $1;}
	| lambdef {$$ = 0;}
	;
opt_IF_ELSE // Used in: test
	: IF or_test ELSE test {$$ = 0;}
	| %empty {$$ = 0;}
	;
or_test // Used in: old_test, test, opt_IF_ELSE, or_test, comp_for
	: and_test {$$ = $1;}
	| or_test OR and_test {$$ = 0;}
	;
and_test // Used in: or_test, and_test
	: not_test {$$ = $1;}
	| and_test AND not_test {$$ = 0;}
	;
not_test // Used in: and_test, not_test
	: NOT not_test {$$ = 0;}
	| comparison {$$ = $1;}
	;
comparison // Used in: not_test, comparison
	: expr {$$ = $1;}
	| comparison comp_op expr {$$ = 0;}
	;
comp_op // Used in: comparison
	: LESS
	| GREATER
	| EQEQUAL
	| GREATEREQUAL
	| LESSEQUAL
	| GRLT
	| NOTEQUAL
	| IN
	| NOT IN
	| IS
	| IS NOT
	;
expr // Used in: exec_stmt, with_item, comparison, expr, exprlist, star_COMMA_expr
	: xor_expr {$$ = $1;}
	| expr BAR xor_expr {$$ = 0;}
	;
xor_expr // Used in: expr, xor_expr
	: and_expr {$$ = $1;}
	| xor_expr CIRCUMFLEX and_expr {$$ = 0;}
	;
and_expr // Used in: xor_expr, and_expr
	: shift_expr {$$ = $1;}
	| and_expr AMPERSAND shift_expr {$$ = 0;}
	;
shift_expr // Used in: and_expr, shift_expr
	: arith_expr 	{ $$ = $1;}
	| shift_expr pick_LEFTSHIFT_RIGHTSHIFT arith_expr { $$ = 0;}
	;
pick_LEFTSHIFT_RIGHTSHIFT // Used in: shift_expr
	: LEFTSHIFT
	| RIGHTSHIFT
	;
arith_expr // Used in: shift_expr, arith_expr
	: term {$$ = $1;}
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
pick_PLUS_MINUS // Used in: arith_expr
	: PLUS { $$ = '+';}
	| MINUS { $$ = '-';}
	;
term // Used in: arith_expr, term
	: factor { $$ = $1;}
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
pick_multop // Used in: term
	: STAR { $$ = '*';}
	| SLASH { $$ = '/';}
	| PERCENT { $$ = '%';}
	| DOUBLESLASH { $$ = '7';}
	;
factor // Used in: term, factor, power
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
	| power { $$ = $1;}
	;
pick_unop // Used in: factor
	: PLUS { $$ = '+';}
	| MINUS { $$ = '-';}
	| TILDE 
	;
power // Used in: factor
	: atom star_trailer DOUBLESTAR factor 
	{ 
	  if($1) {
	  	if($4) {
	  		$$ = new DbStarBinaryNode($1,$4);
			pool.add($$);
	  	}
	  }
	}
	| atom star_trailer {
	 	if(!$2) {
	 		$$ = $1;
	 	}
	 }
	;
star_trailer // Used in: power, star_trailer
	: star_trailer trailer
	| %empty
	;
atom // Used in: power
	: LPAR opt_yield_test RPAR { $$ = $2;}
	| LSQB opt_listmaker RSQB { $$ = 0;}
	| LBRACE opt_dictorsetmaker RBRACE { $$ = 0;}
	| BACKQUOTE testlist1 BACKQUOTE { $$ = 0;}
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
	| plus_STRING { $$ = 0;}
	;
pick_yield_expr_testlist_comp // Used in: opt_yield_test
	: yield_expr {$$ = 0;}
	| testlist_comp {$$ = $1;}
	;
opt_yield_test // Used in: atom
	: pick_yield_expr_testlist_comp
	| %empty
	;
opt_listmaker // Used in: atom
	: listmaker
	| %empty
	;
opt_dictorsetmaker // Used in: atom
	: dictorsetmaker
	| %empty
	;
plus_STRING // Used in: atom, plus_STRING
	: plus_STRING STRING
	| STRING
	;
listmaker // Used in: opt_listmaker
	: test list_for
	| test star_COMMA_test opt_COMMA
	;
testlist_comp // Used in: pick_yield_expr_testlist_comp
	: test comp_for
	| test star_COMMA_test opt_COMMA
	;
lambdef // Used in: test
	: LAMBDA varargslist COLON test
	| LAMBDA COLON test
	;
trailer // Used in: star_trailer
	: LPAR opt_arglist RPAR {$$ = $2;}
	| LSQB subscriptlist RSQB {$$ = 0;}
	| DOT NAME { $$ = 0; delete[] $2;}
	;
subscriptlist // Used in: trailer
	: subscript star_COMMA_subscript COMMA
	| subscript star_COMMA_subscript
	;
star_COMMA_subscript // Used in: subscriptlist, star_COMMA_subscript
	: star_COMMA_subscript COMMA subscript
	| %empty
	;
subscript // Used in: subscriptlist, star_COMMA_subscript
	: DOT DOT DOT
	| test
	| opt_test_only COLON opt_test_only opt_sliceop
	;
opt_test_only // Used in: subscript
	: test
	| %empty
	;
opt_sliceop // Used in: subscript
	: sliceop
	| %empty
	;
sliceop // Used in: opt_sliceop
	: COLON test
	| COLON
	;
exprlist // Used in: del_stmt, for_stmt, list_for, comp_for
	: expr star_COMMA_expr COMMA
	| expr star_COMMA_expr
	;
star_COMMA_expr // Used in: exprlist, star_COMMA_expr
	: star_COMMA_expr COMMA expr
	| %empty
	;
testlist // Used in: expr_stmt, pick_yield_expr_testlist, return_stmt, for_stmt, opt_testlist, yield_expr
	: test star_COMMA_test COMMA { $$ = $1;}
	| test star_COMMA_test { $$ = $1;}
	;
dictorsetmaker // Used in: opt_dictorsetmaker
	: test COLON test pick_for_test_test
	| test pick_for_test
	;
star_test_COLON_test // Used in: star_test_COLON_test, pick_for_test_test
	: star_test_COLON_test COMMA test COLON test
	| %empty
	;
pick_for_test_test // Used in: dictorsetmaker
	: comp_for
	| star_test_COLON_test opt_COMMA
	;
pick_for_test // Used in: dictorsetmaker
	: comp_for
	| star_COMMA_test opt_COMMA
	;
classdef // Used in: decorated, compound_stmt
	: CLASS NAME LPAR opt_testlist RPAR COLON suite {delete[] $2;}
	| CLASS NAME COLON suite { delete[] $2;}
	;
opt_testlist // Used in: classdef
	: testlist
	| %empty
	;
arglist // Used in: opt_arglist
	: star_argument_COMMA pick_argument {$$ = $2;}
	;
star_argument_COMMA // Used in: arglist, star_argument_COMMA
	: star_argument_COMMA argument COMMA
	| %empty
	;
star_COMMA_argument // Used in: star_COMMA_argument, pick_argument
	: star_COMMA_argument COMMA argument
	| %empty
	;
opt_DOUBLESTAR_test // Used in: pick_argument
	: COMMA DOUBLESTAR test
	| %empty
	;
pick_argument // Used in: arglist
	: argument opt_COMMA
	| STAR test star_COMMA_argument opt_DOUBLESTAR_test
	| DOUBLESTAR test
	;
argument // Used in: star_argument_COMMA, star_COMMA_argument, pick_argument
	: test opt_comp_for
	| test EQUAL test
	;
opt_comp_for // Used in: argument
	: comp_for
	| %empty
	;
list_iter // Used in: list_for, list_if
	: list_for
	| list_if
	;
list_for // Used in: listmaker, list_iter
	: FOR exprlist IN testlist_safe list_iter
	| FOR exprlist IN testlist_safe
	;
list_if // Used in: list_iter
	: IF old_test list_iter
	| IF old_test
	;
comp_iter // Used in: comp_for, comp_if
	: comp_for
	| comp_if
	;
comp_for // Used in: testlist_comp, pick_for_test_test, pick_for_test, opt_comp_for, comp_iter
	: FOR exprlist IN or_test comp_iter
	| FOR exprlist IN or_test
	;
comp_if // Used in: comp_iter
	: IF old_test comp_iter
	| IF old_test
	;
testlist1 // Used in: atom, testlist1
	: test
	| testlist1 COMMA test
	;
yield_expr // Used in: pick_yield_expr_testlist, yield_stmt, pick_yield_expr_testlist_comp
	: YIELD testlist
	| YIELD
	;
star_DOT // Used in: pick_dotted_name, star_DOT
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

