#ifndef _TOP_H_
#define _TOP_H_

#include "systemc.h"
#include "tlm.h"
#include "Initiator.h"
#include "Target.h"


using namespace tlm;


struct top : sc_core::sc_module {
public:

  Initiator *A1;
  Target *B1;

  SC_CTOR(top); // Constructor

};

#endif
