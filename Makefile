all : exec

main.o: main.c
         gcc -c main.c -o main.o

effets.o: effets.c effets.h
         gcc -c effets.c -o effets.o

combat.o: combat.c combat.h
         gcc -c combat.c -o combat.o

JoueurVSordi.o: JoueurVSordi.c JoueurVSordi.h
         gcc -c JoueurVSordi.c -o JoueurVSordi.o

interface.o: interface.c interface.h
         gcc -c interface.c -o interface.o

lecture.o: lecture.c lecture.h
         gcc -c lecture.c -o lecture.o



exec: main.o effets.o combat.o JoueurVSordi.o interface.o lecture.o
         gcc main.o effets.o combat.o JoueurVSordi.o interface.o lecture.o -o exec

clean:
         rm -f *.o exec
         






