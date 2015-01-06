# *****************************************************************************
#                         Cadence C-to-Silicon Compiler
#           Version 14.10-p100  (64 bit), build 50398 Tue, 27 May 2014
# 
# File created on Tue Nov 11 22:35:05 2014
# 
# The code contained herein is generated for Cadences customer and third
# parties authorized by customer.  It may be used in accordance with a
# previously executed license agreement between Cadence and that customer.
# Absolutely no disassembling, decompiling, reverse-translations or
# reverse-engineering of the generated code is allowed.
# 
# ******************************************************************************

if {[get_design] != ""} then {
  close_design
}
new_design debayer
set_attr design_dir "debayer" [get_design]
set_attr header_files {../src/debayer.h ../src/wami_params.h} [get_design]
set_attr source_files {../src/debayer.cpp} [get_design]
set_attr top_module_path "debayer" [get_design]
set_attr build_flat true [get_design]
set_attr compile_flags "-w -I../src" [get_design]
define_clock -name clock_0 -period 20000 -rise 0 -fall 10000
define_sim_config -makefile_name ../ctos_sim_unisim/Makefile -model_dir model_unisim -simulator_args "-I../src/ -I../tb/ -sc_main -I../syn/model_unisim/ -D__CTOS__" -testbench_files "../tb/debayer_tb.cpp ../tb/sc_main.cpp ../tb/wami_utils.cpp" -testbench_kind self_checking -success_msg "Done"
define_synth_config -run_dir "run_synth_gates" -standard_flow "default_synthesis_flow" -config_file_name ""
set_attr implementation_target FPGA [get_design]
set_attr fpga_install_path "/tools/xilinx/14.6/ISE_DS/ISE/bin/lin64/xst" [get_design]
set_attr fpga_target {"Xilinx" "virtex7"  "xc7vx485t-2-ffg1761"} [get_design]
set_attr verilog_use_indexed_part_select false [get_design]
set_attr verilog_pragma_keyword "synthesis" [get_design]
set_attr reset_registers "internal_and_outputs" [get_design]
