#include "debayer.h"
#include "wami_debayer.h"
#include "wami_params.h"

void debayer::beh()
{
 RESET:
 OutputImage tmpread;
 InputImage tmpwrite;
 outimg.write(tmpread);
 while(true){
  wait();
  cout<<"DUT"<<sc_time_stamp()<<endl;
  tmpwrite = inimg.read();
  //cout<<"before"<<tmpwrite.bayer<<endl;
  wami_debayer(tmpread.debayered,tmpwrite.bayer);
  outimg.write(tmpread);
  // cout<<"after"<<tmpread.debayered<<endl;
  }
}
