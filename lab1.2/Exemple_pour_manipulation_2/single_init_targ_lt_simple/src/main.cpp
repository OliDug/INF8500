#include "systemc.h"
#include "top.h"

int sc_main(int argc, char *argv[]) {

  top ss_top("SS_TOP");
  sc_start();
  return 0;
}
