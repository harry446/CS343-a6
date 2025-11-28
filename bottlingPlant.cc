#include "bottlingPlant.h"

#include "nameServer.h"
#include "printer.h"
#include "truck.h"

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer,
                              unsigned int numVendingMachines,
                              unsigned int maxShippedPerFlavour,
                              unsigned int maxStockPerFlavour,
                              unsigned int timeBetweenShipments )
    : prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines), 
    maxShippedPerFlavour(maxShippedPerFlavour),
    maxStockPerFlavour(maxStockPerFlavour),
    timeBetweenShipments(timeBetweenShipments) {
        shuttingDown = false;
    }

BottlingPlant::~BottlingPlant() {
    shuttingDown = true;
    prt.print(Printer::BottlingPlant, 'F'); // adding failure message at bottling plant dead moment
}


void BottlingPlant::main() {
    prt.print(Printer::Kind::BottlingPlant, 'S');
    Truck truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);    // begins by creating a truck 

    for(;;) {
        yield( timeBetweenShipments );      // yielding for the time
        
        // Produce a new shipment (production run)
        unsigned int total = 0;
        for ( unsigned int i = 0; i < 4; i++ ) {
            production[i] = prng( maxShippedPerFlavour );
            total += production[i];
        }

        prt.print( Printer::BottlingPlant, 'G', total);     // production run details
        _Accept( ~BottlingPlant ) {
            break;
        }
        or _Accept( getShipment ) {}        // waiting for the truck to pick up the production run 
    }

    // after dtor of bottling plant called, accept another getShipment to exit the 
    try {
        _Accept(getShipment);
    }
    catch(uMutexFailure::RendezvousFailure& ){}; 

}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    if ( shuttingDown ) {
        _Throw Shutdown();     // if shutdown throw the exception at truck
    }

    for ( unsigned int i = 0; i < 4; i++ ) {
        cargo[i] = production[i];
    }

    prt.print( Printer::BottlingPlant, 'P' );       // truck pick up current run, prepare for the next 
}