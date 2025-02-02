#include "Deck.h"
#include "../Card/Card.h"
#include <algorithm>
#include <random>
#include <vector>

// Constructor
Deck::Deck() {}

// Methods
void Deck::init() {
    initWeaponCards();
    initActionCards();
    initPermanentCards();
    shuffle();
}

void Deck::initWeaponCards() {
    WeaponCard nodashi = weaponCardsVector[0]; // Nodashi
    cards.push_back(nodashi);
    WeaponCard nagayari = weaponCardsVector[2]; // Nagayari
    cards.push_back(nagayari);
    WeaponCard tanegashima = weaponCardsVector[3]; // Tanegashima
    cards.push_back(tanegashima);
    WeaponCard daikyu = weaponCardsVector[4]; // Daikyu
    cards.push_back(daikyu);
    WeaponCard katana = weaponCardsVector[7]; // Katana
    cards.push_back(katana);
    WeaponCard kanabo = weaponCardsVector[9]; // Kanabo
    cards.push_back(kanabo);
    WeaponCard wakizashi = weaponCardsVector[12]; // Wakizashi
    cards.push_back(wakizashi);

    for (int i = 0; i < 2; ++i) {
        WeaponCard naginata = weaponCardsVector[1]; // Naginata
        cards.push_back(naginata);
    }
    for (int i = 0; i < 3; ++i) {
        WeaponCard shuriken = weaponCardsVector[8]; // Shuriken
        cards.push_back(shuriken);
    }
    for (int i = 0; i < 4; ++i) {
        WeaponCard kusarigama = weaponCardsVector[6]; // Kusarigama
        cards.push_back(kusarigama);
    }
    for (int i = 0; i < 5; ++i) {
        WeaponCard bo = weaponCardsVector[5]; // Bo
        cards.push_back(bo);
        WeaponCard kiseru = weaponCardsVector[11]; // Kiseru
        cards.push_back(kiseru);
    }
    for (int i = 0; i < 6; ++i) {
        WeaponCard bokken = weaponCardsVector[10]; // Bokken
        cards.push_back(bokken);
    }
}

void Deck::initActionCards() {
    for (int i = 0; i < 3; ++i) {
        ActionCard daimyo = actionCardsVector[2]; // Daimyo
        cards.push_back(daimyo);
        ActionCard jujitsu = actionCardsVector[6]; // Jujitsu
        cards.push_back(jujitsu);
        ActionCard meditation = actionCardsVector[5]; // M�ditation
        cards.push_back(meditation);
    }
    for (int i = 0; i < 4; ++i) {
        ActionCard cri_de_guerre = actionCardsVector[1]; // Cri de guerre
        cards.push_back(cri_de_guerre);
        ActionCard ceremonie_du_the = actionCardsVector[0]; // C�r�monie du th�
        cards.push_back(ceremonie_du_the);
    }
    for (int i = 0; i < 5; ++i) {
        ActionCard diversion = actionCardsVector[3]; // Diversion
        cards.push_back(diversion);
    }
    for (int i = 0; i < 6; ++i) {
        ActionCard geisha = actionCardsVector[4]; // Geisha
        cards.push_back(geisha);
    }
    for (int i = 0; i < 15; ++i) {
        ActionCard parade = actionCardsVector[7]; // Parade
        cards.push_back(parade);
    }
}

void Deck::initPermanentCards() {
    for (int i = 0; i < 2; ++i) {
        PermanentCard codeDuBushido = permanentCardsVector[2]; // Code du Bushido
        cards.push_back(codeDuBushido);
    }
    for (int i = 0; i < 3; ++i) {
        PermanentCard attaqueRapide = permanentCardsVector[0]; // Attaque Rapide
        cards.push_back(attaqueRapide);
    }
    for (int i = 0; i < 4; ++i) {
        PermanentCard armure = permanentCardsVector[3]; // Armure
        cards.push_back(armure);
    }
    for (int i = 0; i < 6; ++i) {
        PermanentCard concentration = permanentCardsVector[1]; // Concentration
        cards.push_back(concentration);
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}