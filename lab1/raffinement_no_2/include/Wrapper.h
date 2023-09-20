///////////////////////////////////////////////////////////////////////////////
//
//	Wrapper.h
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <systemc.h>
#include "LMBIF.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Class Wrapper
//
///////////////////////////////////////////////////////////////////////////////
class Wrapper : public sc_module
{
	public:
		// Ports
		// Between Wrapper and DataRAM
		sc_port<LMBIF>			dataPortRAM;
		// Between Wrapper and Bubble
		sc_in_clk				clk;
		sc_out<unsigned int>	data;
		sc_in<unsigned int>		address;
		sc_out<bool>			ack;
		sc_in<bool>				request;
		
		// Constructor
		Wrapper( sc_module_name zName );

		// Destructor
		~Wrapper();

	private:
		// Process SystemC
		SC_HAS_PROCESS(Wrapper);
	
		void interface(void);
};

#endif
