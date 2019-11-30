parse : lex.yy.c project3.tab.c
	gcc -w lex.yy.c project3.tab.c -o parse && ./parse test.c

lex.yy.c : project3.l
	flex project3.l

project3.tab.c : project3.y
	bison -d project3.y