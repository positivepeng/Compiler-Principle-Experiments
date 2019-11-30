parse : lex.yy.c project3.tab.c tree.c symbol.c IRcode.c AScode.c
	gcc lex.yy.c project3.tab.c tree.c symbol.c IRcode.c AScode.c -o parse

lex.yy.c : project3.l
	flex project3.l

project3.tab.c : project3.y
	bison -d project3.y