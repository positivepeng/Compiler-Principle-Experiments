scanner : lex.yy.c project2.tab.c tree.c
	gcc lex.yy.c project2.tab.c tree.c -o scanner
lex.yy.c : project2.l
	flex project2.l
project2.tab.c : project2.y
	bison -d project2.y