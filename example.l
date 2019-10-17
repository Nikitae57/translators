%{
#include <stdio.h>
%}
D [0-9]
L [a-zA-Z]
ASSINGMENT ":="
ID L(D|L)*

%%

%%
int main(void) {
	yylex();
	printf(yytext);
	return 0;
}