/*
 * debayer-mmap.c
 * Sowftware and hardware-accelerated DEBAYER.
 *
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>

#include "debayer.h"
#include "wami_params.h"
#include "wami_utils.h"
#include "wami_debayer.h"

static const char *devname = "/dev/debayer.0";

static const char usage_str[] =
	"usage: ./mac-mmap <size> <# samples>\n";

static int validate(rgb_pixel *gold_debayer, rgb_pixel* debayered_sw, u16 *debayer, unsigned rows)
{
 unsigned i;
 unsigned mismatches = 0;
for (i = 0; i < (rows-2*WAMI_DEBAYER_PAD)*(rows-2*WAMI_DEBAYER_PAD); i++) {
    if (gold_debayer[i].r != *(debayer+3*i)||
    gold_debayer[i].g != *(debayer+3*i+1) ||
    gold_debayer[i].b != *(debayer+3*i+2)
    ) {
				mismatches++;
    printf("debayer[%d] doesn't match in HW\n", i);
    //printf(" debayer[i] << " result !===! expected " << gold_debayer[i] << endl;
    break;
    }
    
for (i = 0; i < (rows-2*WAMI_DEBAYER_PAD)*(rows-2*WAMI_DEBAYER_PAD); i++) {
    if (gold_debayer[i].r != debayered_sw[i].r||
    gold_debayer[i].g != debayered_sw[i].g ||
    gold_debayer[i].b != debayered_sw[i].b
    ) {
				mismatches++;
    printf("debayer[%d] doesn't match in SW\n", i);
    //printf(" debayer[i] << " result !===! expected " << gold_debayer[i] << endl;
    break;

          }
    }
    return mismatches ;
 }
}


static void init_debayer(u16 *(bayer)[128], unsigned rows)
{
read_image_file(bayer, sizeof(u16),
			"inout/input.bin", ".",
				sizeof(u16) * rows * rows);
}      
        

static void print_array(u16 *a, unsigned rows, unsigned cols)
{
	unsigned i, j;
	for (i = 0; i < rows; i++) {
		printf("sample %d:", i);
		for (j = 0; j < cols; j++)
			printf("% d", a[i * rows + j]);
		printf("\n");
	}
}

int main(int argc, char *argv[])
{

	unsigned flag;
	u16 *buf;
	int fd;
 
	size_t buf_size;
  size_t input_size;

	struct debayer_access desc;
	int rc;

	/* Input directory */
	int rows = 128;
	int num = 1;
      
  u16* bayer;
	rgb_pixel debayered_sw[WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD][WAMI_DEBAYER_IMG_NUM_COLS-2*PAD];
	rgb_pixel* golden_debayer;
	
  
	unsigned i, j;
/*
	if (argc < 3 || argc > 4)//????????????????????
	{
		fprintf(stderr, "%s", usage_str);
		exit(EXIT_FAILURE);
	}
 */
/*
	sz = atoi(argv[1]);
	if (sz < 0) {
		fprintf(stderr, "Invalid MAC input size\n");
		exit(EXIT_FAILURE);
	}
	num = atoi(argv[2]);
	if (num < 0) {
		fprintf(stderr, "Invalid MAC samples number\n");
		exit(EXIT_FAILURE);
	}
 */

 
  bayer = malloc(sizeof(u16) * rows * rows);
  golden_debayer = malloc(sizeof(rgb_pixel) * (rows-2*PAD) * (rows-2*PAD)); 
 
 
	
  if (!bayer || !golden_debayer) {
		fprintf(stderr, "Memory allocation failure\n");
		exit(EXIT_FAILURE);
	}

	//init_debayer(bayer, rows);
 
 read_image_file((void *)bayer, sizeof(u16),
			"inout/input.bin", ".",
				sizeof(u16) * rows * rows);

/*
unsigned m,n;
for( m=0;m<128;m++)
for( n=0;n<128;n++)
 printf("BAYER[%d] = %d\n", 128*m+n, bayer[128*m+n]);
*/

	printf("DEBAYER parameters:\n");
	printf("size      = %d\n", rows);
  printf("# = %d\n", num);

	/* *********************** SOFTWARE ****************************************/
#ifndef NO_SOFTWARE
	printf("\nStart: DEBAYER as SOFTWARE.\n");
  wami_debayer(debayered_sw,bayer);
 
	printf("\nDone: DEBAYER as SOFTWARE.\n");


#endif
	/* *********************** HARDWARE ****************************************/
#ifndef NO_HARDWARE
	printf("Open: %s\n", devname);
	fd = open(devname, O_RDWR, 0);//??????????????????????????
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}

	buf_size = DEBAYER_BUF_SIZE(rows, num);
  printf("buf_size = %d\n", buf_size);
  input_size = DEBAYER_INPUT_SIZE(rows, num);//see .h

printf("input_size = %d\n", input_size);


	/* main memory */
	buf = mmap(NULL, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//??????
	if (buf == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	/* load data - copy data from input data structures*/
//	for (i = 0; i < rows; i++) {
//		memcpy(buf + (i * rows), bayer  + (i * rows), input_size);//first rows x rows are input
		//memcpy(buf + ((2 * i + 1) * sz), coeff + (i * sz), sample_size);
//	}
 memcpy(buf , bayer  , input_size);
 
// for( m=0;m<128;m++)
//for( n=0;n<128;n++)
// printf("buf[%d] = %d\n", 128*m+n, buf[128*m+n]);
 
 //printf("FINISH sizeof(buf) = %d\n", sizeof(buf));
 
 
 
	printf("\nStart: DEBAYER as HARDWARE.\n");
	desc.size = 128;//??????????????????
	desc.num_samples = 1;

	rc = ioctl(fd, DEBAYER_IOC_ACCESS, &desc);//set up device
	if (rc < 0)
	{
		perror("ioctl");
		exit(1);
	}

//	print_array(buf + (rows * rows), 3 * (rows-2*WAMI_DEBAYER_PAD), (rows-2*WAMI_DEBAYER_PAD));//print output after input in buf
 
 
 //for( m=0;m<1;m++)
 //for( n=0;n<124;n=n+3)
 //printf("buf + (rows * rows)[%u] = %u\n", 124*m+n, *(buf+(rows*rows)+124*m+n));
 
 
 // for( m=0;m<124;m++)
//for( n=0;n<124;n++)
// printf("golden_debayer_r[%u] = %u\n", 124*m+n, golden_debayer[124*m+n].r);
 
 
 
 //read golden
  read_image_file((void *)golden_debayer, sizeof(rgb_pixel),
					"inout/golden_output.bin", ".",
					sizeof(rgb_pixel) * (rows-2*PAD)*(rows-2*PAD));
 
 
	flag = validate(golden_debayer, debayered_sw, buf + (rows * rows), rows);
	assert((flag == 0) && "DEBAYER HW Failed!\n");
 printf("flag %d\n",flag);

	printf("Software and Hardware match!\n");
#endif
	free (bayer);
	free (golden_debayer);
	close(fd);

	return 0;
}
