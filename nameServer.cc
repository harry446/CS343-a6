#include "nameServer.h"
#include "printer.h"
#include "vendingMachine.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ): 
    prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
    
    machines = new VendingMachine*[numVendingMachines];
    assignments = new unsigned int[numStudents];

    // student 0 to machine 0, ..., student numVendingMachines to machine 0, student numVendingMachines+1 to machine 1, ...
    for (int i=0; i<numStudents; i++) {
        assignments[i] = i%numVendingMachines;   
    } 
}

NameServer::~NameServer() {
    delete [] machines;
    delete [] assignments;
}


void NameServer::VMregister( VendingMachine * vendingmachine ) {
    prt.print(Printer::Kind::NameServer, 'R', curMachineCnt);   // R v
    machines[curMachineCnt++] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
    unsigned int idx = assignments[id];
    assignments[id] = (idx + 1) % numVendingMachines;     // assign student to the next machine
    prt.print(Printer::Kind::NameServer, 'N', id, idx);   // N s,v
    
    return machines[idx];
}

VendingMachine ** NameServer::getMachineList() {
    return machines;
}


void NameServer::main() {
    prt.print(Printer::Kind::NameServer, 'S');
    for ( ;; ) {
        _Accept(~NameServer) {
            break;
        } or _When(curMachineCnt < numVendingMachines) _Accept(VMregister) {
        } or _When(curMachineCnt == numVendingMachines) _Accept(getMachine || getMachineList) {
        }
    }

    prt.print(Printer::Kind::NameServer, 'F');
}