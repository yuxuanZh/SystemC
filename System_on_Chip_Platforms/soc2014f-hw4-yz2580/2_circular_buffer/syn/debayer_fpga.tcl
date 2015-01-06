
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
# ... complete ...
if {[get_design] != ""} then {
  close_design
}
new_design $MODULE\_$TECH
set_attr design_dir "$MODULE" [get_design]
set_attr header_files {../src/debayer.h ../src/wami_params.h} [get_design]
set_attr source_files {../src/debayer.cpp} [get_design]
set_attr top_module_path "$MODULE" [get_design]
set_attr build_flat true [get_design]
set_attr compile_flags "-w -I../src" [get_design]
define_clock -name clk -period 20000 -rise 0 -fall 10000
#define_clock -name clk -period 10000 -rise 0 -fall 5000

set_attr implementation_target FPGA [get_design]
set_attr fpga_install_path $XST_PATH [get_design]
set_attr fpga_target {"Xilinx" "virtex7" "xc7vx485t-2-ffg1761"} [get_design]
set_attr verilog_use_indexed_part_select false [get_design]


# Correct Makefile configuration is provided
define_sim_config -makefile_name ../ctos_sim_$TECH/Makefile -model_dir ./$MODEL \
    -simulator_args "-I../src/ -I../tb/ -sc_main -I../syn/$MODEL/ -D__CTOS__" \
    -testbench_files "../tb/wami_utils.cpp ../tb/debayer_tb.cpp ../tb/sc_main.cpp" \
    -testbench_kind self_checking -success_msg "Simulation successful!"


# Use low effort for faster scheduling
set_attr default_scheduling_effort "low" [get_design]

# This is necessary to ensure correctness of CMOS synthesized netlist! FGPA resets registers anyway.
set_attr reset_registers internal_and_outputs [get_design]

build
write_sim_makefile -overwrite

# Loops: due to restrictions on memory accesses, design space exploration with HLS
# knobs is not possible. You will improve the deabayer code to enable more DSE in
# the final mini project.
# ... complete ...
unroll_loop /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/$MODULE\_process_debayer/nodes/while_ln799
break_combinational_loop /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/$MODULE\_load_input/nodes/while_ln715
break_combinational_loop /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/$MODULE\_store_output/nodes/for_ln893

# Functions
# ... complete ...
inline /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/wami_debayer

# Arrays: Use Built-in memories which translate to BRAM for FPGA designs, which are dual-port memories
# ... complete ...

allocate_builtin_ram /designs/$MODULE\_$TECH/modules/$MODULE/arrays/debayer_img_pong_r
allocate_builtin_ram /designs/$MODULE\_$TECH/modules/$MODULE/arrays/debayer_img_pong_g
allocate_builtin_ram /designs/$MODULE\_$TECH/modules/$MODULE/arrays/debayer_img_pong_b
allocate_builtin_ram /designs/$MODULE\_$TECH/modules/$MODULE/arrays/debayer_img_ping_r
allocate_builtin_ram /designs/$MODULE\_$TECH/modules/$MODULE/arrays/debayer_img_ping_g
allocate_builtin_ram /designs/$MODULE\_$TECH/modules/$MODULE/arrays/debayer_img_ping_b

allocate_builtin_ram /designs/$MODULE\_$TECH/modules/$MODULE/arrays/bayer_img

# Schedule
# ... complete ...
set_attr relax_latency "true" /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/$MODULE\_config_debayer
set_attr relax_latency "true" /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/$MODULE\_load_input
set_attr relax_latency "true" /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/$MODULE\_store_output
set_attr relax_latency "true" /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/$MODULE\_process_debayer
set_attr relax_latency "true" /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/interp_G_at_RRR_or_G_at_BBB
set_attr relax_latency "true" /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/interp_R_at_GRB_or_B_at_GBR
set_attr relax_latency "true" /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/interp_R_at_GBR_or_B_at_GRB
set_attr relax_latency "true" /designs/$MODULE\_$TECH/modules/$MODULE/behaviors/interp_R_at_BBB_or_B_at_RRR
schedule -passes 200 /designs/$MODULE\_$TECH/modules/$MODULE
allocate_registers /designs/$MODULE\_$TECH/modules/$MODULE

#write rtl
write_sim -type verilog -suffix _final -dir ./$MODEL /designs/$MODULE\_$TECH/modules/$MODULE
write_rtl  -file ./$MODEL/$MODULE\_$TECH\_rtl.v /designs/$MODULE\_$TECH/modules/$MODULE

