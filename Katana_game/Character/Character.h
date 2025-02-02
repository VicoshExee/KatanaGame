#include <string>
#include <vector>

#ifndef CHARACTER_H
#define CHARACTER_H

enum class CharacterName {
    BENKEI,
    CHIYOME,
    GINCHIYO,
    GOEMON,
    HANZO,
    HIDEYOSHI,
    IEYASU,
    MUSASHI,
    NOBUNAGA,
    TOMOE,
    USHIWAKA,
    KOJIRO
};

class Character
{
private:
    CharacterName name;
    int HP;

public:
    bool operator==(const Character& other) const {
        // Compare the cards based on their properties
        return get_name() == other.get_name();
    }
    // Constructor
    Character();
    Character(CharacterName name, int HP);

    // Getters
    CharacterName get_name() const;
    std::string print_name();
    int getHP();
    void print();
};


extern std::vector<Character> characterVector;


#endif