#ifndef MY_DATA_H
#define MY_DATA_H

#include <string.h>

#include "wami_params.h"

#define INPUT_TOKEN_SIZE (WAMI_DEBAYER_IMG_NUM_ROWS * WAMI_DEBAYER_IMG_NUM_COLS)
#define OUTPUT_TOKEN_SIZE (WAMI_DEBAYER_IMG_NUM_ROWS - 2 * WAMI_DEBAYER_PAD) * (WAMI_DEBAYER_IMG_NUM_COLS - 2 * WAMI_DEBAYER_PAD)

class InputDataToken {   // data token encapsulation class
public:
	InputDataToken()    // default constructor (Each element is already initialized)
	{
		memset(data, 0, sizeof(data));
	}

	bool operator == (const InputDataToken &rhs) const // equality comparator
	{
		return memcmp(data, rhs.data, sizeof(data)) == 0;
	}

	friend void sc_trace(sc_trace_file *tf, const InputDataToken &v,
			     const std::string &NAME) // VCD dumping function
	{
/*
		for(int i = 0; i < INPUT_TOKEN_SIZE; i++) {
			std::stringstream sstm;
			sstm << NAME << "El_" << i;
			sc_trace(tf, v.data[i], sstm.str());
		}
*/
	}

	friend ostream& operator << (ostream& os, InputDataToken const &v)
	{   // output operator
		os << std::hex << v.data[0];
		for(int i = 1; i < INPUT_TOKEN_SIZE; i++) {
			os << std::hex << " " << v.data[i];
		}
		return os;
	}

	u16 getElem(int i) const    // get ith digit
	{
		return data[i];
	}

	void setElem(int i, u16 d)
	{
		data[i] = d;
	}

private:

	u16 data[INPUT_TOKEN_SIZE];
};

class OutputDataToken {   // data token encapsulation class
public:
	OutputDataToken()    // default constructor (Each element is already initialized)
	{
		memset(data, 0, sizeof(data));
	}

	bool operator == (const OutputDataToken &rhs) const // equality comparator
	{
		return memcmp(data, rhs.data, sizeof(data)) == 0;
	}

	friend void sc_trace(sc_trace_file *tf, const OutputDataToken &v,
			     const std::string &NAME) // VCD dumping function
	{
/*
		for(int i = 0; i < OUTPUT_TOKEN_SIZE; i++) {
			std::stringstream sstm;
			sstm << NAME << "El_" << i << v;
			sc_trace(tf, "", sstm.str());
		}
*/
	}

	friend ostream& operator << (ostream& os, OutputDataToken const &v)
	{   // output operator
		os << std::hex << "(" << v.data[0].r << "," << v.data[0].g << "," << v.data[0].b << ")";
		for(int i = 1; i < OUTPUT_TOKEN_SIZE; i++) {
			os << std::hex << " " << "(" << v.data[i].r << "," << v.data[i].g << "," << v.data[i].b << ")";
		}
		return os;
	}

	rgb_pixel getElem(int i) const    // get ith digit
	{
		return data[i];
	}

	void setElem(int i, rgb_pixel d)
	{
		data[i] = d;
	}

private:

	rgb_pixel data[OUTPUT_TOKEN_SIZE];
};



#endif
