CARGO ?= /usr/local/share/cargo
QSIM ?= ${CARGO}/qsim
KSRC ?= $(QSIM)/linux
CFLAGS := -O3 -DDEBAYER_SIZE=128
CFLAGS += -m32 -static
CFLAGS += -I../..
LIBS = -lm -lrt

all: tarball

check:
ifeq ($(KSRC),)
	$(error 'Path to kernel in env variable KSRC not found. Exiting')
endif
.PHONY: check

debayer-mmap: debayer-mmap.c wami_debayer.c wami_utils.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

tarball: debayer.tar
.PHONY: tarball

debayer.tar: debayer-mmap debayer.ko runme.sh inout
	tar cf $@ $^

driver: debayer.ko

.PHONY: driver

debayer.ko: check
	make -C $(KSRC) M=`pwd`

clean help: check
	$(MAKE) -C $(KSRC) M=`pwd` $@
distclean: clean
	$(RM) debayer.tar debayer-mmap

.PHONY: all clean help
