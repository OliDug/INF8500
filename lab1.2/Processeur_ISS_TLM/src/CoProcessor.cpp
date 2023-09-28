///////////////////////////////////////////////////////////////////////////////
//
//	CoProcessor.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "CoProcessor.h"
#include <iostream>
#include <vector>
using namespace std;


///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
CoProcessor::CoProcessor( sc_module_name zName )
: sc_module(zName)
{
	SC_THREAD(thread);
//	sensitive << ClockPort.pos();
		
	reg[0] = 0x0000;
	reg[1] = 0x0000;
	reg[2] = 0x0000;
	reg[3] = 0x0000;
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
CoProcessor::~CoProcessor()
{
}


///////////////////////////////////////////////////////////////////////////////
//
//	thread
//
///////////////////////////////////////////////////////////////////////////////
void CoProcessor::thread(void)
{
	// Variables locales
	unsigned int uiAddress; 
	int nbElems; // Current number of element readed
	unsigned int* pElems; // Elements to sort
	int readElemCounter; // Counter for processor's read at address 8196

	cout << "\n/!\\ De nombreux std::cout sont disponibles dans CoProcessor.cpp afin de mieux"
			<< " comprendre le fonctionnement de ce dernier. Pour des raisons de présentation" << endl
			<< " sur le code rendu, ils ont été commentés. Il est cependant très" 
			<< " simple de les dé-commenter en cherchant //cout à l'aide de CTRL+F. /!\\\n" << endl;

	// Boucle infinie
	while(1)
	{
	    // On attend une transaction et on s'assure qu'une donnée est valide (enable)
		do {
			wait(CoProcessor_Enable_InPort.default_event());
		} while(CoProcessor_Enable_InPort.read() == false);

		//Lecture adresse
		uiAddress = CoProcessor_Data_InPort.read();	
		
		// Print some information
		//cout << "CoProcessor - uiAddress: " << uiAddress << endl;;

		if (CoProcessor_RW_InPort.read() == false) {
			/* Write (du point de vue du processeur) */
			// Wake up wrapper with ready signal
			CoProcessor_Ready_OutPort.write(true);

			// Wait coprocessor to send data
			wait(CoProcessor_Enable_InPort.default_event());

			// Read data
			switch(uiAddress) {
				case 8192:
					// Lecture et stockage du nombre d'éléments à trier
					this->reg[0] = CoProcessor_Data_InPort.read();

					// Print some information
					//cout << "CoProcessor - Write number of elements: " << this->reg[0] << endl;

					// Reset elements
					pElems = new unsigned int[this->reg[0]];
					nbElems = 0;
					readElemCounter = 0;

					// Change state
					this->reg[2] = 0;

					break;

				case 8193:
					// Lecture et stockage des éléments à trier
					pElems[nbElems] = CoProcessor_Data_InPort.read();

					// Print some information
					//cout << "CoProcessor - Write element: " << pElems[nbElems] << " at position " << nbElems << endl;

					// Update values
					nbElems++;

					break;

				default:
					cout << "COPROC: Write Address not supported" << endl;
					sc_stop();
					break;
			}
			
			// Wake up wrapper with ready signal again
			CoProcessor_Ready_OutPort.write(false);	

			// Tri (Appel à BubbleSort)
			if(nbElems > 0 && this->reg[2] == 0 && this->reg[0] == nbElems) {
				// Switch context to let wrapper finish TLM2 transaction
//				wait(1);
				// Print some information
				//cout << "CoProcessor - Start sorting!" << endl;
				// Set state to 1
				this->reg[2] = 1;
				// Launch sort
				this->bubbleSort(pElems, nbElems);
				// The call to bubbleSort set this->reg[2] to 2.
			}
			
		} else { /* CoProcessor_RW_InPort.read() == true */
			/* Read (du point de vue du processeur) */
			// Lecture des élément triés
			switch(uiAddress) {
				case 8194:
					// Lecture du registre d'état du coprocesseur
					//cout << "CoProcessor - Read state: " << this->reg[2] << endl;
					// Write value to Data_Outport
					CoProcessor_Data_OutPort.write(this->reg[2]);
					break;
				case 8195:
					// Lecture du nombre d'éléments à envoyer au processeur (check table)
					//cout << "CoProcessor - Read table size: " << this->reg[0] << endl;
					// Write value to Data_Outport
					CoProcessor_Data_OutPort.write(this->reg[0]);
					break;
				case 8196:
					// Lecture des élément triés
					//cout << "CoProcessor - Read sorted elements: " << pElems[0] << endl;
					// Write value to Data_Outport
					CoProcessor_Data_OutPort.write(pElems[readElemCounter]);
					// Circular increment for more than one read
					readElemCounter = (readElemCounter + 1) % nbElems; 
					break;
				default :
					cout << "COPROC: Read Address not supported" << endl;
					sc_stop();
					break;
			}

			// Wake up wrapper with ready signal
			CoProcessor_Ready_OutPort.write(false);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//	bubbleSort
//
///////////////////////////////////////////////////////////////////////////////
void CoProcessor::bubbleSort(unsigned int *ptr, int n_elements)
{
	/* UTF style bubbleSort (no delay with wait) */
	// Variables
	int i, j;
	bool changed;

	// Affichage avant tri
	cout << "CoProcessor - Non-sorted elements: " << endl;
	for (i = 0; i < n_elements; i++) {
		cout << ptr[i] << endl;
	}
	cout << endl;

	// Tri
	for (i = 0; i < n_elements - 1; i++) {
		changed = false;
		for (j = 0; j < n_elements - i - 1; j++) {
			if (ptr[j] > ptr[j + 1]) {
				changed = true;
				int temp = ptr[j];
				ptr[j] = ptr[j + 1];
				ptr[j + 1] = temp;
			}
		}

		// Simple speed improvement on bubble sort
		if (!changed)
			break;
	}

	// Affichage après tri
	cout << "CoProcessor - Sorted elements: " << endl;
	for (i = 0; i < n_elements; i++) {
		cout << ptr[i] << endl;
	}

	// Set state to finish
	this->reg[2] = 2;
}