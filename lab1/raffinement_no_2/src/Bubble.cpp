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
	SC_METHOD(bubble);
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
	switch(state) {
		case INIT:
			address->write(0);
			request->write(false);
			state = SEND_REQUEST;
			break;

		case SEND_REQUEST:
			address->write(4*readElements.size());
			request->write(true);
			state = WAIT_RESPONSE;
			break;

		case WAIT_RESPONSE:
			if (ack.read())
				state = READ_DATA;
			else
				state = WAIT_RESPONSE;
			break;

		case READ_DATA:
			readElements.push_back(data->read());
			state = REMOVE_REQUEST;
			break;

		case REMOVE_REQUEST:
			request->write(false);
			if (readElements.size()-1 == readElements[0]) {
				state = BUBBLESORT;
				// Affichage avant tri
				std::cout << "Avant le tri" << std::endl;
				for(unsigned int k = 0; k < readElements[0]; k++) {
					std::cout << readElements[k+1] << std::endl;
				}
			}
			else
				state = SEND_REQUEST;
			break;

		case BUBBLESORT:
			std::cout << std::endl << "Bubble Begin " << sc_time_stamp() << std::endl ;
			if (i == readElements[0]) {
				// Affichage après tri
				std::cout << "Après le tri" << std::endl;
				for(unsigned int k = 0; k < readElements[0]; k++) {
					std::cout << readElements[k+1] << std::endl;
				}
				sc_stop();
			}
			if (j < readElements[0] - i){
				if (readElements[j] > readElements[j + 1]) {
					int temp = readElements[j];
					readElements[j] = readElements[j + 1];
					readElements[j + 1] = temp;
					swapped = true;
				}
				j++;
			}
			else {
				if (swapped == false) {
					// Affichage après tri
					std::cout << "Après le tri" << std::endl;
					for(unsigned int k = 0; k < readElements[0]; k++) {
						std::cout << readElements[k+1] << std::endl;
					}
					sc_stop();
				}
				i++;
				j = 1;
				swapped = false;
			}
			std::cout << "Bubble end " << sc_time_stamp() << std::endl ;

		default:
			break;
	}
}
