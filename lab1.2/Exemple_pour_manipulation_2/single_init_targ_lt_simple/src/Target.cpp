
#include <sstream>
#include "systemc.h"
#include "tlm.h"
#include "Target.h"

unsigned int
Target::transport_dbg
  ( tlm::tlm_generic_payload  &payload
  )
{
  payload.set_response_status(TLM_OK_RESPONSE); // Assume ok
  unsigned int   len      = payload.get_data_length();
  uint64         addr     = payload.get_address();
  unsigned char* data_ptr = payload.get_data_ptr();
  int *          data     = reinterpret_cast<int *>(data_ptr);
  std::ostringstream msg;

  if (len % 4 != 0) {
    SC_REPORT_ERROR("Target","Address must be word-aligned");
    return 1;
  }
  if (addr > 1023) {
    SC_REPORT_ERROR("Target","Address out of range");
    return 2;
  }

  if (payload.is_read()) {
    if (len == 4) {
      *data = mem[addr];
    }
  } else if (payload.is_write()) {
    if (len == 4) {
      mem[addr] = *data;
    }
  }//endif
  return 0;
}


void Target::b_transport( // Blocking transport
  tlm::tlm_generic_payload& payload,
  sc_core::sc_time&         tLOCAL)
{
  payload.set_response_status(TLM_OK_RESPONSE); // Assume ok
  unsigned int   len      = payload.get_data_length();
  uint64         addr     = payload.get_address();
  unsigned char* data_ptr = payload.get_data_ptr();
  int *          data     = reinterpret_cast<int *>(data_ptr);
  std::ostringstream msg;

  if (len % 4 != 0) {
    SC_REPORT_ERROR("Target","Address must be word-aligned");
    return;
  }
  if (addr > 1023) {
    SC_REPORT_ERROR("Target","Address out of range");
    return;
  }

  tLOCAL += sc_time(20, SC_NS);
  if (payload.is_read()) {
    if (len == 4) {
      *data = mem[addr];
    }
    msg.str("");
    msg << "READ transaction: addr = " << addr << " value =  " << *data;
    SC_REPORT_INFO("Target",msg.str().c_str());
  } else if (payload.is_write()) {
    if (len == 4) {
      mem[addr] = *data;
    }
    msg.str("");
    msg << "WRITE transaction: addr = " << addr << " value =  " << *data;
    SC_REPORT_INFO("Target",msg.str().c_str());
  }//endif
}

