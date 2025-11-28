#include "vendingMachine.h"
#include "nameServer.h"
#include "watCard.h"

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ): 
    prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost) {

    for (int i=0; i<BottlingPlant::Flavour::NUM_OF_FLAVOURS; i++) {
        inv[i] = 0;
    }
}

void VendingMachine::buy( BottlingPlant::Flavours flavour, WATCard & card ) {
    if (card.getBalance() < sodaCost) _Throw Funds();
    if (inv[flavour] == 0) _Throw Stock();

    inv[flavour]--;

    if (prng(4) == 0) {     // 1 in 5 chance soda is free
        prt.print(Printer::Kind::Vending, 'A');
        _Throw Free();        
    }

    prt.print(Printer::Kind::Vending, 'B', flavour, inv[flavour]);
    card.withdraw(sodaCost);
}

unsigned int * VendingMachine::inventory() __attribute__(( warn_unused_result )) {
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
    prt.print(Printer::Kind::Vending, 'S', sodaCost);
    nameServer.VMregister(this);       // register with name server

    for ( ;; ) {
        try {
            _Accept(~VendingMachine()) {
                break;
            } or _Accept(inventory) {
                prt.print(Printer::Kind::Vending, 'r');
                canBuy = false;
            } or _Accept(restocked) {
                prt.print(Printer::Kind::Vending, 'R');
                canBuy = true;
            } or _When(canBuy) _Accept(buy) {}
        }
    }

    prt.print(Printer::Kind::Vending, 'F');
    
}