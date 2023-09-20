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
	SC_THREAD(interface);
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
//	Interface avec le module Reader: sc_thread ou sc_method
//
///////////////////////////////////////////////////////////////////////////////
void Bubble::interface(void)
{	
	// 1ere lecture: nombre d'éléments à trier
	unsigned int nbElements = readPort->Read(0);

	// Lecture des éléments à trier
	unsigned int* elementsToSort = new unsigned int(nbElements);
	for(unsigned int i = 0; i < nbElements; i++) {
		elementsToSort[i] = readPort->Read(4 + 4*i);
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
            if (ptr[j] > ptr[j + 1]) {
				int temp = ptr[j];
    			ptr[j] = ptr[j + 1];
    			ptr[j + 1] = temp;
                swapped = true;
            }
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
