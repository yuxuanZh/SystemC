# *****************************************************************************
#
# DEBAYER accelerator scheduling script
#
# HLS tool          : Cadence CtoS 14.1
# Target Technology : Xilinx Virtex7
#
# Author            : Paolo Mantovani
# Affiliation       : Columbia University
# E-mail            : paolo @ cs . columbia . edu
#
# Last update       : 11/01/2014
#
# ******************************************************************************

#Prereq.
if { [file exist setup_locals.tcl] } {
  # this configuration file should define some path variables
  # - XILINX_PATH

  # DO NOT ADD IT TO THE GIT REPOSITORY!
  source setup_locals.tcl
} else {
  puts "Cannot find \"setup_locals.tcl\"."
  exit
}

set XST_PATH $XILINX_PATH/ISE/bin/lin64/xst

#Names parameters
set MODULE debayer
set TECH unisim
set PRAGMA synthesis
set MODEL model_$TECH

#Setup design
if {[get_design] != ""} then {
  close_design
}
new_design $MODULE\_$TECH
set_attr design_dir "$MODULE" [get_design]
set_attr header_files {../src_v2/debayer.h ../src_v2/wami_params.h} [get_design]
set_attr source_files {../src_v2/debayer.cpp} [get_design]
set_attr top_module_path "$MODULE" [get_design]
set_attr build_flat true [get_design]
#set_attr build_monolithic_structs true [get_design]
set_attr compile_flags "-w -I../src_v2" [get_design]

# Set minimum sustainable clock period!!!
define_clock -name clk -period 3100 -rise 0 -fall 1550

define_sim_config -makefile_name ../ctos_sim_$TECH\_v2/Makefile -model_dir ./$MODEL \
    -simulator_args "-I../src_v2/ -I../tb/ -sc_main -I../syn_v2/$MODEL/ -D__CTOS__" \
    -testbench_files "../tb/wami_utils.cpp ../tb/debayer_tb.cpp ../tb/sc_main.cpp" \
    -testbench_kind self_checking -success_msg "Simulation successful!"

define_synth_config -run_dir "run_synth_gates" -standard_flow "default_synthesis_flow" -config_file_name ""
set_attr implementation_target FPGA [get_design]
set_attr fpga_install_path $XST_PATH [get_design]
set_attr fpga_target {"Xilinx" "virtex7" "xc7vx485t-2-ffg1761"} [get_design]
set_attr verilog_use_indexed_part_select false [get_design]
#set_attr verilog_rtl_model_suffix "_$TECH\_rtl" [get_design]
#set_attr name_module_prefix "$MODULE\_$TECH\_" [get_design]
#set_attr verilog_pragma_keyword $PRAGMA [get_design]

set_attr default_scheduling_effort "low" [get_design]

# This is necessary to ensure correctness of CMOS synthesized netlist! FGPA resets registers anyway.
set_attr reset_registers internal_and_outputs [get_design]

build
write_sim_makefile -overwrite

# Loops
#break_combinational_loop /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/debayer_load_input/nodes/LOAD_INIT_LOOP_for_begin
#break_combinational_loop /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/interp_G_at_RRR_or_G_at_BBB/nodes/FOR_G_BBB_for_begin
# Functions
inline /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/wami_debayer

# Arrays: Use prototype memories with up to 10 ports. All ports must have synchornous-read
#flatten_array  /designs/debayer_unisim/modules/debayer/arrays/pos_tmp
#flatten_array  /designs/debayer_unisim/modules/debayer/arrays/neg_tmp
#flatten_array  /designs/debayer_unisim/modules/debayer/arrays/bayer_tmp
allocate_prototype_memory -interface_types {r r r r r r w w w w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/debayer_img_pingr
allocate_prototype_memory -interface_types {r r r r r r w w w w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/debayer_img_pingg
allocate_prototype_memory -interface_types {r r r r r r w w w w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/debayer_img_pingb
allocate_prototype_memory -interface_types {r r r r r r w w w w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/debayer_img_pongr
allocate_prototype_memory -interface_types {r r r r r r w w w w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/debayer_img_pongg
allocate_prototype_memory -interface_types {r r r r r r w w w w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/debayer_img_pongb
allocate_prototype_memory -interface_types {r r r r r r r r r w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/bayer_img

#allocate_prototype_memory -interface_types {r w w w w w w w w w} -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/pos_tmp
#allocate_prototype_memory -interface_types {r w w w w w w w w w} -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/neg_tmp
#allocate_prototype_memory -interface_types {r w w w w w w w w w} -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/bayer_tmp

#allocate_prototype_memory -interface_types {r w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/coeff

#allocate_prototype_memory -interface_types {r r w w w w w w w w } -clock /designs/debayer_unisim/modules/debayer/nets/clk /designs/debayer_unisim/modules/debayer/arrays/row_col



#Relax latency
#set_attr relax_latency "true" [get_design]

#schedule and allocate registers
schedule -passes 200 /designs/$MODULE\_$TECH/modules/$MODULE
allocate_registers /designs/$MODULE\_$TECH/modules/$MODULE

#write rtl
write_sim -type verilog -suffix _final -dir ./$MODEL /designs/$MODULE\_$TECH/modules/$MODULE

write_rtl  -file ./$MODEL/$MODULE\_$TECH\_rtl.v /designs/$MODULE\_$TECH/modules/$MODULE

#Report time and area
report_timing > timing.txt
report_area > area.txt
