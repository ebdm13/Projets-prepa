default:
	gcc main.c sound.c waveform.c melody.c wav.c -o prog -lm
debug1:
	gcc -Wall -Wextra -pedantic -g -O0 -o ../archive/question_19/prog main.c sound.c waveform.c melody.c wav.c -lm
compile:
	gcc token.c lexer.c error.c AST.c parser.c renderer.c compiler.c sound.c waveform.c melody.c wav.c -o compiler -lm

debug2:
	gcc -Wall -Wextra -pedantic -g -O0 -o compiler token.c lexer.c error.c AST.c parser.c renderer.c compiler.c sound.c waveform.c melody.c wav.c -lm
test:
	gcc -o tests error.c token.c AST.c lexer.c parser.c TEST/main.c TEST/test_lexer.c TEST/test_parser.c TEST/test_AST.c TEST/test_token.c -Wall -Wextra
