#include "fifoh.h"

template <typename T, int SIZE>
void producer<T, SIZE>::feed()
{

    int trigger = rand() % 5;
    int value = 0;

    if (trigger == 0 && reset.read() != true && fifoFlagWrite.read() != true && fifoFlagRead.read() != true) {

        reset.write(true);
        cout << "reset triggered" << " time stamp: "<<sc_time_stamp()<< endl;
        //		fifoFlag.write(false);
    }

    else {

        //   	if (reset.read() || write_bNow.read() || read_bNow.read() || write_nbNow.read() || read_nbNow.read() || fifoFlag.read()){
        if (fifoFlagWrite.read() == false && (trigger == 1 || trigger == 3)) {
            //do nothing;
            cout << "Producer Waiting for Write flags" << " time stamp: "<<sc_time_stamp()<< endl;
            cout << "write_nbNow.read() is now " << write_nbNow.read() << " time stamp: "<<sc_time_stamp()<< endl;
            cout << "fifoFlagWrite is now " << fifoFlagWrite.read() << " time stamp: "<<sc_time_stamp()<< endl;
            cout << "write_bNow.read() is now " << write_bNow.read() << " time stamp: "<<sc_time_stamp()<< endl;

            // 	}

            if (trigger == 1) {
                value = rand() % 60;
                cout << "write_bNowOut triggered with " << value << " time stamp: "<<sc_time_stamp()<< endl;
                nextElement.write(value);
                //	fifoFlag.write(false);
                fifoFlagWrite.write(true);

                write_bNow.write(true);
            }
            else if (trigger == 3) {
                value = rand() % 60;
                nextElement.write(value);
                //	fifoFlag.write(false);
                fifoFlagWrite.write(true);

                write_nbNow.write(true);

                cout << "write_nbNow triggered with value " << value << " time stamp: "<<sc_time_stamp()<< endl;
            }
        }

        else if (fifoFlagRead.read() == false && (trigger == 2 || trigger == 4)) {
            cout << "read_nbNow.read() is now " << read_nbNow.read() << " time stamp: "<<sc_time_stamp()<< endl;
            cout << "read_bNow.read() is now " << read_bNow.read() << " time stamp: "<<sc_time_stamp()<< endl;
            cout << "reset.read() is now " << reset.read() << " time stamp: "<<sc_time_stamp()<< endl;
            cout << "fifoFlagRead.read() is now " << fifoFlagRead.read() << " time stamp: "<<sc_time_stamp()<< endl;

            if (trigger == 2) {

                //	fifoFlag.write(false);
                fifoFlagRead.write(true);
                cout << "read_bNow triggered" << " time stamp: "<<sc_time_stamp()<< endl;
                read_bNow.write(true);
            }

            else if (trigger == 4) {

                //	fifoFlag.write(false);
                fifoFlagRead.write(true);

                read_nbNow.write(true);

                cout << "read_nbNow triggered" << " time stamp: "<<sc_time_stamp()<< endl;
            }
        }
    }

    for (int i = 0; i <= 50; i++) {
        cout << "=";
    }
    cout<< " time stamp: "<<sc_time_stamp()<< endl;

    //next_trigger();
}

template <typename T, int SIZE>
void FIFOMODULE<T, SIZE>::reset_func()
{

    cout << "inside reset function. Resetting the values" << " time stamp: "<<sc_time_stamp()<< endl;
    write_bNow.write(false);
    read_bNow.write(false);
    write_nbNow.write(false);
    read_nbNow.write(false);
    fifoFlagRead.write(false);
    fifoFlagWrite.write(false);
    currentIndex = -1;
    reset.write(false);

    cout << "write_bNow flag is " << write_bNow.read() << " time stamp: "<<sc_time_stamp()<< endl;
    cout << "read_bNow flag is " << read_bNow.read() << " time stamp: "<<sc_time_stamp()<< endl;
    cout << "write_nbNow flag is " << write_nbNow.read() << " time stamp: "<<sc_time_stamp()<< endl;
    cout << "fifoFlagRead flag is " << fifoFlagRead.read() << " time stamp: "<<sc_time_stamp()<< endl;
    cout << "fifoFlagWrite flag is " << fifoFlagWrite.read() << " time stamp: "<<sc_time_stamp()<< endl;
    cout << "currentIndex is " << currentIndex << " time stamp: "<<sc_time_stamp()<< endl;
    cout << "reset flag is " << reset.read() << " time stamp: "<<sc_time_stamp()<< endl;
    cout << "Flags have been reset. Leaving reset function" << " time stamp: "<<sc_time_stamp()<< endl;
    for (int i = 0; i <= 50; i++) {
        cout << "=";
    }
    cout << " time stamp: "<<sc_time_stamp()<< endl;
}

template <typename T, int SIZE>
void FIFOMODULE<T, SIZE>::write_nb_func()
{
    bool tempFlag = false;
    if (currentIndex > SIZE) {

        cout << "non-blocking write - Cannot write. Fifo occupied or full. fifoFlagWrite() is " << fifoFlagWrite.read() << " and write_nbNow.read() is " 
	<< write_nbNow.read() << " time stamp: "<<sc_time_stamp()<< endl;
    }

    else {

        tempFlag = true;
        cout << "non-blocking write successful. Fifo not occupied." << " time stamp: "<<sc_time_stamp()<< endl;
        cout << "fifo flag is " << fifoFlagWrite.read()<< " time stamp: "<<sc_time_stamp()<< endl;
        //fifoFlagWrite.write(true);
        //cout<< "fifo flag  "<< " has been set to "<< fifoFlagWrite.read() <<  endl;

        cout << "Trying to do NB write" << " time stamp: "<<sc_time_stamp()<< endl;

        if (currentIndex >= 0) {
            currentIndex = currentIndex + 1;
            myFifo[currentIndex] = nextElement.read();
            cout << "NB write completed " << " time stamp: "<<sc_time_stamp()<< endl;
            cout << "fifo is " << " time stamp: "<<sc_time_stamp()<< endl;
            for (int i = 0; i <= currentIndex; i++) {
                cout << myFifo[i] << " ,";
            }
            cout << " time stamp: "<<sc_time_stamp()<< endl;
        }
        else {
            cout << "NB with with empty FIFO" << " time stamp: "<<sc_time_stamp()<< endl;

            currentIndex = 0;

            cout << "myFifo created" << " time stamp: "<<sc_time_stamp()<< endl;
            myFifo[0] = nextElement.read();
            cout << "myFifo set to element to be written, i.e. " << myFifo[currentIndex] << " whereas next element was " << nextElement.read() << " time stamp: "<<sc_time_stamp()<< endl;

            cout << "NB write done" << " time stamp: "<<sc_time_stamp()<< endl;
        }
    }

    fifoFlagWrite.write(false);
    write_nbNow.write(false);

    cout << "fifoFlag and write_nbNow falgs set to False. " << fifoFlagWrite.read() << " and " << write_nbNow.read() << " time stamp: "<<sc_time_stamp()<< endl;

    for (int i = 0; i <= 50; i++) {
        cout << "=";
    }
    cout << endl;

    if (tempFlag == true) {

        canReadNow.notify();
    }
}

template <typename T, int SIZE>
void FIFOMODULE<T, SIZE>::read_nb_func()
{

    bool tempFlag = false;

    if (currentIndex < 0) {

        cout << "non-blocking Read - Cannot read. current index is " << currentIndex << " fifo flag is " <<
        fifoFlagRead.read() << " read_nbNow.read() is " << read_nbNow.read() << " time stamp: "<<sc_time_stamp()<< endl;
    }

    else {

        tempFlag = true;

        cout << "non-blocking Read successful. Fifo not occupied." << " time stamp: "<<sc_time_stamp()<< endl;
        fifoFlagRead.write(true);
        cout << "Going for NB read." << " time stamp: "<<sc_time_stamp()<< endl;

        /* This was Stack
  			T element = myFifo[currentIndex];
			cout<<"element is "<< element<<endl;
  			currentElement.write(element);
  			currentIndex=currentIndex-1;

			*/
        T element = myFifo[0];
        for (int i = 0; i <= currentIndex - 1; i++) {
            myFifo[i] = myFifo[i + 1];
        }
        currentIndex = currentIndex - 1;

        cout << "element " << element << " has been read " << endl;
        cout << "fifo is " << endl;
        for (int i = 0; i <= currentIndex; i++) {
            cout << myFifo[i] << " ,";
        }
        cout << " time stamp: "<<sc_time_stamp()<< endl;
    }

    fifoFlagRead.write(false);
    read_nbNow.write(false);
    for (int i = 0; i <= 50; i++) {
        cout << "=";
    }
    cout << " time stamp: "<<sc_time_stamp()<< endl;

    if (tempFlag == true) {

        canWriteNow.notify();
    }
}

template <typename T, int SIZE>
void FIFOMODULE<T, SIZE>::write_b_func()
{
    while (true) {
        bool tempFlag = false;
        if (currentIndex > SIZE) {
            cout << "Blocking write - Waiting" << " time stamp: "<<sc_time_stamp()<< endl;

            wait(canWriteNow);
        }

        cout << "Blocking write successful. Fifo not occupied." << " time stamp: "<<sc_time_stamp()<< endl;

        if (currentIndex >= 0) {

            currentIndex = currentIndex + 1;
            myFifo[currentIndex] = nextElement;
            cout << "fifo is " << " time stamp: "<<sc_time_stamp()<< endl;
            for (int i = 0; i <= currentIndex; i++) {
                cout << myFifo[i] << " ,";
            }
            cout << endl;

            cout << "Fifo updated to myFifo " << " time stamp: "<<sc_time_stamp()<< endl;
        }
        else {

            myFifo[0] = nextElement.read();
            currentIndex = 0;

            cout << "fifo is " << myFifo[0] << endl;
        }
        tempFlag = true;

        fifoFlagWrite.write(false);
        write_bNow.write(false);
        for (int i = 0; i <= 50; i++) {
            cout << "=";
        }
        cout << endl;

        if (tempFlag == true) {

            canReadNow.notify();
        }

        wait();
    }
}
template <typename T, int SIZE>
void FIFOMODULE<T, SIZE>::read_b_func()
{

    while (true) {
        bool tempFlag = false;
        if (currentIndex < 0) {
            cout << "BlockingRead - Waiting" << " time stamp: "<<sc_time_stamp()<< endl;
            //read_bNow.write(true);

            wait(canReadNow);
        }

        cout << "BlockingRead successful. Fifo not occupied." << " time stamp: "<<sc_time_stamp()<< endl;
        /* This is for Stack
 
  		T element = myFifo[currentIndex];
  		cout<<"element is "<< element<<endl;

  		currentElement.write(element);
  		currentIndex=currentIndex -1;
		*/

        T element = myFifo[0];
        for (int i = 0; i <= currentIndex - 1; i++) {
            myFifo[i] = myFifo[i + 1];
        }
        currentIndex = currentIndex - 1;

        cout << "read done and " << element << " has been read. " << " time stamp: "<<sc_time_stamp()<< endl;

        if (currentIndex >= 0) {
            for (int i = 0; i <= currentIndex; i++) {

                cout << myFifo[i] << " ,";
            }
            cout << " time stamp: "<<sc_time_stamp()<< endl;
        }

        tempFlag = true;
        fifoFlagRead.write(false);
        read_bNow.write(false);

        cout << "flags have been set to false " << fifoFlagRead.read() << " " << read_bNow.read() << endl;
        for (int i = 0; i <= 50; i++) {
            cout << "=";
        }
        cout << endl;

        if (tempFlag == true) {

            canWriteNow.notify();
        }

        wait();
    }
}

int sc_main(int argc, char* argv[])
{

    sc_signal<bool, SC_MANY_WRITERS> reset_sig;
    sc_signal<bool, SC_MANY_WRITERS> write_bNow_sig;
    sc_signal<bool, SC_MANY_WRITERS> read_bNow_sig;
    sc_signal<bool, SC_MANY_WRITERS> write_nbNow_sig;
    sc_signal<bool, SC_MANY_WRITERS> read_nbNow_sig;

    sc_signal<bool, SC_MANY_WRITERS> fifoFlagRead_sig;
    sc_signal<bool, SC_MANY_WRITERS> fifoFlagWrite_sig;
    sc_signal<int, SC_MANY_WRITERS> nextElement_sig;
    sc_signal<int, SC_MANY_WRITERS> currentElement_sig;

    FIFOMODULE<int, 16> rst("rst");

    rst.read_nbNow(read_nbNow_sig);
    rst.write_nbNow(write_nbNow_sig);
    rst.read_bNow(read_bNow_sig);
    rst.write_bNow(write_bNow_sig);
    rst.reset(reset_sig);
    rst.fifoFlagRead(fifoFlagRead_sig);
    rst.fifoFlagWrite(fifoFlagWrite_sig);
    rst.nextElement(nextElement_sig);
    rst.currentElement(currentElement_sig);

    producer<int, 16> prc("prc");

    prc.read_nbNow(read_nbNow_sig);
    prc.write_nbNow(write_nbNow_sig);
    prc.read_bNow(read_bNow_sig);
    prc.write_bNow(write_bNow_sig);
    prc.reset(reset_sig);
    prc.fifoFlagRead(fifoFlagRead_sig);
    prc.fifoFlagWrite(fifoFlagWrite_sig);
    prc.nextElement(nextElement_sig);
    prc.currentElement(currentElement_sig);

    sc_trace_file* tf = sc_create_vcd_trace_file("FIFO");

    sc_trace(tf, write_bNow_sig, "write_bNow_sig");
    sc_trace(tf, read_bNow_sig, "read_bNow_sig");
    sc_trace(tf, write_nbNow_sig, "write_nbNow_sig");
    sc_trace(tf, read_nbNow_sig, "read_nbNow_sig");
    sc_trace(tf, reset_sig, "reset_sig");
    sc_trace(tf, fifoFlagRead_sig, "fifoFlagRead_sig");
    sc_trace(tf, fifoFlagWrite_sig, "fifoFlagWrite_sig");
    sc_trace(tf, nextElement_sig, "nextElement_sig");
    sc_trace(tf, currentElement_sig, "currentElement_sig");

    sc_start(100, sc_core::SC_NS);

    sc_stop();
    sc_close_vcd_trace_file(tf);

    return 0;
}
