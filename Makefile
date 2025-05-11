all : exec

main.o: main.c
         gcc -o main.c -o main.o

effets.o: effets.c effets.h
           gcc -o effets.c -o effets.o

combat.o: combat.c combat.h
           gcc -o combat.c -o combat.o

JoueurVSordi.o: JoueurVSordi.c JoueurVSordi.h
                 gcc -o JoueurVSordi.c -o JoueurVSordi.o

interface.o: interface.c interface.h
              gcc -o interface.c -o interface.o

lecture.o: lecture.c lecture.h
            gcc -o lecture.c -o lecture.o



exec: main.o effets.o combat.o JoueurVSordi.o interface.o lecture.o
      gcc main.o effets.o combat.o JoueurVSordi.o interface.o lecture.o -o exec

clean: rm -f *.o exec
         






