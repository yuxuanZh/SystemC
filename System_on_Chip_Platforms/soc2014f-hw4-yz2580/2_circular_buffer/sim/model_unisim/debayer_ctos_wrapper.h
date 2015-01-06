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
#ifndef debayer_ctos_wrapper_P
#define debayer_ctos_wrapper_P

#include <systemc.h>

#define CTOS_TARGET_SUFFIX(ARG) CTOS_TARGET_SUFFIX_TMP(ARG)
#define CTOS_TARGET_SUFFIX_TMP(ARG) "_" #ARG


template <int _N_ctos_module_proxy_debayer>
class ctos_module_proxy_debayer{
};

template <> 
class ctos_module_proxy_debayer<11> : public sc_module {
  public:
    ctos_module_proxy_debayer(sc_module_name name = "")
    :   sc_module(name),
        valid("valid"),
        data("data"),
        ready("ready")
    {}
    sc_in<bool> valid;
    sc_in<unsigned short> data;
    sc_out<bool> ready;
    
    template <typename TP> 
    void bind(TP &par)
    {
        valid.bind(par.valid);
        data.bind(par.data);
        ready.bind(par.ready);
    }
    
    template <typename TP> 
    void operator()(TP &par)
    {
        bind(par);
    }
};

template <> 
class ctos_module_proxy_debayer<16> : public sc_module {
  public:
    ctos_module_proxy_debayer(sc_module_name name = "")
    :   sc_module(name),
        ready("ready"),
        valid("valid"),
        data("data")
    {}
    sc_in<bool> ready;
    sc_out<bool> valid;
    sc_out<rgb_pixel> data;
    
    template <typename TP> 
    void bind(TP &par)
    {
        ready.bind(par.ready);
        valid.bind(par.valid);
        data.bind(par.data);
    }
    
    template <typename TP> 
    void operator()(TP &par)
    {
        bind(par);
    }
};

// This foreign module encapsulates the CtoS generated Verilog models
class debayer_ctos_wrapper_vlog : public ncsc_foreign_module {
    SC_HAS_PROCESS(debayer_ctos_wrapper_vlog);
  public:
    debayer_ctos_wrapper_vlog(sc_module_name name, char  const *ctosModelSuffix)
    :   ncsc_foreign_module(name),
        clk("clk"),
        rst("rst"),
        rd_grant("rd_grant"),
        wr_grant("wr_grant"),
        conf_size("conf_size"),
        conf_done("conf_done"),
        bufdin_valid("bufdin_valid"),
        bufdin_data("bufdin_data"),
        bufdout_ready("bufdout_ready"),
        rd_index("rd_index"),
        rd_length("rd_length"),
        rd_request("rd_request"),
        wr_index("wr_index"),
        wr_length("wr_length"),
        wr_request("wr_request"),
        debayer_done("debayer_done"),
        bufdin_ready("bufdin_ready"),
        bufdout_valid("bufdout_valid"),
        bufdout_data("bufdout_data"),
        m_hdlName("debayer")
    {
        m_hdlName+=ctosModelSuffix;
    }
    sc_in<bool>  clk;
    sc_in<bool>  rst;
    sc_in<sc_lv<1> >  rd_grant;
    sc_in<sc_lv<1> >  wr_grant;
    sc_in<sc_lv<32> >  conf_size;
    sc_in<sc_lv<1> >  conf_done;
    sc_in<sc_lv<1> >  bufdin_valid;
    sc_in<sc_lv<16> >  bufdin_data;
    sc_in<sc_lv<1> >  bufdout_ready;
    sc_out<sc_lv<32> >  rd_index;
    sc_out<sc_lv<32> >  rd_length;
    sc_out<sc_lv<1> >  rd_request;
    sc_out<sc_lv<32> >  wr_index;
    sc_out<sc_lv<32> >  wr_length;
    sc_out<sc_lv<1> >  wr_request;
    sc_out<sc_lv<1> >  debayer_done;
    sc_out<sc_lv<1> >  bufdin_ready;
    sc_out<sc_lv<1> >  bufdout_valid;
    sc_out<sc_lv<48> >  bufdout_data;
    std::string m_hdlName;
    char  const *hdl_name() const
    {
        return m_hdlName.c_str();
    }
};


// This is the main module for the verification wrapper
class debayer_ctos_wrapper : public sc_module {
    SC_HAS_PROCESS(debayer_ctos_wrapper);
  public:
    debayer_ctos_wrapper(sc_module_name name, char  const *ctosDutSuffix = "", char  const *ctosRefSuffix = NULL, bool compare = false)
    :   sc_module(name),
        clk("clk"),
        rst("rst"),
        rd_grant("rd_grant"),
        wr_grant("wr_grant"),
        conf_size("conf_size"),
        conf_done("conf_done"),
        rd_index("rd_index"),
        rd_length("rd_length"),
        rd_request("rd_request"),
        wr_index("wr_index"),
        wr_length("wr_length"),
        wr_request("wr_request"),
        debayer_done("debayer_done"),
        m_enableCompare(true),
        rd_grant_vlog(NULL),
        wr_grant_vlog(NULL),
        conf_size_vlog(NULL),
        conf_done_vlog(NULL),
        bufdin_valid_vlog(NULL),
        bufdin_data_vlog(NULL),
        bufdout_ready_vlog(NULL),
        rd_index_vlog_dut(NULL),
        rd_index_vlog_ref(NULL),
        rd_index_cpp("rd_index_cpp"),
        rd_length_vlog_dut(NULL),
        rd_length_vlog_ref(NULL),
        rd_length_cpp("rd_length_cpp"),
        rd_request_vlog_dut(NULL),
        rd_request_vlog_ref(NULL),
        rd_request_cpp("rd_request_cpp"),
        wr_index_vlog_dut(NULL),
        wr_index_vlog_ref(NULL),
        wr_index_cpp("wr_index_cpp"),
        wr_length_vlog_dut(NULL),
        wr_length_vlog_ref(NULL),
        wr_length_cpp("wr_length_cpp"),
        wr_request_vlog_dut(NULL),
        wr_request_vlog_ref(NULL),
        wr_request_cpp("wr_request_cpp"),
        debayer_done_vlog_dut(NULL),
        debayer_done_vlog_ref(NULL),
        debayer_done_cpp("debayer_done_cpp"),
        bufdin_ready_vlog_dut(NULL),
        bufdin_ready_vlog_ref(NULL),
        bufdin_ready_cpp("bufdin_ready_cpp"),
        bufdout_valid_vlog_dut(NULL),
        bufdout_valid_vlog_ref(NULL),
        bufdout_valid_cpp("bufdout_valid_cpp"),
        bufdout_data_vlog_dut(NULL),
        bufdout_data_vlog_ref(NULL),
        bufdout_data_cpp("bufdout_data_cpp"),
        m_ctos_compare(compare),
        bufdin("bufdin"),
        bufdout("bufdout")
    {
        m_ctos_dut_type=modelTypeLookUp(ctosDutSuffix);
        m_ctos_ref_type=modelTypeLookUp(ctosRefSuffix);
        if (m_ctos_dut_type==ORIG) {
            m_dut_cpp=new debayer("m_dut_cpp");
            m_dut_cpp->clk(clk);
            m_dut_cpp->rst(rst);
            m_dut_cpp->rd_grant(rd_grant);
            m_dut_cpp->wr_grant(wr_grant);
            m_dut_cpp->conf_size(conf_size);
            m_dut_cpp->conf_done(conf_done);
            m_dut_cpp->bufdin.valid(bufdin.valid);
            m_dut_cpp->bufdin.data(bufdin.data);
            m_dut_cpp->bufdout.ready(bufdout.ready);
            m_dut_cpp->rd_index(rd_index);
            m_dut_cpp->rd_length(rd_length);
            m_dut_cpp->rd_request(rd_request);
            m_dut_cpp->wr_index(wr_index);
            m_dut_cpp->wr_length(wr_length);
            m_dut_cpp->wr_request(wr_request);
            m_dut_cpp->debayer_done(debayer_done);
            m_dut_cpp->bufdin.ready(bufdin.ready);
            m_dut_cpp->bufdout.valid(bufdout.valid);
            m_dut_cpp->bufdout.data(bufdout.data);
            std::cout << "CtoS Verification Wrapper: Instantiating module debayer as DUT" << std::endl;
        } else {
            m_dut_vlog=new debayer_ctos_wrapper_vlog("m_dut_vlog", ctosDutSuffix);
            m_dut_vlog->clk(clk);
            m_dut_vlog->rst(rst);
            rd_grant_vlog=new sc_signal<sc_lv<1> > ("rd_grant_vlog");
            m_dut_vlog->rd_grant(*rd_grant_vlog);
            wr_grant_vlog=new sc_signal<sc_lv<1> > ("wr_grant_vlog");
            m_dut_vlog->wr_grant(*wr_grant_vlog);
            conf_size_vlog=new sc_signal<sc_lv<32> > ("conf_size_vlog");
            m_dut_vlog->conf_size(*conf_size_vlog);
            conf_done_vlog=new sc_signal<sc_lv<1> > ("conf_done_vlog");
            m_dut_vlog->conf_done(*conf_done_vlog);
            bufdin_valid_vlog=new sc_signal<sc_lv<1> > ("bufdin_valid_vlog");
            m_dut_vlog->bufdin_valid(*bufdin_valid_vlog);
            bufdin_data_vlog=new sc_signal<sc_lv<16> > ("bufdin_data_vlog");
            m_dut_vlog->bufdin_data(*bufdin_data_vlog);
            bufdout_ready_vlog=new sc_signal<sc_lv<1> > ("bufdout_ready_vlog");
            m_dut_vlog->bufdout_ready(*bufdout_ready_vlog);
            rd_index_vlog_dut=new sc_signal<sc_lv<32> > ("rd_index_vlog_dut");
            m_dut_vlog->rd_index(*rd_index_vlog_dut);
            rd_length_vlog_dut=new sc_signal<sc_lv<32> > ("rd_length_vlog_dut");
            m_dut_vlog->rd_length(*rd_length_vlog_dut);
            rd_request_vlog_dut=new sc_signal<sc_lv<1> > ("rd_request_vlog_dut");
            m_dut_vlog->rd_request(*rd_request_vlog_dut);
            wr_index_vlog_dut=new sc_signal<sc_lv<32> > ("wr_index_vlog_dut");
            m_dut_vlog->wr_index(*wr_index_vlog_dut);
            wr_length_vlog_dut=new sc_signal<sc_lv<32> > ("wr_length_vlog_dut");
            m_dut_vlog->wr_length(*wr_length_vlog_dut);
            wr_request_vlog_dut=new sc_signal<sc_lv<1> > ("wr_request_vlog_dut");
            m_dut_vlog->wr_request(*wr_request_vlog_dut);
            debayer_done_vlog_dut=new sc_signal<sc_lv<1> > ("debayer_done_vlog_dut");
            m_dut_vlog->debayer_done(*debayer_done_vlog_dut);
            bufdin_ready_vlog_dut=new sc_signal<sc_lv<1> > ("bufdin_ready_vlog_dut");
            m_dut_vlog->bufdin_ready(*bufdin_ready_vlog_dut);
            bufdout_valid_vlog_dut=new sc_signal<sc_lv<1> > ("bufdout_valid_vlog_dut");
            m_dut_vlog->bufdout_valid(*bufdout_valid_vlog_dut);
            bufdout_data_vlog_dut=new sc_signal<sc_lv<48> > ("bufdout_data_vlog_dut");
            m_dut_vlog->bufdout_data(*bufdout_data_vlog_dut);
            std::cout << "CtoS Verification Wrapper: Instantiating module " << m_dut_vlog->hdl_name() << " as DUT" << std::endl;
        }
        if (m_ctos_ref_type!=UNKNOWN) {
            if (m_ctos_ref_type==ORIG) {
                m_ref_cpp=new debayer("m_ref_cpp");
                m_ref_cpp->clk(clk);
                m_ref_cpp->rst(rst);
                m_ref_cpp->rd_grant(rd_grant);
                m_ref_cpp->wr_grant(wr_grant);
                m_ref_cpp->conf_size(conf_size);
                m_ref_cpp->conf_done(conf_done);
                m_ref_cpp->bufdin.valid(bufdin.valid);
                m_ref_cpp->bufdin.data(bufdin.data);
                m_ref_cpp->bufdout.ready(bufdout.ready);
                m_ref_cpp->rd_index(rd_index_cpp);
                m_ref_cpp->rd_length(rd_length_cpp);
                m_ref_cpp->rd_request(rd_request_cpp);
                m_ref_cpp->wr_index(wr_index_cpp);
                m_ref_cpp->wr_length(wr_length_cpp);
                m_ref_cpp->wr_request(wr_request_cpp);
                m_ref_cpp->debayer_done(debayer_done_cpp);
                m_ref_cpp->bufdin.ready(bufdin_ready_cpp);
                m_ref_cpp->bufdout.valid(bufdout_valid_cpp);
                m_ref_cpp->bufdout.data(bufdout_data_cpp);
                std::cout << "CtoS Verification Wrapper: Instantiating module debayer as REF" << std::endl;
            } else {
                m_ref_vlog=new debayer_ctos_wrapper_vlog("m_ref_vlog", ctosRefSuffix);
                m_ref_vlog->clk(clk);
                m_ref_vlog->rst(rst);
                if (rd_grant_vlog==NULL) {
                    rd_grant_vlog=new sc_signal<sc_lv<1> > ("rd_grant_vlog");
                }
                m_ref_vlog->rd_grant(*rd_grant_vlog);
                if (wr_grant_vlog==NULL) {
                    wr_grant_vlog=new sc_signal<sc_lv<1> > ("wr_grant_vlog");
                }
                m_ref_vlog->wr_grant(*wr_grant_vlog);
                if (conf_size_vlog==NULL) {
                    conf_size_vlog=new sc_signal<sc_lv<32> > ("conf_size_vlog");
                }
                m_ref_vlog->conf_size(*conf_size_vlog);
                if (conf_done_vlog==NULL) {
                    conf_done_vlog=new sc_signal<sc_lv<1> > ("conf_done_vlog");
                }
                m_ref_vlog->conf_done(*conf_done_vlog);
                if (bufdin_valid_vlog==NULL) {
                    bufdin_valid_vlog=new sc_signal<sc_lv<1> > ("bufdin_valid_vlog");
                }
                m_ref_vlog->bufdin_valid(*bufdin_valid_vlog);
                if (bufdin_data_vlog==NULL) {
                    bufdin_data_vlog=new sc_signal<sc_lv<16> > ("bufdin_data_vlog");
                }
                m_ref_vlog->bufdin_data(*bufdin_data_vlog);
                if (bufdout_ready_vlog==NULL) {
                    bufdout_ready_vlog=new sc_signal<sc_lv<1> > ("bufdout_ready_vlog");
                }
                m_ref_vlog->bufdout_ready(*bufdout_ready_vlog);
                if (rd_index_vlog_ref==NULL) {
                    rd_index_vlog_ref=new sc_signal<sc_lv<32> > ("rd_index_vlog_ref");
                }
                m_ref_vlog->rd_index(*rd_index_vlog_ref);
                if (rd_length_vlog_ref==NULL) {
                    rd_length_vlog_ref=new sc_signal<sc_lv<32> > ("rd_length_vlog_ref");
                }
                m_ref_vlog->rd_length(*rd_length_vlog_ref);
                if (rd_request_vlog_ref==NULL) {
                    rd_request_vlog_ref=new sc_signal<sc_lv<1> > ("rd_request_vlog_ref");
                }
                m_ref_vlog->rd_request(*rd_request_vlog_ref);
                if (wr_index_vlog_ref==NULL) {
                    wr_index_vlog_ref=new sc_signal<sc_lv<32> > ("wr_index_vlog_ref");
                }
                m_ref_vlog->wr_index(*wr_index_vlog_ref);
                if (wr_length_vlog_ref==NULL) {
                    wr_length_vlog_ref=new sc_signal<sc_lv<32> > ("wr_length_vlog_ref");
                }
                m_ref_vlog->wr_length(*wr_length_vlog_ref);
                if (wr_request_vlog_ref==NULL) {
                    wr_request_vlog_ref=new sc_signal<sc_lv<1> > ("wr_request_vlog_ref");
                }
                m_ref_vlog->wr_request(*wr_request_vlog_ref);
                if (debayer_done_vlog_ref==NULL) {
                    debayer_done_vlog_ref=new sc_signal<sc_lv<1> > ("debayer_done_vlog_ref");
                }
                m_ref_vlog->debayer_done(*debayer_done_vlog_ref);
                if (bufdin_ready_vlog_ref==NULL) {
                    bufdin_ready_vlog_ref=new sc_signal<sc_lv<1> > ("bufdin_ready_vlog_ref");
                }
                m_ref_vlog->bufdin_ready(*bufdin_ready_vlog_ref);
                if (bufdout_valid_vlog_ref==NULL) {
                    bufdout_valid_vlog_ref=new sc_signal<sc_lv<1> > ("bufdout_valid_vlog_ref");
                }
                m_ref_vlog->bufdout_valid(*bufdout_valid_vlog_ref);
                if (bufdout_data_vlog_ref==NULL) {
                    bufdout_data_vlog_ref=new sc_signal<sc_lv<48> > ("bufdout_data_vlog_ref");
                }
                m_ref_vlog->bufdout_data(*bufdout_data_vlog_ref);
                std::cout << "CtoS Verification Wrapper: Instantiating module " << m_ref_vlog->hdl_name() << " as REF" << std::endl;
            }
        }
        if ((((unsigned int)m_ctos_dut_type)>1U)||((m_ctos_ref_type!=UNKNOWN)&&(((unsigned int)m_ctos_ref_type)>1U))) {
            SC_METHOD(ctos_convert_input);
            sensitive << rd_grant;
            SC_METHOD(ctos_convert_input_0);
            sensitive << wr_grant;
            SC_METHOD(ctos_convert_input_1);
            sensitive << conf_size;
            SC_METHOD(ctos_convert_input_2);
            sensitive << conf_done;
            SC_METHOD(ctos_convert_input_3);
            sensitive << bufdin.valid;
            SC_METHOD(ctos_convert_input_4);
            sensitive << bufdin.data;
            SC_METHOD(ctos_convert_input_5);
            sensitive << bufdout.ready;
            if (rd_index_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output);
                dont_initialize();
                sensitive << *rd_index_vlog_dut;
            }
            if (rd_length_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_0);
                dont_initialize();
                sensitive << *rd_length_vlog_dut;
            }
            if (rd_request_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_1);
                dont_initialize();
                sensitive << *rd_request_vlog_dut;
            }
            if (wr_index_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_2);
                dont_initialize();
                sensitive << *wr_index_vlog_dut;
            }
            if (wr_length_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_3);
                dont_initialize();
                sensitive << *wr_length_vlog_dut;
            }
            if (wr_request_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_4);
                dont_initialize();
                sensitive << *wr_request_vlog_dut;
            }
            if (debayer_done_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_5);
                dont_initialize();
                sensitive << *debayer_done_vlog_dut;
            }
            if (bufdin_ready_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_6);
                dont_initialize();
                sensitive << *bufdin_ready_vlog_dut;
            }
            if (bufdout_valid_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_7);
                dont_initialize();
                sensitive << *bufdout_valid_vlog_dut;
            }
            if (bufdout_data_vlog_dut!=NULL) {
                SC_METHOD(ctos_convert_output_8);
                dont_initialize();
                sensitive << *bufdout_data_vlog_dut;
            }
        }
        if ((m_ctos_ref_type!=UNKNOWN)&&m_ctos_compare) {
            SC_METHOD(ctos_compare_outputs);
            dont_initialize();
            sensitive << clk.neg();
        }
    }
    // clock clk period 20000 rise 0 fall 10000
    sc_in<bool>  clk;
    // reset rst false
    sc_in<bool>  rst;
    sc_in<bool>  rd_grant;
    sc_in<bool>  wr_grant;
    sc_in<unsigned int>  conf_size;
    sc_in<bool>  conf_done;
    sc_out<unsigned int>  rd_index;
    sc_out<unsigned int>  rd_length;
    sc_out<bool>  rd_request;
    sc_out<unsigned int>  wr_index;
    sc_out<unsigned int>  wr_length;
    sc_out<bool>  wr_request;
    sc_out<bool>  debayer_done;
    sc_event m_ctosWrapperError;
    bool m_enableCompare;
    
    enum SimulateRTLOnlyEnum {
        Unsupported = 0,
        SimulateRTLOnly = 1
    };

  private:
    
    enum ModelTypeEnum {
        ORIG = 0,
        VLOG = 2,
        UNKNOWN = 3
    };

    sc_signal<sc_lv<1> >  *rd_grant_vlog;
    sc_signal<sc_lv<1> >  *wr_grant_vlog;
    sc_signal<sc_lv<32> >  *conf_size_vlog;
    sc_signal<sc_lv<1> >  *conf_done_vlog;
    sc_signal<sc_lv<1> >  *bufdin_valid_vlog;
    sc_signal<sc_lv<16> >  *bufdin_data_vlog;
    sc_signal<sc_lv<1> >  *bufdout_ready_vlog;
    sc_signal<sc_lv<32> >  *rd_index_vlog_dut;
    sc_signal<sc_lv<32> >  *rd_index_vlog_ref;
    sc_signal<unsigned int>  rd_index_cpp;
    sc_signal<sc_lv<32> >  *rd_length_vlog_dut;
    sc_signal<sc_lv<32> >  *rd_length_vlog_ref;
    sc_signal<unsigned int>  rd_length_cpp;
    sc_signal<sc_lv<1> >  *rd_request_vlog_dut;
    sc_signal<sc_lv<1> >  *rd_request_vlog_ref;
    sc_signal<bool>  rd_request_cpp;
    sc_signal<sc_lv<32> >  *wr_index_vlog_dut;
    sc_signal<sc_lv<32> >  *wr_index_vlog_ref;
    sc_signal<unsigned int>  wr_index_cpp;
    sc_signal<sc_lv<32> >  *wr_length_vlog_dut;
    sc_signal<sc_lv<32> >  *wr_length_vlog_ref;
    sc_signal<unsigned int>  wr_length_cpp;
    sc_signal<sc_lv<1> >  *wr_request_vlog_dut;
    sc_signal<sc_lv<1> >  *wr_request_vlog_ref;
    sc_signal<bool>  wr_request_cpp;
    sc_signal<sc_lv<1> >  *debayer_done_vlog_dut;
    sc_signal<sc_lv<1> >  *debayer_done_vlog_ref;
    sc_signal<bool>  debayer_done_cpp;
    sc_signal<sc_lv<1> >  *bufdin_ready_vlog_dut;
    sc_signal<sc_lv<1> >  *bufdin_ready_vlog_ref;
    sc_signal<bool>  bufdin_ready_cpp;
    sc_signal<sc_lv<1> >  *bufdout_valid_vlog_dut;
    sc_signal<sc_lv<1> >  *bufdout_valid_vlog_ref;
    sc_signal<bool>  bufdout_valid_cpp;
    sc_signal<sc_lv<48> >  *bufdout_data_vlog_dut;
    sc_signal<sc_lv<48> >  *bufdout_data_vlog_ref;
    sc_signal<rgb_pixel>  bufdout_data_cpp;
    ModelTypeEnum m_ctos_dut_type;
    ModelTypeEnum m_ctos_ref_type;
    bool m_ctos_compare;
    
    union {
      public:
        debayer *m_ref_cpp;
        debayer_ctos_wrapper_vlog *m_ref_vlog;
    };

    
    union {
      public:
        debayer *m_dut_cpp;
        debayer_ctos_wrapper_vlog *m_dut_vlog;
    };

  public:
    ctos_module_proxy_debayer<11> bufdin;
    ctos_module_proxy_debayer<16> bufdout;
  private:
    // Converts input string to enumeration
    ModelTypeEnum modelTypeLookUp(char  const *str)
    {
        if (!str) {
            return UNKNOWN;
        }
        std::string inputString = str;
        if (inputString.empty()) {
            return ORIG;
        } else {
            return VLOG;
        }
    }
    
    template <class T, class U> 
    bool ctos_check_unknown_values(T  const *dutSig, U  const &refSig, std::string  const &dutName, std::string  const &refName)
    {
        if (!(dutSig->read().is_01())) {
            std::cout << sc_time_stamp()
            	      << ": ctos_check_unknown_values encountered unknown values!"
            	      << std::endl << dutName << ": " << (dutSig->read().to_string())
            	      << std::endl << refName << ": " << refSig << std::endl;
        }
        return true;
    }
    
    template <class T> 
    void ctos_compare_output_values(T  const &dutSig, T  const &refSig, std::string  const &dutName, std::string  const &refName)
    {
        if (!(dutSig==refSig)) {
            std::cout << sc_time_stamp()
            	      << ": ctos_compare_output_values mismatch!"
            	      << std::endl << dutName << ": " << dutSig
            	      << std::endl << refName << ": " << refSig << std::endl;
            m_ctosWrapperError.notify();
        }
    }
    void ctos_convert_input()
    {
        *rd_grant_vlog=sc_lv<1> ((rd_grant.read()));
    }
    void ctos_convert_input_0()
    {
        *wr_grant_vlog=sc_lv<1> ((wr_grant.read()));
    }
    void ctos_convert_input_1()
    {
        *conf_size_vlog=sc_lv<32> ((conf_size.read()));
    }
    void ctos_convert_input_2()
    {
        *conf_done_vlog=sc_lv<1> ((conf_done.read()));
    }
    void ctos_convert_input_3()
    {
        *bufdin_valid_vlog=sc_lv<1> ((bufdin.valid.read()));
    }
    void ctos_convert_input_4()
    {
        *bufdin_data_vlog=sc_lv<16> ((bufdin.data.read()));
    }
    void ctos_convert_input_5()
    {
        *bufdout_ready_vlog=sc_lv<1> ((bufdout.ready.read()));
    }
    void ctos_convert_output()
    {
        rd_index=rd_index_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_0()
    {
        rd_length=rd_length_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_1()
    {
        rd_request=rd_request_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_2()
    {
        wr_index=wr_index_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_3()
    {
        wr_length=wr_length_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_4()
    {
        wr_request=wr_request_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_5()
    {
        debayer_done=debayer_done_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_6()
    {
        bufdin.ready=bufdin_ready_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_7()
    {
        bufdout.valid=bufdout_valid_vlog_dut->read().to_uint();
    }
    void ctos_convert_output_8()
    {
        sc_lv<48>  bufdout_data_vlog_dut_ctos_in = bufdout_data_vlog_dut->read();
        rgb_pixel bufdout_data_ctos_tmp;
        lv_to_rgb_pixel(bufdout_data_vlog_dut_ctos_in, bufdout_data_ctos_tmp);
        bufdout.data.write(bufdout_data_ctos_tmp);
    }
    // Convert from sc_lv to rgb_pixel
    void lv_to_rgb_pixel(sc_lv<48>   const &lvType, rgb_pixel &userType)
    {
        userType.r=(lvType.range(15U,0U)).to_uint();
        userType.g=(lvType.range(31U,16U)).to_uint();
        userType.b=(lvType.range(47U,32U)).to_uint();
    }
    void ctos_compare_outputs()
    {
        if ((rst.read()!=0U)&&m_enableCompare) {
            rgb_pixel bufdout_data_ref;
            rgb_pixel bufdout_data_dut;
            bool bufdout_valid_ref;
            bool bufdout_valid_dut;
            bool bufdin_ready_ref;
            bool bufdin_ready_dut;
            bool debayer_done_ref;
            bool debayer_done_dut;
            bool wr_request_ref;
            bool wr_request_dut;
            unsigned int wr_length_ref;
            unsigned int wr_length_dut;
            unsigned int wr_index_ref;
            unsigned int wr_index_dut;
            bool rd_request_ref;
            bool rd_request_dut;
            unsigned int rd_length_ref;
            unsigned int rd_length_dut;
            unsigned int rd_index_ref;
            unsigned int rd_index_dut;
            rd_index_dut=rd_index->read();
            rd_length_dut=rd_length->read();
            rd_request_dut=rd_request->read();
            wr_index_dut=wr_index->read();
            wr_length_dut=wr_length->read();
            wr_request_dut=wr_request->read();
            debayer_done_dut=debayer_done->read();
            bufdin_ready_dut=bufdin.ready->read();
            bufdout_valid_dut=bufdout.valid->read();
            bufdout_data_dut=bufdout.data->read();
            if (m_ctos_ref_type==ORIG) {
                rd_index_ref=rd_index_cpp.read();
                rd_length_ref=rd_length_cpp.read();
                rd_request_ref=rd_request_cpp.read();
                wr_index_ref=wr_index_cpp.read();
                wr_length_ref=wr_length_cpp.read();
                wr_request_ref=wr_request_cpp.read();
                debayer_done_ref=debayer_done_cpp.read();
                bufdin_ready_ref=bufdin.ready.read();
                bufdout_valid_ref=bufdout.valid.read();
                bufdout_data_ref=bufdout.data.read();
            } else {
                rd_index_ref=rd_index_vlog_ref->read().to_uint();
                ctos_check_unknown_values(rd_index_vlog_ref, rd_index_ref, "rd_index_vlog_ref", "rd_index_ref");
                rd_length_ref=rd_length_vlog_ref->read().to_uint();
                ctos_check_unknown_values(rd_length_vlog_ref, rd_length_ref, "rd_length_vlog_ref", "rd_length_ref");
                rd_request_ref=rd_request_vlog_ref->read().to_uint();
                ctos_check_unknown_values(rd_request_vlog_ref, rd_request_ref, "rd_request_vlog_ref", "rd_request_ref");
                wr_index_ref=wr_index_vlog_ref->read().to_uint();
                ctos_check_unknown_values(wr_index_vlog_ref, wr_index_ref, "wr_index_vlog_ref", "wr_index_ref");
                wr_length_ref=wr_length_vlog_ref->read().to_uint();
                ctos_check_unknown_values(wr_length_vlog_ref, wr_length_ref, "wr_length_vlog_ref", "wr_length_ref");
                wr_request_ref=wr_request_vlog_ref->read().to_uint();
                ctos_check_unknown_values(wr_request_vlog_ref, wr_request_ref, "wr_request_vlog_ref", "wr_request_ref");
                debayer_done_ref=debayer_done_vlog_ref->read().to_uint();
                ctos_check_unknown_values(debayer_done_vlog_ref, debayer_done_ref, "debayer_done_vlog_ref", "debayer_done_ref");
                bufdin_ready_ref=bufdin_ready_vlog_ref->read().to_uint();
                ctos_check_unknown_values(bufdin_ready_vlog_ref, bufdin_ready_ref, "bufdin_ready_vlog_ref", "bufdin_ready_ref");
                bufdout_valid_ref=bufdout_valid_vlog_ref->read().to_uint();
                ctos_check_unknown_values(bufdout_valid_vlog_ref, bufdout_valid_ref, "bufdout_valid_vlog_ref", "bufdout_valid_ref");
                sc_lv<48>  bufdout_data_vlog_ref_ctos_in = bufdout_data_vlog_ref->read();
                lv_to_rgb_pixel(bufdout_data_vlog_ref_ctos_in, bufdout_data_ref);
                ctos_check_unknown_values(bufdout_data_vlog_ref, bufdout_data_ref, "bufdout_data_vlog_ref", "bufdout_data_ref");
            }
            if (m_ctos_dut_type==VLOG) {
                ctos_check_unknown_values(rd_index_vlog_dut, rd_index_ref, "rd_index_vlog_dut", "rd_index_ref");
                ctos_check_unknown_values(rd_length_vlog_dut, rd_length_ref, "rd_length_vlog_dut", "rd_length_ref");
                ctos_check_unknown_values(rd_request_vlog_dut, rd_request_ref, "rd_request_vlog_dut", "rd_request_ref");
                ctos_check_unknown_values(wr_index_vlog_dut, wr_index_ref, "wr_index_vlog_dut", "wr_index_ref");
                ctos_check_unknown_values(wr_length_vlog_dut, wr_length_ref, "wr_length_vlog_dut", "wr_length_ref");
                ctos_check_unknown_values(wr_request_vlog_dut, wr_request_ref, "wr_request_vlog_dut", "wr_request_ref");
                ctos_check_unknown_values(debayer_done_vlog_dut, debayer_done_ref, "debayer_done_vlog_dut", "debayer_done_ref");
                ctos_check_unknown_values(bufdin_ready_vlog_dut, bufdin_ready_ref, "bufdin_ready_vlog_dut", "bufdin_ready_ref");
                ctos_check_unknown_values(bufdout_valid_vlog_dut, bufdout_valid_ref, "bufdout_valid_vlog_dut", "bufdout_valid_ref");
                ctos_check_unknown_values(bufdout_data_vlog_dut, bufdout_data_ref, "bufdout_data_vlog_dut", "bufdout_data_ref");
            }
            ctos_compare_output_values(rd_index_dut, rd_index_ref, "rd_index_dut", "rd_index_ref");
            ctos_compare_output_values(rd_length_dut, rd_length_ref, "rd_length_dut", "rd_length_ref");
            ctos_compare_output_values(rd_request_dut, rd_request_ref, "rd_request_dut", "rd_request_ref");
            ctos_compare_output_values(wr_index_dut, wr_index_ref, "wr_index_dut", "wr_index_ref");
            ctos_compare_output_values(wr_length_dut, wr_length_ref, "wr_length_dut", "wr_length_ref");
            ctos_compare_output_values(wr_request_dut, wr_request_ref, "wr_request_dut", "wr_request_ref");
            ctos_compare_output_values(debayer_done_dut, debayer_done_ref, "debayer_done_dut", "debayer_done_ref");
            ctos_compare_output_values(bufdin_ready_dut, bufdin_ready_ref, "bufdin_ready_dut", "bufdin_ready_ref");
            ctos_compare_output_values(bufdout_valid_dut, bufdout_valid_ref, "bufdout_valid_dut", "bufdout_valid_ref");
            ctos_compare_output_values(bufdout_data_dut, bufdout_data_ref, "bufdout_data_dut", "bufdout_data_ref");
        }
    }
  public:
    // Use this constructor only if your CtoS verilog model has no suffix
    debayer_ctos_wrapper(sc_module_name name, SimulateRTLOnlyEnum simulateRTLOnly)
    :   sc_module(name),
        clk("clk"),
        rst("rst"),
        rd_grant("rd_grant"),
        wr_grant("wr_grant"),
        conf_size("conf_size"),
        conf_done("conf_done"),
        rd_index("rd_index"),
        rd_length("rd_length"),
        rd_request("rd_request"),
        wr_index("wr_index"),
        wr_length("wr_length"),
        wr_request("wr_request"),
        debayer_done("debayer_done"),
        m_enableCompare(false),
        m_ctos_compare(false),
        bufdin("bufdin"),
        bufdout("bufdout")
    {
        m_ctos_dut_type=VLOG;
        m_ctos_ref_type=UNKNOWN;
        m_dut_vlog=new debayer_ctos_wrapper_vlog("m_dut_vlog", "");
        m_dut_vlog->clk(clk);
        m_dut_vlog->rst(rst);
        rd_grant_vlog=new sc_signal<sc_lv<1> > ("rd_grant_vlog");
        m_dut_vlog->rd_grant(*rd_grant_vlog);
        wr_grant_vlog=new sc_signal<sc_lv<1> > ("wr_grant_vlog");
        m_dut_vlog->wr_grant(*wr_grant_vlog);
        conf_size_vlog=new sc_signal<sc_lv<32> > ("conf_size_vlog");
        m_dut_vlog->conf_size(*conf_size_vlog);
        conf_done_vlog=new sc_signal<sc_lv<1> > ("conf_done_vlog");
        m_dut_vlog->conf_done(*conf_done_vlog);
        bufdin_valid_vlog=new sc_signal<sc_lv<1> > ("bufdin_valid_vlog");
        m_dut_vlog->bufdin_valid(*bufdin_valid_vlog);
        bufdin_data_vlog=new sc_signal<sc_lv<16> > ("bufdin_data_vlog");
        m_dut_vlog->bufdin_data(*bufdin_data_vlog);
        bufdout_ready_vlog=new sc_signal<sc_lv<1> > ("bufdout_ready_vlog");
        m_dut_vlog->bufdout_ready(*bufdout_ready_vlog);
        rd_index_vlog_dut=new sc_signal<sc_lv<32> > ("rd_index_vlog_dut");
        m_dut_vlog->rd_index(*rd_index_vlog_dut);
        rd_length_vlog_dut=new sc_signal<sc_lv<32> > ("rd_length_vlog_dut");
        m_dut_vlog->rd_length(*rd_length_vlog_dut);
        rd_request_vlog_dut=new sc_signal<sc_lv<1> > ("rd_request_vlog_dut");
        m_dut_vlog->rd_request(*rd_request_vlog_dut);
        wr_index_vlog_dut=new sc_signal<sc_lv<32> > ("wr_index_vlog_dut");
        m_dut_vlog->wr_index(*wr_index_vlog_dut);
        wr_length_vlog_dut=new sc_signal<sc_lv<32> > ("wr_length_vlog_dut");
        m_dut_vlog->wr_length(*wr_length_vlog_dut);
        wr_request_vlog_dut=new sc_signal<sc_lv<1> > ("wr_request_vlog_dut");
        m_dut_vlog->wr_request(*wr_request_vlog_dut);
        debayer_done_vlog_dut=new sc_signal<sc_lv<1> > ("debayer_done_vlog_dut");
        m_dut_vlog->debayer_done(*debayer_done_vlog_dut);
        bufdin_ready_vlog_dut=new sc_signal<sc_lv<1> > ("bufdin_ready_vlog_dut");
        m_dut_vlog->bufdin_ready(*bufdin_ready_vlog_dut);
        bufdout_valid_vlog_dut=new sc_signal<sc_lv<1> > ("bufdout_valid_vlog_dut");
        m_dut_vlog->bufdout_valid(*bufdout_valid_vlog_dut);
        bufdout_data_vlog_dut=new sc_signal<sc_lv<48> > ("bufdout_data_vlog_dut");
        m_dut_vlog->bufdout_data(*bufdout_data_vlog_dut);
        std::cout << "CtoS Verification Wrapper: Instantiating RTL module " << m_dut_vlog->hdl_name() << " as DUT" << std::endl;
        SC_METHOD(ctos_convert_input);
        sensitive << rd_grant;
        SC_METHOD(ctos_convert_input_0);
        sensitive << wr_grant;
        SC_METHOD(ctos_convert_input_1);
        sensitive << conf_size;
        SC_METHOD(ctos_convert_input_2);
        sensitive << conf_done;
        SC_METHOD(ctos_convert_input_3);
        sensitive << bufdin.valid;
        SC_METHOD(ctos_convert_input_4);
        sensitive << bufdin.data;
        SC_METHOD(ctos_convert_input_5);
        sensitive << bufdout.ready;
        SC_METHOD(ctos_convert_output);
        dont_initialize();
        sensitive << *rd_index_vlog_dut;
        SC_METHOD(ctos_convert_output_0);
        dont_initialize();
        sensitive << *rd_length_vlog_dut;
        SC_METHOD(ctos_convert_output_1);
        dont_initialize();
        sensitive << *rd_request_vlog_dut;
        SC_METHOD(ctos_convert_output_2);
        dont_initialize();
        sensitive << *wr_index_vlog_dut;
        SC_METHOD(ctos_convert_output_3);
        dont_initialize();
        sensitive << *wr_length_vlog_dut;
        SC_METHOD(ctos_convert_output_4);
        dont_initialize();
        sensitive << *wr_request_vlog_dut;
        SC_METHOD(ctos_convert_output_5);
        dont_initialize();
        sensitive << *debayer_done_vlog_dut;
        SC_METHOD(ctos_convert_output_6);
        dont_initialize();
        sensitive << *bufdin_ready_vlog_dut;
        SC_METHOD(ctos_convert_output_7);
        dont_initialize();
        sensitive << *bufdout_valid_vlog_dut;
        SC_METHOD(ctos_convert_output_8);
        dont_initialize();
        sensitive << *bufdout_data_vlog_dut;
    }
};




    typedef debayer_ctos_wrapper debayer_ctos;
  #define DEBAYER_CTOS_INSTANCE(INSTNAME) debayer_ctos(INSTNAME, CTOS_TARGET_SUFFIX(CTOS_MODEL), NULL, false)
  #define DEBAYER_CTOS_COMPARE_INSTANCE(INSTNAME, REFNAME) debayer_ctos(INSTNAME, CTOS_TARGET_SUFFIX(CTOS_MODEL), REFNAME, true)
  #define DEBAYER_CTOS_RTL_ONLY_INSTANCE(INSTNAME) debayer_ctos(INSTNAME, debayer_ctos::SimulateRTLOnly)
#endif // ifdef debayer_ctos_wrapper_P

