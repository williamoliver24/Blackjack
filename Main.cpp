#include <algorithm> // std::shuffle
#include <array>
#include <cassert>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937

// Maximum score before losing.
constexpr int g_maximumScore{ 21 };

// Minimum score that the dealer has to have.
constexpr int g_minimumDealerScore{ 17 };

class Card
{
public:

    enum Suit
    {
        club,
        diamond,
        heart,
        spade,

        max_suits
    };

    enum Rank
    {
        rank_2,
        rank_3,
        rank_4,
        rank_5,
        rank_6,
        rank_7,
        rank_8,
        rank_9,
        rank_10,
        rank_jack,
        rank_queen,
        rank_king,
        rank_ace,

        max_ranks
    };

private:
    int m_rank;
    int m_suit;

public:
    Card() = default;

    Card(Rank rank, Suit suit)
        : m_rank{ rank }, m_suit{ suit }
    {
    }

    void print() const
    {
        switch (m_rank)
        {
        case rank_2:      std::cout << '2';   break;
        case rank_3:      std::cout << '3';   break;
        case rank_4:      std::cout << '4';   break;
        case rank_5:      std::cout << '5';   break;
        case rank_6:      std::cout << '6';   break;
        case rank_7:      std::cout << '7';   break;
        case rank_8:      std::cout << '8';   break;
        case rank_9:      std::cout << '9';   break;
        case rank_10:     std::cout << "10";   break;
        case rank_jack:   std::cout << "Jack";   break;
        case rank_queen:  std::cout << "Queen";   break;
        case rank_king:   std::cout << "King";   break;
        case rank_ace:    std::cout << "Ace";   break;
        default:
            std::cout << '?';
            break;
        }

        std::cout << " of ";

        switch (m_suit)
        {
        case club:       std::cout << "Clubs";   break;
        case diamond:    std::cout << "Diamonds";   break;
        case heart:      std::cout << "Hearts";   break;
        case spade:      std::cout << "Spades";   break;
        default:
            std::cout << '?';
            break;
        }
    }

    int value() const
    {
        switch (m_rank)
        {
        case rank_2:        return 2;
        case rank_3:        return 3;
        case rank_4:        return 4;
        case rank_5:        return 5;
        case rank_6:        return 6;
        case rank_7:        return 7;
        case rank_8:        return 8;
        case rank_9:        return 9;
        case rank_10:       return 10;
        case rank_jack:     return 10;
        case rank_queen:    return 10;
        case rank_king:     return 10;
        case rank_ace:      return 11;
        default:
            assert(false && "should never happen");
            return 0;
        }
    }

};

class Deck
{
private:
    std::array<Card, 52> m_deck;
    int m_cardIndex{};

public:
    Deck()
    {
        int i{};

        for (int suit{}; suit < static_cast<int>(Card::Suit::max_suits); ++suit)
        {
            for (int rank{ 0 }; rank < static_cast<int>(Card::Rank::max_ranks); ++rank)
            {
                m_deck[i] = Card(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit));
                ++i;
            }
        }
    }

    void print()
    {
        for (const Card& card : m_deck)
        {
            card.print();
            std::cout << ", ";
        }

        std::cout << '\n';
    }

    void shuffle()
    {
        static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

        std::shuffle(m_deck.begin(), m_deck.end(), mt);

        m_cardIndex = 0;
    }

    const Card& deal()
    {
        return m_deck[m_cardIndex++];
    }
};

class Player
{
private:
    int m_score{};
    int m_playerAces{};

public:
    Card drawCard(Deck& deck)
    {
        Card card = deck.deal();
        int value{ card.value() };

        if (value == 11)
            ++m_playerAces;

        m_score += value;

        return card;
    }

    int score()
    {
        return m_score;
    }

    bool isBust()
    {
        return (m_score > g_maximumScore);
    }

    void resolveAces()
    {
        if (m_score > g_maximumScore && m_playerAces > 0)
        {
            m_score -= 10;
            --m_playerAces;
        }
    }
};

bool hitOrStick()
{
    while (true)
    {
        std::cout << "(h) to hit, or (s) to stick: ";

        char choice{};
        std::cin >> choice;

        switch (choice)
        {
        case 'h':
            return true;
        case 's':
            return false;
        }
    }
}

bool playersGo(Deck& deck, Player& player) // returns true if the player is bust
{
    while (true)
    {
        if (player.isBust())
        {
            std::cout << "You're bust!";
            return true;
        }
        else
        {
            if (hitOrStick())
            {
                auto card{ player.drawCard(deck) };
                player.resolveAces();

                std::cout << "You drew the ";
                card.print();
                std::cout << " and now have " << player.score() << '\n';
            }
            else
            {
                // player isn't bust
                return false;
            }
        }
    }
    return false;
}

bool dealersGo(Deck& deck, Player& dealer) // returns true if the dealer is bust
{
    while (dealer.score() < g_minimumDealerScore)
    {
        auto dealerCard{ dealer.drawCard(deck) };
        dealer.resolveAces();

        std::cout << "The dealer drew the ";
        dealerCard.print();
        std::cout << " and now has " << dealer.score() << '\n';
    }

    if (dealer.isBust())
    {
        std::cout << "The dealer is bust!\n";
        return true;
    }

    std::cout << "The dealer sticks on " << dealer.score() << '\n';
    return false;
}

bool playBlackjack(Deck& deck)
{
    Player dealer{};

    std::cout << "The dealer is showing the ";
    dealer.drawCard(deck).print();
    std::cout << '\n' << '\n';


    Player player{};
    std::cout << "Your first card is the ";
    player.drawCard(deck).print();
    std::cout << '\n';

    std::cout << "Your second card is the ";
    player.drawCard(deck).print();
    std::cout << '\n';

    std::cout << "You have: " << player.score() << '\n';

    if (playersGo(deck, player))
    {
        return false;
    }

    std::cout << '\n';

    if (dealersGo(deck, dealer))
    {
        return true;
    }

    return (player.score() > dealer.score());
}

bool replay()
{
    while (true)
    {
        std::cout << "Would you like to play again? \n" << "(y) for yes or (n) for no: ";

        char choice{};
        std::cin >> choice;

        switch (choice)
        {
        case 'y':
            return true;
        case 'n':
            return false;
        }
    }
}

int main()
{
    bool play{ true };
    while (play)
    {
        Deck deck{};

        deck.shuffle();

        if (playBlackjack(deck))
        {
            std::cout << "You win!\n";
        }
        else
        {
            std::cout << "You lose!\n";
        }

        play = replay();
        std::cout << '\n';
    }


    return 0;
}