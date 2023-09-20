///////////////////////////////////////////////////////////////////////////////
//
//	Bubble.h
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _BUBBLE_H_
#define _BUBBLE_H_

#include <systemc.h>
#include <stdio.h>
#include "InterfaceRead.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
//	Class Bubble
//
///////////////////////////////////////////////////////////////////////////////
class Bubble : public sc_module
{
	public: 
		// Ports    
		sc_in_clk				clk;  
		sc_out<unsigned int>	address;
		sc_in<unsigned int>		data;
		sc_out<bool>			request;
		sc_in<bool>				ack;
	
		// Constructor
		Bubble( sc_module_name zName );
		
		// Destructor
		~Bubble();
		
	private:
		// Process SystemC
		SC_HAS_PROCESS(Bubble);
		
		void interface(void);
		void bubbleSort(unsigned int *ptr, int counter);
};

#endif
