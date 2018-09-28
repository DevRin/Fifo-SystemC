#include "Checking.h"


   void initializer::initialize_Me(){
   	//wait();
	reset.write(false);
	up_ctrl.write(false);
	counter_en.write(true);
	ovf_intr.write(false);
	und_intr.write(false);
	cnt_out.write(pow(2,2)-1);
	cout<<"got into initializer, values have been set now"<<endl;
//	myValue = "11111111111111111";
//	wait();
   }
   void testBench::myTest(){
   	//if (cnt_out.read().to_int() >=0 && cnt_out.read().to_int() < pow(2,17)-1){

		//}
	//while(1){
		if (ovf_intr.read()==false && und_intr.read()==false){
			reset.write(false);
			up_ctrl.write(false);
			counter_en.write(true);
			cout<< "overflow and underflow flags were false. counter has been enabled"<<endl;
			}
		else if (ovf_intr.read()==true && und_intr.read()==false){
			counter_en.write(false);
			reset.write(true);
			up_ctrl.write(false);
			counter_en.write(true);
			cout<< "overflow flag was true. Resetting and counting downwards"<<endl;

		}
		else if (ovf_intr.read()==false && und_intr.read()==true){
			counter_en.write(false);
			reset.write(false);
			up_ctrl.write(true);
			counter_en.write(true);
			cout<< "underflow flag was true. counting upwards"<<endl;
		}
		else if (ovf_intr.read()==true && und_intr.read()==true){
			cout<<"both underflow and overflow signals are true, something went wrong"<<endl;
			counter_en.write(false);

			//break;

		}
	//}

}




   void triggered::myTrigger(){


   	myfile.open ("/home/n_shirali18/Documents/newLogFile.txt", ios::out | ios::app);

	cout << "Time: " << sc_time_stamp() << endl;
        cout << "Counter value: " <<  cnt_out.read();
        cout << endl;
        myfile << "\n"<<"Time: " << sc_time_stamp() << "," << "Counter value: "<< "," << cnt_out.read();

        myfile.close();


        }



   void counter::myCounter(){
	//cout<<"hello from counter!"<<endl;
	if (reset.read()== true && reset_flag==true && (overFlow_reset_flag||underFlow_reset_flag)){
		if (overFlow_reset_flag==true){
			cout<<"reset is true so resetting to max"<<endl;
			myValue = pow(2,2)-1;
			cnt_out.write(myValue);
			overFlow_reset_flag = false;

		}


		else if (underFlow_reset_flag==true){
			cout<<"reset is true so resetting so reseting to next min"<<endl;
			myValue = 1;
			cnt_out.write(myValue);
			underFlow_reset_flag = false;

		}
	}
	if (counter_en.read()==true){
		cout<< "counter is enabled"<<endl;

		if (myValue >= (int) pow(2,2)-1 && reset_flag== false){
			cout<< "myValue greater than the expected one, so setting the overflow flag"<<endl;
			ovf_intr.write(true);
			und_intr.write(false);
			overFlow_reset_flag = true;
			reset_flag=true;
		}
		else if (myValue <= 0 && reset_flag==false){
			cout<< "myValue less than the expected one, so setting the underflow flag"<<endl;
			ovf_intr.write(false);
			und_intr.write(true);
			underFlow_reset_flag = true;
			reset_flag=true;

		}

		//wait();
		else if (up_ctrl.read() == true){
			cout<<"up_ctrl is true so counting upwards"<<endl;

			myValue = myValue+1;
			cnt_out.write(myValue);
			reset_flag = false;
			//und_intr.write(false);

		}
		else if (up_ctrl.read() == false){
			cout<<"up_ctrl is false so counting downwards"<<endl;
			myValue = myValue-1;
			cnt_out.write(myValue);
			reset_flag = false;
			//ovf_intr.write(false);
		}
		//cnt_out.write(myValue);


		//cout<<"counter is "<<myValue<<endl;



	}
//&& myValue.to_int()>0 && myValue.to_int()<=(pow(2,17)-1)



	}

int sc_main(int argc, char * argv[]) {




	sc_signal<bool, SC_MANY_WRITERS> counter_en_sig;
	sc_signal< sc_biguint<2> , SC_MANY_WRITERS> cnt_out_sig;
	//sc_clock myClock;
	sc_signal<bool, SC_MANY_WRITERS> reset_sig;
	sc_signal<bool, SC_MANY_WRITERS> up_ctrl_sig;
	sc_signal<bool, SC_MANY_WRITERS> ovf_intr_sig;
	sc_signal<bool, SC_MANY_WRITERS> und_intr_sig;

	initializer init("init");

	init.counter_en(counter_en_sig);
	//count.clk(myClock);
	init.cnt_out(cnt_out_sig);
	init.reset(reset_sig);
	init.up_ctrl(up_ctrl_sig);
	init.ovf_intr(ovf_intr_sig);
	init.und_intr(und_intr_sig);

	counter count("count");
	count.counter_en(counter_en_sig);
	//count.clk(myClock);
	count.cnt_out(cnt_out_sig);
	count.reset(reset_sig);
	count.up_ctrl(up_ctrl_sig);
	count.ovf_intr(ovf_intr_sig);
	count.und_intr(und_intr_sig);

	testBench test("test");
	test.counter_en(counter_en_sig);
	//count.clk(myClock);
	test.cnt_out(cnt_out_sig);
	test.reset(reset_sig);
	test.up_ctrl(up_ctrl_sig);
	test.ovf_intr(ovf_intr_sig);
	test.und_intr(und_intr_sig);


	triggered trg("trg");
	trg.cnt_out(cnt_out_sig);
	trg.counter_en(counter_en_sig);
	//trg.clk(myClock);


	sc_trace_file *tf = sc_create_vcd_trace_file("COUNTER");

	sc_trace(tf,counter_en_sig,"counter_en_sig");
	sc_trace(tf,cnt_out_sig,"cnt_out_sig");
	sc_trace(tf,reset_sig,"reset_sig");
	sc_trace(tf,up_ctrl_sig,"up_ctrl_sig");
	sc_trace(tf,ovf_intr_sig,"ovf_intr_sig");
	sc_trace(tf,und_intr_sig,"und_intr_sig");



	sc_start(10, SC_NS);


	//sc_start();



	return 0;

}
