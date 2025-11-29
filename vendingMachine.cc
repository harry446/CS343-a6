#include "vendingMachine.h"
#include "nameServer.h"
#include "watCard.h"
#include "printer.h"
#include "bottlingPlant.h"

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ): 
    prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost) {

    for (int i=0; i<BottlingPlant::Flavours::NUM_OF_FLAVOURS; i++) {
        inv[i] = 0;
    }
}

void VendingMachine::buy( BottlingPlant::Flavours flavour, WATCard & card ) {
    if (card.getBalance() < sodaCost) _Throw Funds();       // check funds first
    if (inv[flavour] == 0) _Throw Stock();      // then check flavour stock

    inv[flavour]--;

    if (prng(5) == 0) {     // 1 in 5 chance soda is free
        prt.print(Printer::Kind::Vending, id, 'A');
        _Throw Free();        
    }

    prt.print(Printer::Kind::Vending, id, 'B', flavour, inv[flavour]);
    card.withdraw(sodaCost);
}

unsigned int * VendingMachine::inventory() {
    return inv;
}

void VendingMachine::restocked() {}

_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}

void VendingMachine::main() {
    prt.print(Printer::Kind::Vending, id, 'S', sodaCost);
    nameServer.VMregister(this);       // register with name server
    bool canBuy;

    for ( ;; ) {
        try {
            _Accept(~VendingMachine) {
                break;
            } or _Accept(inventory) {
                prt.print(Printer::Kind::Vending, id, 'r');
                canBuy = false;     // block buy()
            } or _Accept(restocked) {
                prt.print(Printer::Kind::Vending, id, 'R');
                canBuy = true;      // unblock buy()
            } or _When(canBuy) _Accept(buy) {}
        } catch(uMutexFailure::RendezvousFailure& ){}; 

    }

    prt.print(Printer::Kind::Vending, id, 'F');
    
}