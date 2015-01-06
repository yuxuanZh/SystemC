#ifndef __DODEBAYER_H__
#define __DODEBAYER_H__

#include "systemc.h"
#include "tlm.h"
#include "mydata.h"

using namespace tlm;

class doDebayer : public sc_module,
              public virtual tlm_transport_if<InputDataToken,OutputDataToken> {
 public:
  InputDataToken bayer;
  sc_export<tlm_transport_if<InputDataToken, OutputDataToken> > debayer_slave;//export tlm transport interface
  
  OutputDataToken transport(const InputDataToken &);
  //constructor
   SC_CTOR(doDebayer) {
     debayer_slave(*this);
   }
};
#endif
