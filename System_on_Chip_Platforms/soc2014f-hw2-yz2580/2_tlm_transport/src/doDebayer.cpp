#include "doDebayer.h"
#include "wami_debayer.h"
 
OutputDataToken doDebayer::transport(const InputDataToken & bayer)//dodebayer method
{
  OutputDataToken tmpdebayered;
  u16 input_data[WAMI_DEBAYER_IMG_NUM_ROWS][WAMI_DEBAYER_IMG_NUM_COLS];
  rgb_pixel output_data[WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD][WAMI_DEBAYER_IMG_NUM_COLS-2*PAD];
  
 DODEBYER: 
  for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS; i++)
			for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS; j++)
				input_data[i][j] = bayer.getElem(i * WAMI_DEBAYER_IMG_NUM_COLS + j);
        
  wami_debayer(output_data,input_data);
  
  for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD; i++)
			for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS-2*PAD; j++)
				tmpdebayered.setElem(i * (WAMI_DEBAYER_IMG_NUM_COLS-2*PAD) + j,
					       output_data[i][j]);
                                                                            
  return(tmpdebayered);
}
