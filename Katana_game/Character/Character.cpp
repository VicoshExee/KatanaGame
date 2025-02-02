#include <iostream>
#include <string>

#include "Character.h"


std::vector<Character> characterVector = {
    Character(CharacterName::BENKEI, 5),
    Character(CharacterName::CHIYOME, 4),
    Character(CharacterName::GINCHIYO, 4),
    Character(CharacterName::GOEMON, 5),
    Character(CharacterName::HANZO, 4),
    Character(CharacterName::HIDEYOSHI, 4),
    Character(CharacterName::IEYASU, 5),
    Character(CharacterName::KOJIRO, 5),
    Character(CharacterName::MUSASHI, 5),
    Character(CharacterName::NOBUNAGA, 5),
    Character(CharacterName::TOMOE, 5),
    Character(CharacterName::USHIWAKA, 4)
};


Character::Character() : name(CharacterName::BENKEI), HP(4) {}
Character::Character(CharacterName name, int HP) : name(name), HP(HP) {}

CharacterName Character::get_name() const {
    return name;
}

std::string Character::print_name() {
    switch (name) {
    case CharacterName::BENKEI:
        return "Benkei";
    case CharacterName::CHIYOME:
        return "Chiyome";
    case CharacterName::GINCHIYO:
        return "Ginchiyo";
    case CharacterName::GOEMON:
        return "Goemon";
    case CharacterName::HANZO:
        return "Hanzo";
    case CharacterName::HIDEYOSHI:
        return "Hideyoshi";
    case CharacterName::IEYASU:
        return "Ieyasu";
    case CharacterName::KOJIRO:
        return "Kojiro";
    case CharacterName::MUSASHI:
        return "Musashi";
    case CharacterName::NOBUNAGA:
        return "Nobunaga";
    case CharacterName::TOMOE:
        return "Tomoe";
    case CharacterName::USHIWAKA:
        return "Ushiwaka";
    default:
        return "Unknown name";
    }
}

int Character::getHP() {
    return HP;
}

void Character::print() {
    std::cout << "Name: " << static_cast<int>(name) << std::endl;
    std::cout << "HP: " << HP << std::endl;
}