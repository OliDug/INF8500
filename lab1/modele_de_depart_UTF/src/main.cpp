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

	// Instanciation des composants
	Bubble iBubble("Bubble");
	Wrapper iWrapper("Wrapper");
	DataRAM iDataRAM("DataRAM", "memory/mem.hex", RAMSIZE, false);

	// Connexion des ports
	iWrapper.dataPortRAM(iDataRAM);
	iBubble.readPort(iWrapper);

	// Démarrage de l'application
	if (!m_bError)
	{
		cout << "Demarrage de la simulation." << endl;
		sc_start(20, SC_MS);		
		cout << endl << "Simulation s'est terminee a " << sc_time_stamp() << " ns" << endl;
	}
	// Fin du programme
	return 0;
}
