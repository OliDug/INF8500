///////////////////////////////////////////////////////////////////////////////
//
//	Bubble.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "Bubble.h"
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
Bubble::Bubble( sc_module_name zName )
: sc_module(zName)
{
	SC_METHOD(bubble);		//Nous n'avons plus besoin des wait
	this->State = INIT;		//Etat initial
	sensitive << clk.pos();	//Sensibilité sur l'horloge
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
//	Bubble: communication + traitement dans une machine à états
//
///////////////////////////////////////////////////////////////////////////////
void Bubble::bubble(void)
{
	//Tiré de https://www.geeksforgeeks.org/bubble-sort/
	switch(this->State) {
		//Initialisation
		case INIT:
			this->address->write(0);
			this->request->write(false);
			State = SEND_REQUEST;
			break;

		//Envoi de la requête
		case SEND_REQUEST:
			//Envoyer l’adresse à être lue
			this->address->write(4*tab_a_trier.size());
			//Envoyer une requête
			this->request->write(true);
			State = WAIT_RESPONSE;
			break;

		//Attente acknoledgement
		case WAIT_RESPONSE:
			if (this->ack.read())
				State = READ_DATA;
			else
				State = WAIT_RESPONSE;
			break;

		//Lecture donnée
		case READ_DATA:
			this->data->read();
			this->tab_a_trier.push_back(this->data);
			State = REMOVE_REQUEST;
			break;

		//La requête est retirée
		case REMOVE_REQUEST:
			this->request.write(false);
			if ((tab_a_trier.size() - 1) == tab_a_trier[0]) {
				State = BUBBLE_SORT;
				std::cout << "Nombre d'éléments à trier : " << tab_a_trier[0] << std::endl;
				//Affichage avant tri
				std::cout << "Avant le tri :" << std::endl;
				for (int k = 1; (unsigned int) k < tab_a_trier[0] + 1; k++)
					std::cout << tab_a_trier[k] << std::endl;
			}
			else
				State = SEND_REQUEST;
			break;

		//Tri
		//Source: https://www.geeksforgeeks.org/bubble-sort/
		case BUBBLE_SORT: {
			if((i != 1) or (j != 1))
				cout << "Bubble end " << sc_time_stamp() << endl ;
			
			cout << endl << "Bubble Begin " << sc_time_stamp() << endl ;
			if (tab_a_trier[j] > tab_a_trier[j + 1]) { //Echange de deux éléments s'ils sont dans l'ordre décroissant
                int temp = tab_a_trier[j];
				tab_a_trier[j] = tab_a_trier[j+1];
				tab_a_trier[j+1] = temp;
                swapped = true;
			}

			if((unsigned int) i == this->tab_a_trier[0]) {
				std::cout << "Après le tri :" << std::endl;
				for (int k = 1; (unsigned int) k < tab_a_trier[0] + 1; k++)
					std::cout << tab_a_trier[k] << std::endl;
				sc_stop();
			}
			else {
				if((unsigned int) j == tab_a_trier[0] - i) {
					if (swapped == false) {
						std::cout << "Après le tri :" << std::endl;
						for (int k = 1; (unsigned int) k < tab_a_trier[0] + 1; k++)
							std::cout << tab_a_trier[k] << std::endl;
            			sc_stop();
					}
					i++;
					j = 1;
					swapped = false;
				}
				else
					j++;
			}
		}
	}
}
