#include <systemc.h>
#include <fstream>

SC_MODULE(counter)
{

   sc_out < sc_biguint<17> > cnt_out;
   sc_in <bool> reset;
   sc_in <bool> up_ctrl;
   sc_in <bool> counter_en;
   sc_clock clk;
   sc_out <bool> ovf_intr;
   sc_out <bool> und_intr;
   sc_biguint<2> myValue = pow(2,2)-1;
  bool ovf_flag = false;
  bool und_flag = false;
  bool overFlow_reset_flag = false;
  bool underFlow_reset_flag = false;
  bool reset_flag = true;
  void myCounter();

    SC_CTOR(counter){
    SC_METHOD(myCounter);
    sensitive<<counter_en<<clk<<reset<<up_ctrl;
    dont_initialize();
      }

};

SC_MODULE(triggered)
{
   sc_in < sc_biguint<2> > cnt_out;
   sc_clock clk;
   sc_in <bool> counter_en;
   ofstream myfile;

  void myTrigger();
  void fileWriter();

  SC_CTOR(triggered){
    	SC_METHOD(myTrigger);

    	sensitive<<cnt_out<<counter_en;
    dont_initialize();

      }
};

SC_MODULE(initializer)
{
   	sc_out < sc_biguint<2> > cnt_out;
   	sc_clock clk;
	sc_out <bool> reset;
	sc_out <bool> counter_en;
	sc_out <bool> und_intr;
  	sc_out <bool> ovf_intr;
  	sc_out <bool> up_ctrl;

  void initialize_Me();

  SC_CTOR(initializer){
    SC_THREAD(initialize_Me);

      }
};

SC_MODULE(testBench)
{
   sc_in < sc_biguint<2> > cnt_out;
   sc_out <bool> reset;
   sc_out <bool> up_ctrl;
   sc_out <bool> counter_en;
   sc_clock clk;
   sc_in <bool> ovf_intr;
   sc_in <bool> und_intr;
   void myTest();

  SC_CTOR(testBench){
	SC_METHOD(myTest);

    	sensitive<<ovf_intr<<und_intr;
        dont_initialize();


      }
};


