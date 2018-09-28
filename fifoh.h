
#include <systemc.h>

template <typename T, int SIZE>
SC_MODULE(FIFOMODULE)

{

    sc_inout<bool> read_nbNow;
    sc_inout<bool> write_nbNow;
    sc_inout<bool> read_bNow;
    sc_inout<bool> write_bNow;
    sc_inout<bool> reset;

    sc_clock clk;

    void write_b_func();
    void read_b_func();
    void write_nb_func();
    void read_nb_func();
    void reset_func();

    //	void initialize_Me();

    SC_CTOR(FIFOMODULE)
    {

        SC_THREAD(write_b_func);
        sensitive << write_bNow.pos();
        dont_initialize();

        SC_THREAD(read_b_func);
        sensitive << read_bNow.pos();
        dont_initialize();

        SC_METHOD(read_nb_func);
        sensitive << read_nbNow.pos();
        dont_initialize();

        SC_METHOD(write_nb_func);
        sensitive << write_nbNow.pos();
        dont_initialize();

        SC_METHOD(reset_func);
        sensitive << reset.pos();
        dont_initialize();

        //    	SC_THREAD(initialize_Me);

        myFifo = new T[SIZE];
        currentIndex = -1;
    }

public:
    sc_inout<bool> fifoFlagRead;
    sc_inout<bool> fifoFlagWrite;
    sc_inout<T> nextElement;
    sc_inout<T> currentElement;
    sc_event canReadNow;
    sc_event canWriteNow;

    int currentIndex;
    T* myFifo;
};

template <typename T, int SIZE>
SC_MODULE(producer)
{

    sc_inout<bool> read_nbNow;
    sc_inout<bool> write_nbNow;
    sc_inout<bool> read_bNow;
    sc_inout<bool> write_bNow;
    sc_inout<bool> reset;

    sc_clock clk;

    void feed();
    SC_CTOR(producer)
    {

        SC_METHOD(feed);
        sensitive_pos << clk;
        dont_initialize();
    }

public:
    sc_inout<bool> fifoFlagRead;
    sc_inout<bool> fifoFlagWrite;
    sc_inout<T> nextElement;
    sc_inout<T> currentElement;
};
