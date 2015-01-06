#################################################################################
#										#
# System-on-Chip Platforms -- Fall 2014						#
#										#
# Homework 2									#
#										#
# DO NOT MODIFY THE MAKEFILE! This forces you to keep the directory and file	#
# names given in your assignment.						#
#										#
#################################################################################

TARGET_ARCH := linux64
CC := g++

LIBS   := -lsystemc
LIBDIR = -L. -L$(SYSTEMC)/lib-$(TARGET_ARCH)
INCDIR = -I../src -I../tb -I$(SYSTEMC)/include -I$(TLM)/include/tlm
CFLAGS = -g -Wall

TARGET := wami_debayer
VPATH := ../src:../tb
C_SRCS := wami_debayer.c wami_utils.c
CPP_SRCS := debayer.cpp debayer_tb.cpp sc_main.cpp
HDRS := wami_debayer.h wami_utils.h wami_params.h debayer.h debayer_tb.h mydata.h
OBJS = $(CPP_SRCS:.cpp=.o)
OBJS += $(C_SRCS:.c=.o)

.SUFFIXES: .c .cpp .cc .o
$(TARGET): $(OBJS)
	$(CC) ${INCDIR} ${LIBDIR} -o $@ $(OBJS) ${LIBS}

$(OBJS) : $(HDRS)

.PHONY: clean run test

.cpp.o:
	$(CC) $(CFLAGS) ${INCDIR} -c $<
.c.o:
	$(CC) $(CFLAGS) ${INCDIR} -c $<

clean:
	rm -f *.o $(TARGET) *.vcd
	rm -f result.txt
	rm -rf ../output/*

test: $(TARGET)
	@./$(TARGET)

run: $(TARGET)
	rm -rf ../output
	mkdir -p ../output
	@./$(TARGET)
