///////////////////////////////////////////////////////////////////////////////
//
//	Bubble.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "Bubble.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
Bubble::Bubble( sc_module_name zName )
: sc_module(zName)
{
	SC_THREAD(interface) // Pour utiliser des wait
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
Bubble::~Bubble()
{
}


///////////////////////////////////////////////////////////////////////////////
//
//	Interface avec le module Reader
//
///////////////////////////////////////////////////////////////////////////////
void Bubble::interface(void)
{
	// Initialisation du handshaking au démarrage du thread
	address->write(0);
	request->write(false);
	wait(clk->posedge_event());

	// 1ere lecture: nombre d'éléments à trier
	// Envoyer l'adresse à lire
	address->write(0);
	// Envoyer une requête
	request->write(true);
	// Attendre un acknowledgement
	do {
		wait(clk->posedge_event());
	} while (!ack.read());
	// Lire la donnée reçue
	unsigned int nbElements = data->read();
	// Enlever la requête
	request->write(false);

	// Lecture des éléments à trier
	unsigned int* elementsToSort = new unsigned int(nbElements); //Les éléments sont stockés dans un tableau
	for(unsigned int i = 0; i < nbElements; i++) {
		address->write(4 + 4*i);
		request->write(true);
		do {
			wait(clk->posedge_event());
		} while (!ack.read());
		elementsToSort[i] = data->read();
		request->write(false);
	}

	//Appel à bubble sort
	bubbleSort(elementsToSort, nbElements);
		
	// Arrêt de la simulation
	sc_stop();
}


///////////////////////////////////////////////////////////////////////////////
//
//	bubbleSort
//
///////////////////////////////////////////////////////////////////////////////
void Bubble::bubbleSort(unsigned int *ptr, int counter)
{
	std::cout << std::endl << "Bubble Begin " << sc_time_stamp() << endl ;
	// Affichage avant tri
	std::cout << "Avant le tri" << std::endl;
	for(int i = 0; i < counter; i++) {
		std::cout << ptr[i] << std::endl;
	}

	// Tri
	//Source: https://www.geeksforgeeks.org/bubble-sort/
    bool swapped;
    for (int i = 0; i < counter - 1; i++) {
        swapped = false;
        for (int j = 0; j < counter - i - 1; j++) {
			std::cout << std::endl << "Bubble Begin " << sc_time_stamp() << std::endl ;
            if (ptr[j] > ptr[j + 1]) { //Echange de deux éléments s'ils sont dans l'ordre décroissant
				int temp = ptr[j];
    			ptr[j] = ptr[j + 1];
    			ptr[j + 1] = temp;
                swapped = true;
            }
			wait(clk->posedge_event());
			std::cout << "Bubble end " << sc_time_stamp() << std::endl ;
        }
        if (swapped == false)
            break;
    }

	// Affichage après tri
	std::cout << "Après le tri" << std::endl;
	for(int i = 0; i < counter; i++) {
		std::cout << ptr[i] << std::endl;
	}
}
