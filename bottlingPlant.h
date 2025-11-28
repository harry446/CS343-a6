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
    
  public:
	enum Flavours {
        BluesBlackCherry = 0,
        ClassicCreamSoda = 1,
        RockRootBeer = 2,
        JazzLime = 3,
        NUM_OF_FLAVOURS = 4
    };
	_Exception Shutdown {};					// shutdown plant
    ~BottlingPlant();
	BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	void getShipment( unsigned int cargo[] );

  private:
    unsigned int production[Flavours::NUM_OF_FLAVOURS];            // last production batch
};


#endif
