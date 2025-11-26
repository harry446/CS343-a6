#include "watCard.h"

WATCard::WATCard() {
    balances = 0;   // initialize balances to 0 as watcard creation
}

// deposit to add balance to a watcard
void WATCard::deposit(unsigned int amount) {
    balances += amount;
}

// withdraw balances from watcard
void WATCard::withdraw(unsigned int amount) {
    balances -= amount; 
}

unsigned int WATCard::getBalance() {
    return balances;
}