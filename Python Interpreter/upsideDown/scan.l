%{
// Flex handles ambiguity by matching longest string;
// in case of tie, it matches the first one.

#include <iostream>
#include <string>

std::string s = "";
%}
letter      [a-zA-Z]
blank       [ ]
number      [0-9]


%%
{blank}      { s = " " + s;               }
a            { s = u8"\u0250" + s;	      }
b            { s = u8"\u0071" + s;	      }
c            { s = u8"\u0254" + s;	      }
d            { s = u8"\u0070" + s;	      }
e            { s = u8"\u01DD" + s;	      }
f            { s = u8"\u025F" + s;	      }
g            { s = u8"\u0253" + s;	      }
h            { s = u8"\u0265" + s;	      }
i            { s = u8"\u0131" + s;	      }
j            { s = u8"\u027E" + s;	      }
k            { s = u8"\u029E" + s;	      }
l            { s = u8"\u006C" + s;	      }
m            { s = u8"\u026F" + s;	      }
n            { s = u8"\u0075" + s;	      }
o            { s = u8"\u006F" + s;	      }
p            { s = u8"\u0064" + s;	      }
q            { s = u8"\u0062" + s;	      }
r            { s = u8"\u0279" + s;	      }
s            { s = u8"\u0073" + s;	      }
t            { s = u8"\u0287" + s;	      }
u            { s = u8"\u006E" + s;	      }
v            { s = u8"\u028C" + s;	      }
w            { s = u8"\u028D" + s;	      }
x            { s = u8"\u0078" + s;	      }
y            { s = u8"\u028E" + s;	      }
z            { s = u8"\u007A" + s;	      }
A            { s = u8"\u2200" + s;	      }
B            { s = u8"\u0412" + s;	      }
C            { s = u8"\u0186" + s;	      }
D            { s = u8"\u15E1" + s;	      }
E            { s = u8"\u018E" + s;	      }
F            { s = u8"\u2132" + s;	      }
G            { s = u8"\u2141" + s;	      }
H            { s = u8"\u0048" + s;	      }
I            { s = u8"\u0049" + s;	      }
J            { s = u8"\u017F" + s;	      }
K            { s = u8"\u22CA" + s;	      }
L            { s = u8"\u2142" + s;	      }
M            { s = u8"\u0057" + s;	      }
N            { s = u8"\u004E" + s;	      }
O            { s = u8"\u004F" + s;	      }
P            { s = u8"\u0500" + s;	      }
Q            { s = u8"\u038C" + s;	      }
R            { s = u8"\u1D1A" + s;	      }
S            { s = u8"\u0053" + s;	      }
T            { s = u8"\u22A5" + s;	      }
U            { s = u8"\u2229" + s;	      }
V            { s = u8"\u039B" + s;	      }
W            { s = u8"\u004D" + s;	      }
X            { s = u8"\u0058" + s;	      }
Y            { s = u8"\u2144" + s;	      }
Z            { s = u8"\u005A" + s;	      }
1            { s = u8"\u21C2" + s;	      }
2            { s = u8"\u218A" + s;	      }
3            { s = u8"\u218B" + s;	      }
4            { s = u8"\u07C8" + s;	      }
5            { s = u8"\u03DA" + s;	      }
6            { s = u8"\u0039" + s;	      }
7            { s = u8"\u3125" + s;	      }
8            { s = u8"\u0038" + s;	      }
9            { s = u8"\u0036" + s;	      }
0            { s = u8"\u0030" + s;	      }
\&           { s = u8"\u214B" + s;	      }
\_           { s = u8"\u203E" + s;	      }
\?           { s = u8"\u00BF" + s;	      }
\!           { s = u8"\u00A1" + s;	      }
\"           { s = u8"\u201E" + s;	      }
\'           { s = u8"\u002C" + s;	      }
\;           { s = u8"\u061B" + s;	      }
\'           { s = u8"\u0027" + s;	      }
\.           { s = u8"\u02D9" + s;	      }
\n           { std::cout << s << std::endl; 
				s = "";}
%%






int yywrap() { 
	yylex_destroy();
	return 1; 
}

