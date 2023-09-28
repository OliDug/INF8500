///////////////////////////////////////////////////////////////////////////////
//
//	SimpleBusLT.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SIMPLEBUSLT_H__
#define __SIMPLEBUSLT_H__

#include "busLT_standalone_definition.h"
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <systemc.h>
#include <stdio.h>

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>

//Class SimpleBusLT
class SimpleBusLT : public sc_core::sc_module
{
	typedef tlm_utils::simple_target_socket_tagged<SimpleBusLT>		target_socket_type;
	typedef tlm_utils::simple_initiator_socket_tagged<SimpleBusLT>	initiator_socket_type;
	
	public:
		target_socket_type target_socket[NR_OF_INITIATORS];
		initiator_socket_type initiator_socket[NR_OF_TARGETS];

		SC_HAS_PROCESS(SimpleBusLT);	
		//	Constructeur
		SimpleBusLT(sc_core::sc_module_name name) : sc_module(name)
		{
			for (unsigned int i = 0; i < NR_OF_INITIATORS; ++i) {
			   target_socket[i].register_b_transport( this, &SimpleBusLT::initiatorBTransport,i);
			}
		}
		
		//	Decode
		unsigned int decode(const sc_dt::uint64& address)
		{
			// decode address:
			// return initiator socket id
			if(address >= ADRESSE_CONSOLE_DEBUT && address <= ADRESSE_CONSOLE_FIN ){ 
				return  CONSOLE_INITIATOR_ID;
			}
			else if(address >= ADRESSE_COPROCESSOR_DEBUT && address <= ADRESSE_COPROCESSOR_FIN){
				return COPROCESSOR_INITIATOR_ID;	
			}
			else{
				SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");
				return 0;
			}

		}

		//	Interface method(LT protocol)
		//	Forward each call to the target/initiator
		void initiatorBTransport(int SocketId, transaction_type& trans, sc_time& t)
		{	
			initiator_socket_type* decodeSocket;

			unsigned int portId = decode(trans.get_address());
			
			assert(portId < NR_OF_TARGETS);
			decodeSocket = &initiator_socket[portId];
			trans.set_address(trans.get_address());

			(*decodeSocket)->b_transport(trans, t);
		}

};

#endif
