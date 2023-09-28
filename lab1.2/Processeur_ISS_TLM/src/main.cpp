///////////////////////////////////////////////////////////////////////////////
//
//	main.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include <systemc.h>
#include "processor.h"
#include "CoProcessor.h"
#include "DataRAM.h"
#include "InstRAM.h"
#include "Console.h"
#include "wrapper_console_TLM.h"
#include "wrapper_processor_TLM.h"
#include "wrapper_coProcessor_TLM.h"
#include "SimpleBusLT.h"
#include "busLT_standalone_definition.h"


#define RAMSIZE 0x800

// Variable gloable
bool m_bError = false;

///////////////////////////////////////////////////////////////////////////////
//
//	Entrée principale de l'application
//
///////////////////////////////////////////////////////////////////////////////
int sc_main(int arg_count, char **arg_value)
{
	
	// Horloge
	sc_clock clk( "SysClock", 40, SC_NS, 0.5 );

	// Instanciation des composants
	processor inst_processor( "processor", RAMSIZE, RAMSIZE, false );
	InstRAM inst_InstRAM( "InstRAM", "memory/app.elf", RAMSIZE, false );
	DataRAM inst_DataRAM( "DataRAM", "memory/data_mem.hex", RAMSIZE, false );
	Console inst_Console( "Console" );
	wrapper_console_TLM inst_wrapper_console("wrapper_console", ADRESSE_CONSOLE_DEBUT, ADRESSE_CONSOLE_FIN);
	wrapper_processor_TLM inst_wrapper_processor("wrapper_processor", false);
	CoProcessor inst_CoProcessor("CoProcessor");
	wrapper_coProcessor_TLM inst_wrapper_coProcessor("wrapper_coProcessor", ADRESSE_COPROCESSOR_DEBUT, ADRESSE_COPROCESSOR_FIN);
	
	// Bus LT
    SimpleBusLT<1,2> busLT("busLT");
	
	// Signaux
	sc_signal<unsigned int> sig_Console_Data;
    sc_buffer<bool> sig_Console_Enable;
	sc_buffer<bool> sig_Console_Ready;
	sc_signal<unsigned int> sig_Processor_Data;
	sc_signal<unsigned int> sig_Processor_Data_;
    sc_signal<unsigned int> sig_Processor_Addr;
	sc_signal<bool> sig_Processor_Enable;
    sc_signal<bool> sig_Processor_Ready;
	sc_buffer<bool> sig_Processor_RW;
	
	sc_signal<unsigned int>		sig_CoProcessor_Data_InPort;
	sc_signal<unsigned int>		sig_CoProcessor_Data_OutPort;
	sc_buffer<bool>				sig_CoProcessor_Enable;	
	sc_buffer<bool>				sig_CoProcessor_Ready;
	sc_buffer<bool>				sig_CoProcessor_RW;
	
	// Branchement
	inst_processor.ClockPort( clk );
	inst_processor.InstPort( inst_InstRAM );
	inst_processor.DataPort( inst_DataRAM );
	
	inst_Console.ClockPort( clk );
	inst_Console.Console_Data_InPort( sig_Console_Data );
	inst_Console.Console_Enable_InPort( sig_Console_Enable );
	inst_Console.Console_Ready_OutPort( sig_Console_Ready );
	inst_wrapper_console.ClockPort( clk );
	inst_wrapper_console.Wrapper_Data_OutPort( sig_Console_Data );
	inst_wrapper_console.Wrapper_Enable_OutPort( sig_Console_Enable );
	inst_wrapper_console.Wrapper_Ready_InPort( sig_Console_Ready );
	
	inst_wrapper_processor.ClockPort( clk );
	inst_wrapper_processor.Wrapper_Data_InPort( sig_Processor_Data );
	inst_wrapper_processor.Wrapper_Data_OutPort(sig_Processor_Data_);
	inst_wrapper_processor.Wrapper_Address_InPort( sig_Processor_Addr );
	inst_wrapper_processor.Wrapper_Enable_InPort( sig_Processor_Enable );
	inst_wrapper_processor.Wrapper_Ready_OutPort( sig_Processor_Ready );
	inst_wrapper_processor.Wrapper_RW_InPort(sig_Processor_RW);
	
	
	inst_processor.Processor_Data_InPort(sig_Processor_Data_);
	inst_processor.Processor_Data_OutPort( sig_Processor_Data );
	inst_processor.Processor_Address_OutPort( sig_Processor_Addr );
	inst_processor.Processor_Enable_OutPort( sig_Processor_Enable );
	inst_processor.Processor_Ready_InPort( sig_Processor_Ready );
	inst_processor.Processor_RW_OutPort(sig_Processor_RW);
	
	inst_CoProcessor.ClockPort( clk );
	inst_CoProcessor.CoProcessor_Data_InPort(sig_CoProcessor_Data_InPort);
	inst_CoProcessor.CoProcessor_Data_OutPort(sig_CoProcessor_Data_OutPort);	
	inst_CoProcessor.CoProcessor_Enable_InPort(sig_CoProcessor_Enable);
	inst_CoProcessor.CoProcessor_Ready_OutPort(sig_CoProcessor_Ready);
	inst_CoProcessor.CoProcessor_RW_InPort(sig_CoProcessor_RW);
		
	inst_wrapper_coProcessor.ClockPort(clk);
	inst_wrapper_coProcessor.Wrapper_Data_InPort(sig_CoProcessor_Data_OutPort);
	inst_wrapper_coProcessor.Wrapper_Data_OutPort(sig_CoProcessor_Data_InPort);
	inst_wrapper_coProcessor.Wrapper_Enable_OutPort(sig_CoProcessor_Enable);
	inst_wrapper_coProcessor.Wrapper_Ready_InPort(sig_CoProcessor_Ready);
	inst_wrapper_coProcessor.Wrapper_RW_OutPort(sig_CoProcessor_RW);
	
	// Bus busLT  
	inst_wrapper_processor.socket(busLT.target_socket[PROCESSOR_TARGET_ID]);
  
	busLT.initiator_socket[COPROCESSOR_INITIATOR_ID](inst_wrapper_coProcessor.socket);
	busLT.initiator_socket[CONSOLE_INITIATOR_ID](inst_wrapper_console.socket);
	
	// Démarrage de l'application
	cout << "Demarrage de la simulation." << endl;
	sc_start(40, SC_MS);

	cout << endl << "Simulation s'est terminee a " << sc_time_stamp() << endl << endl;

	// Fin du programme
	return 0;
}
