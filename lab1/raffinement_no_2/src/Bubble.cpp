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
	/*
	
	À compléter
	
	*/
	SC_METHOD(bubble);
	this->State = INIT;
	sensitive << clk.pos();
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
	/*
	
	À compléter
	
	*/
	//Tiré de https://www.geeksforgeeks.org/bubble-sort/
	switch(this->State) {
		case INIT: //Initialisation
			this->address->write(0);
			this->request->write(false);
			State = SEND_REQUEST;
			// std::cout << "Ok 1" << endl;
			break;
		case SEND_REQUEST:
			//Envoyer l’adresse à être lue
			this->address->write(4*tab_a_trier.size());
			//Envoyer une requête
			this->request->write(true);
			State = WAIT_RESPONSE;
			// std::cout << "Ok 2" << endl;
			break;
		case WAIT_RESPONSE:
			if (this->ack.read())
				State = READ_DATA;
			else
				State = WAIT_RESPONSE;
			break;
		case READ_DATA:
			this->data->read();
			this->tab_a_trier.push_back(this->data);
			State = REMOVE_REQUEST;
			break;
		case REMOVE_REQUEST:
			this->request.write(false);
			if ((tab_a_trier.size() - 1) == tab_a_trier[0]) {
				State = BUBBLE_SORT;
				std::cout << "Nombre d'éléments à trier : " << tab_a_trier[0] << std::endl;
				std::cout << "Avant le tri :" << std::endl;
				for (int k = 1; (unsigned int) k < tab_a_trier[0] + 1; k++)
					std::cout << tab_a_trier[k] << std::endl;
			}
			else
				State = SEND_REQUEST;
			break;
		case BUBBLE_SORT: {
			if((i != 1) or (j != 1))
				cout << "Bubble end " << sc_time_stamp() << endl ;
			
			cout << endl << "Bubble Begin " << sc_time_stamp() << endl ;
			if (tab_a_trier[j] > tab_a_trier[j + 1]) {
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
