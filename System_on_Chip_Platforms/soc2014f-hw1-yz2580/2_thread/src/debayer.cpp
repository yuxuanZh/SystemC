#include "debayer.h"
#include "wami_debayer.h"
#include "wami_params.h"

void debayer::beh()
{
 RESET:
 //reset in constructor
  OutputImage tmpread;
  InputImage tmpwrite;
  outimg.write(tmpread);
  wait(rst.posedge_event());
 
 while(true){
   wait(clk.posedge_event());
   //race condition
   wait(SC_ZERO_TIME);
   cout<<"DUT:@"<<sc_time_stamp()<<endl;
  
  INPUT: 
   tmpwrite = inimg.read();
  //cout<<"before"<<tmpwrite.bayer<<endl;
  
  DEBAYER:
   wami_debayer(tmpread.debayered,tmpwrite.bayer);
   
  OUTPUT: 
   outimg.write(tmpread);
  // cout<<"after"<<tmpread.debayered<<endl;
  }
}
