
#ifndef _TARGET_H_
#define _TARGET_H_

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

using namespace tlm;


class Target : public sc_core::sc_module {
public:
  tlm_utils::simple_target_socket<Target> targ_socket;

  int mem[1024];

  SC_CTOR(Target) {
    targ_socket.register_b_transport(this, &Target::b_transport);
    targ_socket.register_transport_dbg(this, &Target::transport_dbg);
  }

  void b_transport(tlm::tlm_generic_payload& payload, sc_core::sc_time& tLOCAL);
  unsigned int transport_dbg(tlm::tlm_generic_payload &payload);

};

#endif
