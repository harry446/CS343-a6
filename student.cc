#include "student.h"
#include "printer.h"
#include "nameServer.h"
#include "watCardOffice.h"
#include "groupoff.h"
#include "watCard.h"
#include "bottlingPlant.h"


Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases ) : 
    prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases)  {}

void Student::main() {
    unsigned int purchases = prng(1, maxPurchases);     // [1, maxPurchases]
    unsigned int flavour = prng(BottlingPlant::Flavours::NUM_OF_FLAVOURS);     // 4 flavours, [0, 4)

    prt.print(Printer::Kind::Student, 'S', flavour, purchases);

    WATCard::FWATCard card = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard(id);

    VendingMachine * curMachine = nameServer.getMachine(id);
    prt.print(Printer::Kind::Student, 'V', curMachine->getId());

    for (int i=0; i<purchases; i++) {
        yield(prng(1, 10));

        _Select(giftCard) {
            for ( ;; ) {        // for loop to skip yield(prng(1, 10)) after free bottle
                try {
                    _Enable {
                        curMachine->buy(flavour, giftCard);     // two possible exceptions: Free or Stock
                    }
                } catch (VendingMachine::Free &) {      // free drink, gift card still has money => use it again
                    prt.print(Printer::Kind::Student, 'A', flavour);
                    if (prng(2) == 0) {     // 50% chance to watch ad, [0, 2)
                        yield(4);
                    } else {
                        prt.print(Printer::Kind::Student, 'X');
                    }
                    continue;       // loop again, skip purchase yielding
                } catch (VendingMachine::Stock &) {     // out of stock, call nameServer to get a new machine
                    VendingMachine * curMachine = nameServer.getMachine(id);
                    prt.print(Printer::Kind::Student, 'V', curMachine->getId());

                    // purchase attempted but not successful, do not count as a "purchase" but need to yield(prng(1, 10)) again
                    i--;        
                }

                break;
            }
        } or _Select(card) {
            for ( ;; ) {        // for loop to skip yield(prng(1, 10)) after free bottle
                try {
                    _Enable {
                        curMachine->buy(flavour, card);         // three possible exceptions: Free or Stock orr Fund
                    }
                } catch (VendingMachine::Free &) {
                    prt.print(Printer::Kind::Student, 'A', flavour);
                    if (prng(2) == 0) {     // 50% chance to watch ad, [0, 2)
                        yield(4);
                    } else {
                        prt.print(Printer::Kind::Student, 'X');
                    }
                    continue;       // loop again, skip purchase yielding
                } catch (VendingMachine::Stock &) {
                    VendingMachine * curMachine = nameServer.getMachine(id);
                    prt.print(Printer::Kind::Student, 'V', curMachine->getId());

                    // purchase attempted but not successful, do not count as a "purchase" but need to yield(prng(1, 10)) again
                    i--;   
                } catch (VendingMaachine::Fund &) {
                    

                    // purchase attempted but not successful, do not count as a "purchase" but need to yield(prng(1, 10)) again
                    i--;
                }


                break;
            }
        }
    }
}