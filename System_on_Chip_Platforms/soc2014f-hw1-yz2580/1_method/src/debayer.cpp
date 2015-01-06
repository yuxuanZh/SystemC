#include "debayer.h"
#include "wami_debayer.h"
#include "wami_params.h"

OutputImage tmpread;
InputImage tmpwrite;

void debayer::beh()
{
if(!rst.read()){
 RESET:
 outimg.write(tmpread);}
 else if(clk.event()){
  cout<<"DUT"<<sc_time_stamp()<<endl;
  tmpwrite = inimg.read();
  //cout<<"before"<<tmpwrite.bayer<<endl;
  wami_debayer(tmpread.debayered,tmpwrite.bayer);
  outimg.write(tmpread);
  // cout<<"after"<<tmpread.debayered<<endl;
  }
}
