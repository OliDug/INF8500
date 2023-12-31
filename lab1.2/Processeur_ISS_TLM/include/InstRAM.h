///////////////////////////////////////////////////////////////////////////////
//
//	InstRAM.h
//
///////////////////////////////////////////////////////////////////////////////
#ifndef INSTRAM_H_
#define INSTRAM_H_

#include <systemc.h>
#include "RAM.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Classe InstRAM
//
///////////////////////////////////////////////////////////////////////////////
class InstRAM : public RAM
{
	public: 

		// Constructeur
		InstRAM( sc_module_name zName, const char *zBinaryFileName, unsigned long ulRAMSize = 0, bool bVerbose = false );
		
		// Destructeur
		~InstRAM();

		// Méthode
		unsigned int Read(unsigned int uiAddr);
		void Write(unsigned int uiAddr, unsigned int uiData);

	private:	

		// Variable
};

#endif
