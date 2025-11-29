#ifndef NAME_SERVER_H_
#define NAME_SERVER_H_

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
    Printer & prt;
    const unsigned int numVendingMachines;
    const unsigned int numStudents;

    VendingMachine ** machines;     // array of machine ptrs
    unsigned int curMachineCnt = 0;

    unsigned int * assignments;

	void main();
  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
	void VMregister( VendingMachine * vendingmachine );
	VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
	VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};

#endif