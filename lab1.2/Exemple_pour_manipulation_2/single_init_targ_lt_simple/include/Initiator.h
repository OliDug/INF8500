
#ifndef _INITIATOR_H_
#define _INITIATOR_H_

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "tlm_utils/simple_initiator_socket.h"

//using namespace tlm;

struct Initiator : public sc_core::sc_module
{
  tlm_utils::simple_initiator_socket<Initiator> init_socket;

  unsigned int src_addr;
  unsigned int dest_addr;
  unsigned int len;
  tlm_utils::tlm_quantumkeeper qk;

  void initiator_thread(void); // Process

  SC_CTOR(Initiator) :
    src_addr(0),
    dest_addr(0),
    len(0)
  {
    SC_THREAD(initiator_thread);
    qk.set_global_quantum(sc_time(200, SC_NS));
    qk.reset();
  }

};

#endif
