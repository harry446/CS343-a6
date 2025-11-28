#ifndef VENDING_MACHINE_H_
#define VENDING_MACHINE_H_

#include "bottlingPlant.h"

_Monitor Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
	Printer & prt;
	NameServer & nameServer;
	const unsigned int id, sodaCost;

	unsigned int inv[BottlingPlant::Flavours::NUM_OF_FLAVOURS];

	void main();
  public:
	_Exception Funds {};					// insufficient funds
	_Exception Stock {};					// flavour out of stock
	_Exception Free {};						// free, advertisement
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
	void buy( BottlingPlant::Flavours flavour, WATCard & card );
	unsigned int * inventory() __attribute__(( warn_unused_result ));
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};

#endif