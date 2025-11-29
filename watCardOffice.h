#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

#include "watCard.h"
#include <queue>

using namespace std;

_Monitor Printer;
_Monitor Bank;

_Task WATCardOffice {
  public:
    struct Args {           // call arguments for a student's create and transfer request 
        unsigned int studentID;
        unsigned int amount;
        WATCard *card;
        Args(unsigned int studentID, unsigned int amount, WATCard * card);
    };

  private:
	struct Job {							// marshalled arguments and return future, one async call
		Args args;							// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args( args ) {}
	};

	_Task Courier {     // courier object to communicate with the bank, perform actual bank withdraw and watcard update 
        Printer &prt;
        const unsigned int id;
        WATCardOffice &office;
        Bank &bank;
        void main();
    public:
        Courier( Printer &prt, unsigned int id, WATCardOffice &office, Bank &bank );
    }; // communicates with bank (YOU DEFINE "Courier")

	void main();
    Printer &prt;
    Bank &bank;
    const unsigned int numCouriers;
    bool shuttingDown = false;

    queue<Job*> jobQueue; // FIFO of pending student requests
    Courier **couriers; // array of courier tasks created 

  public:
	_Exception Lost {};						// lost WATCard
    ~WATCardOffice();
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));
	Job * requestWork() __attribute__(( warn_unused_result ));
};


#endif