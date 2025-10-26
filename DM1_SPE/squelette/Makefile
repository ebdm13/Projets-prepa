vector.o : vector.c vector.h
	gcc -O3 -c vector.c -o vector.o

database.o : database.c database.h vector.o
	gcc -O3 -c database.c  -o database.o

tests_section_1 : tests_section_1.c vector.o  database.o
	gcc -O3 tests_section_1.c vector.o database.o -o tests_section_1 -lm


candidats.o : candidats.c candidats.h database.h vector.h
	gcc -O3 -c candidats.c -o candidats.o

tests_section_2 : tests_section_2.c vector.o  database.o candidats.o
	gcc -O3 tests_section_2.c vector.o database.o  candidats.o -o tests_section_2 -lm

knn.o : knn.c knn.h candidats.h database.h vector.h
	gcc -O3 -c knn.c -o knn.o

jeu_quadrant.o : jeu_quadrant.c jeu_quadrant.h database.h vector.h
	gcc -O3 -c jeu_quadrant.c -o jeu_quadrant.o

tests_section_5 : tests_section_5.c database.o vector.o candidats.o knn.o jeu_quadrant.o
	gcc -O3 tests_section_5.c database.o vector.o candidats.o knn.o jeu_quadrant.o -o tests_section_5 -lm


lecture_mnist.o : lecture_mnist.c lecture_mnist.h database.h vector.h
	gcc -O3 -c lecture_mnist.c -o lecture_mnist.o


tests_section_6 : tests_section_6.c database.o vector.o candidats.o knn.o jeu_quadrant.o lecture_mnist.o
	gcc -O3 tests_section_6.c lecture_mnist.o database.o vector.o candidats.o knn.o jeu_quadrant.o -o tests_section_6 -lm


all : tests_section_1 tests_section_2  tests_section_5 tests_section_6

clean :
	rm -f *.o
	rm -f tests_section_1
	rm -f tests_section_2
	rm -f tests_section_5
	rm -f tests_section_6
