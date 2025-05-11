all : exec

effets.o : effets.c effets.h
           gcc -o effets.c -o effets.o

combat.o : combat.c combat.h
           gcc -o combat.c -o combat.o

type.o :
           gcc 

