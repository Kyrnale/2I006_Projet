all: ChainMain

chaine.o: chaine.c entree_sortie.h SVGwriter.h
	gcc -c chaine.c

entree_sortie.o: entree_sortie.c
	gcc -c entree_sortie.c

ChainMain.o: ChainMain.c entree_sortie.h
	gcc -c ChainMain.c

SVGwriter.o: SVGwriter.c
	gcc -c SVGwriter.c

reseau.o: reseau.c
	gcc -c reseau.c 


ChainMain: ChainMain.o entree_sortie.o chaine.o SVGwriter.o reseau.o
	gcc -lm -o ChainMain ChainMain.o entree_sortie.o chaine.o SVGwriter.o reseau.o


clean:
	rm -f ChainMain *.o