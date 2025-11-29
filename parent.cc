#include "parent.h"
#include "bank.h"
#include "printer.h"

Parent::Parent(Printer& prt, Bank& bank, unsigned int numStudents, unsigned int parentalDelay) : 
    prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}


void Parent::main() {
    prt.print(Printer::Kind::Parent, 'S');

    for ( ;; ) {
        // Parent must check for a call to its dtor
        _Accept (~Parent) {
            break; 
        }
        _Else {         // if no dtor being called
            yield(parentalDelay);       // yield before transfer 
            unsigned int selectedStudentID = prng(0, numStudents-1); 
            unsigned int amount; 
            if (prng(20) == 0) {    // 1 in 20 chance [0, 20)
                amount = 10; 
                prt.print(Printer::Kind::Parent, 'E', selectedStudentID, amount);
            } 
            else {
                amount = prng(1, 3);        // [1, 3]
                prt.print(Printer::Kind::Parent, 'D', selectedStudentID, amount);
            }
            bank.deposit(selectedStudentID, amount);       // bank deposit 
            totalAmount += amount; 
        }
    }
    prt.print(Printer::Kind::Parent, 'F', totalAmount);
}

