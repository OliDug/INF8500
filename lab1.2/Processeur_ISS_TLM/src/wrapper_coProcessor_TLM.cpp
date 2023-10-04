///////////////////////////////////////////////////////////////////////////////
//
//	wrapper_coProcessor_TLM.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "wrapper_coProcessor_TLM.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
wrapper_coProcessor_TLM::wrapper_coProcessor_TLM(sc_module_name zName, unsigned long ulStartAdress, unsigned long ulEndAdress)
: sc_module(zName)
{
	//Assigne les variables
	m_ulStartAdress = ulStartAdress;
	m_ulEndAdress = ulEndAdress;
	socket.register_b_transport(this, &wrapper_coProcessor_TLM::b_transport);
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
wrapper_coProcessor_TLM::~wrapper_coProcessor_TLM()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//	b_transport : répond aux requÊtes envoyées par le initiator
//
///////////////////////////////////////////////////////////////////////////////
void wrapper_coProcessor_TLM::b_transport( transaction_type& trans, sc_time& delay )
{
	command_type cmd = trans.get_command();
	sc_dt::uint64    add = trans.get_address();
	unsigned char*   ptr = trans.get_data_ptr();
	unsigned int     len = trans.get_data_length();
	unsigned char*   byt = trans.get_byte_enable_ptr();
	unsigned int     wid = trans.get_streaming_width();
	sc_time dly = sc_time(50, SC_NS);

	//Vérification 
	if (byt != 0 || wid < len)
	  SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");
	
	// On vérifie si la requête est de lecture ou écriture
	if ( cmd == tlm::TLM_READ_COMMAND ){
		busLT_slave_read(add, ptr, len);
	}
	else if ( cmd == tlm::TLM_WRITE_COMMAND ){
		busLT_slave_write(add, ptr, len);
	}
	
	delay += dly;
	// Aquittement
	trans.set_response_status( tlm::TLM_OK_RESPONSE );
	
}
///////////////////////////////////////////////////////////////////////////////
//
//	busLT_slave_read
//
///////////////////////////////////////////////////////////////////////////////
void wrapper_coProcessor_TLM::busLT_slave_read(sc_dt::uint64 add, unsigned char* ptrData, unsigned int len)
{
	unsigned long data ;
	
// Code a completer
	// On envoi l'adresse de lecture au copro
	Wrapper_Data_OutPort.write( (int)add );

	// On indique que la transaction est une lecture
	Wrapper_RW_OutPort.write(true);

	// On indique que la donnée est prête
	Wrapper_Enable_OutPort.write( true );

	wait(Wrapper_Ready_InPort.default_event());

	// On indique que la donnée n'est plus prête
	Wrapper_Enable_OutPort.write( false );

	// On récupère la donnée
	data = Wrapper_Data_InPort.read();
	memcpy(ptrData, &data, len);
}


///////////////////////////////////////////////////////////////////////////////
//
//	opb_slave_write
//
///////////////////////////////////////////////////////////////////////////////
void wrapper_coProcessor_TLM::busLT_slave_write(sc_dt::uint64 add, unsigned char* ptrData, unsigned int     len)
{
	unsigned data ;
	memcpy(&data, ptrData, len);
	
	// On récupère la donnée
	Wrapper_Data_OutPort.write( (int)add );
	
	// On indique que la transaction est une écriture
	Wrapper_RW_OutPort.write(false);

	// On indique que la donnée est prête
	Wrapper_Enable_OutPort.write( true );	
	
	wait(Wrapper_Ready_InPort.default_event());
	
	// On indique que la donnée n'est plus prête
	Wrapper_Enable_OutPort.write( false );
	
	// On récupère la donnée
	Wrapper_Data_OutPort.write( data );

	wait(Wrapper_Ready_InPort.default_event());
	
}


