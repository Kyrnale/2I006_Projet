#include <stdio.h>
#include <math.h>
#include "entree_sortie.h"
#include "chaine.h"
#include "SVGwriter.h"

Chaines* lectureChaine(FILE *f){
	if(!f){
		return NULL;
	}

	Chaines *listeDeChaine = (Chaines*)malloc(sizeof(Chaines));

	char poubelle[50];
	GetChaine(f, 50, poubelle);
	listeDeChaine->nbChaines = GetEntier(f);
	GetChaine(f, 50, poubelle);

	listeDeChaine->gamma = GetEntier(f);
	listeDeChaine->chaines = NULL;

	int cpt = 0;
	int cptPoint = 0;
	CellChaine *tmp;
	CellPoint *tmpPoint;
	while(cpt < listeDeChaine->nbChaines){
		cpt++;
		if(!listeDeChaine->chaines){
			listeDeChaine->chaines = (CellChaine*)malloc(sizeof(CellChaine));
			listeDeChaine->chaines->suiv = NULL;
		}else{
			tmp = (CellChaine*)malloc(sizeof(CellChaine));
			tmp->suiv = listeDeChaine->chaines;
			listeDeChaine->chaines = tmp;
		}

		listeDeChaine->chaines->numero = GetEntier(f);
		Skip(f);
		listeDeChaine->chaines->nbPoints = GetEntier(f);
		Skip(f);

		cptPoint = 0;
		while(cptPoint < listeDeChaine->chaines->nbPoints){
			cptPoint++;

			if(!listeDeChaine->chaines->points){
				listeDeChaine->chaines->points = (CellPoint*)malloc(sizeof(CellPoint));
				listeDeChaine->chaines->points->suiv = NULL;
			}else{
				tmpPoint = (CellPoint*)malloc(sizeof(CellPoint));
				tmpPoint->suiv = listeDeChaine->chaines->points;
				listeDeChaine->chaines->points = tmpPoint;
			}

			listeDeChaine->chaines->points->x = GetReel(f);
			Skip(f);
			listeDeChaine->chaines->points->y = GetReel(f);
			Skip(f);
		}

	}

	return listeDeChaine;
}

void ecrireChaineTxt(Chaines *C, FILE *f){
	if(!C || !f){
		return;
	}

	fprintf(f, "NbChain: %d\n", C->nbChaines);
	fprintf(f, "Gamma : %d\n", C->gamma);
	fprintf(f, "\n", C->gamma);
	
	CellChaine *itChaine = C->chaines;
	CellPoint *itPoint = itChaine->points;

	while(itChaine){
		fprintf(f, "%d %d ", itChaine->numero, itChaine->nbPoints);
		itPoint = itChaine->points;
		while(itPoint){
			fprintf(f, "%.2f %.2f ",itPoint->x, itPoint->y);
			itPoint = itPoint->suiv;
		}
		fprintf(f, "\n");
		itChaine = itChaine->suiv;
	}
}

void afficheChaineSVG(Chaines *C, char* nomInstance){
	SVGwriter *monSVG = (SVGwriter*)malloc(sizeof(SVGwriter));

	

	CellChaine *itChaine = C->chaines;
	CellPoint *itPoint = itChaine->points;

	double xprec=-1, yprec=-1; //x et y du point precedant pour dessiner la ligne

	

	int minX = itPoint->x;
	int minY = itPoint->y;
	int maxX = itPoint->x;
	int maxY = itPoint->y;

	while(itChaine){
		itPoint = itChaine->points;
		while(itPoint){
			if(itPoint->x < minX){
				minX = itPoint->x;
			}
			if(itPoint->y < minY){
				minY = itPoint->y;
			}
			if(itPoint->x > maxX){
				maxX = itPoint->x;
			}
			if(itPoint->y > maxY){
				maxY = itPoint->y;
			}
			itPoint = itPoint->suiv;
		}
		itChaine=itChaine->suiv;
	}

	SVGinit(monSVG, nomInstance, maxX-minX, maxX-minX);
	SVGpointColor(monSVG, "black");


	itChaine = C->chaines;

	while(itChaine){
		itPoint = itChaine->points;
		SVGlineRandColor(monSVG);
		xprec = yprec = -1;
		while(itPoint){
			SVGpoint(monSVG, itPoint->x-minX, itPoint->y-minY);
			if(xprec!=-1 && yprec!=-1){
				SVGline(monSVG, itPoint->x-minX, itPoint->y-minY, xprec-minX, yprec-minY);
			}
			xprec = itPoint->x;
			yprec = itPoint->y;
			itPoint = itPoint->suiv;
		}
		itChaine = itChaine->suiv;
	}



	SVGfinalize(monSVG);
}

double longueurChaine(CellChaine *c){
	double distance = 0;

	double xprec;
	double yprec;

	CellPoint *itPoint;
	while(c){
		itPoint = c->points;
		xprec = -1;
		yprec = -1;
		while(itPoint){
			if(xprec!=-1 && yprec!=-1){
				distance+= sqrt(((itPoint->x - xprec)*(itPoint->x - xprec) + (itPoint->y - yprec)*(itPoint->y - yprec)));
			}
			xprec = itPoint->x;
			yprec = itPoint->y;
			itPoint = itPoint->suiv;
		}
		c = c->suiv;
	}
	return distance;
}

double longueurTotale(Chaines *c){
	double distance = 0;
	CellChaine* itChaine = c->chaines;

	while(itChaine){
		distance+=longueurChaine(itChaine);
		itChaine = itChaine->suiv;
	}

	return distance;
}