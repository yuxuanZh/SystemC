#include "debayer.h"
#include "wami_debayer.h"
#include "wami_params.h"

void debayer::process_debayer()
{

 RESET:
 const OutputDataToken zero;
 OutputDataToken tmpread;
 InputDataToken tmpwrite;
 //sink solves invalid data in reset
 data_out->put(zero);
 cout<<"time to initial dut"<<sc_time_stamp()<<endl;
  while(true) {
    wait();
    cout<<"DUT"<<sc_time_stamp()<<endl;
    
   INPUT:
    tmpwrite = data_in->get();
    cout<<"DUT after get before put"<<sc_time_stamp()<<endl;
  
   DODEBAYER:
    tmpread = doDebayer->transport(tmpwrite);//dodebayer method

   OUTPUT:
    data_out->put(tmpread);
    cout<<"DUT after put before next"<<sc_time_stamp()<<endl;
    }
}