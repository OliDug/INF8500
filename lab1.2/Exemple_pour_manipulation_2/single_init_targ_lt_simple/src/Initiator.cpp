
#include <sstream>
#include "systemc.h"
#include "tlm.h"
#include "Initiator.h"

using namespace tlm;

void Initiator::initiator_thread() {
  tlm_generic_payload payload;
  unsigned int  addr;
  int data;
  unsigned char *data_ptr;
  sc_time tLOCAL(sc_core::SC_ZERO_TIME);
  std::ostringstream msg;
  data_ptr = reinterpret_cast<unsigned char *>(&data);

  src_addr = 0;
  dest_addr = 100;
  len = 10;

   payload.set_data_ptr(data_ptr);
   payload.set_data_length(4);
   payload.set_write();
   for (int i = 0; i < len; i++) {
      data = 5000+i;
      addr = src_addr + i;
      payload.set_address(addr);
      init_socket->transport_dbg(payload);
   }

   for (int i = 0; i < len; i++) {
      addr = src_addr + i;
      payload.set_address(addr);
      payload.set_data_ptr(data_ptr);
      payload.set_data_length(4);
      payload.set_read();
      msg.str("");
      msg << sc_time_stamp() << ": Doing a READ transaction from address " << addr;
      SC_REPORT_INFO("Initiator", msg.str().c_str());

      tLOCAL = qk.get_local_time();
      init_socket->b_transport(payload,tLOCAL);

      // Code below can use qk.set_and_sync(tLOCAL) in TLM 2.0.1 and later
      // ------------------------
      qk.set(tLOCAL);
      if (qk.need_sync())
        qk.sync();
      // ------------------------

      qk.inc(sc_time(10, SC_NS));

      if (payload.is_response_error()) {
        SC_REPORT_ERROR("Initiator","Received error reply.");
      }

      addr = static_cast<unsigned int>(dest_addr + i);
      payload.set_address(addr);
      payload.set_write();
      msg.str("");
      msg << sc_time_stamp() << ": Doing a WRITE transaction to address " << addr;
      SC_REPORT_INFO("Initiator", msg.str().c_str());

      tLOCAL = qk.get_local_time();
      init_socket->b_transport(payload,tLOCAL);

      // Code below can use qk.set_and_sync(tLOCAL) in TLM 2.0.1 and later
      // ------------------------
      qk.set(tLOCAL);
      if (qk.need_sync())
        qk.sync();
      // ------------------------

      qk.inc(sc_time(10, SC_NS));

      if (payload.is_response_error()) {
        SC_REPORT_ERROR("Initiator","Received error reply.");
      }
   } // for
}

