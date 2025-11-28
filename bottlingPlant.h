#ifndef BOTTLING_PLANT_H_
#define BOTTLING_PLANT_H_

_Monitor Printer;
_Task NameServer;

_Task BottlingPlant {
    Printer &prt; 
    NameServer & nameServer; 
    unsigned int numVendingMachines; 
    unsigned int maxShippedPerFlavour; 
    unsigned int maxStockPerFlavour; 
    unsigned int timeBetweenShipments; 
	void main();
    bool shuttingDown;                     // whether plant is closing
    unsigned int production[4];            // last production batch
    
  public:
	enum Flavours {
        BluesBlackCherry,
        ClassicCreamSoda,
        RockRootBeer,
        JazzLime};
	_Exception Shutdown {};					// shutdown plant
    ~BottlingPlant();
	BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	void getShipment( unsigned int cargo[] );
};


#endif
