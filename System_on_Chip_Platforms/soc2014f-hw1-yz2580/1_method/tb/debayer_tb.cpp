#include <string>

#include "debayer_tb.h"
#include "wami_utils.h"
#include "wami_params.h"
#include "wami_debayer.h"

#define MAXSIZE 50

static const char *input_txt = "../input/input.txt";
static const char *output_txt = "../input/golden_output.txt";
static const char *result_txt = "../output/result.txt";
static const char *goldfilename1 = "0000.bin";
static const char *goldfilename2 = "0001.bin";
static const char *goldfilename3 = "0002.bin";

const char output_dir[] = "../output/";
const u16 output_channels = 3;
  
//parse input.txt var
int tclk,trst,tsim,t20,t30,t40,t50;
string ip1,ip2,ip3,ip4;
  
//parse output.txt var
int tout1,tout2,tout3,tnext,tcurrent;
string goldfile1,goldfile2,goldfile3,gold_dir;
  
char dir_and_filename[MAXSIZE];
string input_dir,output_filename;  
int flag = 1; 

//read&write
ifstream input(input_txt);
ifstream output(output_txt);
ofstream result(result_txt);

void debayer_tb::source()
{
 RESET:
  //reset in constructor
  InputImage tmpsource;
  OutputImage tmpgold;
  inimg.write(tmpsource);
  //read txt
  input>>tclk>>trst>>tsim>>t20>>ip1>>t30>>ip2>>t40>>ip3>>t50>>ip4;
  output>>tout1>>goldfile1>>tout2>>goldfile2>>tout3>>goldfile3;
  wait(rst.posedge_event());
 while(true)
 {
  wait(clk.posedge_event());
  cout<<"souce:@"<<sc_time_stamp()<<endl;
  //parse
  int localtime = sc_time_stamp().value()/1000;
  if (localtime == t20){ input_dir = ip1;
         //cout<<input_dir.c_str()<<endl;
         //cout<<"case1"<<endl;	
  }else if(localtime == t30)
  {     input_dir = ip2; 
        //cout<<input_dir.c_str()<<endl;
        //cout<<"case2"<<endl;
  }else if(localtime == t40)
  {      input_dir = ip3; 
        //cout<<input_dir.c_str()<<endl;
        //cout<<"case3"<<endl;
  }else if(localtime == t50)
  {     input_dir = ip4; 
        //cout<<input_dir.c_str()<<endl;
	      //cout<<"50ns"<<endl;
  }else{ cout<<"time overflow"<<endl;
  }
  //read input.txt
  read_image_file(
       tmpsource.bayer,
       sizeof(u16),
       input_dir.c_str(),//input dir changed into one
       sizeof(u16) * WAMI_DEBAYER_IMG_NUM_ROWS 
       * WAMI_DEBAYER_IMG_NUM_COLS);
  
  //DEBUG: 
       //cout<<gold_dir.c_str()<<endl;
       // cout<<"beforetran"<<tmpsource.bayer<<endl;
       //cout<<"gold"<<tmpgold.debayered<<endl;
  
 OUTPUT:
  inimg.write(tmpsource);
 }
}

void debayer_tb::sink()
{
  RESET:
   //reset in constructor
   OutputImage tmpsink;
   OutputImage tmpgold;
   goldimg.write(tmpgold);
   //read txt
   input>>tclk>>trst>>tsim>>t20>>ip1>>t30>>ip2>>t40>>ip3>>t50>>ip4;
   output>>tout1>>goldfile1>>tout2>>goldfile2>>tout3>>goldfile3;
   wait(rst.posedge_event());
   //wait one clk cycle
   wait(tout1-trst,SC_NS);
  
   while(true)
  { 
   wait(clk.posedge_event());
   cout<<"sink:@"<<sc_time_stamp()<<endl;
   //parse 
   int localtime = sc_time_stamp().value()/1000;
   if (localtime == t30){
	     gold_dir = goldfile1;
       output_filename = goldfilename1;
       //cout<<gold_dir.c_str()<<sc_time_stamp()<<endl;	
  }else if(localtime == t40)
  {    gold_dir = goldfile2;
       output_filename =goldfilename2;
       //cout<<gold_dir.c_str()<<sc_time_stamp()<<endl;
  }else if(localtime == t50)
  {    gold_dir = goldfile3;
       output_filename = goldfilename3;
       //cout<<gold_dir.c_str()<<sc_time_stamp()<<endl;
  }else if(localtime == t50)
  {    //cout<<gold_dir.c_str()<<sc_time_stamp()<<endl;
  }else{cout<<"timeoverflow"<<endl;
  }
     //read gold.txt  
     read_image_file(
       tmpgold.debayered,
       sizeof(u16),
       gold_dir.c_str(),
       sizeof(rgb_pixel) * (WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD) 
       * (WAMI_DEBAYER_IMG_NUM_COLS-2*PAD));
  
  INPUT:
    tmpsink = outimg.read();
  
    //output img
    write_image_file(
       tmpsink.debayered,
       sizeof(u16),
       output_filename.c_str(),
       output_dir,
        WAMI_DEBAYER_IMG_NUM_COLS - 2*PAD,
        WAMI_DEBAYER_IMG_NUM_ROWS - 2*PAD,
        output_channels);
    
    //write result.txt
    result<<localtime<<" "<<output_dir<<output_filename<<"\n"; 
    // cout<<tmpgold.debayered<<endl;
    
  DEBUG:
    for(int i=0;i<WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD;i++)
     for(int j=0;j<WAMI_DEBAYER_IMG_NUM_COLS-2*PAD;j++){
      if(tmpsink.debayered[i][j].r == tmpgold.debayered[i][j].r&&
         tmpsink.debayered[i][j].g == tmpgold.debayered[i][j].g&&
         tmpsink.debayered[i][j].b == tmpgold.debayered[i][j].b);
         else { flag = -1; 
         cout << "Expecting " << tmpgold.debayered[i][j].r <<" "
         << tmpgold.debayered[i][j].g << " " << tmpgold.debayered[i][j].b <<endl
         << " But received " << tmpsink.debayered[i][j].r << " " 
         << tmpsink.debayered[i][j].g << " " <<tmpsink.debayered[i][j].b <<endl;}
         }
  if (flag == -1)
     cout << "Error:@" << sc_time_stamp()<<endl;
  else cout << "Matched:@" <<sc_time_stamp()<<endl;

  }
 }
