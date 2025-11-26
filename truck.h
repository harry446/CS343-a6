#ifndef TRUCK_H_
#define TRUCK_H_ 

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;

_Task Truck {
    Printer & ptr; 
    NameServer & ns; 
    BottlingPlant & bplt; 
    unsigned int numVM; 
    unsigned int maxSPF; 
	void main();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};



#endif
