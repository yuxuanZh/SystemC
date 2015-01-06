# ******* Do not edit this CtoS generated Makefile ******* 
# *****************************************************************************
#                         Cadence C-to-Silicon Compiler
#           Version 14.10-p100  (64 bit), build 50398 Tue, 27 May 2014
# 
# File created on Wed Nov 12 20:47:08 2014
# 
# The code contained herein is generated for Cadences customer and third
# parties authorized by customer.  It may be used in accordance with a
# previously executed license agreement between Cadence and that customer.
# Absolutely no disassembling, decompiling, reverse-translations or
# reverse-engineering of the generated code is allowed.
# 
# ******************************************************************************
# This makefile runs simulations from the CtoS GUI or as a stand-alone
# makefile to run simulations in batch mode.
# 
# The following variables can be set in the command line as follows: 
# 	 make 
# 
# CTOS_EXE - Specify path to CtoS executable if it is not in your path
# 
# USER_ARGS - Specify additional arguments to the simulator
# 
# SIM_GUI - Bring up SimVision

# *****************************************************************************
# ***** CtoS Section *****
# *****************************************************************************
CTOS_EXE		?= $(shell echo `which ctos`)
#CTOS_EXE		?= /tools/ctos141/tools.lnx86/ctos/bin/64bit/ctos
CTOS_DIR		:= $(shell echo `dirname ${CTOS_EXE}`)
CTOS_ROOT		:= ${CTOS_DIR}/..
DESIGN_NAME		 ?= debayer_unisim
TOP_MODULE_NAME		 = debayer
DESIGN_SRC_FILES	 = ../src/debayer.cpp 
DESIGN_TB_FILES		 = ../tb/wami_utils.cpp ../tb/debayer_tb.cpp \
			../tb/sc_main.cpp 
MODEL_DIR		 ?= ../syn/model_unisim
CTOS_INCLUDES		 := -I./$(MODEL_DIR) -I$(CTOS_ROOT)/share/ctos/include \
			 -I$(CTOS_ROOT)/share/ctos/include/ctos_fx \
			 -I$(CTOS_ROOT)/share/ctos/include/ctos_tlm \
			 -I$(CTOS_ROOT)/share/ctos/include/ctos_flex_channels \
			 -I$(CTOS_ROOT)/share/ctos/include/ctos_clock_gating
DIFF			 ?= diff -r -x '.*' -B
REF_DIR			 ?= run_orig_sim

# *****************************************************************************
# ***** Simulator Section *****
# *****************************************************************************
SIM_EXE			 = irun

SIM_GUI_ARGS		 = -DSIM_GUI -layout cdebug -ncelab_args,-scCreateViewables \
			-linedebug +access+rwc
SIM_STANDARD_ARGS	 = -sysc -gnu -ncsc_runargs "-SINGLE_LOG"
SIM_CONFIG_ARGS		 := -I../src/ -I../tb/ -sc_main -I../syn/model_unisim/ \
			-D__CTOS__ 

SIM_INCLUDES		 ?= $(CTOS_INCLUDES)
ORIG_ARGS		 := $(SIM_INCLUDES) $(SIM_STANDARD_ARGS) \
			$(SIM_CONFIG_ARGS)  \
			$(DESIGN_SRC_FILES) $(DESIGN_TB_FILES)
LOG_DIR			 ?= log

ifneq ($(origin USER_ARGS), undefined)
	ORIG_ARGS	 := $(ORIG_ARGS) $(USER_ARGS)
endif

ifneq ($(origin SIM_GUI), undefined)
	ORIG_ARGS	 := $(ORIG_ARGS) $(SIM_GUI_ARGS)
endif

SIM_ARGS		 := $(ORIG_ARGS)
WORK_LIBS		 := work_orig_sim \
			    work_post_build_sim \
			    work_current_sim \
			    work_final_sim work_rtl_sim

RUN_LIBS		 := $(REF_DIR) \
			    run_post_build_sim \
			    run_current_sim \
			    run_final_sim run_rtl_sim


# *****************************************************************************
# ***** Rules and Targets Section *****
# *****************************************************************************
all:	setup orig_sim \
	post_build_sim \
	final_sim rtl_sim

setup:	info clean
	mkdir -p $(LOG_DIR)

info:
	@echo Test run at `date` on `hostname`
	@echo Test run in `pwd`

clean:

	rm -rf core* $(LOG_DIR)/*_sim.log $(WORK_LIBS)			 $(RUN_LIBS)

ctos_found:
	@if [ ! ${CTOS_EXE} ]; then \
		echo ""; \
		echo "============================="; \
		echo "Error: ctos not found in PATH"; \
		echo "============================="; \
		echo "Two ways to specify location of ctos:"; \
		echo "1. Run 'make CTOS_EXE=<ctos_install_path>/bin/ctos' OR"; \
		echo "2. Add <ctos_install_path>/bin to your PATH"; \
		echo ""; \
		exit 1; \
	fi;

	@if [ ! -e ${CTOS_EXE} ]; then \
		echo ""; \
		echo "================================================="; \
		echo "Error: Value of CTOS_EXE is not executable file"; \
		echo "================================================="; \
		echo "Check that the value of CTOS_EXE specified is an executable."; \
		echo ""; \
		exit 1; \
	fi;

sim_found:	ctos_found
	@if [ ! $(shell echo `which irun`) ]; then \
		echo ""; \
		echo "=================================="; \
		echo "Error: irun not found in PATH"; \
		echo "=================================="; \
		echo "Add <ius_install_path>/tools/bin to your PATH"; \
		echo ""; \
		exit 1; \
	fi;

orig_sim:	sim_found
	@echo "======================================"
	@echo "Simulating the original SystemC model "
	@echo "======================================"
	rm -rf run_orig_sim/*
	mkdir -p $(LOG_DIR) work_orig_sim run_orig_sim
	$(SIM_EXE) $(ORIG_ARGS) \
		-nclibdirpath work_orig_sim \
		-l $(LOG_DIR)/orig_sim.log
	grep '^Simulation successful!' $(LOG_DIR)/orig_sim.log > /dev/null

post_build_sim:	sim_found
	@echo "==================================================="
	@echo "Simulating the post-build Verilog simulation model "
	@echo "==================================================="
	rm -rf run_post_build_sim/*
	mkdir -p $(LOG_DIR) work_post_build_sim run_post_build_sim
	export CTOS_TARGET_NAME=post_build_sim; export CTOS_MODEL=post_build; \
	$(SIM_EXE) $(SIM_ARGS) \
		$(MODEL_DIR)/*_post_build.v \
		-DCTOS_post_build -DCTOS_MODEL=post_build \
		-nclibdirpath work_post_build_sim \
		-l $(LOG_DIR)/post_build_sim.log
	grep '^Simulation successful!' $(LOG_DIR)/post_build_sim.log > /dev/null

current_sim:	sim_found
	@echo "================================================"
	@echo "Simulating the current Verilog simulation model "
	@echo "================================================"
	rm -rf run_current_sim/*
	mkdir -p $(LOG_DIR) work_current_sim run_current_sim
	export CTOS_TARGET_NAME=current_sim; export CTOS_MODEL=current; \
	$(SIM_EXE) $(SIM_ARGS) \
		$(MODEL_DIR)/*_current.v \
		-DCTOS_current -DCTOS_MODEL=current \
		-nclibdirpath work_current_sim \
		-l $(LOG_DIR)/current_sim.log
	grep '^Simulation successful!' $(LOG_DIR)/current_sim.log > /dev/null

final_sim:	sim_found
	@echo "=============================================="
	@echo "Simulating the final Verilog simulation model "
	@echo "=============================================="
	rm -rf run_final_sim/*
	mkdir -p $(LOG_DIR) work_final_sim run_final_sim
	export CTOS_TARGET_NAME=final_sim; export CTOS_MODEL=final; \
	$(SIM_EXE) $(SIM_ARGS) \
		$(MODEL_DIR)/*_final.v \
		-DCTOS_final -DCTOS_MODEL=final \
		-nclibdirpath work_final_sim \
		-l $(LOG_DIR)/final_sim.log
	grep '^Simulation successful!' $(LOG_DIR)/final_sim.log > /dev/null

rtl_sim:	sim_found
	@echo "========================="
	@echo "Simulating the RTL model "
	@echo "========================="
	rm -rf run_rtl_sim/*
	mkdir -p $(LOG_DIR) work_rtl_sim run_rtl_sim
	export CTOS_TARGET_NAME=rtl_sim; export CTOS_MODEL=rtl; \
	$(SIM_EXE) $(SIM_ARGS) \
		$(MODEL_DIR)/*_rtl.v \
		-DCTOS_rtl -DCTOS_MODEL=rtl \
		-nclibdirpath work_rtl_sim \
		-l $(LOG_DIR)/rtl_sim.log
	grep '^Simulation successful!' $(LOG_DIR)/rtl_sim.log > /dev/null

