#include <iostream>
#include <algorithm>
#include <random>

#include "Player.h"
#include "../Character/Character.h"

// Constructor
Player::Player(std::string name)
{
    this->HP = 0;
    this->HonorPoints = 0;
    this->name = name;
    this->additional_difficulty = 0;
    this->additional_weapon = 0;
    this->damage_mitigation = 0;
    this->additional_draw = 0;
    this->additional_damage = 0;
}

// Getters
int Player::get_HP()
{
    return HP;
}

int Player::get_HonorPoints()
{
    return HonorPoints;
}

std::string Player::get_name() const
{
    return name;
}

std::string Player::get_role() const
{
    return role;
}

std::vector<Card> Player::get_hand()
{
    return hand;
}

std::vector<Card> Player::get_permanent_card_played()
{
	return permanent_card_played;
}

int Player::get_additional_difficulty()
{
    return additional_difficulty;
}

int Player::get_additional_weapon()
{
    return additional_weapon;
}

int Player::get_damage_mitigation()
{
    return damage_mitigation;
}

int Player::get_additional_draw()
{
    return additional_draw;
}

int Player::get_additional_damage()
{
    return additional_damage;
}

CharacterName Player::get_character() const
{
    return this->character.get_name();
}

std::string Player::print_character()
{
    return this->character.print_name();
}

// Setters
void Player::set_role(std::string role)
{
    this->role = role;
}

void Player::set_character(Character character)
{
    this->character = character;
    this->HP = character.getHP();
    CharacterName name = character.get_name();
    switch (name) {
    case CharacterName::BENKEI:
        this->additional_difficulty += 1;
    case CharacterName::GINCHIYO:
        this->damage_mitigation += 1;
    case CharacterName::GOEMON:
        this->additional_weapon += 1;
    case CharacterName::HIDEYOSHI:
        this->additional_draw += 1;
    case CharacterName::MUSASHI:
        this->additional_damage += 1;
    }
}

void Player::recover_HP() {
    this->HP = this->character.getHP();
}

void Player::set_HP(int HP)
{
	this->HP = HP;
}

void Player::honor_loss() {
    this->HonorPoints -= 1;
}

// Methods
void Player::draw(Deck& deck, int nbr_of_draw)
{
    int usual_draw = 8;
    if (nbr_of_draw == -1) { // Si aucune valeur n'est sp�cifi�e -> c'est une pioche classique de d�but de tour

        for (int i = 0; i < usual_draw + additional_draw; i++) {
            if (deck.cards.size() == 0) {
                deck.init();
            }
            this->hand.push_back(deck.cards.back());
            deck.cards.pop_back();
        }
    }
    else { // Si une valeur est sp�cifi�e -> c'est une pioche avec une carte action, donc un montant sp�cifique
        for (int i = 0; i < nbr_of_draw; i++) {
            this->hand.push_back(deck.cards.back());
            deck.cards.pop_back();
        }
    }
}

Card Player::discard(int cardIndex, std::string list_of_cards) {
    // SI c'est une d�fausse classique -> depuis la main
    if (list_of_cards == "hand") {
        if (cardIndex < 0 || cardIndex >= this->hand.size()) {
            throw std::out_of_range("Card index out of range");
        }
        Card discarded_card = this->hand[cardIndex];
        this->hand.erase(this->hand.begin() + cardIndex);
        return discarded_card;
    }
    // SI c'est une d�fausse de carte permanente -> suite � une carte action
    else {
        if (cardIndex < 0 || cardIndex >= this->hand.size()) {
            throw std::out_of_range("Card index out of range");
        }
        Card discarded_card = this->permanent_card_played[cardIndex];
        this->permanent_card_played.erase(this->permanent_card_played.begin() + cardIndex);
        return discarded_card;
    }
}

Card Player::get_stealed_card() {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, this->hand.size() - 1);

    int random_index = distrib(gen);
    Card card = hand[random_index];
    this->hand.erase(this->hand.begin() + random_index);
    return card;
}

void Player::add_card_to_hand(Card& card) {
    this->hand.push_back(card);
}

void Player::play_permanent_card(int cardIndex)
{
    Card played_card = hand[cardIndex];
    CardName name = played_card.get_name();
    switch (name) {
    case CardName::ARMURE:
        this->additional_difficulty += 1;
        std::cout << "Votre armure augmente de 1, vous avec maintenant " << this->get_additional_difficulty() << " d'armure suppl�mentaire." << std::endl;
        break;
    case CardName::CONCENTRATION:
        this->additional_weapon += 1;
        std::cout << "Votre nombre d'armes jouables pendant le m�me tour augmente de 1, vous pouvez maintenant jouer " << this->get_additional_weapon() << " armes(s) suppl�mentaire." << std::endl;
        break;
    case CardName::ATTAQUE_RAPIDE:
        this->additional_damage += 1;
        std::cout << "Vos d�gats d'armes bonus augmentent 1, elles inflignent maintenant " << this->get_additional_damage() << " d�gats suppl�mentaire." << std::endl;
        break;
    default:
        std::cout << "Unknown caca" << std::endl;
    }
    this->hand.erase(this->hand.begin() + cardIndex);
    this->permanent_card_played.push_back(played_card);
}

void Player::discard_permanent_card(int cardIndex) 
{
    Card discarded_card = hand[cardIndex];
    CardName name = discarded_card.get_name();
    switch (name) {
    case CardName::ARMURE:
        this->additional_difficulty -= 1;
    case CardName::CONCENTRATION:
        this->additional_weapon -= 1;
    case CardName::ATTAQUE_RAPIDE:
        this->additional_damage -= 1;
    default:
        std::cout << "Unknown card" << std::endl;
    }
    discard(cardIndex, "permanent_card_played"); // On d�fausse depuis les cartes permanentes d�j� jou�es
}

void Player::recover_HP_phase() {
    if (HP < 1) {
        HP = character.getHP();
    }
}

void Player::draw_phase(Deck& deck) {
    draw(deck);
}

int Player::play_card_phase() {
    print_hand();
    std::cout << "Entrez le num�ro d'une carte pour la jouer, ou -1 si vous avez fini / passez votre tour : " << std::endl;
    int response;
    std::cin >> response;
    if (response == -1) {
		return -1;
	}
    while (std::cin.fail() || response >= hand.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore le reste de la ligne
        std::cout << "Ce n'est pas un nombre valide" << std::endl;
        std::cout << "Entrez le num�ro d'une carte pour la jouer, ou -1 si vous avez fini / passez votre tour : " << std::endl;
        std::cin >> response;
    }
    system("cls");
    std::cout << "Vous avez pr�c�demment jou� : " << hand[response].print_name() << std::endl;
    std::cout << std::endl;
    return response;
}

std::vector<Card> Player::discard_phase() {
    std::vector<Card> discarded_cards_vector;
    while (hand.size() > 7) {
        int element_index_to_discard;
        std::cout << "Votre main est pleine veuillez d�fausser une carte." << std::endl;
        print_hand();
        std::cout << "Veuillez entrer un nombre entre 0 et " << hand.size() - 1 << " : ";
        std::cin >> element_index_to_discard;
        while (std::cin.fail() || element_index_to_discard > hand.size()-1) {
            std::cout << "Ce n'est pas un nombre valide" << std::endl;
            std::cout << "Veuillez entrer un nombre entre 0 et " << hand.size()-1 << " : ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore le reste de la ligne
            std::cin >> element_index_to_discard;
        }
        system("cls");
        discarded_cards_vector.push_back(discard(element_index_to_discard));
    }
    if (discarded_cards_vector.size() > 0) { // Si des cartes ont �t�s d�fauss�es on affiche le r�sum�
        system("cls");
        std::cout << "Vous avez d�fauss� : " << std::endl;
        for (Card card : discarded_cards_vector) {
            std::cout << card.print_name() << std::endl;
        }
    }
    return discarded_cards_vector;
}

std::vector<Card> Player::play_turn(Deck& deck) {
    recover_HP_phase();
    draw_phase(deck);
    play_card_phase();
    return discard_phase(); // discarded_cards
}

void Player::print_hand()
{
    std::cout << "---------- Joueur " << this->name << ", votre main ----------" << std::endl;
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << i << "." << hand[i].print_name() << std::endl;
    }
    std::cout << std::endl;
}

int Player::get_attacked(Player& player, Card card) {
    // On v�rifie si le joueur attaqu� a une parade
    auto it = std::find_if(hand.begin(), hand.end(), [](const Card& card) {
        return card.get_name() == CardName::PARADE;  
        });
    // Si c'est le cas, on demande au joueur attaqu� s'il souhaite l'utiliser
    if (it != hand.end()) {
        std::cout << "---------- Joueur " << this->name << ", vos stats ----------" << std::endl;
        std::cout << "HP : " << this->HP << std::endl;
        std::cout << "D�fense : " << this->damage_mitigation << std::endl;
        std::cout << "Vous avez �t� attaqu� par " << player.get_name() << " avec la carte " << card.print_name() << ". Vous allez subir " << card.get_damage() + player.get_additional_damage() - this->get_damage_mitigation() << " d�gat(s)." << std::endl;
        std::cout << "Vous avez une parade dans votre main, souhaitez vous l'utiliser ? (1 pour oui, 0 pour non)" << std::endl;
		int response;
		std::cin >> response;
        while (std::cin.fail() || response < 0 || response > 1) {
			std::cout << "Ce n'est pas un nombre valide" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore le reste de la ligne
			std::cin >> response;
		}
        if (response == 1) {
            int index = std::distance(this->hand.begin(), it);
			discard(index);
			std::cout << "Vous avez utilis� une parade, vous n'avez pas subi de d�gat" << std::endl;
			return index;
		}
        else {
			std::cout << "Vous n'avez pas utilis� de parade, vous subissez " << card.get_damage() << " d�gat(s)" << std::endl;
        }
    }
    else {
        std::cout << "Joueur " << this->name << " vous n'avez pas de parade dans votre main, vous subissez " << card.get_damage() + player.get_additional_damage() - this->get_damage_mitigation() << " d�gats" << std::endl;
        this->HP -= card.get_damage() + player.get_additional_damage() - this->get_damage_mitigation();
        std::cout << "Vous avez maintenant " << this->HP << " HP." << std::endl;
    }
    return -1;
}

void Player::displayInfos()
{
    std::cout << "Name: " << this->name << std::endl;
    std::cout << "Role: " << this->role << std::endl;
    std::cout << "HP: " << this->HP << std::endl;
    std::cout << "Honor Points: " << this->HonorPoints << std::endl;
    std::cout << "Hand: " << std::endl;
    for (Card card : this->hand)
    {
        std::cout << card.print_name() << std::endl;
    }
    std::cout << "Additional difficulty: " << this->additional_difficulty << std::endl;
    std::cout << "Additional weapon: " << this->additional_weapon << std::endl;
    std::cout << "Damage mitigation: " << this->damage_mitigation << std::endl;
    std::cout << "Additional draw: " << this->additional_draw << std::endl;
    std::cout << "Additional damage: " << this->additional_damage << std::endl;
    std::cout << "Character: " << this->character.print_name() << std::endl;
}



int Player::select_target(int nbrPlayer) {
    std::cout << "Entrez le num�ro du joueur que vous souhaitez cibler : " << std::endl;
    int response;
    std::cin >> response;
    while (std::cin.fail() || response < -1 || response > nbrPlayer - 1) {
        std::cout << "Ce n'est pas un nombre valide" << std::endl;
        std::cout << "Entrez le num�ro du joueur que vous souhaitez cibler : " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore le reste de la ligne
        std::cin >> response;
    }
    return response;
}

void Player::get_Jujitsued() {
    std::vector<Card> weapon_cards_in_hand;
    for (Card card : hand) {
        if (card.get_type() == CardType::WEAPON) {
            weapon_cards_in_hand.push_back(card);
        }
    }
    std::cout << " vous avez �t� affect� par la carte Jujitsu" << std::endl; // le formatage �trange de la ligne est volontaire
    // SI le joueur a des cartes armes en main
    if (weapon_cards_in_hand.size() > 0) {
        std::cout << "Vous possedez des armes dans votre main :" << std::endl;
        for (size_t i = 0; i < weapon_cards_in_hand.size(); ++i) {
            std::cout << i << "." << weapon_cards_in_hand[i].print_name() << std::endl;
        }
        std::cout << "Vous devez en d�fausser une ou perdre un point de vie." << std::endl;
        std::cout << "Que souhaitez vous faire ? (entrez un index pour d�fausser la carte en question, ou -1 pour perdre un point de vie) : " << std::endl;
        int response;
        std::cin >> response;
        if (response == -1) {
            this->HP -= 1;
            return;
        }
        while (std::cin.fail() || response < -1 || response > hand.size() - 1) {
            std::cout << "R�ponse invalide" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore le reste de la ligne
            std::cout << "Que souhaitez vous faire ? (entrez un index pour d�fausser la carte en question, ou -1 pour perdre un point de vie) : " << std::endl;
            std::cin >> response;
        }
        Card card = weapon_cards_in_hand[response];
        auto it = std::find_if(hand.begin(), hand.end(), [&card](const Card& hand_card) {
            return hand_card.get_name() == card.get_name();
            });
        discard(it - this->hand.begin());
        system("cls");
    }
    // SI le joueur n'a pas de cartes arme en main
    else {
        std::cout << "Vous ne possedez pas de cartes armes, par cons�quent vous �tes forc� de perdre un point de vie" << std::endl;
        this->HP -= 1;
        std::cout << "Press enter to continue...";
        std::cin.get();
        system("cls");
    }
}

int Player::select_geihsha_mode(Player& target) {
    int response;
    if (target.get_permanent_card_played().size() > 0) {
        std::cout << "Les cartes permanentes du joueur" << target.get_name() << "sont les suivantes :" << std::endl;
        for (Card card : target.get_permanent_card_played()) {
            std::cout << card.print_name() << std::endl;
        }
        std::cout << "Souhaitez vous d�fausser une carte permanente d�j� jou�e (1) ou une carte al�atoire de sa main (2) ? :" << std::endl;
        std::cin >> response;
        while (std::cin.fail() || response < 1 || response > 2) {
            std::cout << "R�ponse invalide" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore le reste de la ligne
            std::cout << "Souhaitez vous d�fausser une carte permanente d�j� jou�e (1) ou une carte al�atoire de sa main (2) ? :" << std::endl;
            std::cin >> response;
        }
        return response;
    }
    else {
        std::cout << "Le joueur" << target.get_name() << "n'a pas encore jou� de cartes permanentes" << std::endl;
        std::cout << "Vous devez donc d�fausser une carte al�atoire de sa main" << std::endl;
		return 2;
    }
}

void Player::get_geishaed(int geishaMode, int cardIndexToDiscard) {
    std::cout << " vous avez �t� affect� par la carte Geisha" << std::endl; // le formatage �trange de la ligne est volontaire
    if (geishaMode == 1) {
        std::cout << "Cette carte permanente a �t� d�fauss�e : " << this->permanent_card_played[cardIndexToDiscard].print_name() << std::endl;
        discard_permanent_card(cardIndexToDiscard);
    }
    else {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, this->get_permanent_card_played().size()-1);

        int random_index = distrib(gen);
		std::cout << "Cette carte de la main a �t� d�fauss�e : " << this->hand[random_index].print_name() << std::endl;
		discard(random_index);
    }
}

void Player::get_Cri_de_guerred() {
    std::vector<Card> parade_in_hand;
    for (Card card : hand) {
        if (card.get_name() == CardName::PARADE) {
            parade_in_hand.push_back(card);
        }
    }
    std::cout << " vous avez �t� affect� par la carte Cri de guerre" << std::endl; // le formatage �trange de la ligne est volontaire
    // SI le joueur a des cartes armes en main
    if (parade_in_hand.size() > 0) {
        std::cout << "Vous possedez des parade dans votre main :" << std::endl;
        for (size_t i = 0; i < parade_in_hand.size(); ++i) {
            std::cout << i << "." << parade_in_hand[i].print_name() << std::endl;
        }
        std::cout << "Vous devez en d�fausser une ou perdre un point de vie." << std::endl;
        std::cout << "Que souhaitez vous faire ? (entrez un 1 pour d�fausser une parade, ou 0 pour perdre un point de vie) : " << std::endl;
        int response;
        std::cin >> response;
        while (std::cin.fail() || response < 0 || response > 1) {
            std::cout << "R�ponse invalide" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore le reste de la ligne
            std::cout << "Que souhaitez vous faire ? (entrez un 1 pour d�fausser une parade, ou 0 pour perdre un point de vie) : " << std::endl;
            std::cin >> response;
        }
        if (response == 0) {
            this->HP -= 1;
        }
        else {
            // On trouve la premi�re parade dans la main du joueur et on la d�fausse
            auto it = std::find_if(hand.begin(), hand.end(), [](const Card& card) {
                return card.get_name() == CardName::PARADE;
				});
            discard(it - this->hand.begin());
        }
        system("cls");
    }
    // SI le joueur n'a pas de cartes arme en main
    else {
        std::cout << "Vous ne possedez pas de cartes armes, par cons�quent vous �tes forc� de perdre un point de vie" << std::endl;
        this->HP -= 1;
        std::cout << "Press enter to continue...";
        std::cin.get();
        system("cls");
    }
}






/*
void Player::game_phase()
{
    bool play = true;
    while (play && HP > 0) {
        std::cout << "=== Votre tour ===" << std::endl;
        print_hand();
        std::cout << "Choisissez une carte � jouer (entrez l'index de la carte) :";
        int cardIndex;
        std::cin >> cardIndex;
        if (cardIndex < 0 || cardIndex >= hand.size()) {
            std::cout << "Index de la carte invalide, veuillez entrer un index valide" << std::endl;
            continue;
        }
        Card card = hand[cardIndex];
        if (card.get_cost() > HP) {
            std::cout << "Vous n'avez pas assez d'HP pour jouer cette carte, choisissez en une autre" << std::endl;
            continue;
        }
        if (card.is_permanent_card()) {
            play_permanent_card(card.get_permanent_card());
        }
        else {
            // TODO faut impl�menter la fonction attack

        }
        HP -= card.get_HP();
        hand.erase(hand.begin() + cardIndex);
        std::cout << "Carte jou�e " << card.print_name() << std::endl;
    }
    if (HP <= 0) {
        std::cout << "perdu looser bozo" << std::endl;
    }
}
*/