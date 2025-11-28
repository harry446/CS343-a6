#include "watCardOffice.h"

#include "printer.h"
#include "bank.h"
#include "watCard.h"

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) {

}
	
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result )) {

}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result )) {

}

Job * WATCardOffice::requestWork() __attribute__(( warn_unused_result )) {

}


// Courier
Courier::Courier( Printer &prt, unsigned int id, WATCardOffice &office, Bank &bank ) {

}

void Courier::main() {

}