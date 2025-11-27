#ifndef GROUPOFF_H
#define GROUPOFF_H

#include <vector>

#include "watCard.h"

using namespace std;

_Monitor Printer;

_Task Groupoff {
	void main();
    Printer &prt; 
    unsigned int numStudents; 
    unsigned int sodaCost; 
    unsigned int groupoffDelay; 
    vector<bool> assigned;
    vector<WATCard::FWATCard> futures;  // for eacg student store their future value 

  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard( unsigned int id );
};

