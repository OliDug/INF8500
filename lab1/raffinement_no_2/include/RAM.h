///////////////////////////////////////////////////////////////////////////////
//
//	RAM.h
//
///////////////////////////////////////////////////////////////////////////////
#ifndef RAM_H_
#define RAM_H_

#include <systemc.h>
#include "LMBIF.h"

// Variable externe
extern bool m_bError;

///////////////////////////////////////////////////////////////////////////////
//
//	Classe RAM
//
///////////////////////////////////////////////////////////////////////////////
class RAM : public sc_module, public LMBIF
{
	public: 

		// Constructeur
		RAM( sc_module_name zName, const char *zFileName, unsigned long ulRAMSize = 0, bool bVerbose = false );
		
		// Destructeur
		~RAM();

		// Méthode
		virtual unsigned int Read(unsigned int uiAddr);
		virtual void Write(unsigned int uiAddr, unsigned int uiData)	= 0;

	private:	

		// Variable 
		
	protected:
	
		// Variable 
		void* m_ptrRAM;
		unsigned long m_ulSize;
		unsigned long m_ulRAMSize;
		bool m_bVerbose;
};

#endif
