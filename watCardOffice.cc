#include "watCardOffice.h"

#include "printer.h"
#include "bank.h"
#include "watCard.h"

#include <iostream>

// Args
WATCardOffice::Args::Args(unsigned int studentID, unsigned int amount, WATCard * card) : 
    studentID(studentID), amount(amount), card(card) {}

// Courier
WATCardOffice::Courier::Courier(Printer &prt, unsigned int id, WATCardOffice &office, Bank &bank) :
    prt(prt), id(id), office(office), bank(bank) {}

void WATCardOffice::Courier::main() {
    prt.print( Printer::Courier, id, 'S' );        // starting

    for( ;; ) { 
        Job *job = office.requestWork();          // constantly pull for available jobs to perform
        if ( job == nullptr ) break;        // office shutting down if no more jobs to perform 

        unsigned int sid = job->args.studentID;
        unsigned int amount = job->args.amount;
        WATCard *card = job->args.card;     // fetch the current card of student to add fund

        prt.print( Printer::Courier, id, 't', sid, amount );  // start transfer for the current courrier 
        bank.withdraw( sid, amount );           // perform the actual amount transfer
        card->deposit( amount );


        if(prng(6) == 0) {          // there is a 1 in 6 chance that the courier lost the watcard 
            prt.print(Printer::Kind::Courier, id, 'L', job->args.studentID);

            delete card;
            job->result.delivery(new WATCardOffice::Lost());
        } else {
            prt.print( Printer::Courier, id, 'T', sid, amount );  // success adding amount 
            job->result.delivery( card );                         // deliver card to student
        }

        delete job;     // current job is finished executing, safely delete it
    }

    prt.print(Printer::Kind::Courier, id, 'F');
}


WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
    : prt(prt), bank(bank), numCouriers(numCouriers) {

    // Create courier pool
    couriers = new Courier*[numCouriers];
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        couriers[i] = new Courier( prt, i, *this, bank );
    }
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    // Create a new WATCard students called it first 
    WATCard *watcard = new WATCard();

    Args args(sid, amount, watcard);
    Job *job = new Job( args );
    jobQueue.push( job );
    prt.print( Printer::WATCardOffice, 'C', sid, amount );

    // Return future to student
    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    // create the job object for the call body 
    Args args(sid, amount, card); 
    Job *job = new Job (args); 
    jobQueue.push( job );

    // return future to student 
    prt.print( Printer::WATCardOffice, 'T', sid, amount );
    return job->result; 
}

WATCardOffice::Job* WATCardOffice::requestWork() {
    if (shuttingDown || jobQueue.empty()) {     // after office is closing, return nullptr so couriers can shut down
        return nullptr; 
    }

    Job *job = jobQueue.front();        // otherwise do FIFO and return the earilest arrival job
    jobQueue.pop();
    return job;
}


WATCardOffice::~WATCardOffice() {
    // Delete couriers
    for (unsigned int i = 0; i < numCouriers; i++) {
        delete couriers[i];
    }
    delete[] couriers;
}

void WATCardOffice::main() {
    prt.print(Printer::Kind::WATCardOffice, 'S');
    for ( ;; ) {
        _Accept( ~WATCardOffice ) {              // destructor called
            shuttingDown = true;                 // signal requestWork() to return nullptr
            break;                               // exit main(), couriers can now be deleted safely
        }
        or _Accept( create ) {}
        or _Accept( transfer ) {}                // student calls transfer()
        or _When(!jobQueue.empty()) _Accept( requestWork ) {        // courier asks for work 
            prt.print( Printer::WATCardOffice, 'W' );
        }
    }

    // accept numCouriers number of times to allow each Courier to break and terminate
    // WATCardOffice destructor called after this, so no deadlock when deleting Couriers
    for (unsigned int i=0; i<numCouriers; i++) {
        _Accept(requestWork){}
    }
    prt.print( Printer::WATCardOffice, 'F' );    // Finish
}
