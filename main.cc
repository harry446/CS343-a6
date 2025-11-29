#include <uPRNG.h>

#include <iostream>
#include <string>

#include "bank.h"
#include "bottlingPlant.h"
#include "config.h"
#include "groupoff.h"
#include "nameServer.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "vendingMachine.h"
#include "watCardOffice.h"

using namespace std;


int main (int argc, char * argv[]) {
    // initialization, value might be overwritten later depending on input
    int processors = 1;
    string configFile = "soda.config";

    struct cmd_error {};
    try {
        switch (argc) {
            case 4:
                if (strcmp(argv[3], "d") != 0) {
                    processors = convert(argv[3]);
                    if (processors <= 0) throw cmd_error{};     	    // (0, inf]
                }
                // FALL THROUGH
            case 3:
                if (strcmp(argv[2], "d") != 0) {
                    int seed = convert(argv[2]);
                    if (seed <= 0 ) throw cmd_error{};     			    // (0, inf]
					set_seed(seed);
                }
                // FALL THROUGH
            case 2:
                if (strcmp(argv[1], "d") != 0) {
                    configFile = argv[1];
                }
                // FALL THROUGH
            case 1:
                break;
            default:
                throw cmd_error{};
        }
    } catch( ... ) {									// command-line argument failed (invalid_argument)
		cerr << "Usage: " << argv[0] << " [ config-file | 'd' (default file soda.config) [ seed (> 0) | 'd' (default random) [ processors (> 0) | 'd' (default 1) ] ] ]" << endl;
		exit( EXIT_FAILURE );							// TERMINATE
	} // try

    uProcessor p[processors - 1] __attribute__(( unused ));

    ConfigParms config;
    processConfigFile(config_file.c_str(), config);

    Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);

    Bank bank(config.numStudents);

    Parent parent(printer, config.numStudents, config.sodaCost, config.groupoffDelay);

    WATCardOffice cardOffice(printer, bank, config.numCouriers);

    Groupoff groupoff(printer, config.numStudents, config.sodaCost, config.groupoffDelay);

    NameServer nameServer(printer, config.numVendingMachines, config.numStudents);

    {
        uArray(VendingMachine, machines, (size_t)config.numVendingMachines);
        for (unsigned int i=0; i<config.numVendingMachines; i++) {
            machines[i](printer, nameServer, i, config.sodaCost);
        }

        {
            BottlingPlant plant(printer, nameServer, config.numVendingMachines, config.maxShippedPerFlavour, config.maxStockPerFlavour, config.timeBetweenShipments);

            {
                uArray(Student, students, (size_t)config.numStudents);
                for (unsigned int i=0; i<config.numStudents; i++) {
                    students[i](printer, nameServer, cardOffice, groupoff, i, config.maxPurchases);
                }
            }   // wait for students to finish
        }   // delete plant
        cout << "PLANT DELETED" << endl;
    }   // delete machines
    


    return 0;
}