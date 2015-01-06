#ifndef __MYDATA_H__
#define __MYDATA_H__

#include <iostream>

#include "wami_params.h"
#include "wami_debayer.h"

using namespace std;

class InputImage
{ 
  public:
    inline InputImage();
  
    InputImage& operator = (const InputImage& rhs) {
        bayer = rhs.bayer;
      return *this;
    }

    bool operator == (const InputImage& rhs) const {
      return (rhs.bayer == bayer);
    }

    friend ostream& operator << (ostream& os, const  InputImage& x )   {
      os << "(" << x.bayer << ")";
      return os;
    }

    friend void sc_trace(sc_trace_file *tf, const InputImage& x, const std::string& NAME){
      sc_trace(tf,x.bayer, NAME + ".bayer");
    } 

   u16 (*bayer)[WAMI_DEBAYER_IMG_NUM_COLS];
};
  InputImage::InputImage()
  {
   bayer = (u16 (*)[WAMI_DEBAYER_IMG_NUM_COLS]) new u16 [WAMI_DEBAYER_IMG_NUM_ROWS * WAMI_DEBAYER_IMG_NUM_COLS];
   //initialize
   for(int i=0;i<WAMI_DEBAYER_IMG_NUM_COLS;i++){
   for(int j=0;j<WAMI_DEBAYER_IMG_NUM_ROWS;j++){
     bayer[i][j] = 0;
  }
  }
  }

class OutputImage
{
 public:
  inline OutputImage();
  
   OutputImage& operator = (const OutputImage& rhs) {
      debayered = rhs.debayered;
      return *this;
    }
    
   bool operator == (const OutputImage& rhs) const {
      return (rhs.debayered == debayered);
    }
    
   friend ostream& operator << ( ostream& os,  OutputImage const& x)
    {
     os << "(" << x.debayered << ")";
      return os;
    }
    
   friend void sc_trace(sc_trace_file *tf, const OutputImage& x,const std::string& NAME){
      sc_trace(tf,x.debayered, NAME + ".debayered");
    }

   rgb_pixel (*debayered)[WAMI_DEBAYER_IMG_NUM_COLS-2*PAD]; 
};
 
  OutputImage::OutputImage()
{
  debayered = (rgb_pixel (*)[WAMI_DEBAYER_IMG_NUM_COLS-2*PAD]) new rgb_pixel [(WAMI_DEBAYER_IMG_NUM_COLS-2*PAD)*(WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD)];
  for(int i=0;i<WAMI_DEBAYER_IMG_NUM_COLS-2*PAD;i++){
  //initialize
  for(int j=0;j<WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD;j++){
   debayered[i][j].r = 0;
   debayered[i][j].g = 0;
   debayered[i][j].b = 0;
   }
   }
}

#endif
