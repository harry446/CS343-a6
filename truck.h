#ifndef TRUCK_H_
#define TRUCK_H_ 

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;

_Task Truck {
    Printer & prt; 
    NameServer & nameServer; 
    BottlingPlant & plant; 
    unsigned int numVendingMachines; 
    unsigned int maxStockPerFlavour; 

    unsigned int curMachine = 0;

	  void main();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};



#endif
