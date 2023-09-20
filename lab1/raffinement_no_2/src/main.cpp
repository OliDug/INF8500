///////////////////////////////////////////////////////////////////////////////
//
//	main.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include <systemc.h>
#include "Bubble.h"
#include "Wrapper.h"
#include "DataRAM.h"

#define RAMSIZE 0x400

// Global variables
bool m_bError = false;

///////////////////////////////////////////////////////////////////////////////
//
//	Main
//
///////////////////////////////////////////////////////////////////////////////
int sc_main(int arg_count, char **arg_value)
{
	// Variables
	int sim_units = 2; //SC_NS 

	// Clock
	sc_clock sClk( "SysClock", 4000, SC_NS, 0.5 );

	// Instanciation des composants
	Bubble iBubble("Bubble");
	Wrapper iWrapper("Wrapper");
	DataRAM iDataRAM("DataRAM", "memory/mem.hex", RAMSIZE, false);

	// Déclaration des signaux
	sc_signal<unsigned int> address;
	sc_signal<unsigned int> data;
	sc_signal<bool> request;
	sc_signal<bool> ack;

	// Connexion des ports
	iBubble.clk(sClk);
	iBubble.address(address);
	iBubble.data(data);
	iBubble.request(request);
	iBubble.ack(ack);
	iWrapper.clk(sClk);
	iWrapper.address(address);
	iWrapper.data(data);
	iWrapper.request(request);
	iWrapper.ack(ack);
	iWrapper.dataPortRAM(iDataRAM);

	// Démarrage de l'application
	if (!m_bError)
	{
		cout << "Demarrage de la simulation." << endl;
		sc_start( 20, SC_MS );
		cout << endl << "Simulation s'est terminee a " << sc_time_stamp() << endl;
	}
	// Fin du programme
	return 0;
}
