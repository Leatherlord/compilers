all: clean
	gcc -c -o ast.o ast.c
	flex -o lex.yy.c lexer.flex
	bison -d -o parser.tab.c parser.y
	gcc -o main parser.tab.c lex.yy.c ast.o

clean:
	rm -f *.yy.c *.tab.* *.o main
