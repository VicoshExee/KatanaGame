#include <iostream>
#include <string>

#include "Card.h"


std::vector<ActionCard> actionCardsVector = {
    ActionCard(CardName::CEREMONIE_DU_THE),
    ActionCard(CardName::CRI_DE_GUERRE),
    ActionCard(CardName::DAIMYO),
    ActionCard(CardName::DIVERSION),
    ActionCard(CardName::GEISHA),
    ActionCard(CardName::MEDITATION),
    ActionCard(CardName::JUJITSU),
    ActionCard(CardName::PARADE)
};

std::vector<PermanentCard> permanentCardsVector = {
    PermanentCard(CardName::ATTAQUE_RAPIDE),
    PermanentCard(CardName::CONCENTRATION),
    PermanentCard(CardName::CODE_DU_BUSHIDO),
    PermanentCard(CardName::ARMURE)
};

std::vector<WeaponCard> weaponCardsVector = {
    WeaponCard(CardName::NODACHI, 3, 3),
    WeaponCard(CardName::NAGINATA, 4, 1),
    WeaponCard(CardName::NAGAYARI, 4, 2),
    WeaponCard(CardName::TANEGASHIMA, 5, 1),
    WeaponCard(CardName::DAIKYU, 5, 2),
    WeaponCard(CardName::BO, 2, 1),
    WeaponCard(CardName::KUSARIGAMA, 2, 2),
    WeaponCard(CardName::KATANA, 2, 3),
    WeaponCard(CardName::SHURIKEN, 3, 1),
    WeaponCard(CardName::KANABO, 3, 2),
    WeaponCard(CardName::BOKKEN, 1, 1),
    WeaponCard(CardName::KISERU, 1, 2),
    WeaponCard(CardName::WAKIZASHI, 1, 3)
};




// Constructor
Card::Card(CardName name, CardType type, int difficulty = 0, int damage = 0)
    : name(name), type(type), difficulty(difficulty), damage(damage) {}

// Getter
CardName Card::get_name() const
{
    return name;
}

std::string Card::print_name() {
    switch (name) {
    case CardName::CEREMONIE_DU_THE:
        return "Ceremonie du The";
    case CardName::CRI_DE_GUERRE:
        return "Cri de Guerre";
    case CardName::DAIMYO:
        return "Daimyo";
    case CardName::DIVERSION:
        return "Diversion";
    case CardName::GEISHA:
        return "Geisha";
    case CardName::MEDITATION:
        return "Meditation";
    case CardName::JUJITSU:
        return "Jujitsu";
    case CardName::PARADE:
        return "Parade";
    case CardName::ATTAQUE_RAPIDE:
        return "Attaque Rapide";
    case CardName::CONCENTRATION:
        return "Concentration";
    case CardName::CODE_DU_BUSHIDO:
        return "Code du Bushido";
    case CardName::ARMURE:
        return "Armure";
    case CardName::NODACHI:
        return "Nodachi";
    case CardName::NAGINATA:
        return "Naginata";
    case CardName::NAGAYARI:
        return "Nagayari";
    case CardName::TANEGASHIMA:
        return "Tanegashima";
    case CardName::DAIKYU:
        return "Daikyu";
    case CardName::BO:
        return "Bo";
    case CardName::KUSARIGAMA:
        return "Kusarigama";
    case CardName::KATANA:
        return "Katana";
    case CardName::SHURIKEN:
        return "Shuriken";
    case CardName::KANABO:
        return "Kanabo";
    case CardName::BOKKEN:
        return "Bokken";
    case CardName::KISERU:
        return "Kiseru";
    case CardName::WAKIZASHI:
        return "Wakizashi";
    }
    return "Unknown";
}

CardType Card::get_type() {
	return type;
}

int Card::get_difficulty() {
	return difficulty;
}

int Card::get_damage() {
	return damage;
}

std::string Card::print_type() {
    switch (type) {
	case CardType::ACTION:
		return "Action";
	case CardType::PERMANENT:
		return "Permanent";
	case CardType::WEAPON:
		return "Weapon";
	}
	return "Unknown";
}

// Constructors
ActionCard::ActionCard(CardName name) : Card(name, CardType::ACTION) {}

PermanentCard::PermanentCard(CardName name) : Card(name, CardType::PERMANENT) {}

WeaponCard::WeaponCard(CardName name, int difficulty, int damage) : Card(name, CardType::WEAPON, difficulty, damage) {}