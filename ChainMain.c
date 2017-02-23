#include <stdio.h>
#include "chaine.h"
#include <time.h>

int main(void){
	srand(time(NULL));
	FILE *f = fopen("burma.cha", "r");
	Chaines *maChaine = lectureChaine(f);

	afficheChaineSVG(maChaine, "RepGraph");

	fclose(f);
	return 0;

}