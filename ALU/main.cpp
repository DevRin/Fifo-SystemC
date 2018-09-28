#include "alu.h"
#include <cstdlib>

   void initializer::initialize_Me(){

	op_sel.write("000");
   	reset.write(false);
   	sc_bv <32> rand_vector_maker;
    	register_one_in.write("00000000000000000000000000000000");
    	register_two_in.write("00000000000000000000000000000000");
    	register_one_out.write("00000000000000000000000000000000");
    	register_two_out.write("00000000000000000000000000000000");
    	accumulator.write("00000000000000000000000000000000");
    	status.write("00000000");

	cout<<"got into initializer, values have been set now"<<endl;

   }



      void reseting::reset_Me(){
	if (resetRead.read()==true){
	op_sel.write("000");
   	sc_bv <32> rand_vector_maker;
	for (int i=0; i<=31;i++){

   		rand_vector_maker[i]="0";

	}
    	register_one_in.write(rand_vector_maker);
    	register_two_in.write(rand_vector_maker);
    	register_one_out.write("00000000000000000000000000000000");
    	register_two_out.write("00000000000000000000000000000000");
    	accumulator.write("00000000000000000000000000000000");
    	status.write("00000000");

	cout<<"got into Reset, values have been reset now"<<endl;
	}

	resetWrite.write(false);
   }

   void alu::operations(){

   	sc_bv<3> statusFlags;
   	statusFlags = "00000000";
   	statusFlags[0]='1';


	sc_bv<32> myAccumulator;

	if (reset.read()!=true){

		if (op_sel.read() == "000"){
			myAccumulator=register_one_in.read() & register_two_in.read();
			}
		else if (op_sel.read() == "001"){
			myAccumulator=register_one_in.read() | register_two_in.read();
		}
		else if (op_sel.read() == "010"){
			myAccumulator=register_one_in.read() ^ register_two_in.read();
		}
		else if (op_sel.read() == "011"){
			myAccumulator=register_one_in.read() >> 1;
		}
		else if (op_sel.read() == "100"){
			myAccumulator=register_one_in.read() << 1;
		}
		else if (op_sel.read() == "101"){
			myAccumulator=(register_one_in.read().to_int() + register_two_in.read().to_int());

			if (myAccumulator.to_int()<register_one_in.read().to_int() || myAccumulator.to_int()<register_two_in.read().to_int()){
				statusFlags[3]='1';
				statusFlags[1]='1';

			}

			}
		else if (op_sel.read() == "110"){
			myAccumulator=(register_one_in.read().to_int() * register_two_in.read().to_int());
		}
		else if (op_sel.read() == "111"){
			myAccumulator=(register_one_in.read().to_int() / register_two_in.read().to_int());
		}

		accumulator.write(myAccumulator);

		if (op_sel.read()!= "100" || op_sel.read()!= "011"){
		cout << "Operatior is: "<<op_sel.read()<<endl;
		cout << "register_one is "<<register_one_in.read()<<endl;
		cout << "register_two is "<<register_two_in.read()<<endl;
		cout << "Accumulator Value is "<<myAccumulator<<endl;
			}



		if (op_sel.read()== "100" || op_sel.read()== "011"){
		cout << "Operatior is: "<<op_sel.read()<<endl;
		cout << "register_one is "<<register_one_in.read()<<endl;
		cout << "Accumulator value is "<<myAccumulator<<endl;
			}

		for (int i=0;i<=100;i++)
		{

			cout << "=";

		}
		cout << endl;


		for (int i=0;i<32;i++){

			if (myAccumulator[i]=='1'){
				statusFlags[0] = '0';

			}

		}

		if (myAccumulator[31]=='1')
		{    	statusFlags[2]='1'; }

		status.write(statusFlags);







		}




}

   void testBench::feed(){
   	sc_bv <3> rand_operation_maker;
   	int rst_feed;
   	rand_operation_maker[0]=rand()%2;
   	rand_operation_maker[1]=rand()%2;
   	rand_operation_maker[2]=rand()%2;
   	op_sel.write(rand_operation_maker);

	for (int i=0; i<=31;i++){

   		rand_operation_maker[i]=rand()%2;

	}


    	register_one_in.write(rand_operation_maker);


    	for (int i=0; i<=31;i++){

   		rand_operation_maker[i]=rand()%2;

	}
    	register_two_in.write(rand_operation_maker);

    	rst_feed = rand()%10;
    	if (rst_feed<1){

    	reset.write(true);
    	}




   }

int sc_main(int argc, char * argv[]) {

    sc_signal < sc_bv<3>, SC_MANY_WRITERS > op_sel_sig;
    sc_signal <bool, SC_MANY_WRITERS > reset_sig;
    sc_signal < sc_bv<32>, SC_MANY_WRITERS  > register_one_in_sig;
    sc_signal < sc_bv<32>, SC_MANY_WRITERS  > register_one_out_sig;
    sc_signal < sc_bv<32>, SC_MANY_WRITERS  > register_two_in_sig;
    sc_signal < sc_bv<32>, SC_MANY_WRITERS  > register_two_out_sig;
    sc_signal < sc_bv<32>, SC_MANY_WRITERS  > accumulator_sig;
    sc_signal < sc_bv<8>, SC_MANY_WRITERS  > status_sig;



	initializer init("init");

	init.op_sel(op_sel_sig);
	//count.clk(myClock);
	init.register_one_in(register_one_in_sig);
	init.register_one_out(register_one_out_sig);
	init.register_two_in(register_two_in_sig);
	init.register_two_out(register_two_out_sig);
	init.accumulator(accumulator_sig);
	init.status(status_sig);
	init.reset(reset_sig);

	alu alu("alu");

	alu.op_sel(op_sel_sig);
	//count.clk(myClock);
	alu.register_one_in(register_one_in_sig);
	alu.register_one_out(register_one_out_sig);
	alu.register_two_in(register_two_in_sig);
	alu.register_two_out(register_two_out_sig);
	alu.accumulator(accumulator_sig);
	alu.status(status_sig);
	alu.reset(reset_sig);

	testBench test("test");

	test.op_sel(op_sel_sig);
	//count.clk(myClock);
	test.register_one_in(register_one_in_sig);
	test.register_one_out(register_one_out_sig);
	test.register_two_in(register_two_in_sig);
	test.register_two_out(register_two_out_sig);
	test.accumulator(accumulator_sig);
	test.status(status_sig);
	test.reset(reset_sig);



	reseting rst("rst");

	rst.op_sel(op_sel_sig);
	//count.clk(myClock);
	rst.register_one_in(register_one_in_sig);
	rst.register_one_out(register_one_out_sig);
	rst.register_two_in(register_two_in_sig);
	rst.register_two_out(register_two_out_sig);
	rst.accumulator(accumulator_sig);
	rst.status(status_sig);
	rst.resetRead(reset_sig);
	rst.resetWrite(reset_sig);



	sc_trace_file *tf = sc_create_vcd_trace_file("ALU");

	sc_trace(tf,op_sel_sig,"op_sel_sig");
	sc_trace(tf,reset_sig,"reset_sig");
	sc_trace(tf,register_one_in_sig,"register_one_in_sig");
	sc_trace(tf,register_one_out_sig,"register_one_out_sig");
	sc_trace(tf,register_two_in_sig,"register_two_in_sig");
	sc_trace(tf,register_two_out_sig,"register_two_out_sig");
	sc_trace(tf,accumulator_sig,"accumulator_sig");
	sc_trace(tf,status_sig,"status_sig");


	sc_start(10, SC_NS);

	//sc_start();

	return 0;

}
