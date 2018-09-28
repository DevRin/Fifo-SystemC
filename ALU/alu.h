#include <systemc.h>


SC_MODULE(alu)
{
    sc_in < sc_bv<3> > op_sel;
    sc_in <bool> reset;
    sc_clock clk;
    sc_in < sc_bv<32> > register_one_in;
    sc_out < sc_bv<32> > register_one_out;
    sc_in < sc_bv<32> > register_two_in;
    sc_out < sc_bv<32> > register_two_out;
    sc_out < sc_bv<32> > accumulator;
    sc_out < sc_bv<8> > status;

   void operations();
    SC_CTOR(alu){
    SC_METHOD(operations);
    sensitive<<op_sel;
    dont_initialize();
      }
};

SC_MODULE(initializer)
{
    sc_out < sc_bv<3> > op_sel;
    sc_out <bool> reset;
    sc_clock clk;
    sc_out < sc_bv<32> > register_one_in;
    sc_out < sc_bv<32> > register_one_out;
    sc_out < sc_bv<32> > register_two_in;
    sc_out < sc_bv<32> > register_two_out;
    sc_out < sc_bv<32> > accumulator;
    sc_out < sc_bv<8> > status;

    void initialize_Me();

  SC_CTOR(initializer){
    SC_THREAD(initialize_Me);
      }


};


SC_MODULE(reseting)
{
    sc_out < sc_bv<3> > op_sel;
    sc_in <bool> resetRead;
    sc_out <bool> resetWrite;
    sc_clock clk;
    sc_out < sc_bv<32> > register_one_in;
    sc_out < sc_bv<32> > register_one_out;
    sc_out < sc_bv<32> > register_two_in;
    sc_out < sc_bv<32> > register_two_out;
    sc_out < sc_bv<32> > accumulator;
    sc_out < sc_bv<8> > status;

    void reset_Me();

  SC_CTOR(reseting){
    SC_METHOD(reset_Me);
    sensitive<<resetRead;
    dont_initialize();
      }


};


SC_MODULE(testBench)
{
    sc_out < sc_bv<3> > op_sel;
    sc_out <bool> reset;
    sc_clock clk;
    sc_in < sc_bv<32> > register_one_out;
    sc_out < sc_bv<32> > register_one_in;
    sc_out < sc_bv<32> > register_two_in;
    sc_in < sc_bv<32> > register_two_out;
    sc_in < sc_bv<32> > accumulator;
    sc_in < sc_bv<8> > status;

   void feed();
    SC_CTOR(testBench){
    SC_METHOD(feed);
    sensitive<<clk;
    dont_initialize();
      }
};

