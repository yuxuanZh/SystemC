#################################################################################
#										#
# System-on-Chip Platforms -- Spring 2013					#
#										#
# Homework 4 - Problem 1							#
#										#
# DO NOT MODIFY THE MAKEFILE! This forces you to keep the directory and file	#
# names given in your assignment.						#
#										#
#################################################################################

TARGET_ARCH := linux64
WAVE_VIEWER := simvision
CC := g++

LIBS   := -lsystemc
LIBDIR = -L. -L$(SYSTEMC)/lib-$(TARGET_ARCH)
CTOS_ROOT := $(CTOS_PATH)
CTOS_INCLUDES := -I$(CTOS_ROOT)/tools/ctos/include \
		-I$(CTOS_ROOT)/tools/ctos/include/ctos_fx \
		-I$(CTOS_ROOT)/tools/ctos/include/ctos_tlm \
		-I$(CTOS_ROOT)/tools/ctos/include/ctos_flex_channels

INCDIR = -I../src -I../tb -I$(SYSTEMC)/include -I$(TLM)/include/tlm $(CTOS_INCLUDES)

CFLAGS := #-Wall

TARGET := wami_debayer
VPATH := ../src:../tb
C_SRCS := wami_utils.c
CPP_SRCS := debayer.cpp debayer_tb.cpp sc_main.cpp
HDRS := wami_utils.h wami_params.h debayer.h debayer_tb.h
OBJS = $(CPP_SRCS:.cpp=.o)
OBJS += $(C_SRCS:.c=.o)

.SUFFIXES: .c .cpp .cc .o
$(TARGET): $(OBJS)
	$(CC) ${INCDIR} ${LIBDIR} -o $@ $(OBJS) ${LIBS}

.PHONY: clean run wave test

.cpp.o:  $< .h
	$(CC) $(CFLAGS) ${INCDIR} -c $<

.c.o:
	$(CC) $(CFLAGS) ${INCDIR} -c $<

$(OBJS): $(HDRS)

clean:
	rm -f *.o $(TARGET) *.dsn *.trn *.vcd
	rm -f ../output/*
	if [ -d .simvision ]; then rm -r .simvision; fi

run: $(TARGET)
	./$(TARGET)

test: run
	@ ./test.sh

wave:
	$(WAVE_VIEWER) $(TARGET).vcd
