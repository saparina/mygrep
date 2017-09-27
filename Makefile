all: mygrep
mygrep: main.o lex_analyzer.o syn_analyzer.o fit_to_reg.o
	g++ main.o lex_analyzer.o fit_to_reg.o  syn_analyzer.o -o mygrep
fit_to_reg.o: fit_to_reg.cpp fit_to_reg.h lex_analyzer.h
	g++ -c fit_to_reg.cpp -O2 -Wall -Werror -std=gnu++14 -fpermissive
main.o: main.cpp syn_analyzer.h fit_to_reg.h
	g++ -c main.cpp -O2 -Wall -Werror -std=gnu++14 -fpermissive
syn_analyzer.o: syn_analyzer.cpp syn_analyzer.h lex_analyzer.h
	g++ -c syn_analyzer.cpp -O2 -Wall -Werror -std=gnu++14 -fpermissive
lex_analyzer.o: lex_analyzer.cpp lex_analyzer.h
	g++ -c lex_analyzer.cpp -O2 -Wall -Werror -std=gnu++14 -fpermissive
clean:
	rm -f *.o genrand
