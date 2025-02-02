#include <random>
#include <algorithm>
#include <iostream>
#include <vector>

#include "Game.h"

// Constructor
Game::Game() {
    start_game();
};

// Methods
void Game::start_game()
{
    createPlayers(); // Create players
    attributeRoles(); // Create and attribute roles to players
    distribute_characters(); // Distribute characters to players

    // Init deck and shuffle it
    deck.init();

    // Draw starting hand
    draw_starting_hand();
    while (true) {
        for (Player player : players) {
            player_play_turn(player);
            system("cls");
            
        }
        bool endGame = check_end_game();
        if (endGame == false) {
            break;
        }
    }
    end_game();
}

int Game::getNbrPlayers() {
    int nbrPlayers;

    do {
        std::cout << "Combien y a-t-il de joueurs ? ";
        std::cin >> nbrPlayers;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Veuillez entrer un nombre entier entre 4 et 7." << std::endl;
        }
        else if (nbrPlayers < 4 || nbrPlayers > 7) {
            std::cout << "Veuillez entrer un nombre entier entre 4 et 7." << std::endl;
        }
        else {
            system("cls");
            return nbrPlayers;
        }

    } while (true);

    return nbrPlayers;
}

std::vector<std::string> Game::createRoles() {
    int nbrPlayers = this->players.size();
    std::vector<std::string> roles = { "Shogun", "Samourai", "Ninja", "Ninja" };

    // Pour chaque joueur en plus, on ajoute un rôle
    if (nbrPlayers >= 5) {
        roles.push_back("Ronin");
    }
    if (nbrPlayers >= 6) {
        roles.push_back("Ninja");
    }
    if (nbrPlayers >= 7) {
        roles.push_back("Samourai");
    }

    // Initialisation du générateur de nombres aléatoires avec une graine
    std::random_device rd;
    std::mt19937 generator(rd());

    // Mélange des rôles
    std::shuffle(roles.begin(), roles.end(), generator);

    // Affichage du vecteur mélangé
    std::cout << "Cartes mélangées : ";
    for (const auto& role : roles) {
        std::cout << role << " ";
    }
    std::cout << std::endl;
    // Effacer l'écran
    system("cls");

    // Retourne le vecteur mélangé des rôles
    return roles;
}

std::vector<Player> Game::createPlayers() {
    int nbrPlayers = getNbrPlayers();
    std::string name;

    for (int i = 0; i < nbrPlayers; i++) {
        std::cout << "Joueur " << i + 1 << ", quel est votre nom ? ";
        std::cin >> name;

        Player player(name);
        players.push_back(player);
        system("cls");
    }
    return players;
}

void Game::move_shogun_to_first() {
    while (!players.empty() && players[0].get_role() != "Shogun") {
        // Déplacer le joueur à l'index 0 à la fin du vecteur
        std::rotate(players.begin(), players.begin() + 1, players.end());
    }
}

void Game::attributeRoles() {
    std::vector<std::string> roles = createRoles();
    // Vérifier que les deux vecteurs ont la même taille
    if (roles.size() == players.size()) {
        // Attribution des rôles aux joueurs
        for (size_t i = 0; i < players.size(); ++i) {
            players[i].set_role(roles[i]);
        }
    }
    else {
        std::cout << "Les vecteurs n'ont pas la même taille." << std::endl;
    }
    move_shogun_to_first();
}

void Game::distribute_characters() {
    // Initialisation du générateur de nombres aléatoires avec une graine
    std::random_device rd;
    std::mt19937 generator(rd());

    // Mélange des personnages
    std::shuffle(characterVector.begin(), characterVector.end(), generator);

    // On attribue personnages aux joueurs
    for (size_t i = 0; i < players.size(); ++i) {
        players[i].set_character(characterVector[i]);
    }
}

void Game::draw_starting_hand()
{
    // Define the number of cards to draw for each player
    for (size_t i = 0; i < players.size(); ++i) {
        Player& player = players[i];
        int nbr_of_draw;
        // Shogun always draw 4 cards
        if (player.get_role() == "Shogun") {
            nbr_of_draw = 4;
        }
        else {
            // Otherwise the number of cards to draw depends on the number of players
            switch (i) {
            case 1:
            case 2:
				nbr_of_draw = 5;
				break;
            case 3:
            case 4:
                nbr_of_draw = 6;
                break;
            case 5:
            case 6:
                nbr_of_draw = 7;
                break;
            default:
                nbr_of_draw = 6;  // Default number of cards to draw (error case)
                break;
            }
        }
        // Draw the cards
        player.draw(deck, nbr_of_draw);
    }
}

void Game::display_players_infos() {
    for (Player player : players) {
        std::cout << "Joueur : " << player.get_name() << " | HP : " << player.get_HP() << " | Honor Points : " << player.get_HonorPoints();
        if (player.get_role() == "Shogun") {
            std::cout << " | Role : Shogun";
        }
        std::cout << " | Cartes permanentes jouées : ";
        if (player.get_permanent_card_played().empty()) {
            std::cout << "Aucune";
        }
        else {
            for (Card card : player.get_permanent_card_played()) {
                std::cout << card.print_name() << " / ";
            }
        }
        std::cout << " | Personnage : " << player.print_character();
        std::cout << std::endl;
    }
}

void Game::draw_card(Player& player)
{
    player.draw(deck);
}

void Game::draw_card_discard_pile(Player& player)
{
    player.draw(discard_pile);
}

void Game::discard(Player& player, int cardIndex)
{
    Card discarded_card = player.discard(cardIndex);
    this->discard_pile.cards.push_back(discarded_card);
}

void Game::play_card(Player& player, int cardIndex) {
    CharacterName character_of_player = player.get_character();
    Card card = player.get_hand()[cardIndex];
    std::vector<Player*> other_players;
    auto indexFinder = std::find(this->players.begin(), this->players.end(), player);
    int playerIndex = std::distance(this->players.begin(), indexFinder);
    int numberOfCardWeaponPlayed = 0;
    
    // On créer un vecteur avec tous les joueurs sauf le joueur actuel
    for (Player player : this->players) {
        if (player.get_character() != character_of_player) {
            other_players.push_back(&player);
        }
    }

    switch (card.get_type()) {
        case CardType::WEAPON:
        {
            // On vérifie qu'il ne joue pas trop de carte arme
            if (numberOfCardWeaponPlayed >= player.get_additional_weapon() + 1) {
                std::cout << "Vous avez déjà joué votre maximum de carte arme !" << std::endl;
                return;
            }
            else {
                numberOfCardWeaponPlayed += 1;
            }
            std::cout << "Choisissez une cible : " << std::endl;
            print_players_difficulty(player);
            std::cout << "Votre arme à une précision de " << card.get_difficulty() << " et inflige " << card.get_damage() << " dégats" << std::endl;
            int target = player.select_target(this->players.size());
            while (card.get_difficulty() < player_get_difficulty(player, target)) {
				std::cout << "La précision de l'arme est insuffisante pour toucher la cible." << std::endl;
                print_players_difficulty(player);
				target = player.select_target(this->players.size());
			}
		    attack(player, target, card);
			break;
		}
        case CardType::PERMANENT:
        {
            player.play_permanent_card(cardIndex);
            return;
        }
        case CardType::ACTION:
            switch (card.get_name()) {
                case CardName::CEREMONIE_DU_THE:
                {
                    std::cout << "Vous piochez 3 cartes, et tous les autres joueurs piochent 1 carte." << std::endl;
                    player.draw(deck, 3);
                    for (Player* other_player : other_players) {
                        other_player->draw(deck, 1);
                    }
                    break;
                }
                case CardName::CRI_DE_GUERRE:
                {
                    std::cout << "Tous les autres joueurs en vie doivent soit défausser une parade, soit perdre 1 point de vie." << std::endl;
                    for (Player* player : other_players) {
                        if (player->get_HP() > 0) {
                            std::cout << "Joueur " << player->get_name();
                            player->get_Cri_de_guerred();
                        }
                        else {
                            std::cout << "Joueur " << player->get_name() << " est mort, il ne peut pas être ciblé par Cri de guerre" << std::endl;
                        }
                    }
                    break;
                }
                case CardName::DAIMYO:
                {
                    player.draw(deck, 2);
                    break;
                }
                case CardName::DIVERSION:
                {
                    std::cout << "Vous pouvez choisir un joueur pour lui voler une carte." << std::endl;
                    display_players_infos();
                    int targetIndex = player.select_target(players.size());
                    while (targetIndex == playerIndex) {
                        std::cout << "Vous ne pouvez pas vous cibler vous même !" << std::endl;
                        display_players_infos();
                        targetIndex = player.select_target(players.size());
                    }
                    Card card = (players[targetIndex].get_stealed_card());
                    player.add_card_to_hand(card);
                    break;
                }
                case CardName::GEISHA:
                {
                    std::cout << "Vous pouvez défausser une carte permanente ou une carte aléatoire de la main d'un joueur adverse." << std::endl;
                    display_players_infos();
                    int targetIndex = player.select_target(this->players.size());
                    while (targetIndex == playerIndex) {
                        std::cout << "Vous ne pouvez pas vous cibler vous même !" << std::endl;
                        display_players_infos();
                        targetIndex = player.select_target(players.size());
                    }
                    Player* playerTarget = &this->players[targetIndex];
                    int geishaMode = player.select_geihsha_mode(*playerTarget);
                    playerTarget->get_geishaed(geishaMode);
                    break;
                }
                case CardName::MEDITATION:
                {
                    std::cout << "Vous récupérez tous vos points de vie et vous choisissez un joueur qui piochera une carte." << std::endl;
                    player.recover_HP();
                    display_players_infos();
                    int target = player.select_target(this->players.size());
                    players[target].draw(this->deck, 1);
                    break;
                }
                case CardName::JUJITSU:
                    std::cout << "Tous les autres joueurs en vie doivent soit défausser une arme, soit perdre 1 point de vie." << std::endl;
                    for (Player* player : other_players) {
                        if (player->get_HP() > 0) {
                            std::cout << "Joueur " << player->get_name();
                            player->get_Jujitsued();
                        }
                        else {
                            std::cout << "Joueur " << player->get_name() << " est mort, il ne peut pas être ciblé par Jujitsu" << std::endl;
                        }
                    }
                    break;
                case CardName::PARADE:
                {
                    std::cout << "Vous ne pouvez jouer cette carte que si vous êtes attaqué !" << std::endl;
                    return;
                }
                default:
                    std::cout << "Carte non reconnue" << std::endl;
                    break;
            }
    }
    discard(player, cardIndex);
    return;
}

void Game::attack(Player& player, int& target, Card card) 
{
    erase_screen();
    int result = this->players[target].get_attacked(player, card);
    if (result > -1) {
        discard(this->players[target], result);
    }
}

int Game::player_get_difficulty(Player player, int targetIndex) {
    auto indexFinder = std::find(this->players.begin(), this->players.end(), player);
    int playerIndex = std::distance(this->players.begin(), indexFinder);

    // Calculer la distance dans le sens direct
    int distance1 = (targetIndex >= playerIndex) ? targetIndex - playerIndex : targetIndex - playerIndex + players.size();

    // Calculer la distance dans le sens inverse
    int distance2 = (targetIndex >= playerIndex) ? playerIndex - targetIndex + players.size() : playerIndex - targetIndex;

    // On prend la distance la plus courte
    int distance = std::min(distance1, distance2);

    return distance + this->players[targetIndex].get_additional_difficulty();
}


void Game::print_players_difficulty(Player player) {
    auto indexFinder = std::find(this->players.begin(), this->players.end(), player);
    int playerIndex = std::distance(this->players.begin(), indexFinder);
    for (Player target : players) {
        if (player == target) {
			continue;
		}
        auto targetIndexFinder = std::find(this->players.begin(), this->players.end(), target);
        int targetIndex = std::distance(this->players.begin(), targetIndexFinder);

        int difficulty = player_get_difficulty(player, targetIndex);
        if (this->players[targetIndex].get_character() == CharacterName::BENKEI) {
            std::cout << targetIndex << ".Joueur " << target.get_name() << " a " << target.get_HP() << " HP, et une difficulté de " << difficulty << " : Benkei" << std::endl;
		}
		else {
            std::cout << targetIndex << ".Joueur " << target.get_name() << " a " << target.get_HP() << " HP, et une difficulté de " << difficulty << std::endl;
	    }
	}
    return;
} 

void Game::erase_screen() {
    system("cls");
    std::cout << "Appuyez sur Entrée pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    system("cls");
}

void Game::player_play_turn(Player& player) {
    player.recover_HP_phase();
    player.draw_phase(this->deck);

    // Play card phase
    while (true) { // Tant que le joueur joue des cartes
        display_players_infos();
        int cardIndex = player.play_card_phase();
        if (cardIndex < 0) { // Si le joueur ne joue pas de carte
            system("cls");
            std::cout << "Joueur " << player.get_name() << ", vous avez arrêté votre tour." << std::endl;
            break;
        }
        play_card(player, cardIndex);
	}

    // Discard phase
    std::vector<Card> discarded_cards = player.discard_phase();
    this->discard_pile.cards.insert(discard_pile.cards.end(), discarded_cards.begin(), discarded_cards.end()); // On met d'un coup toutes les cartes défaussées dans la pile de défausse
    std::cout << "Pile de défausse : ";
    for (Card card : this->discard_pile.cards) {
        std::cout << card.print_name() << std::endl;
    }
}

void Game::shuffle_deck()
{
    this->deck.shuffle();
}

void Game::check_dead_players() {
    for (Player& player : players) {
        if (player.get_HP() < 1) {
            auto it = std::find(this->players.begin(), this->players.end(), player);
            if (it != this->players.end()) {
                this->dead_players.push_back(*it);
                this->players.erase(it);
            }
        }
    }
}

void Game::revive_players(bool end) {
    for (Player player : this->dead_players) {
        if (end == false) {
            player.honor_loss();
        }
        this->players.push_back(player);
        check_end_game();
    }
}

bool Game::check_end_game() {
    for (Player player : this->players) {
        if (player.get_HonorPoints() <= 0) {
            return false;
        }
    }
}

void Game::end_game()
{
    int scoreNinjas = 0;
    int scoreShogunSamurai = 0;
    int scoreRonin = 0;
    revive_players(true);
    for (Player& player : players) {
        if (player.get_role() == "Ninja") {
            scoreNinjas += player.get_HonorPoints();
        }
        else if (player.get_role() == "Ronin") {
            scoreRonin += player.get_HonorPoints();
        }
        else {
            scoreShogunSamurai += player.get_HonorPoints();
        }
    }
    std::cout << "Score des ninjas : " << scoreNinjas << std::endl;
    std::cout << "Score du Ronin : " << scoreRonin << std::endl;
    std::cout << "Score du Shogun et des Samurai : " << scoreShogunSamurai << std::endl;

    if (scoreNinjas > scoreRonin && scoreNinjas > scoreShogunSamurai) {
        std::cout << "Les Ninjas ont gagné!" << std::endl;
    }
    else if (scoreRonin > scoreNinjas && scoreRonin > scoreShogunSamurai) {
        std::cout << "Le Ronin a gagné!" << std::endl;
    }
    else {
        std::cout << "Le Shogun et les Samurai ont gagné!" << std::endl;
    }
}