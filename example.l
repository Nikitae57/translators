%{
#include <stdio.h>
%}
D [0-9]
L [a-zA-Z]
ID {L}({D}|{L})*
SPACE " "
ASSINGMENT {SPACE}*{ID}{SPACE}*:={SPACE}*({ID}|{D}){SPACE}*
%%
{ID} { printf("ID"); }
{ASSINGMENT} { printf("ASSINGMENT"); }
%%
int main(void) {
	yylex();
	return 0;
}
