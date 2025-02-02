#include <algorithm>
#include <random>

#ifndef DECK_H
#define DECK_H

#include "Card/Card.h"

class Deck {
    public :
        std::vector<Card> cards;

        // Constructor
        Deck();

        // Methods
        void init();
        void initWeaponCards();
        void initActionCards();
        void initPermanentCards();
        void shuffle();
};

#endif