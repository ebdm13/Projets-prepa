default:
	gcc main.c sound.c waveform.c melody.c -o prog -lm
debug1:
	gcc -Wall -Wextra -pedantic -g -O0 -o ../archive/question_19/prog main.c sound.c waveform.c melody.c -lm
compile:
	gcc token.c lexer.c error.c AST.c -o compiler

debug2:
	gcc -Wall -Wextra -pedantic -g -O0 -o compiler token.c lexer.c error.c AST.c
