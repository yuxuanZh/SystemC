// *****************************************************************************
//                         Cadence C-to-Silicon Compiler
//          Version 14.10-p100  (64 bit), build 50398 Tue, 27 May 2014
// 
// File created on Tue Nov 11 22:37:44 2014
// 
// The code contained herein is generated for Cadences customer and third
// parties authorized by customer.  It may be used in accordance with a
// previously executed license agreement between Cadence and that customer.
// Absolutely no disassembling, decompiling, reverse-translations or
// reverse-engineering of the generated code is allowed.
// 
//*****************************************************************************
// 
// **********************************************************
// SystemC structure port(s) --> Behavioral flattened representation
// **********************************************************
// bufdout.b_put_initiator_imp<rgb_pixel, SIG_TRAITS_pCLK_nRST, 1>::data.r maps to bufdout_data[15:0]
// bufdout.b_put_initiator_imp<rgb_pixel, SIG_TRAITS_pCLK_nRST, 1>::data.g maps to bufdout_data[31:16]
// bufdout.b_put_initiator_imp<rgb_pixel, SIG_TRAITS_pCLK_nRST, 1>::data.b maps to bufdout_data[47:32]

module debayer_post_build(clk, rst, rd_grant, wr_grant, conf_size, conf_done, 
bufdin_valid, bufdin_data, bufdout_ready, rd_index, rd_length, rd_request, 
wr_index, wr_length, wr_request, debayer_done, bufdin_ready, bufdout_valid, 
bufdout_data);
  input clk;
  input rst;
  input rd_grant;
  input wr_grant;
  input [31:0] conf_size;
  input conf_done;
  input bufdin_valid;
  input [15:0] bufdin_data;
  input bufdout_ready;
  output reg [31:0] rd_index;
  output reg [31:0] rd_length;
  output reg rd_request;
  output reg [31:0] wr_index;
  output reg [31:0] wr_length;
  output reg wr_request;
  output reg debayer_done;
  output reg bufdin_ready;
  output reg bufdout_valid;
  output reg [47:0] bufdout_data;
  reg [31:0] size;
  reg init_done;
  reg input_done;
  reg process_start;
  reg process_done;
  reg output_start;
  reg [15:0] bayer_img[0:12287];
  reg [15:0] debayer_img_ping_r[0:2043];
  reg [15:0] debayer_img_ping_g[0:2043];
  reg [15:0] debayer_img_ping_b[0:2043];
  reg [15:0] debayer_img_pong_r[0:2043];
  reg [15:0] debayer_img_pong_g[0:2043];
  reg [15:0] debayer_img_pong_b[0:2043];

  always begin : debayer_config_debayer
    reg [1:0] state_debayer_config_debayer;
    reg [1:0] joins_debayer_config_debayer;
    reg read_debayer_conf_done_ln541;
    reg [31:0] read_debayer_conf_size_ln542;

    joins_debayer_config_debayer = 2'h0;
    if (!rst) begin
      init_done <= 1'b0;
      size <= 32'h0;
      state_debayer_config_debayer <= 2'h0;
    end
    else 
      case (state_debayer_config_debayer)
        2'h0: // Wait_ln536
          joins_debayer_config_debayer = 2'h1;
        2'h1: // Wait_ln540
          begin
            read_debayer_conf_done_ln541 = conf_done;
            if (read_debayer_conf_done_ln541) begin
              read_debayer_conf_size_ln542 = conf_size;
              size <= read_debayer_conf_size_ln542;
              init_done <= 1'b1;
              joins_debayer_config_debayer = 2'h2;
            end
            else 
              joins_debayer_config_debayer = 2'h1;
          end
        2'h2: // Wait_ln550
          joins_debayer_config_debayer = 2'h2;
      endcase
    // do_ln540
    if (joins_debayer_config_debayer == 2'h1) begin
      joins_debayer_config_debayer = 2'h0;
      state_debayer_config_debayer <= 2'h1;
    end
    // while_ln549
    if (joins_debayer_config_debayer == 2'h2) begin
      joins_debayer_config_debayer = 2'h0;
      state_debayer_config_debayer <= 2'h2;
    end
    @(posedge clk);
  end
  always begin : debayer_load_input
    reg [3:0] state_debayer_load_input;
    reg [3:0] joins_debayer_load_input;
    reg lt_ln596;
    reg [31:0] add_ln596;
    reg [31:0] add_ln605;
    reg [31:0] add_ln606;
    reg [2:0] add_ln584;
    reg [31:0] mul_ln619;
    reg eq_ln619;
    reg gt_ln637;
    reg [31:0] sub_ln638;
    reg lt_ln641;
    reg [31:0] add_ln641;
    reg [31:0] add_ln647;
    reg [31:0] add_ln648;
    reg read_debayer_init_done_ln575;
    reg [31:0] read_debayer_size_ln576;
    reg [1:0] mux_k_ln584;
    reg signed [31:0] mux_row_counter_bayer_ln584;
    reg signed [31:0] mux_index_rd_ln584;
    reg read_debayer_rd_grant_ln589;
    reg read_debayer_rd_grant_ln593;
    reg signed [31:0] mux_i_ln596;
    reg [13:0] add_ln598;
    reg [31:0] read_debayer_rd_length_ln605;
    reg signed [31:0] mux_row_counter_bayer_ln614;
    reg signed [31:0] mux_index_rd_ln614;
    reg read_debayer_rd_grant_ln629;
    reg read_debayer_rd_grant_ln633;
    reg signed [31:0] mux_index_bayer_ln637;
    reg signed [31:0] mux_i_0_ln641;
    reg [13:0] add_ln642;
    reg [31:0] read_debayer_rd_length_ln647;
    reg read_debayer_process_start_ln664;
    reg [15:0] read_debayer_bufdin_data_ln238;
    reg read_debayer_bufdin_valid_ln237;
    reg [15:0] read_debayer_bufdin_data_ln238_0;
    reg read_debayer_bufdin_valid_ln237_0;

    joins_debayer_load_input = 4'h0;
    if (!rst) begin
      input_done <= 1'b0;
      bufdin_ready <= 1'b0;
      rd_length <= 32'h0;
      rd_index <= 32'h0;
      state_debayer_load_input <= 4'h0;
    end
    else 
      case (state_debayer_load_input)
        4'h0: // Wait_ln570
          joins_debayer_load_input = 4'h1;
        4'h1: // Wait_ln574
          begin
            read_debayer_init_done_ln575 = init_done;
            if (read_debayer_init_done_ln575) begin
              read_debayer_size_ln576 = size;
              rd_length <= read_debayer_size_ln576;
              mux_k_ln584 = 2'h0;
              mux_row_counter_bayer_ln584 = 32'sh0;
              mux_index_rd_ln584 = 32'sh0;
              joins_debayer_load_input = 4'h5;
            end
            else 
              joins_debayer_load_input = 4'h1;
          end
        4'h2: // Wait_ln588
          begin
            read_debayer_rd_grant_ln589 = rd_grant;
            if (read_debayer_rd_grant_ln589) begin
              rd_request <= 1'b0;
              joins_debayer_load_input = 4'h2;
            end
            else 
              joins_debayer_load_input = 4'h6;
          end
        4'h3: // Wait_ln592
          begin
            read_debayer_rd_grant_ln593 = rd_grant;
            if (read_debayer_rd_grant_ln593) 
              joins_debayer_load_input = 4'h2;
            else begin
              mux_i_ln596 = 32'sh0;
              joins_debayer_load_input = 4'h3;
            end
          end
        4'h4: // Wait_ln237
          begin
            read_debayer_bufdin_valid_ln237 = bufdin_valid;
            if (read_debayer_bufdin_valid_ln237) begin
              read_debayer_bufdin_data_ln238 = bufdin_data;
              bufdin_ready <= 1'b0;
              begin
                add_ln598 = mux_i_ln596[13:0] + {mux_row_counter_bayer_ln584[2:0], 
                11'h0};
                bayer_img[add_ln598] = read_debayer_bufdin_data_ln238;
                state_debayer_load_input <= 4'h5;
              end
            end
            else 
              joins_debayer_load_input = 4'h4;
          end
        4'h5: // Wait_ln601
          begin
            add_ln596 = $unsigned(mux_i_ln596) + 1'b1;
            begin
              mux_i_ln596 = $signed(add_ln596);
              joins_debayer_load_input = 4'h3;
            end
          end
        4'h6: // Wait_ln618
          begin
            mul_ln619 = read_debayer_size_ln576 * read_debayer_size_ln576;
            eq_ln619 = $unsigned(mux_index_rd_ln614) == mul_ln619;
            if (eq_ln619) 
              joins_debayer_load_input = 4'he;
            else begin
              rd_request <= 1'b1;
              joins_debayer_load_input = 4'h7;
            end
          end
        4'h7: // Wait_ln628
          begin
            read_debayer_rd_grant_ln629 = rd_grant;
            if (read_debayer_rd_grant_ln629) begin
              rd_request <= 1'b0;
              joins_debayer_load_input = 4'h8;
            end
            else 
              joins_debayer_load_input = 4'h7;
          end
        4'h8: // Wait_ln632
          begin
            read_debayer_rd_grant_ln633 = rd_grant;
            if (read_debayer_rd_grant_ln633) 
              joins_debayer_load_input = 4'h8;
            else begin
              mux_index_bayer_ln637 = mux_row_counter_bayer_ln614;
              joins_debayer_load_input = 4'h9;
            end
          end
        4'h9: // Wait_ln237_0
          begin
            read_debayer_bufdin_valid_ln237_0 = bufdin_valid;
            if (read_debayer_bufdin_valid_ln237_0) begin
              read_debayer_bufdin_data_ln238_0 = bufdin_data;
              bufdin_ready <= 1'b0;
              begin
                add_ln642 = mux_i_0_ln641[13:0] + {mux_index_bayer_ln637[2:0], 
                11'h0};
                bayer_img[add_ln642] = read_debayer_bufdin_data_ln238_0;
                state_debayer_load_input <= 4'ha;
              end
            end
            else 
              joins_debayer_load_input = 4'hb;
          end
        4'ha: // Wait_ln644
          begin
            add_ln641 = $unsigned(mux_i_0_ln641) + 1'b1;
            begin
              mux_i_0_ln641 = $signed(add_ln641);
              joins_debayer_load_input = 4'ha;
            end
          end
        4'hb: // Wait_ln663
          begin
            read_debayer_process_start_ln664 = process_start;
            if (read_debayer_process_start_ln664) begin
              input_done <= 1'b0;
              begin
                mux_row_counter_bayer_ln614 = $signed(add_ln648);
                mux_index_rd_ln614 = $signed(add_ln647);
                joins_debayer_load_input = 4'hd;
              end
            end
            else 
              joins_debayer_load_input = 4'hc;
          end
      endcase
    while (joins_debayer_load_input) 
      begin
        // do_ln574
        if (joins_debayer_load_input == 4'h1) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'h1;
        end
        // do_ln592
        if (joins_debayer_load_input == 4'h2) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'h3;
        end
        // for_ln596
        if (joins_debayer_load_input == 4'h3) begin
          joins_debayer_load_input = 4'h0;
          lt_ln596 = mux_i_ln596 < $signed(read_debayer_size_ln576);
          if (lt_ln596) begin
            bufdin_ready <= 1'b1;
            joins_debayer_load_input = 4'h4;
          end
          else begin
            read_debayer_rd_length_ln605 = rd_length;
            add_ln605 = $unsigned(mux_index_rd_ln584) + 
            read_debayer_rd_length_ln605;
            add_ln606 = $unsigned(mux_row_counter_bayer_ln584) + 1'b1;
            rd_index <= add_ln605;
            add_ln584 = mux_k_ln584 + 1'b1;
            if (add_ln584[2]) begin
              mux_row_counter_bayer_ln614 = $signed(add_ln606);
              mux_index_rd_ln614 = $signed(add_ln605);
              joins_debayer_load_input = 4'hd;
            end
            else begin
              mux_k_ln584 = add_ln584[1:0];
              mux_row_counter_bayer_ln584 = $signed(add_ln606);
              mux_index_rd_ln584 = $signed(add_ln605);
              joins_debayer_load_input = 4'h5;
            end
          end
        end
        // WAIT_B_GET_do_begin
        if (joins_debayer_load_input == 4'h4) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'h4;
        end
        // for_ln584
        if (joins_debayer_load_input == 4'h5) begin
          joins_debayer_load_input = 4'h0;
          rd_request <= 1'b1;
          joins_debayer_load_input = 4'h6;
        end
        // do_ln588
        if (joins_debayer_load_input == 4'h6) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'h2;
        end
        // do_ln628
        if (joins_debayer_load_input == 4'h7) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'h7;
        end
        // do_ln632
        if (joins_debayer_load_input == 4'h8) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'h8;
        end
        // while_ln637
        if (joins_debayer_load_input == 4'h9) begin
          joins_debayer_load_input = 4'h0;
          gt_ln637 = $signed(mux_index_bayer_ln637[31:1]) > 3'sh2;
          if (gt_ln637) begin
            sub_ln638 = $unsigned(mux_index_bayer_ln637) - 3'h6;
            begin
              mux_index_bayer_ln637 = $signed(sub_ln638);
              joins_debayer_load_input = 4'h9;
            end
          end
          else begin
            mux_i_0_ln641 = 32'sh0;
            joins_debayer_load_input = 4'ha;
          end
        end
        // for_ln641
        if (joins_debayer_load_input == 4'ha) begin
          joins_debayer_load_input = 4'h0;
          lt_ln641 = mux_i_0_ln641 < $signed(read_debayer_size_ln576);
          if (lt_ln641) begin
            bufdin_ready <= 1'b1;
            joins_debayer_load_input = 4'hb;
          end
          else begin
            read_debayer_rd_length_ln647 = rd_length;
            add_ln647 = $unsigned(mux_index_rd_ln614) + 
            read_debayer_rd_length_ln647;
            add_ln648 = $unsigned(mux_row_counter_bayer_ln614) + 1'b1;
            rd_index <= add_ln647;
            input_done <= 1'b1;
            joins_debayer_load_input = 4'hc;
          end
        end
        // WAIT_B_GET_do_begin_0
        if (joins_debayer_load_input == 4'hb) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'h9;
        end
        // do_ln663
        if (joins_debayer_load_input == 4'hc) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'hb;
        end
        // LOAD_INPUT_WHILE_while_begin
        if (joins_debayer_load_input == 4'hd) begin
          joins_debayer_load_input = 4'h0;
          joins_debayer_load_input = 4'he;
        end
        // do_ln618
        if (joins_debayer_load_input == 4'he) begin
          joins_debayer_load_input = 4'h0;
          state_debayer_load_input <= 4'h6;
        end
      end
    @(posedge clk);
  end
  always begin : debayer_store_output
    reg [3:0] state_debayer_store_output;
    reg [3:0] joins_debayer_store_output;
    reg [31:0] sub_ln773;
    reg [31:0] mul_ln780;
    reg eq_ln780;
    reg if_ln780;
    reg lt_ln815;
    reg if_ln819;
    reg [31:0] add_ln815;
    reg [31:0] add_ln828;
    reg read_debayer_init_done_ln771;
    reg [31:0] read_debayer_size_ln772;
    reg mux_ping_ln776;
    reg signed [31:0] mux_index_wr_ln776;
    reg read_debayer_process_done_ln788;
    reg read_debayer_process_done_ln796;
    reg read_debayer_wr_grant_ln804;
    reg read_debayer_wr_grant_ln810;
    reg signed [31:0] mux_j_ln815;
    reg [15:0] memread_debayer_debayer_img_ping_r_ln820;
    reg [15:0] memread_debayer_debayer_img_ping_g_ln820;
    reg [15:0] memread_debayer_debayer_img_ping_b_ln820;
    reg [15:0] memread_debayer_debayer_img_pong_r_ln825;
    reg [15:0] memread_debayer_debayer_img_pong_g_ln825;
    reg [15:0] memread_debayer_debayer_img_pong_b_ln825;
    reg [31:0] read_debayer_wr_length_ln828;
    reg read_debayer_bufdout_ready_ln222;
    reg read_debayer_bufdout_ready_ln222_0;

    joins_debayer_store_output = 4'h0;
    if (!rst) begin
      wr_index <= 32'h0;
      wr_length <= 32'h0;
      output_start <= 1'b0;
      debayer_done <= 1'b0;
      bufdout_valid <= 1'b0;
      state_debayer_store_output <= 4'h0;
    end
    else 
      case (state_debayer_store_output)
        4'h0: // Wait_ln766
          joins_debayer_store_output = 4'h1;
        4'h1: // Wait_ln770
          begin
            read_debayer_init_done_ln771 = init_done;
            if (read_debayer_init_done_ln771) begin
              read_debayer_size_ln772 = size;
              sub_ln773 = read_debayer_size_ln772 - 3'h4;
              wr_length <= sub_ln773;
              mul_ln780 = sub_ln773 * sub_ln773;
              mux_ping_ln776 = 1'b0;
              mux_index_wr_ln776 = 32'sh0;
              joins_debayer_store_output = 4'h7;
            end
            else 
              joins_debayer_store_output = 4'h1;
          end
        4'h2: // Wait_ln781
          begin
            if (if_ln780) 
              joins_debayer_store_output = 4'h8;
            else begin
              if (if_ln780) 
                debayer_done <= 1'b1;
              joins_debayer_store_output = 4'h2;
            end
          end
        4'h3: // Wait_ln787
          begin
            read_debayer_process_done_ln788 = process_done;
            if (read_debayer_process_done_ln788) begin
              output_start <= 1'b1;
              joins_debayer_store_output = 4'h3;
            end
            else 
              joins_debayer_store_output = 4'h2;
          end
        4'h4: // Wait_ln795
          begin
            read_debayer_process_done_ln796 = process_done;
            if (read_debayer_process_done_ln796) 
              joins_debayer_store_output = 4'h3;
            else begin
              output_start <= 1'b0;
              wr_request <= 1'b1;
              joins_debayer_store_output = 4'h4;
            end
          end
        4'h5: // Wait_ln803
          begin
            read_debayer_wr_grant_ln804 = wr_grant;
            if (read_debayer_wr_grant_ln804) begin
              wr_request <= 1'b0;
              joins_debayer_store_output = 4'h5;
            end
            else 
              joins_debayer_store_output = 4'h4;
          end
        4'h6: // Wait_ln809
          begin
            read_debayer_wr_grant_ln810 = wr_grant;
            if (read_debayer_wr_grant_ln810) 
              joins_debayer_store_output = 4'h5;
            else begin
              mux_j_ln815 = 32'sh0;
              joins_debayer_store_output = 4'h6;
            end
          end
        4'h7: // Wait_ln222
          begin
            read_debayer_bufdout_ready_ln222 = bufdout_ready;
            if (read_debayer_bufdout_ready_ln222) begin
              bufdout_valid <= 1'b0;
              joins_debayer_store_output = 4'ha;
            end
            else 
              joins_debayer_store_output = 4'h9;
          end
        4'h8: // Wait_ln222_0
          begin
            read_debayer_bufdout_ready_ln222_0 = bufdout_ready;
            if (read_debayer_bufdout_ready_ln222_0) begin
              bufdout_valid <= 1'b0;
              joins_debayer_store_output = 4'hc;
            end
            else 
              joins_debayer_store_output = 4'hb;
          end
      endcase
    while (joins_debayer_store_output) 
      begin
        // do_ln770
        if (joins_debayer_store_output == 4'h1) begin
          joins_debayer_store_output = 4'h0;
          state_debayer_store_output <= 4'h1;
        end
        // do_ln787
        if (joins_debayer_store_output == 4'h2) begin
          joins_debayer_store_output = 4'h0;
          state_debayer_store_output <= 4'h3;
        end
        // do_ln795
        if (joins_debayer_store_output == 4'h3) begin
          joins_debayer_store_output = 4'h0;
          state_debayer_store_output <= 4'h4;
        end
        // do_ln803
        if (joins_debayer_store_output == 4'h4) begin
          joins_debayer_store_output = 4'h0;
          state_debayer_store_output <= 4'h5;
        end
        // do_ln809
        if (joins_debayer_store_output == 4'h5) begin
          joins_debayer_store_output = 4'h0;
          state_debayer_store_output <= 4'h6;
        end
        // for_ln815
        if (joins_debayer_store_output == 4'h6) begin
          joins_debayer_store_output = 4'h0;
          lt_ln815 = mux_j_ln815 < $signed(sub_ln773);
          if (lt_ln815) 
            if (if_ln819) begin
              memread_debayer_debayer_img_ping_r_ln820 = debayer_img_ping_r[
              mux_j_ln815[10:0]];
              memread_debayer_debayer_img_ping_g_ln820 = debayer_img_ping_g[
              mux_j_ln815[10:0]];
              memread_debayer_debayer_img_ping_b_ln820 = debayer_img_ping_b[
              mux_j_ln815[10:0]];
              begin
                bufdout_data <= {memread_debayer_debayer_img_ping_b_ln820, 
                memread_debayer_debayer_img_ping_g_ln820, 
                memread_debayer_debayer_img_ping_r_ln820};
                bufdout_valid <= 1'b1;
                joins_debayer_store_output = 4'h9;
              end
            end
            else 
              joins_debayer_store_output = 4'ha;
          else begin
            read_debayer_wr_length_ln828 = wr_length;
            add_ln828 = $unsigned(mux_index_wr_ln776) + 
            read_debayer_wr_length_ln828;
            wr_index <= add_ln828;
            begin
              mux_ping_ln776 = !if_ln819;
              mux_index_wr_ln776 = $signed(add_ln828);
              joins_debayer_store_output = 4'h7;
            end
          end
        end
        // STORE_OUTPUT_WHILE_while_begin
        if (joins_debayer_store_output == 4'h7) begin
          joins_debayer_store_output = 4'h0;
          if_ln819 = mux_ping_ln776;
          eq_ln780 = $unsigned(mux_index_wr_ln776) == mul_ln780;
          if_ln780 = eq_ln780;
          if (if_ln780) 
            debayer_done <= 1'b1;
          joins_debayer_store_output = 4'h8;
        end
        // do_ln781
        if (joins_debayer_store_output == 4'h8) begin
          joins_debayer_store_output = 4'h0;
          state_debayer_store_output <= 4'h2;
        end
        // WAIT_B_PUT_do_begin
        if (joins_debayer_store_output == 4'h9) begin
          joins_debayer_store_output = 4'h0;
          state_debayer_store_output <= 4'h7;
        end
        // ifBot_ln819
        if (joins_debayer_store_output == 4'ha) begin
          joins_debayer_store_output = 4'h0;
          if (!if_ln819) begin
            memread_debayer_debayer_img_pong_r_ln825 = debayer_img_pong_r[
            mux_j_ln815[10:0]];
            memread_debayer_debayer_img_pong_g_ln825 = debayer_img_pong_g[
            mux_j_ln815[10:0]];
            memread_debayer_debayer_img_pong_b_ln825 = debayer_img_pong_b[
            mux_j_ln815[10:0]];
            begin
              bufdout_data <= {memread_debayer_debayer_img_pong_b_ln825, 
              memread_debayer_debayer_img_pong_g_ln825, 
              memread_debayer_debayer_img_pong_r_ln825};
              bufdout_valid <= 1'b1;
              joins_debayer_store_output = 4'hb;
            end
          end
          else 
            joins_debayer_store_output = 4'hc;
        end
        // WAIT_B_PUT_do_begin_0
        if (joins_debayer_store_output == 4'hb) begin
          joins_debayer_store_output = 4'h0;
          state_debayer_store_output <= 4'h8;
        end
        // ifBot_ln824
        if (joins_debayer_store_output == 4'hc) begin
          joins_debayer_store_output = 4'h0;
          add_ln815 = $unsigned(mux_j_ln815) + 1'b1;
          begin
            mux_j_ln815 = $signed(add_ln815);
            joins_debayer_store_output = 4'h6;
          end
        end
      end
    @(posedge clk);
  end
  always begin : debayer_process_debayer
    reg [2:0] state_debayer_process_debayer;
    reg [3:0] joins_debayer_process_debayer;
    reg [31:0] mul_ln698;
    reg eq_ln698;
    reg [31:0] add_ln719;
    reg gt_ln721;
    reg [31:0] sub_ln722;
    reg wami_debayer_ln728;
    reg unary_nor_ln729;
    reg [31:0] add_ln732;
    reg read_debayer_init_done_ln690;
    reg [31:0] read_debayer_size_ln691;
    reg signed [31:0] mux_row_counter_debayer_ln694;
    reg mux_ping_ln694;
    reg signed [31:0] mux_index_rd_ln694;
    reg read_debayer_input_done_ln702;
    reg [31:0] read_debayer_rd_index_ln708;
    reg read_debayer_input_done_ln711;
    reg signed [31:0] mux_cb_row_ln721;
    reg read_debayer_output_start_ln745;

    joins_debayer_process_debayer = 4'h0;
    if (!rst) begin
      process_start <= 1'b0;
      process_done <= 1'b0;
      state_debayer_process_debayer <= 3'h0;
    end
    else 
      case (state_debayer_process_debayer)
        3'h0: // Wait_ln685
          joins_debayer_process_debayer = 4'h1;
        3'h1: // Wait_ln689
          begin
            read_debayer_init_done_ln690 = init_done;
            if (read_debayer_init_done_ln690) begin
              read_debayer_size_ln691 = size;
              mux_row_counter_debayer_ln694 = 32'sh0;
              mux_ping_ln694 = 1'b0;
              mux_index_rd_ln694 = 32'sh0;
              joins_debayer_process_debayer = 4'h5;
            end
            else 
              joins_debayer_process_debayer = 4'h1;
          end
        3'h2: // Wait_ln697
          begin
            mul_ln698 = read_debayer_size_ln691 * read_debayer_size_ln691;
            eq_ln698 = $unsigned(mux_index_rd_ln694) == mul_ln698;
            if (eq_ln698) 
              joins_debayer_process_debayer = 4'h6;
            else 
              joins_debayer_process_debayer = 4'h2;
          end
        3'h3: // Wait_ln701
          begin
            read_debayer_input_done_ln702 = input_done;
            if (read_debayer_input_done_ln702) begin
              read_debayer_rd_index_ln708 = rd_index;
              process_start <= 1'b1;
              joins_debayer_process_debayer = 4'h3;
            end
            else 
              joins_debayer_process_debayer = 4'h2;
          end
        3'h4: // Wait_ln710
          begin
            read_debayer_input_done_ln711 = input_done;
            if (read_debayer_input_done_ln711) 
              joins_debayer_process_debayer = 4'h3;
            else begin
              process_start <= 1'b0;
              add_ln719 = $unsigned(mux_row_counter_debayer_ln694) + 2'h2;
              mux_cb_row_ln721 = $signed(add_ln719);
              joins_debayer_process_debayer = 4'h4;
            end
          end
        3'h5: // Wait_ln744
          begin
            read_debayer_output_start_ln745 = output_start;
            if (read_debayer_output_start_ln745) begin
              process_done <= 1'b0;
              begin
                mux_row_counter_debayer_ln694 = $signed(add_ln732);
                mux_ping_ln694 = unary_nor_ln729;
                mux_index_rd_ln694 = $signed(read_debayer_rd_index_ln708);
                joins_debayer_process_debayer = 4'h5;
              end
            end
            else 
              joins_debayer_process_debayer = 4'h8;
          end
      endcase
    while (joins_debayer_process_debayer) 
      begin
        // do_ln689
        if (joins_debayer_process_debayer == 4'h1) begin
          joins_debayer_process_debayer = 4'h0;
          state_debayer_process_debayer <= 3'h1;
        end
        // do_ln701
        if (joins_debayer_process_debayer == 4'h2) begin
          joins_debayer_process_debayer = 4'h0;
          state_debayer_process_debayer <= 3'h3;
        end
        // do_ln710
        if (joins_debayer_process_debayer == 4'h3) begin
          joins_debayer_process_debayer = 4'h0;
          state_debayer_process_debayer <= 3'h4;
        end
        // while_ln721
        if (joins_debayer_process_debayer == 4'h4) begin
          joins_debayer_process_debayer = 4'h0;
          gt_ln721 = $signed(mux_cb_row_ln721[31:1]) > 3'sh2;
          if (gt_ln721) begin
            sub_ln722 = $unsigned(mux_cb_row_ln721) - 3'h6;
            begin
              mux_cb_row_ln721 = $signed(sub_ln722);
              joins_debayer_process_debayer = 4'h4;
            end
          end
          else 
            joins_debayer_process_debayer = 4'h7;
        end
        // DEBAYER_WHILE_while_begin
        if (joins_debayer_process_debayer == 4'h5) begin
          joins_debayer_process_debayer = 4'h0;
          joins_debayer_process_debayer = 4'h6;
        end
        // do_ln697
        if (joins_debayer_process_debayer == 4'h6) begin
          joins_debayer_process_debayer = 4'h0;
          state_debayer_process_debayer <= 3'h2;
        end
        // wami_debayer_funcCallJoin_ln728
        if (joins_debayer_process_debayer == 4'h7) begin
          joins_debayer_process_debayer = 4'h0;
          wami_debayer($signed(read_debayer_size_ln691), $signed(
          mux_row_counter_debayer_ln694[0]), mux_cb_row_ln721, mux_ping_ln694, 
          wami_debayer_ln728);
          begin
            unary_nor_ln729 = !mux_ping_ln694;
            add_ln732 = $unsigned(mux_row_counter_debayer_ln694) + 1'b1;
            process_done <= 1'b1;
            joins_debayer_process_debayer = 4'h8;
          end
        end
        // do_ln744
        if (joins_debayer_process_debayer == 4'h8) begin
          joins_debayer_process_debayer = 4'h0;
          state_debayer_process_debayer <= 3'h5;
        end
      end
    @(posedge clk);
  end
  task wami_debayer;
    input signed [31:0] cols_in;
    input signed img_row_in;
    input signed [31:0] cb_row_in;
    input ping_in;
    output reg CtoS_func_done_wami_debayer;
    reg [4:0] state_wami_debayer;
    reg [4:0] state_wami_debayer_next;
    reg [5:0] joins_wami_debayer;
    reg signed [31:0] read_cols_in;
    reg signed read_img_row_in;
    reg signed [31:0] read_cb_row_in;
    reg read_ping_in;
    reg [31:0] sub_ln342;
    reg lt_ln342;
    reg [10:0] sub_ln345;
    reg [31:0] add_ln342;
    reg lt_ln359;
    reg [10:0] sub_ln362;
    reg [31:0] add_ln359;
    reg lt_ln375;
    reg [10:0] sub_ln378;
    reg [31:0] add_ln375;
    reg lt_ln390;
    reg [10:0] sub_ln393;
    reg [31:0] add_ln390;
    reg lt_ln407;
    reg [13:0] interp_G_at_RRR_or_G_at_BBB_ln410_interp_G_at_RRR_or_G_at_BBB_out;
    reg interp_G_at_RRR_or_G_at_BBB_ln410_CtoS_func_done;
    reg [10:0] sub_ln410;
    reg [31:0] add_ln407;
    reg [13:0] interp_G_at_RRR_or_G_at_BBB_ln412_interp_G_at_RRR_or_G_at_BBB_out;
    reg interp_G_at_RRR_or_G_at_BBB_ln412_CtoS_func_done;
    reg [10:0] sub_ln412;
    reg lt_ln424;
    reg [13:0] interp_G_at_RRR_or_G_at_BBB_ln427_interp_G_at_RRR_or_G_at_BBB_out;
    reg interp_G_at_RRR_or_G_at_BBB_ln427_CtoS_func_done;
    reg [10:0] sub_ln427;
    reg [31:0] add_ln424;
    reg [13:0] interp_G_at_RRR_or_G_at_BBB_ln429_interp_G_at_RRR_or_G_at_BBB_out;
    reg interp_G_at_RRR_or_G_at_BBB_ln429_CtoS_func_done;
    reg [10:0] sub_ln429;
    reg lt_ln439;
    reg [13:0] interp_R_at_GRB_or_B_at_GBR_ln442_interp_R_at_GRB_or_B_at_GBR_out;
    reg interp_R_at_GRB_or_B_at_GBR_ln442_CtoS_func_done;
    reg [10:0] sub_ln442;
    reg [31:0] add_ln439;
    reg [13:0] interp_R_at_GRB_or_B_at_GBR_ln444_interp_R_at_GRB_or_B_at_GBR_out;
    reg interp_R_at_GRB_or_B_at_GBR_ln444_CtoS_func_done;
    reg [10:0] sub_ln444;
    reg lt_ln454;
    reg [13:0] interp_R_at_GRB_or_B_at_GBR_ln457_interp_R_at_GRB_or_B_at_GBR_out;
    reg interp_R_at_GRB_or_B_at_GBR_ln457_CtoS_func_done;
    reg [10:0] sub_ln457;
    reg [31:0] add_ln454;
    reg [13:0] interp_R_at_GRB_or_B_at_GBR_ln459_interp_R_at_GRB_or_B_at_GBR_out;
    reg interp_R_at_GRB_or_B_at_GBR_ln459_CtoS_func_done;
    reg [10:0] sub_ln459;
    reg lt_ln469;
    reg [13:0] interp_R_at_GBR_or_B_at_GRB_ln472_interp_R_at_GBR_or_B_at_GRB_out;
    reg interp_R_at_GBR_or_B_at_GRB_ln472_CtoS_func_done;
    reg [10:0] sub_ln472;
    reg [31:0] add_ln469;
    reg [13:0] interp_R_at_GBR_or_B_at_GRB_ln474_interp_R_at_GBR_or_B_at_GRB_out;
    reg interp_R_at_GBR_or_B_at_GRB_ln474_CtoS_func_done;
    reg [10:0] sub_ln474;
    reg lt_ln484;
    reg [13:0] interp_R_at_GBR_or_B_at_GRB_ln487_interp_R_at_GBR_or_B_at_GRB_out;
    reg interp_R_at_GBR_or_B_at_GRB_ln487_CtoS_func_done;
    reg [10:0] sub_ln487;
    reg [31:0] add_ln484;
    reg [13:0] interp_R_at_GBR_or_B_at_GRB_ln489_interp_R_at_GBR_or_B_at_GRB_out;
    reg interp_R_at_GBR_or_B_at_GRB_ln489_CtoS_func_done;
    reg [10:0] sub_ln489;
    reg lt_ln500;
    reg [13:0] interp_R_at_BBB_or_B_at_RRR_ln503_interp_R_at_BBB_or_B_at_RRR_out;
    reg interp_R_at_BBB_or_B_at_RRR_ln503_CtoS_func_done;
    reg [10:0] sub_ln503;
    reg [31:0] add_ln500;
    reg [13:0] interp_R_at_BBB_or_B_at_RRR_ln505_interp_R_at_BBB_or_B_at_RRR_out;
    reg interp_R_at_BBB_or_B_at_RRR_ln505_CtoS_func_done;
    reg [10:0] sub_ln505;
    reg lt_ln515;
    reg [13:0] interp_R_at_BBB_or_B_at_RRR_ln518_interp_R_at_BBB_or_B_at_RRR_out;
    reg interp_R_at_BBB_or_B_at_RRR_ln518_CtoS_func_done;
    reg [10:0] sub_ln518;
    reg [31:0] add_ln515;
    reg [13:0] interp_R_at_BBB_or_B_at_RRR_ln520_interp_R_at_BBB_or_B_at_RRR_out;
    reg interp_R_at_BBB_or_B_at_RRR_ln520_CtoS_func_done;
    reg [10:0] sub_ln520;
    reg [31:0] mux_col_ln342;
    reg [13:0] add_ln345;
    reg [15:0] memread_debayer_bayer_img_ln345;
    reg [15:0] memread_debayer_bayer_img_ln347;
    reg [31:0] mux_col_ln359;
    reg [13:0] add_ln362;
    reg [15:0] memread_debayer_bayer_img_ln362;
    reg [15:0] memread_debayer_bayer_img_ln364;
    reg [31:0] mux_col_ln375;
    reg [13:0] add_ln378;
    reg [15:0] memread_debayer_bayer_img_ln378;
    reg [15:0] memread_debayer_bayer_img_ln380;
    reg [31:0] mux_col_ln390;
    reg [13:0] add_ln393;
    reg [15:0] memread_debayer_bayer_img_ln393;
    reg [15:0] memread_debayer_bayer_img_ln395;
    reg [31:0] mux_col_ln407;
    reg [31:0] mux_col_ln424;
    reg [31:0] mux_col_ln439;
    reg [31:0] mux_col_ln454;
    reg [31:0] mux_col_ln469;
    reg [31:0] mux_col_ln484;
    reg [31:0] mux_col_ln500;
    reg [31:0] mux_col_ln515;
    reg ctrlAnd_for_ln342;
    reg ctrlAnd_for_ln359;
    reg ctrlAnd_for_ln375;
    reg ctrlAnd_for_ln390;
    reg ctrlAnd_for_ln407;
    reg ctrlAnd_for_ln424;
    reg ctrlAnd_for_ln454;
    reg ctrlAnd_for_ln469;
    reg ctrlAnd_for_ln484;
    reg ctrlAnd_for_ln500;
    reg ctrlAnd_for_ln515;

    begin : wami_debayer_body
      joins_wami_debayer = 6'h0;
      CtoS_func_done_wami_debayer = 1'b0;
      begin
        read_cols_in = cols_in;
        read_img_row_in = img_row_in;
        read_cb_row_in = cb_row_in;
        read_ping_in = ping_in;
        sub_ln342 = $unsigned(read_cols_in) - 2'h2;
        mux_col_ln342 = 32'h2;
        joins_wami_debayer = 6'h1;
      end
      // This value for state is interpreted as undefined
      state_wami_debayer = 5'h1c;
      while (!CtoS_func_done_wami_debayer) 
        begin
          case (state_wami_debayer)
            5'h0: // Wait_ln348
              begin
                add_ln342 = mux_col_ln342 + 2'h2;
                begin
                  mux_col_ln342 = add_ln342;
                  joins_wami_debayer = 6'h1;
                end
              end
            5'h1: // Wait_ln365
              begin
                add_ln359 = mux_col_ln359 + 2'h2;
                begin
                  mux_col_ln359 = add_ln359;
                  joins_wami_debayer = 6'h2;
                end
              end
            5'h2: // Wait_ln381
              begin
                add_ln375 = mux_col_ln375 + 2'h2;
                begin
                  mux_col_ln375 = add_ln375;
                  joins_wami_debayer = 6'h3;
                end
              end
            5'h3: // Wait_ln396
              begin
                add_ln390 = mux_col_ln390 + 2'h2;
                begin
                  mux_col_ln390 = add_ln390;
                  joins_wami_debayer = 6'h4;
                end
              end
            5'h5: // Wait_ln413
              begin
                add_ln407 = mux_col_ln407 + 2'h2;
                begin
                  mux_col_ln407 = add_ln407;
                  joins_wami_debayer = 6'h5;
                end
              end
            5'h8: // Wait_ln430
              begin
                add_ln424 = mux_col_ln424 + 2'h2;
                begin
                  mux_col_ln424 = add_ln424;
                  joins_wami_debayer = 6'h9;
                end
              end
            5'hb: // Wait_ln445
              begin
                add_ln439 = mux_col_ln439 + 2'h2;
                begin
                  mux_col_ln439 = add_ln439;
                  joins_wami_debayer = 6'hd;
                end
              end
            5'he: // Wait_ln460
              begin
                add_ln454 = mux_col_ln454 + 2'h2;
                begin
                  mux_col_ln454 = add_ln454;
                  joins_wami_debayer = 6'h11;
                end
              end
            5'h11: // Wait_ln475
              begin
                add_ln469 = mux_col_ln469 + 2'h2;
                begin
                  mux_col_ln469 = add_ln469;
                  joins_wami_debayer = 6'h15;
                end
              end
            5'h14: // Wait_ln490
              begin
                add_ln484 = mux_col_ln484 + 2'h2;
                begin
                  mux_col_ln484 = add_ln484;
                  joins_wami_debayer = 6'h19;
                end
              end
            5'h17: // Wait_ln506
              begin
                add_ln500 = mux_col_ln500 + 2'h2;
                begin
                  mux_col_ln500 = add_ln500;
                  joins_wami_debayer = 6'h1d;
                end
              end
            5'h1a: // Wait_ln521
              begin
                add_ln515 = mux_col_ln515 + 2'h2;
                begin
                  mux_col_ln515 = add_ln515;
                  joins_wami_debayer = 6'h21;
                end
              end
          endcase
          while (joins_wami_debayer) 
            begin
              // for_ln342
              if (joins_wami_debayer == 6'h1) begin
                joins_wami_debayer = 6'h0;
                if (!$unsigned(read_img_row_in)) 
                  lt_ln342 = mux_col_ln342 < sub_ln342;
                ctrlAnd_for_ln342 = &{lt_ln342, !$unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln342) begin
                  add_ln345 = mux_col_ln342[13:0] + {read_cb_row_in[2:0], 11'h0};
                  if (!read_ping_in) 
                    memread_debayer_bayer_img_ln345 = bayer_img[add_ln345];
                  sub_ln345 = mux_col_ln342[10:0] - 2'h2;
                  if (!read_ping_in) 
                    debayer_img_pong_r[sub_ln345] = 
                    memread_debayer_bayer_img_ln345;
                  if (read_ping_in) begin
                    memread_debayer_bayer_img_ln347 = bayer_img[add_ln345];
                    debayer_img_ping_r[sub_ln345] = 
                    memread_debayer_bayer_img_ln347;
                  end
                  state_wami_debayer_next = 5'h0;
                end
                else begin
                  mux_col_ln359 = 32'h3;
                  joins_wami_debayer = 6'h2;
                end
              end
              // for_ln359
              if (joins_wami_debayer == 6'h2) begin
                joins_wami_debayer = 6'h0;
                if (!$unsigned(read_img_row_in)) 
                  lt_ln359 = mux_col_ln359 < sub_ln342;
                ctrlAnd_for_ln359 = &{lt_ln359, !$unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln359) begin
                  add_ln362 = mux_col_ln359[13:0] + {read_cb_row_in[2:0], 11'h0};
                  if (!read_ping_in) 
                    memread_debayer_bayer_img_ln362 = bayer_img[add_ln362];
                  sub_ln362 = mux_col_ln359[10:0] - 2'h2;
                  if (!read_ping_in) 
                    debayer_img_pong_g[sub_ln362] = 
                    memread_debayer_bayer_img_ln362;
                  if (read_ping_in) begin
                    memread_debayer_bayer_img_ln364 = bayer_img[add_ln362];
                    debayer_img_ping_g[sub_ln362] = 
                    memread_debayer_bayer_img_ln364;
                  end
                  state_wami_debayer_next = 5'h1;
                end
                else begin
                  mux_col_ln375 = 32'h2;
                  joins_wami_debayer = 6'h3;
                end
              end
              // for_ln375
              if (joins_wami_debayer == 6'h3) begin
                joins_wami_debayer = 6'h0;
                if ($unsigned(read_img_row_in)) 
                  lt_ln375 = mux_col_ln375 < sub_ln342;
                ctrlAnd_for_ln375 = &{lt_ln375, $unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln375) begin
                  add_ln378 = mux_col_ln375[13:0] + {read_cb_row_in[2:0], 11'h0};
                  if (!read_ping_in) 
                    memread_debayer_bayer_img_ln378 = bayer_img[add_ln378];
                  sub_ln378 = mux_col_ln375[10:0] - 2'h2;
                  if (!read_ping_in) 
                    debayer_img_pong_g[sub_ln378] = 
                    memread_debayer_bayer_img_ln378;
                  if (read_ping_in) begin
                    memread_debayer_bayer_img_ln380 = bayer_img[add_ln378];
                    debayer_img_ping_g[sub_ln378] = 
                    memread_debayer_bayer_img_ln380;
                  end
                  state_wami_debayer_next = 5'h2;
                end
                else begin
                  mux_col_ln390 = 32'h3;
                  joins_wami_debayer = 6'h4;
                end
              end
              // for_ln390
              if (joins_wami_debayer == 6'h4) begin
                joins_wami_debayer = 6'h0;
                if ($unsigned(read_img_row_in)) 
                  lt_ln390 = mux_col_ln390 < sub_ln342;
                ctrlAnd_for_ln390 = &{lt_ln390, $unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln390) begin
                  add_ln393 = mux_col_ln390[13:0] + {read_cb_row_in[2:0], 11'h0};
                  if (!read_ping_in) 
                    memread_debayer_bayer_img_ln393 = bayer_img[add_ln393];
                  sub_ln393 = mux_col_ln390[10:0] - 2'h2;
                  if (!read_ping_in) 
                    debayer_img_pong_b[sub_ln393] = 
                    memread_debayer_bayer_img_ln393;
                  if (read_ping_in) begin
                    memread_debayer_bayer_img_ln395 = bayer_img[add_ln393];
                    debayer_img_ping_b[sub_ln393] = 
                    memread_debayer_bayer_img_ln395;
                  end
                  state_wami_debayer_next = 5'h3;
                end
                else begin
                  mux_col_ln407 = 32'h2;
                  joins_wami_debayer = 6'h5;
                end
              end
              // for_ln407
              if (joins_wami_debayer == 6'h5) begin
                joins_wami_debayer = 6'h0;
                if (!$unsigned(read_img_row_in)) 
                  lt_ln407 = mux_col_ln407 < sub_ln342;
                ctrlAnd_for_ln407 = &{lt_ln407, !$unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln407) 
                  if (read_ping_in) 
                    joins_wami_debayer = 6'h8;
                  else 
                    joins_wami_debayer = 6'h6;
                else begin
                  mux_col_ln424 = 32'h3;
                  joins_wami_debayer = 6'h9;
                end
              end
              // interp_G_at_RRR_or_G_at_BBB_funcCallJoin_ln410
              if (joins_wami_debayer == 6'h6) begin
                joins_wami_debayer = 6'h0;
                interp_G_at_RRR_or_G_at_BBB($unsigned(read_cb_row_in), 
                mux_col_ln407[13:0], 
                interp_G_at_RRR_or_G_at_BBB_ln410_interp_G_at_RRR_or_G_at_BBB_out, 
                interp_G_at_RRR_or_G_at_BBB_ln410_CtoS_func_done);
                sub_ln410 = mux_col_ln407[10:0] - 2'h2;
                debayer_img_pong_g[sub_ln410] = {{2{
                interp_G_at_RRR_or_G_at_BBB_ln410_interp_G_at_RRR_or_G_at_BBB_out
                [13]}}, 
                interp_G_at_RRR_or_G_at_BBB_ln410_interp_G_at_RRR_or_G_at_BBB_out};
                joins_wami_debayer = 6'h7;
              end
              // ifBot_ln409
              if (joins_wami_debayer == 6'h7) begin
                joins_wami_debayer = 6'h0;
                state_wami_debayer_next = 5'h5;
              end
              // interp_G_at_RRR_or_G_at_BBB_funcCallJoin_ln412
              if (joins_wami_debayer == 6'h8) begin
                joins_wami_debayer = 6'h0;
                interp_G_at_RRR_or_G_at_BBB($unsigned(read_cb_row_in), 
                mux_col_ln407[13:0], 
                interp_G_at_RRR_or_G_at_BBB_ln412_interp_G_at_RRR_or_G_at_BBB_out, 
                interp_G_at_RRR_or_G_at_BBB_ln412_CtoS_func_done);
                sub_ln412 = mux_col_ln407[10:0] - 2'h2;
                debayer_img_ping_g[sub_ln412] = {{2{
                interp_G_at_RRR_or_G_at_BBB_ln412_interp_G_at_RRR_or_G_at_BBB_out
                [13]}}, 
                interp_G_at_RRR_or_G_at_BBB_ln412_interp_G_at_RRR_or_G_at_BBB_out};
                joins_wami_debayer = 6'h7;
              end
              // for_ln424
              if (joins_wami_debayer == 6'h9) begin
                joins_wami_debayer = 6'h0;
                if ($unsigned(read_img_row_in)) 
                  lt_ln424 = mux_col_ln424 < sub_ln342;
                ctrlAnd_for_ln424 = &{lt_ln424, $unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln424) 
                  if (read_ping_in) 
                    joins_wami_debayer = 6'hc;
                  else 
                    joins_wami_debayer = 6'ha;
                else begin
                  mux_col_ln439 = 32'h3;
                  joins_wami_debayer = 6'hd;
                end
              end
              // interp_G_at_RRR_or_G_at_BBB_funcCallJoin_ln427
              if (joins_wami_debayer == 6'ha) begin
                joins_wami_debayer = 6'h0;
                interp_G_at_RRR_or_G_at_BBB($unsigned(read_cb_row_in), 
                mux_col_ln424[13:0], 
                interp_G_at_RRR_or_G_at_BBB_ln427_interp_G_at_RRR_or_G_at_BBB_out, 
                interp_G_at_RRR_or_G_at_BBB_ln427_CtoS_func_done);
                sub_ln427 = mux_col_ln424[10:0] - 2'h2;
                debayer_img_pong_g[sub_ln427] = {{2{
                interp_G_at_RRR_or_G_at_BBB_ln427_interp_G_at_RRR_or_G_at_BBB_out
                [13]}}, 
                interp_G_at_RRR_or_G_at_BBB_ln427_interp_G_at_RRR_or_G_at_BBB_out};
                joins_wami_debayer = 6'hb;
              end
              // ifBot_ln426
              if (joins_wami_debayer == 6'hb) begin
                joins_wami_debayer = 6'h0;
                state_wami_debayer_next = 5'h8;
              end
              // interp_G_at_RRR_or_G_at_BBB_funcCallJoin_ln429
              if (joins_wami_debayer == 6'hc) begin
                joins_wami_debayer = 6'h0;
                interp_G_at_RRR_or_G_at_BBB($unsigned(read_cb_row_in), 
                mux_col_ln424[13:0], 
                interp_G_at_RRR_or_G_at_BBB_ln429_interp_G_at_RRR_or_G_at_BBB_out, 
                interp_G_at_RRR_or_G_at_BBB_ln429_CtoS_func_done);
                sub_ln429 = mux_col_ln424[10:0] - 2'h2;
                debayer_img_ping_g[sub_ln429] = {{2{
                interp_G_at_RRR_or_G_at_BBB_ln429_interp_G_at_RRR_or_G_at_BBB_out
                [13]}}, 
                interp_G_at_RRR_or_G_at_BBB_ln429_interp_G_at_RRR_or_G_at_BBB_out};
                joins_wami_debayer = 6'hb;
              end
              // for_ln439
              if (joins_wami_debayer == 6'hd) begin
                joins_wami_debayer = 6'h0;
                lt_ln439 = mux_col_ln439 < sub_ln342;
                if (lt_ln439) 
                  if (read_ping_in) 
                    joins_wami_debayer = 6'h10;
                  else 
                    joins_wami_debayer = 6'he;
                else begin
                  mux_col_ln454 = 32'h2;
                  joins_wami_debayer = 6'h11;
                end
              end
              // interp_R_at_GRB_or_B_at_GBR_funcCallJoin_ln442
              if (joins_wami_debayer == 6'he) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_GRB_or_B_at_GBR($unsigned(read_cb_row_in), 
                mux_col_ln439[13:0], 
                interp_R_at_GRB_or_B_at_GBR_ln442_interp_R_at_GRB_or_B_at_GBR_out, 
                interp_R_at_GRB_or_B_at_GBR_ln442_CtoS_func_done);
                sub_ln442 = mux_col_ln439[10:0] - 2'h2;
                debayer_img_pong_r[sub_ln442] = {{2{
                interp_R_at_GRB_or_B_at_GBR_ln442_interp_R_at_GRB_or_B_at_GBR_out
                [13]}}, 
                interp_R_at_GRB_or_B_at_GBR_ln442_interp_R_at_GRB_or_B_at_GBR_out};
                joins_wami_debayer = 6'hf;
              end
              // ifBot_ln441
              if (joins_wami_debayer == 6'hf) begin
                joins_wami_debayer = 6'h0;
                state_wami_debayer_next = 5'hb;
              end
              // interp_R_at_GRB_or_B_at_GBR_funcCallJoin_ln444
              if (joins_wami_debayer == 6'h10) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_GRB_or_B_at_GBR($unsigned(read_cb_row_in), 
                mux_col_ln439[13:0], 
                interp_R_at_GRB_or_B_at_GBR_ln444_interp_R_at_GRB_or_B_at_GBR_out, 
                interp_R_at_GRB_or_B_at_GBR_ln444_CtoS_func_done);
                sub_ln444 = mux_col_ln439[10:0] - 2'h2;
                debayer_img_ping_r[sub_ln444] = {{2{
                interp_R_at_GRB_or_B_at_GBR_ln444_interp_R_at_GRB_or_B_at_GBR_out
                [13]}}, 
                interp_R_at_GRB_or_B_at_GBR_ln444_interp_R_at_GRB_or_B_at_GBR_out};
                joins_wami_debayer = 6'hf;
              end
              // for_ln454
              if (joins_wami_debayer == 6'h11) begin
                joins_wami_debayer = 6'h0;
                if ($unsigned(read_img_row_in)) 
                  lt_ln454 = mux_col_ln454 < sub_ln342;
                ctrlAnd_for_ln454 = &{lt_ln454, $unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln454) 
                  if (read_ping_in) 
                    joins_wami_debayer = 6'h14;
                  else 
                    joins_wami_debayer = 6'h12;
                else begin
                  mux_col_ln469 = 32'h2;
                  joins_wami_debayer = 6'h15;
                end
              end
              // interp_R_at_GRB_or_B_at_GBR_funcCallJoin_ln457
              if (joins_wami_debayer == 6'h12) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_GRB_or_B_at_GBR($unsigned(read_cb_row_in), 
                mux_col_ln454[13:0], 
                interp_R_at_GRB_or_B_at_GBR_ln457_interp_R_at_GRB_or_B_at_GBR_out, 
                interp_R_at_GRB_or_B_at_GBR_ln457_CtoS_func_done);
                sub_ln457 = mux_col_ln454[10:0] - 2'h2;
                debayer_img_pong_b[sub_ln457] = {{2{
                interp_R_at_GRB_or_B_at_GBR_ln457_interp_R_at_GRB_or_B_at_GBR_out
                [13]}}, 
                interp_R_at_GRB_or_B_at_GBR_ln457_interp_R_at_GRB_or_B_at_GBR_out};
                joins_wami_debayer = 6'h13;
              end
              // ifBot_ln456
              if (joins_wami_debayer == 6'h13) begin
                joins_wami_debayer = 6'h0;
                state_wami_debayer_next = 5'he;
              end
              // interp_R_at_GRB_or_B_at_GBR_funcCallJoin_ln459
              if (joins_wami_debayer == 6'h14) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_GRB_or_B_at_GBR($unsigned(read_cb_row_in), 
                mux_col_ln454[13:0], 
                interp_R_at_GRB_or_B_at_GBR_ln459_interp_R_at_GRB_or_B_at_GBR_out, 
                interp_R_at_GRB_or_B_at_GBR_ln459_CtoS_func_done);
                sub_ln459 = mux_col_ln454[10:0] - 2'h2;
                debayer_img_ping_b[sub_ln459] = {{2{
                interp_R_at_GRB_or_B_at_GBR_ln459_interp_R_at_GRB_or_B_at_GBR_out
                [13]}}, 
                interp_R_at_GRB_or_B_at_GBR_ln459_interp_R_at_GRB_or_B_at_GBR_out};
                joins_wami_debayer = 6'h13;
              end
              // for_ln469
              if (joins_wami_debayer == 6'h15) begin
                joins_wami_debayer = 6'h0;
                if ($unsigned(read_img_row_in)) 
                  lt_ln469 = mux_col_ln469 < sub_ln342;
                ctrlAnd_for_ln469 = &{lt_ln469, $unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln469) 
                  if (read_ping_in) 
                    joins_wami_debayer = 6'h18;
                  else 
                    joins_wami_debayer = 6'h16;
                else begin
                  mux_col_ln484 = 32'h3;
                  joins_wami_debayer = 6'h19;
                end
              end
              // interp_R_at_GBR_or_B_at_GRB_funcCallJoin_ln472
              if (joins_wami_debayer == 6'h16) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_GBR_or_B_at_GRB($unsigned(read_cb_row_in), 
                mux_col_ln469[13:0], 
                interp_R_at_GBR_or_B_at_GRB_ln472_interp_R_at_GBR_or_B_at_GRB_out, 
                interp_R_at_GBR_or_B_at_GRB_ln472_CtoS_func_done);
                sub_ln472 = mux_col_ln469[10:0] - 2'h2;
                debayer_img_pong_r[sub_ln472] = {{2{
                interp_R_at_GBR_or_B_at_GRB_ln472_interp_R_at_GBR_or_B_at_GRB_out
                [13]}}, 
                interp_R_at_GBR_or_B_at_GRB_ln472_interp_R_at_GBR_or_B_at_GRB_out};
                joins_wami_debayer = 6'h17;
              end
              // ifBot_ln471
              if (joins_wami_debayer == 6'h17) begin
                joins_wami_debayer = 6'h0;
                state_wami_debayer_next = 5'h11;
              end
              // interp_R_at_GBR_or_B_at_GRB_funcCallJoin_ln474
              if (joins_wami_debayer == 6'h18) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_GBR_or_B_at_GRB($unsigned(read_cb_row_in), 
                mux_col_ln469[13:0], 
                interp_R_at_GBR_or_B_at_GRB_ln474_interp_R_at_GBR_or_B_at_GRB_out, 
                interp_R_at_GBR_or_B_at_GRB_ln474_CtoS_func_done);
                sub_ln474 = mux_col_ln469[10:0] - 2'h2;
                debayer_img_ping_r[sub_ln474] = {{2{
                interp_R_at_GBR_or_B_at_GRB_ln474_interp_R_at_GBR_or_B_at_GRB_out
                [13]}}, 
                interp_R_at_GBR_or_B_at_GRB_ln474_interp_R_at_GBR_or_B_at_GRB_out};
                joins_wami_debayer = 6'h17;
              end
              // for_ln484
              if (joins_wami_debayer == 6'h19) begin
                joins_wami_debayer = 6'h0;
                if (!$unsigned(read_img_row_in)) 
                  lt_ln484 = mux_col_ln484 < sub_ln342;
                ctrlAnd_for_ln484 = &{lt_ln484, !$unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln484) 
                  if (read_ping_in) 
                    joins_wami_debayer = 6'h1c;
                  else 
                    joins_wami_debayer = 6'h1a;
                else begin
                  mux_col_ln500 = 32'h3;
                  joins_wami_debayer = 6'h1d;
                end
              end
              // interp_R_at_GBR_or_B_at_GRB_funcCallJoin_ln487
              if (joins_wami_debayer == 6'h1a) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_GBR_or_B_at_GRB($unsigned(read_cb_row_in), 
                mux_col_ln484[13:0], 
                interp_R_at_GBR_or_B_at_GRB_ln487_interp_R_at_GBR_or_B_at_GRB_out, 
                interp_R_at_GBR_or_B_at_GRB_ln487_CtoS_func_done);
                sub_ln487 = mux_col_ln484[10:0] - 2'h2;
                debayer_img_pong_b[sub_ln487] = {{2{
                interp_R_at_GBR_or_B_at_GRB_ln487_interp_R_at_GBR_or_B_at_GRB_out
                [13]}}, 
                interp_R_at_GBR_or_B_at_GRB_ln487_interp_R_at_GBR_or_B_at_GRB_out};
                joins_wami_debayer = 6'h1b;
              end
              // ifBot_ln486
              if (joins_wami_debayer == 6'h1b) begin
                joins_wami_debayer = 6'h0;
                state_wami_debayer_next = 5'h14;
              end
              // interp_R_at_GBR_or_B_at_GRB_funcCallJoin_ln489
              if (joins_wami_debayer == 6'h1c) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_GBR_or_B_at_GRB($unsigned(read_cb_row_in), 
                mux_col_ln484[13:0], 
                interp_R_at_GBR_or_B_at_GRB_ln489_interp_R_at_GBR_or_B_at_GRB_out, 
                interp_R_at_GBR_or_B_at_GRB_ln489_CtoS_func_done);
                sub_ln489 = mux_col_ln484[10:0] - 2'h2;
                debayer_img_ping_b[sub_ln489] = {{2{
                interp_R_at_GBR_or_B_at_GRB_ln489_interp_R_at_GBR_or_B_at_GRB_out
                [13]}}, 
                interp_R_at_GBR_or_B_at_GRB_ln489_interp_R_at_GBR_or_B_at_GRB_out};
                joins_wami_debayer = 6'h1b;
              end
              // for_ln500
              if (joins_wami_debayer == 6'h1d) begin
                joins_wami_debayer = 6'h0;
                if ($unsigned(read_img_row_in)) 
                  lt_ln500 = mux_col_ln500 < sub_ln342;
                ctrlAnd_for_ln500 = &{lt_ln500, $unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln500) 
                  if (read_ping_in) 
                    joins_wami_debayer = 6'h20;
                  else 
                    joins_wami_debayer = 6'h1e;
                else begin
                  mux_col_ln515 = 32'h2;
                  joins_wami_debayer = 6'h21;
                end
              end
              // interp_R_at_BBB_or_B_at_RRR_funcCallJoin_ln503
              if (joins_wami_debayer == 6'h1e) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_BBB_or_B_at_RRR($unsigned(read_cb_row_in), 
                mux_col_ln500[13:0], 
                interp_R_at_BBB_or_B_at_RRR_ln503_interp_R_at_BBB_or_B_at_RRR_out, 
                interp_R_at_BBB_or_B_at_RRR_ln503_CtoS_func_done);
                sub_ln503 = mux_col_ln500[10:0] - 2'h2;
                debayer_img_pong_r[sub_ln503] = {{2{
                interp_R_at_BBB_or_B_at_RRR_ln503_interp_R_at_BBB_or_B_at_RRR_out
                [13]}}, 
                interp_R_at_BBB_or_B_at_RRR_ln503_interp_R_at_BBB_or_B_at_RRR_out};
                joins_wami_debayer = 6'h1f;
              end
              // ifBot_ln502
              if (joins_wami_debayer == 6'h1f) begin
                joins_wami_debayer = 6'h0;
                state_wami_debayer_next = 5'h17;
              end
              // interp_R_at_BBB_or_B_at_RRR_funcCallJoin_ln505
              if (joins_wami_debayer == 6'h20) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_BBB_or_B_at_RRR($unsigned(read_cb_row_in), 
                mux_col_ln500[13:0], 
                interp_R_at_BBB_or_B_at_RRR_ln505_interp_R_at_BBB_or_B_at_RRR_out, 
                interp_R_at_BBB_or_B_at_RRR_ln505_CtoS_func_done);
                sub_ln505 = mux_col_ln500[10:0] - 2'h2;
                debayer_img_ping_r[sub_ln505] = {{2{
                interp_R_at_BBB_or_B_at_RRR_ln505_interp_R_at_BBB_or_B_at_RRR_out
                [13]}}, 
                interp_R_at_BBB_or_B_at_RRR_ln505_interp_R_at_BBB_or_B_at_RRR_out};
                joins_wami_debayer = 6'h1f;
              end
              // for_ln515
              if (joins_wami_debayer == 6'h21) begin
                joins_wami_debayer = 6'h0;
                if (!$unsigned(read_img_row_in)) 
                  lt_ln515 = mux_col_ln515 < sub_ln342;
                ctrlAnd_for_ln515 = &{lt_ln515, !$unsigned(read_img_row_in)};
                if (ctrlAnd_for_ln515) 
                  if (read_ping_in) 
                    joins_wami_debayer = 6'h24;
                  else 
                    joins_wami_debayer = 6'h22;
                else begin
                  CtoS_func_done_wami_debayer = 1'b1;
                  joins_wami_debayer = 6'h0;
                end
              end
              // interp_R_at_BBB_or_B_at_RRR_funcCallJoin_ln518
              if (joins_wami_debayer == 6'h22) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_BBB_or_B_at_RRR($unsigned(read_cb_row_in), 
                mux_col_ln515[13:0], 
                interp_R_at_BBB_or_B_at_RRR_ln518_interp_R_at_BBB_or_B_at_RRR_out, 
                interp_R_at_BBB_or_B_at_RRR_ln518_CtoS_func_done);
                sub_ln518 = mux_col_ln515[10:0] - 2'h2;
                debayer_img_pong_b[sub_ln518] = {{2{
                interp_R_at_BBB_or_B_at_RRR_ln518_interp_R_at_BBB_or_B_at_RRR_out
                [13]}}, 
                interp_R_at_BBB_or_B_at_RRR_ln518_interp_R_at_BBB_or_B_at_RRR_out};
                joins_wami_debayer = 6'h23;
              end
              // ifBot_ln517
              if (joins_wami_debayer == 6'h23) begin
                joins_wami_debayer = 6'h0;
                state_wami_debayer_next = 5'h1a;
              end
              // interp_R_at_BBB_or_B_at_RRR_funcCallJoin_ln520
              if (joins_wami_debayer == 6'h24) begin
                joins_wami_debayer = 6'h0;
                interp_R_at_BBB_or_B_at_RRR($unsigned(read_cb_row_in), 
                mux_col_ln515[13:0], 
                interp_R_at_BBB_or_B_at_RRR_ln520_interp_R_at_BBB_or_B_at_RRR_out, 
                interp_R_at_BBB_or_B_at_RRR_ln520_CtoS_func_done);
                sub_ln520 = mux_col_ln515[10:0] - 2'h2;
                debayer_img_ping_b[sub_ln520] = {{2{
                interp_R_at_BBB_or_B_at_RRR_ln520_interp_R_at_BBB_or_B_at_RRR_out
                [13]}}, 
                interp_R_at_BBB_or_B_at_RRR_ln520_interp_R_at_BBB_or_B_at_RRR_out};
                joins_wami_debayer = 6'h23;
              end
            end
          state_wami_debayer = state_wami_debayer_next;
          joins_wami_debayer = 6'h0;
          if (!CtoS_func_done_wami_debayer) @(posedge clk);
        end
    end
  endtask

  task interp_G_at_RRR_or_G_at_BBB;
    input [31:0] row_in;
    input [13:0] col_in;
    output reg [13:0] interp_G_at_RRR_or_G_at_BBB_out;
    output reg CtoS_func_done_interp_G_at_RRR_or_G_at_BBB;
    reg state_interp_G_at_RRR_or_G_at_BBB;
    reg state_interp_G_at_RRR_or_G_at_BBB_next;
    reg [31:0] read_row_in;
    reg [13:0] read_col_in;
    reg [1:0] eq_ln88;
    reg [2:0] sub_ln89;
    reg [2:0] add_ln89;
    reg [2:0] sub_ln90;
    reg [2:0] add_ln90;
    reg [2:0] add_ln91;
    reg [2:0] add_ln92;
    reg [13:0] sub_ln121;
    reg [14:0] add_ln120;
    reg [14:0] add_ln121;
    reg [13:0] add_ln123;
    reg [14:0] add_ln122;
    reg [14:0] add_ln123_0;
    reg [13:0] add_ln129;
    reg [15:0] add_ln129_0;
    reg [13:0] sub_ln131;
    reg [15:0] add_ln130;
    reg [15:0] add_ln131;
    reg [13:0] compute_and_clamp_pixel_ln134;
    reg [1:0] eq_ln94;
    reg [2:0] add_ln95;
    reg [1:0] eq_ln100;
    reg [2:0] sub_ln101;
    reg [1:0] eq_ln106;
    reg [2:0] sub_ln107;
    reg [2:0] sub_ln108;
    reg [2:0] mux_row_min_2_ln88;
    reg [2:0] mux_row_min_1_ln88;
    reg [2:0] mux_row_add_2_ln88;
    reg [2:0] mux_row_add_1_ln88;
    reg [13:0] add_ln120_0;
    reg [15:0] memread_debayer_bayer_img_ln120;
    reg [13:0] add_ln121_0;
    reg [15:0] memread_debayer_bayer_img_ln121;
    reg [13:0] add_ln122_0;
    reg [15:0] memread_debayer_bayer_img_ln122;
    reg [13:0] add_ln123_1;
    reg [15:0] memread_debayer_bayer_img_ln123;
    reg [13:0] add_ln124;
    reg [15:0] memread_debayer_bayer_img_ln124;
    reg [13:0] add_ln129_1;
    reg [15:0] memread_debayer_bayer_img_ln129;
    reg [13:0] add_ln130_0;
    reg [15:0] memread_debayer_bayer_img_ln130;
    reg [13:0] add_ln131_0;
    reg [15:0] memread_debayer_bayer_img_ln131;
    reg [13:0] add_ln132;
    reg [15:0] memread_debayer_bayer_img_ln132;
    reg [2:0] case_mux_row_add_2_ln100;
    reg [2:0] case_mux_row_min_2_ln94;
    reg and_case_mux_row_min_2_ln94;
    reg and_case_mux_row_add_2_ln100_2;
    reg and_case_mux_row_add_2_ln100_0_0;
    reg and_case_mux_row_add_2_ln100_1_0;
    reg or_eq_ln88_Z_0;
    reg or_eq_ln88_Z_0_0;
    reg or_and_case_mux_row_min_2_ln94_0_Z_0;
    reg and_or_and_case_mux_row_min_2_ln94_0_Z_0;

    begin : interp_G_at_RRR_or_G_at_BBB_body
      CtoS_func_done_interp_G_at_RRR_or_G_at_BBB = 1'b0;
      begin
        read_row_in = row_in;
        read_col_in = col_in;
        case (read_row_in)
          32'h0: eq_ln88 = 2'h1;
          default: eq_ln88 = 2'h2;
        endcase
        sub_ln89 = read_row_in[2:0] - 2'h2;
        if (eq_ln88[0]) 
          add_ln89 = sub_ln89 + 3'h6;
        sub_ln90 = read_row_in[2:0] - 1'b1;
        if (eq_ln88[0]) 
          add_ln90 = sub_ln90 + 3'h6;
        add_ln91 = read_row_in[2:0] + 1'b1;
        add_ln92 = read_row_in[2:0] + 2'h2;
        if (eq_ln88[1]) begin
          case (read_row_in)
            32'h1: eq_ln94 = 2'h1;
            default: eq_ln94 = 2'h2;
          endcase
          if (eq_ln94[0]) 
            add_ln95 = sub_ln89 + 3'h6;
          if (eq_ln94[1]) begin
            case (read_row_in)
              32'h4: eq_ln100 = 2'h1;
              default: eq_ln100 = 2'h2;
            endcase
            if (eq_ln100[0]) 
              sub_ln101 = add_ln92 - 3'h6;
            if (eq_ln100[1]) begin
              case (read_row_in)
                32'h5: eq_ln106 = 2'h1;
                default: eq_ln106 = 2'h2;
              endcase
              if (eq_ln106[0]) begin
                sub_ln107 = add_ln91 - 3'h6;
                sub_ln108 = add_ln92 - 3'h6;
              end
            end
            case (1'b1)
              eq_ln100[0]: case_mux_row_add_2_ln100 = 3'h1;
              eq_ln106[0]: case_mux_row_add_2_ln100 = 3'h2;
              default: case_mux_row_add_2_ln100 = 3'h4;
            endcase
          end
          case (1'b1)
            eq_ln94[0]: case_mux_row_min_2_ln94 = 3'h1;
            eq_ln100[0]: case_mux_row_min_2_ln94 = 3'h2;
            default: case_mux_row_min_2_ln94 = 3'h4;
          endcase
        end
        and_case_mux_row_min_2_ln94 = &{eq_ln88[1], case_mux_row_min_2_ln94[0]};
        or_and_case_mux_row_min_2_ln94_0_Z_0 = |case_mux_row_min_2_ln94[2:1];
        and_or_and_case_mux_row_min_2_ln94_0_Z_0 = &{
        or_and_case_mux_row_min_2_ln94_0_Z_0, eq_ln88[1]};
        case (1'b1)
          eq_ln88[0]: mux_row_min_2_ln88 = add_ln89;
          and_case_mux_row_min_2_ln94: mux_row_min_2_ln88 = add_ln95;
          and_or_and_case_mux_row_min_2_ln94_0_Z_0: mux_row_min_2_ln88 = 
            sub_ln89;
          default: mux_row_min_2_ln88 = 3'hX;
        endcase
        mux_row_min_1_ln88 = eq_ln88[0] ? add_ln90 : sub_ln90;
        and_case_mux_row_add_2_ln100_2 = &{eq_ln94[1], case_mux_row_add_2_ln100[
        0], eq_ln88[1]};
        and_case_mux_row_add_2_ln100_0_0 = &{eq_ln94[1], 
        case_mux_row_add_2_ln100[1], eq_ln88[1]};
        and_case_mux_row_add_2_ln100_1_0 = &{eq_ln94[1], 
        case_mux_row_add_2_ln100[2], eq_ln88[1]};
        or_eq_ln88_Z_0 = |{and_case_mux_row_add_2_ln100_1_0, eq_ln94[0], eq_ln88
        [0]};
        case (1'b1)
          or_eq_ln88_Z_0: mux_row_add_2_ln88 = add_ln92;
          and_case_mux_row_add_2_ln100_2: mux_row_add_2_ln88 = sub_ln101;
          and_case_mux_row_add_2_ln100_0_0: mux_row_add_2_ln88 = sub_ln108;
          default: mux_row_add_2_ln88 = 3'hX;
        endcase
        or_eq_ln88_Z_0_0 = |{and_case_mux_row_add_2_ln100_1_0, 
        and_case_mux_row_add_2_ln100_2, eq_ln94[0], eq_ln88[0]};
        mux_row_add_1_ln88 = or_eq_ln88_Z_0_0 ? add_ln91 : sub_ln107;
        add_ln120_0 = {mux_row_min_1_ln88, 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln120 = bayer_img[add_ln120_0];
        sub_ln121 = read_col_in - 1'b1;
        add_ln121_0 = {read_row_in[2:0], 11'h0} + sub_ln121;
        memread_debayer_bayer_img_ln121 = bayer_img[add_ln121_0];
        add_ln120 = memread_debayer_bayer_img_ln120[14:0] + 
        memread_debayer_bayer_img_ln121[14:0];
        add_ln122_0 = {read_row_in[2:0], 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln122 = bayer_img[add_ln122_0];
        add_ln121 = add_ln120 + {memread_debayer_bayer_img_ln122[13:0], 1'b0};
        add_ln123 = read_col_in + 1'b1;
        add_ln123_1 = {read_row_in[2:0], 11'h0} + add_ln123;
        memread_debayer_bayer_img_ln123 = bayer_img[add_ln123_1];
        add_ln122 = add_ln121 + memread_debayer_bayer_img_ln123[14:0];
        add_ln124 = {mux_row_add_1_ln88, 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln124 = bayer_img[add_ln124];
        add_ln123_0 = add_ln122 + memread_debayer_bayer_img_ln124[14:0];
        state_interp_G_at_RRR_or_G_at_BBB_next = 1'b0;
      end
      // This value for state is interpreted as undefined
      state_interp_G_at_RRR_or_G_at_BBB = 1'b1;
      while (!CtoS_func_done_interp_G_at_RRR_or_G_at_BBB) 
        begin
          case (state_interp_G_at_RRR_or_G_at_BBB)
            1'b0: // Wait_ln126
              begin
                add_ln129 = read_col_in + 2'h2;
                add_ln129_1 = {read_row_in[2:0], 11'h0} + add_ln129;
                memread_debayer_bayer_img_ln129 = bayer_img[add_ln129_1];
                add_ln130_0 = {mux_row_min_2_ln88, 11'h0} + read_col_in;
                memread_debayer_bayer_img_ln130 = bayer_img[add_ln130_0];
                add_ln129_0 = memread_debayer_bayer_img_ln129 + 
                memread_debayer_bayer_img_ln130;
                sub_ln131 = read_col_in - 2'h2;
                add_ln131_0 = {read_row_in[2:0], 11'h0} + sub_ln131;
                memread_debayer_bayer_img_ln131 = bayer_img[add_ln131_0];
                add_ln130 = memread_debayer_bayer_img_ln131 + add_ln129_0;
                add_ln132 = {mux_row_add_2_ln88, 11'h0} + read_col_in;
                memread_debayer_bayer_img_ln132 = bayer_img[add_ln132];
                add_ln131 = memread_debayer_bayer_img_ln132 + add_ln130;
                compute_and_clamp_pixel({add_ln123_0, 1'b0}, add_ln131, 
                compute_and_clamp_pixel_ln134);
                interp_G_at_RRR_or_G_at_BBB_out = compute_and_clamp_pixel_ln134;
                CtoS_func_done_interp_G_at_RRR_or_G_at_BBB = 1'b1;
              end
          endcase
          state_interp_G_at_RRR_or_G_at_BBB = 
          state_interp_G_at_RRR_or_G_at_BBB_next;
          if (!CtoS_func_done_interp_G_at_RRR_or_G_at_BBB) @(posedge clk);
        end
    end
  endtask

  task compute_and_clamp_pixel;
    input [15:0] pos_in;
    input [15:0] neg_in;
    output reg [13:0] compute_and_clamp_pixel_out;
    reg [15:0] read_pos_in;
    reg [15:0] read_neg_in;
    reg lt_ln9;
    reg [16:0] sub_ln15;
    reg [13:0] mux_compute_and_clamp_pixel_ln5;

    begin : compute_and_clamp_pixel_body
      begin
        read_pos_in = pos_in;
        read_neg_in = neg_in;
        lt_ln9 = read_pos_in < read_neg_in;
        if (!lt_ln9) 
          sub_ln15 = read_pos_in - read_neg_in;
        mux_compute_and_clamp_pixel_ln5 = lt_ln9 ? 14'h0 : sub_ln15[16:3];
        compute_and_clamp_pixel_out = mux_compute_and_clamp_pixel_ln5;
      end
    end
  endtask

  task interp_R_at_GRB_or_B_at_GBR;
    input [31:0] row_in;
    input [13:0] col_in;
    output reg [13:0] interp_R_at_GRB_or_B_at_GBR_out;
    output reg CtoS_func_done_interp_R_at_GRB_or_B_at_GBR;
    reg state_interp_R_at_GRB_or_B_at_GBR;
    reg state_interp_R_at_GRB_or_B_at_GBR_next;
    reg [31:0] read_row_in;
    reg [13:0] read_col_in;
    reg [1:0] eq_ln148;
    reg [2:0] sub_ln149;
    reg [2:0] add_ln149;
    reg [2:0] sub_ln150;
    reg [2:0] add_ln150;
    reg [2:0] add_ln151;
    reg [2:0] add_ln152;
    reg [16:0] add_ln180;
    reg [13:0] sub_ln181;
    reg [15:0] add_ln180_0;
    reg [15:0] add_ln181;
    reg [13:0] add_ln183;
    reg [15:0] add_ln182;
    reg [15:0] add_ln188;
    reg [13:0] sub_ln190;
    reg [15:0] add_ln189_0;
    reg [13:0] add_ln191;
    reg [15:0] add_ln190;
    reg [15:0] add_ln191_0;
    reg [15:0] add_ln192;
    reg [13:0] compute_and_clamp_pixel_ln195;
    reg [1:0] eq_ln154;
    reg [2:0] add_ln155;
    reg [1:0] eq_ln160;
    reg [2:0] sub_ln161;
    reg [1:0] eq_ln166;
    reg [2:0] sub_ln167;
    reg [2:0] sub_ln168;
    reg [2:0] mux_row_min_2_ln148;
    reg [2:0] mux_row_min_1_ln148;
    reg [2:0] mux_row_add_2_ln148;
    reg [2:0] mux_row_add_1_ln148;
    reg [13:0] add_ln180_1;
    reg [15:0] memread_debayer_bayer_img_ln180;
    reg [13:0] add_ln180_3;
    reg [15:0] memread_debayer_bayer_img_ln180_0;
    reg [13:0] add_ln181_0;
    reg [15:0] memread_debayer_bayer_img_ln181;
    reg [13:0] add_ln182_0;
    reg [15:0] memread_debayer_bayer_img_ln182;
    reg [13:0] add_ln183_0;
    reg [15:0] memread_debayer_bayer_img_ln183;
    reg [13:0] add_ln188_0;
    reg [15:0] memread_debayer_bayer_img_ln188;
    reg [13:0] add_ln189_1;
    reg [15:0] memread_debayer_bayer_img_ln189;
    reg [13:0] add_ln190_0;
    reg [15:0] memread_debayer_bayer_img_ln190;
    reg [13:0] add_ln191_1;
    reg [15:0] memread_debayer_bayer_img_ln191;
    reg [13:0] add_ln192_0;
    reg [15:0] memread_debayer_bayer_img_ln192;
    reg [13:0] add_ln193_0;
    reg [15:0] memread_debayer_bayer_img_ln193;
    reg [15:0] add_ln182_2;
    reg [2:0] case_mux_row_add_2_ln160;
    reg [2:0] case_mux_row_min_2_ln154;
    reg and_case_mux_row_min_2_ln154;
    reg and_case_mux_row_add_2_ln160_2;
    reg and_case_mux_row_add_2_ln160_0_0;
    reg and_case_mux_row_add_2_ln160_1_0;
    reg or_eq_ln148_Z_0;
    reg or_eq_ln148_Z_0_0;
    reg or_and_case_mux_row_min_2_ln154_0_Z_0;
    reg and_or_and_case_mux_row_min_2_ln154_0_Z_0;

    begin : interp_R_at_GRB_or_B_at_GBR_body
      CtoS_func_done_interp_R_at_GRB_or_B_at_GBR = 1'b0;
      begin
        read_row_in = row_in;
        read_col_in = col_in;
        case (read_row_in)
          32'h0: eq_ln148 = 2'h1;
          default: eq_ln148 = 2'h2;
        endcase
        sub_ln149 = read_row_in[2:0] - 2'h2;
        if (eq_ln148[0]) 
          add_ln149 = sub_ln149 + 3'h6;
        sub_ln150 = read_row_in[2:0] - 1'b1;
        if (eq_ln148[0]) 
          add_ln150 = sub_ln150 + 3'h6;
        add_ln151 = read_row_in[2:0] + 1'b1;
        add_ln152 = read_row_in[2:0] + 2'h2;
        if (eq_ln148[1]) begin
          case (read_row_in)
            32'h1: eq_ln154 = 2'h1;
            default: eq_ln154 = 2'h2;
          endcase
          if (eq_ln154[0]) 
            add_ln155 = sub_ln149 + 3'h6;
          if (eq_ln154[1]) begin
            case (read_row_in)
              32'h4: eq_ln160 = 2'h1;
              default: eq_ln160 = 2'h2;
            endcase
            if (eq_ln160[0]) 
              sub_ln161 = add_ln152 - 3'h6;
            if (eq_ln160[1]) begin
              case (read_row_in)
                32'h5: eq_ln166 = 2'h1;
                default: eq_ln166 = 2'h2;
              endcase
              if (eq_ln166[0]) begin
                sub_ln167 = add_ln151 - 3'h6;
                sub_ln168 = add_ln152 - 3'h6;
              end
            end
            case (1'b1)
              eq_ln160[0]: case_mux_row_add_2_ln160 = 3'h1;
              eq_ln166[0]: case_mux_row_add_2_ln160 = 3'h2;
              default: case_mux_row_add_2_ln160 = 3'h4;
            endcase
          end
          case (1'b1)
            eq_ln154[0]: case_mux_row_min_2_ln154 = 3'h1;
            eq_ln160[0]: case_mux_row_min_2_ln154 = 3'h2;
            default: case_mux_row_min_2_ln154 = 3'h4;
          endcase
        end
        and_case_mux_row_min_2_ln154 = &{eq_ln148[1], case_mux_row_min_2_ln154[0]};
        or_and_case_mux_row_min_2_ln154_0_Z_0 = |case_mux_row_min_2_ln154[2:1];
        and_or_and_case_mux_row_min_2_ln154_0_Z_0 = &{
        or_and_case_mux_row_min_2_ln154_0_Z_0, eq_ln148[1]};
        case (1'b1)
          eq_ln148[0]: mux_row_min_2_ln148 = add_ln149;
          and_case_mux_row_min_2_ln154: mux_row_min_2_ln148 = add_ln155;
          and_or_and_case_mux_row_min_2_ln154_0_Z_0: mux_row_min_2_ln148 = 
            sub_ln149;
          default: mux_row_min_2_ln148 = 3'hX;
        endcase
        mux_row_min_1_ln148 = eq_ln148[0] ? add_ln150 : sub_ln150;
        and_case_mux_row_add_2_ln160_2 = &{eq_ln154[1], case_mux_row_add_2_ln160
        [0], eq_ln148[1]};
        and_case_mux_row_add_2_ln160_0_0 = &{eq_ln154[1], 
        case_mux_row_add_2_ln160[1], eq_ln148[1]};
        and_case_mux_row_add_2_ln160_1_0 = &{eq_ln154[1], 
        case_mux_row_add_2_ln160[2], eq_ln148[1]};
        or_eq_ln148_Z_0 = |{and_case_mux_row_add_2_ln160_1_0, eq_ln154[0], 
        eq_ln148[0]};
        case (1'b1)
          or_eq_ln148_Z_0: mux_row_add_2_ln148 = add_ln152;
          and_case_mux_row_add_2_ln160_2: mux_row_add_2_ln148 = sub_ln161;
          and_case_mux_row_add_2_ln160_0_0: mux_row_add_2_ln148 = sub_ln168;
          default: mux_row_add_2_ln148 = 3'hX;
        endcase
        or_eq_ln148_Z_0_0 = |{and_case_mux_row_add_2_ln160_1_0, 
        and_case_mux_row_add_2_ln160_2, eq_ln154[0], eq_ln148[0]};
        mux_row_add_1_ln148 = or_eq_ln148_Z_0_0 ? add_ln151 : sub_ln167;
        add_ln180_1 = {mux_row_min_2_ln148, 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln180 = bayer_img[add_ln180_1];
        add_ln180_3 = {mux_row_add_2_ln148, 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln180_0 = bayer_img[add_ln180_3];
        add_ln180 = memread_debayer_bayer_img_ln180 + 
        memread_debayer_bayer_img_ln180_0;
        sub_ln181 = read_col_in - 1'b1;
        add_ln181_0 = {read_row_in[2:0], 11'h0} + sub_ln181;
        memread_debayer_bayer_img_ln181 = bayer_img[add_ln181_0];
        add_ln180_0 = add_ln180[16:1] + {memread_debayer_bayer_img_ln181[13:0], 
        2'h0};
        add_ln182_0 = {read_row_in[2:0], 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln182 = bayer_img[add_ln182_0];
        add_ln182_2 = {memread_debayer_bayer_img_ln182[13:0], 2'h0} + 
        memread_debayer_bayer_img_ln182;
        add_ln181 = add_ln180_0 + add_ln182_2;
        add_ln183 = read_col_in + 1'b1;
        add_ln183_0 = {read_row_in[2:0], 11'h0} + add_ln183;
        memread_debayer_bayer_img_ln183 = bayer_img[add_ln183_0];
        add_ln182 = add_ln181 + {memread_debayer_bayer_img_ln183[13:0], 2'h0};
        state_interp_R_at_GRB_or_B_at_GBR_next = 1'b0;
      end
      // This value for state is interpreted as undefined
      state_interp_R_at_GRB_or_B_at_GBR = 1'b1;
      while (!CtoS_func_done_interp_R_at_GRB_or_B_at_GBR) 
        begin
          case (state_interp_R_at_GRB_or_B_at_GBR)
            1'b0: // Wait_ln185
              begin
                add_ln188_0 = {mux_row_min_1_ln148, 11'h0} + sub_ln181;
                memread_debayer_bayer_img_ln188 = bayer_img[add_ln188_0];
                add_ln189_1 = {mux_row_min_1_ln148, 11'h0} + add_ln183;
                memread_debayer_bayer_img_ln189 = bayer_img[add_ln189_1];
                add_ln188 = memread_debayer_bayer_img_ln188 + 
                memread_debayer_bayer_img_ln189;
                sub_ln190 = read_col_in - 2'h2;
                add_ln190_0 = {read_row_in[2:0], 11'h0} + sub_ln190;
                memread_debayer_bayer_img_ln190 = bayer_img[add_ln190_0];
                add_ln189_0 = memread_debayer_bayer_img_ln190 + add_ln188;
                add_ln191 = read_col_in + 2'h2;
                add_ln191_1 = {read_row_in[2:0], 11'h0} + add_ln191;
                memread_debayer_bayer_img_ln191 = bayer_img[add_ln191_1];
                add_ln190 = memread_debayer_bayer_img_ln191 + add_ln189_0;
                add_ln192_0 = {mux_row_add_1_ln148, 11'h0} + sub_ln181;
                memread_debayer_bayer_img_ln192 = bayer_img[add_ln192_0];
                add_ln191_0 = memread_debayer_bayer_img_ln192 + add_ln190;
                add_ln193_0 = {mux_row_add_1_ln148, 11'h0} + add_ln183;
                memread_debayer_bayer_img_ln193 = bayer_img[add_ln193_0];
                add_ln192 = memread_debayer_bayer_img_ln193 + add_ln191_0;
                compute_and_clamp_pixel(add_ln182, add_ln192, 
                compute_and_clamp_pixel_ln195);
                interp_R_at_GRB_or_B_at_GBR_out = compute_and_clamp_pixel_ln195;
                CtoS_func_done_interp_R_at_GRB_or_B_at_GBR = 1'b1;
              end
          endcase
          state_interp_R_at_GRB_or_B_at_GBR = 
          state_interp_R_at_GRB_or_B_at_GBR_next;
          if (!CtoS_func_done_interp_R_at_GRB_or_B_at_GBR) @(posedge clk);
        end
    end
  endtask

  task interp_R_at_GBR_or_B_at_GRB;
    input [31:0] row_in;
    input [13:0] col_in;
    output reg [13:0] interp_R_at_GBR_or_B_at_GRB_out;
    output reg CtoS_func_done_interp_R_at_GBR_or_B_at_GRB;
    reg state_interp_R_at_GBR_or_B_at_GRB;
    reg state_interp_R_at_GBR_or_B_at_GRB_next;
    reg [31:0] read_row_in;
    reg [13:0] read_col_in;
    reg [1:0] eq_ln210;
    reg [2:0] sub_ln211;
    reg [2:0] add_ln211;
    reg [2:0] sub_ln212;
    reg [2:0] add_ln212;
    reg [2:0] add_ln213;
    reg [2:0] add_ln214;
    reg [13:0] sub_ln243;
    reg [13:0] add_ln243;
    reg [16:0] add_ln243_0;
    reg [15:0] add_ln242;
    reg [15:0] add_ln243_1;
    reg [15:0] add_ln244;
    reg [13:0] sub_ln251;
    reg [15:0] add_ln250;
    reg [13:0] add_ln252;
    reg [15:0] add_ln251;
    reg [15:0] add_ln252_0;
    reg [15:0] add_ln253;
    reg [15:0] add_ln254_0;
    reg [13:0] compute_and_clamp_pixel_ln257;
    reg [1:0] eq_ln216;
    reg [2:0] add_ln217;
    reg [1:0] eq_ln222;
    reg [2:0] sub_ln223;
    reg [1:0] eq_ln228;
    reg [2:0] sub_ln229;
    reg [2:0] sub_ln230;
    reg [2:0] mux_row_min_2_ln210;
    reg [2:0] mux_row_min_1_ln210;
    reg [2:0] mux_row_add_2_ln210;
    reg [2:0] mux_row_add_1_ln210;
    reg [13:0] add_ln242_0;
    reg [15:0] memread_debayer_bayer_img_ln242;
    reg [13:0] add_ln243_2;
    reg [15:0] memread_debayer_bayer_img_ln243;
    reg [13:0] add_ln243_4;
    reg [15:0] memread_debayer_bayer_img_ln243_0;
    reg [13:0] add_ln244_0;
    reg [15:0] memread_debayer_bayer_img_ln244;
    reg [13:0] add_ln245;
    reg [15:0] memread_debayer_bayer_img_ln245;
    reg [13:0] add_ln250_0;
    reg [15:0] memread_debayer_bayer_img_ln250;
    reg [13:0] add_ln251_0;
    reg [15:0] memread_debayer_bayer_img_ln251;
    reg [13:0] add_ln252_1;
    reg [15:0] memread_debayer_bayer_img_ln252;
    reg [13:0] add_ln253_0;
    reg [15:0] memread_debayer_bayer_img_ln253;
    reg [13:0] add_ln254_1;
    reg [15:0] memread_debayer_bayer_img_ln254;
    reg [13:0] add_ln255;
    reg [15:0] memread_debayer_bayer_img_ln255;
    reg [15:0] add_ln244_2;
    reg [2:0] case_mux_row_add_2_ln222;
    reg [2:0] case_mux_row_min_2_ln216;
    reg and_case_mux_row_min_2_ln216;
    reg and_case_mux_row_add_2_ln222_2;
    reg and_case_mux_row_add_2_ln222_0_0;
    reg and_case_mux_row_add_2_ln222_1_0;
    reg or_eq_ln210_Z_0;
    reg or_eq_ln210_Z_0_0;
    reg or_and_case_mux_row_min_2_ln216_0_Z_0;
    reg and_or_and_case_mux_row_min_2_ln216_0_Z_0;

    begin : interp_R_at_GBR_or_B_at_GRB_body
      CtoS_func_done_interp_R_at_GBR_or_B_at_GRB = 1'b0;
      begin
        read_row_in = row_in;
        read_col_in = col_in;
        case (read_row_in)
          32'h0: eq_ln210 = 2'h1;
          default: eq_ln210 = 2'h2;
        endcase
        sub_ln211 = read_row_in[2:0] - 2'h2;
        if (eq_ln210[0]) 
          add_ln211 = sub_ln211 + 3'h6;
        sub_ln212 = read_row_in[2:0] - 1'b1;
        if (eq_ln210[0]) 
          add_ln212 = sub_ln212 + 3'h6;
        add_ln213 = read_row_in[2:0] + 1'b1;
        add_ln214 = read_row_in[2:0] + 2'h2;
        if (eq_ln210[1]) begin
          case (read_row_in)
            32'h1: eq_ln216 = 2'h1;
            default: eq_ln216 = 2'h2;
          endcase
          if (eq_ln216[0]) 
            add_ln217 = sub_ln211 + 3'h6;
          if (eq_ln216[1]) begin
            case (read_row_in)
              32'h4: eq_ln222 = 2'h1;
              default: eq_ln222 = 2'h2;
            endcase
            if (eq_ln222[0]) 
              sub_ln223 = add_ln214 - 3'h6;
            if (eq_ln222[1]) begin
              case (read_row_in)
                32'h5: eq_ln228 = 2'h1;
                default: eq_ln228 = 2'h2;
              endcase
              if (eq_ln228[0]) begin
                sub_ln229 = add_ln213 - 3'h6;
                sub_ln230 = add_ln214 - 3'h6;
              end
            end
            case (1'b1)
              eq_ln222[0]: case_mux_row_add_2_ln222 = 3'h1;
              eq_ln228[0]: case_mux_row_add_2_ln222 = 3'h2;
              default: case_mux_row_add_2_ln222 = 3'h4;
            endcase
          end
          case (1'b1)
            eq_ln216[0]: case_mux_row_min_2_ln216 = 3'h1;
            eq_ln222[0]: case_mux_row_min_2_ln216 = 3'h2;
            default: case_mux_row_min_2_ln216 = 3'h4;
          endcase
        end
        and_case_mux_row_min_2_ln216 = &{eq_ln210[1], case_mux_row_min_2_ln216[0]};
        or_and_case_mux_row_min_2_ln216_0_Z_0 = |case_mux_row_min_2_ln216[2:1];
        and_or_and_case_mux_row_min_2_ln216_0_Z_0 = &{
        or_and_case_mux_row_min_2_ln216_0_Z_0, eq_ln210[1]};
        case (1'b1)
          eq_ln210[0]: mux_row_min_2_ln210 = add_ln211;
          and_case_mux_row_min_2_ln216: mux_row_min_2_ln210 = add_ln217;
          and_or_and_case_mux_row_min_2_ln216_0_Z_0: mux_row_min_2_ln210 = 
            sub_ln211;
          default: mux_row_min_2_ln210 = 3'hX;
        endcase
        mux_row_min_1_ln210 = eq_ln210[0] ? add_ln212 : sub_ln212;
        and_case_mux_row_add_2_ln222_2 = &{eq_ln216[1], case_mux_row_add_2_ln222
        [0], eq_ln210[1]};
        and_case_mux_row_add_2_ln222_0_0 = &{eq_ln216[1], 
        case_mux_row_add_2_ln222[1], eq_ln210[1]};
        and_case_mux_row_add_2_ln222_1_0 = &{eq_ln216[1], 
        case_mux_row_add_2_ln222[2], eq_ln210[1]};
        or_eq_ln210_Z_0 = |{and_case_mux_row_add_2_ln222_1_0, eq_ln216[0], 
        eq_ln210[0]};
        case (1'b1)
          or_eq_ln210_Z_0: mux_row_add_2_ln210 = add_ln214;
          and_case_mux_row_add_2_ln222_2: mux_row_add_2_ln210 = sub_ln223;
          and_case_mux_row_add_2_ln222_0_0: mux_row_add_2_ln210 = sub_ln230;
          default: mux_row_add_2_ln210 = 3'hX;
        endcase
        or_eq_ln210_Z_0_0 = |{and_case_mux_row_add_2_ln222_1_0, 
        and_case_mux_row_add_2_ln222_2, eq_ln216[0], eq_ln210[0]};
        mux_row_add_1_ln210 = or_eq_ln210_Z_0_0 ? add_ln213 : sub_ln229;
        add_ln242_0 = {mux_row_min_1_ln210, 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln242 = bayer_img[add_ln242_0];
        sub_ln243 = read_col_in - 2'h2;
        add_ln243_2 = {read_row_in[2:0], 11'h0} + sub_ln243;
        memread_debayer_bayer_img_ln243 = bayer_img[add_ln243_2];
        add_ln243 = read_col_in + 2'h2;
        add_ln243_4 = {read_row_in[2:0], 11'h0} + add_ln243;
        memread_debayer_bayer_img_ln243_0 = bayer_img[add_ln243_4];
        add_ln243_0 = memread_debayer_bayer_img_ln243 + 
        memread_debayer_bayer_img_ln243_0;
        add_ln242 = {memread_debayer_bayer_img_ln242[13:0], 2'h0} + add_ln243_0[
        16:1];
        add_ln244_0 = {read_row_in[2:0], 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln244 = bayer_img[add_ln244_0];
        add_ln244_2 = {memread_debayer_bayer_img_ln244[13:0], 2'h0} + 
        memread_debayer_bayer_img_ln244;
        add_ln243_1 = add_ln242 + add_ln244_2;
        add_ln245 = {mux_row_add_1_ln210, 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln245 = bayer_img[add_ln245];
        add_ln244 = add_ln243_1 + {memread_debayer_bayer_img_ln245[13:0], 2'h0};
        state_interp_R_at_GBR_or_B_at_GRB_next = 1'b0;
      end
      // This value for state is interpreted as undefined
      state_interp_R_at_GBR_or_B_at_GRB = 1'b1;
      while (!CtoS_func_done_interp_R_at_GBR_or_B_at_GRB) 
        begin
          case (state_interp_R_at_GBR_or_B_at_GRB)
            1'b0: // Wait_ln247
              begin
                add_ln250_0 = {mux_row_min_2_ln210, 11'h0} + read_col_in;
                memread_debayer_bayer_img_ln250 = bayer_img[add_ln250_0];
                sub_ln251 = read_col_in - 1'b1;
                add_ln251_0 = {mux_row_min_1_ln210, 11'h0} + sub_ln251;
                memread_debayer_bayer_img_ln251 = bayer_img[add_ln251_0];
                add_ln250 = memread_debayer_bayer_img_ln250 + 
                memread_debayer_bayer_img_ln251;
                add_ln252 = read_col_in + 1'b1;
                add_ln252_1 = {mux_row_min_1_ln210, 11'h0} + add_ln252;
                memread_debayer_bayer_img_ln252 = bayer_img[add_ln252_1];
                add_ln251 = memread_debayer_bayer_img_ln252 + add_ln250;
                add_ln253_0 = {mux_row_add_1_ln210, 11'h0} + sub_ln251;
                memread_debayer_bayer_img_ln253 = bayer_img[add_ln253_0];
                add_ln252_0 = memread_debayer_bayer_img_ln253 + add_ln251;
                add_ln254_1 = {mux_row_add_1_ln210, 11'h0} + add_ln252;
                memread_debayer_bayer_img_ln254 = bayer_img[add_ln254_1];
                add_ln253 = memread_debayer_bayer_img_ln254 + add_ln252_0;
                add_ln255 = {mux_row_add_2_ln210, 11'h0} + read_col_in;
                memread_debayer_bayer_img_ln255 = bayer_img[add_ln255];
                add_ln254_0 = memread_debayer_bayer_img_ln255 + add_ln253;
                compute_and_clamp_pixel(add_ln244, add_ln254_0, 
                compute_and_clamp_pixel_ln257);
                interp_R_at_GBR_or_B_at_GRB_out = compute_and_clamp_pixel_ln257;
                CtoS_func_done_interp_R_at_GBR_or_B_at_GRB = 1'b1;
              end
          endcase
          state_interp_R_at_GBR_or_B_at_GRB = 
          state_interp_R_at_GBR_or_B_at_GRB_next;
          if (!CtoS_func_done_interp_R_at_GBR_or_B_at_GRB) @(posedge clk);
        end
    end
  endtask

  task interp_R_at_BBB_or_B_at_RRR;
    input [31:0] row_in;
    input [13:0] col_in;
    output reg [13:0] interp_R_at_BBB_or_B_at_RRR_out;
    output reg CtoS_func_done_interp_R_at_BBB_or_B_at_RRR;
    reg state_interp_R_at_BBB_or_B_at_RRR;
    reg state_interp_R_at_BBB_or_B_at_RRR_next;
    reg [31:0] read_row_in;
    reg [13:0] read_col_in;
    reg [1:0] eq_ln273;
    reg [2:0] sub_ln274;
    reg [2:0] add_ln274;
    reg [2:0] sub_ln275;
    reg [2:0] add_ln275;
    reg [2:0] add_ln276;
    reg [2:0] add_ln277;
    reg [13:0] sub_ln305;
    reg [13:0] add_ln306;
    reg [14:0] add_ln305;
    reg [14:0] add_ln306_0;
    reg [14:0] add_ln307;
    reg [14:0] add_ln308;
    reg [13:0] sub_ln315;
    reg [15:0] add_ln314;
    reg [13:0] add_ln316;
    reg [15:0] add_ln315;
    reg [15:0] add_ln316_0;
    reg [13:0] compute_and_clamp_pixel_fractional_neg_ln319;
    reg [1:0] eq_ln279;
    reg [2:0] add_ln280;
    reg [1:0] eq_ln285;
    reg [2:0] sub_ln286;
    reg [1:0] eq_ln291;
    reg [2:0] sub_ln292;
    reg [2:0] sub_ln293;
    reg [2:0] mux_row_min_2_ln273;
    reg [2:0] mux_row_min_1_ln273;
    reg [2:0] mux_row_add_2_ln273;
    reg [2:0] mux_row_add_1_ln273;
    reg [13:0] add_ln305_0;
    reg [15:0] memread_debayer_bayer_img_ln305;
    reg [13:0] add_ln306_1;
    reg [15:0] memread_debayer_bayer_img_ln306;
    reg [13:0] add_ln307_0;
    reg [15:0] memread_debayer_bayer_img_ln307;
    reg [13:0] add_ln308_0;
    reg [15:0] memread_debayer_bayer_img_ln308;
    reg [13:0] add_ln309_0;
    reg [15:0] memread_debayer_bayer_img_ln309;
    reg [13:0] add_ln314_0;
    reg [15:0] memread_debayer_bayer_img_ln314;
    reg [13:0] add_ln315_0;
    reg [15:0] memread_debayer_bayer_img_ln315;
    reg [13:0] add_ln316_1;
    reg [15:0] memread_debayer_bayer_img_ln316;
    reg [13:0] add_ln317;
    reg [15:0] memread_debayer_bayer_img_ln317;
    reg [14:0] add_ln307_2;
    reg [15:0] add_ln314_2;
    reg [15:0] add_ln315_2;
    reg [15:0] add_ln316_3;
    reg [15:0] add_ln317_1;
    reg [2:0] case_mux_row_add_2_ln285;
    reg [2:0] case_mux_row_min_2_ln279;
    reg and_case_mux_row_min_2_ln279;
    reg and_case_mux_row_add_2_ln285_2;
    reg and_case_mux_row_add_2_ln285_0_0;
    reg and_case_mux_row_add_2_ln285_1_0;
    reg or_eq_ln273_Z_0;
    reg or_eq_ln273_Z_0_0;
    reg or_and_case_mux_row_min_2_ln279_0_Z_0;
    reg and_or_and_case_mux_row_min_2_ln279_0_Z_0;

    begin : interp_R_at_BBB_or_B_at_RRR_body
      CtoS_func_done_interp_R_at_BBB_or_B_at_RRR = 1'b0;
      begin
        read_row_in = row_in;
        read_col_in = col_in;
        case (read_row_in)
          32'h0: eq_ln273 = 2'h1;
          default: eq_ln273 = 2'h2;
        endcase
        sub_ln274 = read_row_in[2:0] - 2'h2;
        if (eq_ln273[0]) 
          add_ln274 = sub_ln274 + 3'h6;
        sub_ln275 = read_row_in[2:0] - 1'b1;
        if (eq_ln273[0]) 
          add_ln275 = sub_ln275 + 3'h6;
        add_ln276 = read_row_in[2:0] + 1'b1;
        add_ln277 = read_row_in[2:0] + 2'h2;
        if (eq_ln273[1]) begin
          case (read_row_in)
            32'h1: eq_ln279 = 2'h1;
            default: eq_ln279 = 2'h2;
          endcase
          if (eq_ln279[0]) 
            add_ln280 = sub_ln274 + 3'h6;
          if (eq_ln279[1]) begin
            case (read_row_in)
              32'h4: eq_ln285 = 2'h1;
              default: eq_ln285 = 2'h2;
            endcase
            if (eq_ln285[0]) 
              sub_ln286 = add_ln277 - 3'h6;
            if (eq_ln285[1]) begin
              case (read_row_in)
                32'h5: eq_ln291 = 2'h1;
                default: eq_ln291 = 2'h2;
              endcase
              if (eq_ln291[0]) begin
                sub_ln292 = add_ln276 - 3'h6;
                sub_ln293 = add_ln277 - 3'h6;
              end
            end
            case (1'b1)
              eq_ln285[0]: case_mux_row_add_2_ln285 = 3'h1;
              eq_ln291[0]: case_mux_row_add_2_ln285 = 3'h2;
              default: case_mux_row_add_2_ln285 = 3'h4;
            endcase
          end
          case (1'b1)
            eq_ln279[0]: case_mux_row_min_2_ln279 = 3'h1;
            eq_ln285[0]: case_mux_row_min_2_ln279 = 3'h2;
            default: case_mux_row_min_2_ln279 = 3'h4;
          endcase
        end
        and_case_mux_row_min_2_ln279 = &{eq_ln273[1], case_mux_row_min_2_ln279[0]};
        or_and_case_mux_row_min_2_ln279_0_Z_0 = |case_mux_row_min_2_ln279[2:1];
        and_or_and_case_mux_row_min_2_ln279_0_Z_0 = &{
        or_and_case_mux_row_min_2_ln279_0_Z_0, eq_ln273[1]};
        case (1'b1)
          eq_ln273[0]: mux_row_min_2_ln273 = add_ln274;
          and_case_mux_row_min_2_ln279: mux_row_min_2_ln273 = add_ln280;
          and_or_and_case_mux_row_min_2_ln279_0_Z_0: mux_row_min_2_ln273 = 
            sub_ln274;
          default: mux_row_min_2_ln273 = 3'hX;
        endcase
        mux_row_min_1_ln273 = eq_ln273[0] ? add_ln275 : sub_ln275;
        and_case_mux_row_add_2_ln285_2 = &{eq_ln279[1], case_mux_row_add_2_ln285
        [0], eq_ln273[1]};
        and_case_mux_row_add_2_ln285_0_0 = &{eq_ln279[1], 
        case_mux_row_add_2_ln285[1], eq_ln273[1]};
        and_case_mux_row_add_2_ln285_1_0 = &{eq_ln279[1], 
        case_mux_row_add_2_ln285[2], eq_ln273[1]};
        or_eq_ln273_Z_0 = |{and_case_mux_row_add_2_ln285_1_0, eq_ln279[0], 
        eq_ln273[0]};
        case (1'b1)
          or_eq_ln273_Z_0: mux_row_add_2_ln273 = add_ln277;
          and_case_mux_row_add_2_ln285_2: mux_row_add_2_ln273 = sub_ln286;
          and_case_mux_row_add_2_ln285_0_0: mux_row_add_2_ln273 = sub_ln293;
          default: mux_row_add_2_ln273 = 3'hX;
        endcase
        or_eq_ln273_Z_0_0 = |{and_case_mux_row_add_2_ln285_1_0, 
        and_case_mux_row_add_2_ln285_2, eq_ln279[0], eq_ln273[0]};
        mux_row_add_1_ln273 = or_eq_ln273_Z_0_0 ? add_ln276 : sub_ln292;
        sub_ln305 = read_col_in - 1'b1;
        add_ln305_0 = {mux_row_min_1_ln273, 11'h0} + sub_ln305;
        memread_debayer_bayer_img_ln305 = bayer_img[add_ln305_0];
        add_ln306 = read_col_in + 1'b1;
        add_ln306_1 = {mux_row_min_1_ln273, 11'h0} + add_ln306;
        memread_debayer_bayer_img_ln306 = bayer_img[add_ln306_1];
        add_ln305 = memread_debayer_bayer_img_ln305[14:0] + 
        memread_debayer_bayer_img_ln306[14:0];
        add_ln307_0 = {read_row_in[2:0], 11'h0} + read_col_in;
        memread_debayer_bayer_img_ln307 = bayer_img[add_ln307_0];
        add_ln307_2 = {memread_debayer_bayer_img_ln307[13:0], 1'b0} + 
        memread_debayer_bayer_img_ln307[14:0];
        add_ln306_0 = add_ln305 + add_ln307_2;
        add_ln308_0 = {mux_row_add_1_ln273, 11'h0} + sub_ln305;
        memread_debayer_bayer_img_ln308 = bayer_img[add_ln308_0];
        add_ln307 = add_ln306_0 + memread_debayer_bayer_img_ln308[14:0];
        add_ln309_0 = {mux_row_add_1_ln273, 11'h0} + add_ln306;
        memread_debayer_bayer_img_ln309 = bayer_img[add_ln309_0];
        add_ln308 = add_ln307 + memread_debayer_bayer_img_ln309[14:0];
        state_interp_R_at_BBB_or_B_at_RRR_next = 1'b0;
      end
      // This value for state is interpreted as undefined
      state_interp_R_at_BBB_or_B_at_RRR = 1'b1;
      while (!CtoS_func_done_interp_R_at_BBB_or_B_at_RRR) 
        begin
          case (state_interp_R_at_BBB_or_B_at_RRR)
            1'b0: // Wait_ln311
              begin
                add_ln314_0 = {mux_row_min_2_ln273, 11'h0} + read_col_in;
                memread_debayer_bayer_img_ln314 = bayer_img[add_ln314_0];
                add_ln314_2 = {memread_debayer_bayer_img_ln314[14:0], 1'b0} + 
                memread_debayer_bayer_img_ln314;
                sub_ln315 = read_col_in - 2'h2;
                add_ln315_0 = {read_row_in[2:0], 11'h0} + sub_ln315;
                memread_debayer_bayer_img_ln315 = bayer_img[add_ln315_0];
                add_ln315_2 = {memread_debayer_bayer_img_ln315[14:0], 1'b0} + 
                memread_debayer_bayer_img_ln315;
                add_ln314 = add_ln314_2 + add_ln315_2;
                add_ln316 = read_col_in + 2'h2;
                add_ln316_1 = {read_row_in[2:0], 11'h0} + add_ln316;
                memread_debayer_bayer_img_ln316 = bayer_img[add_ln316_1];
                add_ln316_3 = {memread_debayer_bayer_img_ln316[14:0], 1'b0} + 
                memread_debayer_bayer_img_ln316;
                add_ln315 = add_ln314 + add_ln316_3;
                add_ln317 = {mux_row_add_2_ln273, 11'h0} + read_col_in;
                memread_debayer_bayer_img_ln317 = bayer_img[add_ln317];
                add_ln317_1 = {memread_debayer_bayer_img_ln317[14:0], 1'b0} + 
                memread_debayer_bayer_img_ln317;
                add_ln316_0 = add_ln315 + add_ln317_1;
                compute_and_clamp_pixel_fractional_neg(add_ln308, add_ln316_0, 
                compute_and_clamp_pixel_fractional_neg_ln319);
                interp_R_at_BBB_or_B_at_RRR_out = 
                compute_and_clamp_pixel_fractional_neg_ln319;
                CtoS_func_done_interp_R_at_BBB_or_B_at_RRR = 1'b1;
              end
          endcase
          state_interp_R_at_BBB_or_B_at_RRR = 
          state_interp_R_at_BBB_or_B_at_RRR_next;
          if (!CtoS_func_done_interp_R_at_BBB_or_B_at_RRR) @(posedge clk);
        end
    end
  endtask

  task compute_and_clamp_pixel_fractional_neg;
    input [14:0] pos_in;
    input [15:0] neg_in;
    output reg [13:0] compute_and_clamp_pixel_fractional_neg_out;
    reg [14:0] read_pos_in;
    reg [15:0] read_neg_in;
    reg lt_ln42;
    reg [17:0] sub_ln48;
    reg [13:0] mux_compute_and_clamp_pixel_fractional_neg_ln28;

    begin : compute_and_clamp_pixel_fractional_neg_body
      begin
        read_pos_in = pos_in;
        read_neg_in = neg_in;
        lt_ln42 = {read_pos_in, 2'h0} < read_neg_in;
        if (!lt_ln42) 
          sub_ln48 = {read_pos_in, 2'h0} - read_neg_in;
        mux_compute_and_clamp_pixel_fractional_neg_ln28 = lt_ln42 ? 14'h0 : 
        sub_ln48[17:4];
        compute_and_clamp_pixel_fractional_neg_out = 
        mux_compute_and_clamp_pixel_fractional_neg_ln28;
      end
    end
  endtask

  always @(posedge clk) // Disable set of sequential processes
    begin
      if (!rst) begin
        disable debayer_process_debayer;
        disable wami_debayer;
        disable interp_G_at_RRR_or_G_at_BBB;
        disable interp_R_at_GRB_or_B_at_GBR;
        disable interp_R_at_GBR_or_B_at_GRB;
        disable interp_R_at_BBB_or_B_at_RRR;
      end
      if (!rst) disable debayer_store_output;
      if (!rst) disable debayer_load_input;
      if (!rst) disable debayer_config_debayer;
    end
endmodule


