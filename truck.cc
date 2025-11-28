#include "truck.h"
#include "vendingMachine.h"


Truck::Truck(Printer& prt, NameServer& nameServer, BottlingPlant& plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour)
    : prt(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {}


void Truck::main() {
    prt.print(Printer::Kind::Truck, 'S');

    VendingMachines ** machines = nameServer.getMachineList();

    try {
        for ( ;; ) {
            _Accept(~Truck) {
                break;
            } _Else {
                yield(prng(1, 10));

                // get shipment from plant
                unsigned int cargo[BottlingPlant::Flavours::NUM_OF_FLAVOURS] = {};
                _Enable{
                    plant.getShipment(cargo);
                }

                // total number of bottles received
                unsigned int total = 0;
                for (unsigned int k : cargo) {
                    total += k; 
                }

                prt.print(Printer::Kind::Truck, 'P', total);        // P a

                // restock vending machines
                for (unsigned int i=0; i<numVendingMachines && total != 0; i++) {
                    prt.print(Printer::Kind::Truck, 'd', machines[currentMachine]->getId(), total);     // d v,r

                    unsigned int * inv = machines[curMachine]->inventory();

                    // for each flavour
                    unsigned int notReplenished = 0;
                    for (int j=0; j<BottlingPlant::Flavours::NUM_OF_FLAVOURS; j++) {
                        if (inv[j] != maxStockPerFlavour && cargo[j] != 0) {    // flavour is not full and truck has stock
                            unsigned int amount = min(maxStockPerFlavour - inv[j], cargo[j]);       // can transfer as many bottle as possible (bring the stock to at most maxStockPerFlavour)

                            cargo[j] -= amount;
                            inv[j] += amount;
                            total -= amount;
                            // invariant: cargo[j] >= 0, inv[j] <= maxStockPerFlavour, total >= 0

                            notReplenished += (maxStockPerFlavour - inv[j]);
                        }
                    }

                    machines[curMachine]->restocked();

                    if (notReplenished != 0) prt.print(Printer::Kind::Truck, 'U', machines[currentMachine]->getId(), notREplenished);     // U v,b
                    prt.print(Printer::Kind::Truck, 'D', machines[currentMachine]->getId(), total);     // D v,r

                    curMachine = (curMachine + 1) % numVendingMachines;     // point to next machine


                    if (prng(9) == 0) {     // 1 in 10 chance of robbery
                        prt.print(Printer::Kind::Truck, 'R', total);     // R, s
                        total = 0;
                    }
                }
            }
        }
    } catch (BottlingPlant::Shutdown &) {}      // catch error from plant.getShipment(cargo);

    prt.print(Printer::Kind::Truck, 'F');  

}