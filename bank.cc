#include "bank.h"

// when Bank was creaed, it initialize all student's account with a balance of 0 
Bank::Bank(unsigned int numStudents) : numStudents(numStudents) {
    // create the balances per number of students 
  balances = new unsigned int[numStudents];
  for (unsigned int i = 0; i < num_students; i++) {
    balances[i] = 0;    // initialize starts with 0 balances
  }
}

Bank::~Bank() { 
    delete[] balances;      // when destruct, clear the balances
}

// deposit to add balance to a student account
void Bank::deposit(unsigned int id, unsigned int amount) {
    balances[id] += amount; 
}

// withdraw have to wait until enough money has been deposited to be able to withdraw 
void Bank::withdraw(unsigned int id, unsigned int amount ) {
    while(balances[id] < amount) {      // if there are not enough money in the account, keep accepting deposit action
        _Accept(deposit);
    }

    balances[id] -= amount; 

}