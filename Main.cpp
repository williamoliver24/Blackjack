#include <iostream>
#include <string>
#include <vector>
#include <limits> // For clearing input
#include <random> // To generate numbers for card draws
#include <chrono> // To seed number generation



// These constants represent the number of decks to be drawn from, the maximum hand value and the dealer's hit threshold
// You can change their values to change how the game plays
constexpr int numberOfDecks{ 1 };
constexpr int maxHandValue{ 21 };
constexpr int dealerMaxValue{ 17 };

// This generator will give us a random number corresponding to a card
std::mt19937 mt{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_int_distribution<> randomCard{ 1, 52 * numberOfDecks };

// This function will allow us to clear any extraneous input
void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// This function will allow the player to make choices by entering 1 for yes or 0 for no
int getInput()
{
	while (true)
	{
		std::cout << "Enter 1 for yes or 0 for no: ";
		int input{};
		std::cin >> input;
		ignoreLine();

		switch (input)
		{
		case 0:
		case 1:
			return input;
		default:
			std::cerr << "Sorry, that input is invalid. Please try again. \n";
		}
	}
}

//This function prints the name of a card based on its number in the deck
void cardName(int num)
{
	std::string suit{};
	std::string label{};

	if (num % 52 <= 13)
		suit = "Hearts";
	else if (num % 52 <= 26)
		suit = "Spades";
	else if (num % 52 <= 39)
		suit = "Diamonds";
	else
		suit = "Clubs";

	switch (num % 13)
	{
	case 1:
		label = "Ace";
		break;
	case 11:
		label = "Jack";
		break;
	case 12:
		label = "Queen";
		break;
	case 0:
		label = "King";
		break;
	default:
		label = std::to_string(num % 13);
		break;
	}

	std::cout << label << " of " << suit << '\n';
}

// This function takes a players current point total and the value of a new card and returns the value of the new card
int cardValue(int num)
{
	if (num % 13 == 1)
		return 11;
	else if (num % 13 >= 11 || num % 13 == 0)
		return 10;
	else
		return (num % 13);
}

// This function generates a number corresponding to a card not yet in play
int dealCard(std::vector<int>& cardsInPlay)
{	
	while (true)
	{
		int cardNo{ randomCard(mt) };
		bool newCard{ true };
		while (newCard)
		{
			for (int i{}; i <= cardsInPlay.size() - 1; ++i)
			{
				if (cardsInPlay[i] == cardNo)
					newCard = false;
			}

			if (newCard)
			{
				cardsInPlay.push_back(cardNo);
				return cardNo;
			}
		}
	}
}



int main()
{
	// This preamble tells the player the constant values defined above - it will only show once even if the player plays multiple times
	(numberOfDecks == 1) ? std::cout << "There is one deck in play," : std::cout << "There are " << numberOfDecks << " decks in play,";
	std::cout << " the max point value is " << maxHandValue << " and";
	std::cout << " the dealer will stick on "<< dealerMaxValue << " or more.\n" << '\n';

	// The player is asked whether they want to play
	// The playState variable can take value 0 to not start another game, 1 to represent an unfinished game in progress or 2 to represent a game in progress that has finished early
	std::cout << "Would you like to play Blackjack?" << '\n';
	int playState{ getInput() };
	std::cout << '\n';
	while (playState == 1)
	{
		// These variables track the player and dealer's scores and number of aces
		int playerTotal{};
		int playerAces{};
		int dealerTotal{};
		int dealerAces{};
		int currentCardNo{};
		//This vector holds the numbers of the cards in play
		std::vector<int> cardsInPlay{0};

		// The first card is dealt to the player
		currentCardNo = dealCard(cardsInPlay);
		std::cout << "Your first card is the ";
		cardName(currentCardNo);
		std::cout << '\n';
		playerTotal += cardValue(currentCardNo);
		((currentCardNo % 13 == 1) ? ++playerAces : 0);

		// The second card is dealt to the player
		currentCardNo = dealCard(cardsInPlay);
		std::cout << "Your second card is the ";
		cardName(currentCardNo);
		std::cout << '\n';
		playerTotal += cardValue(currentCardNo);
		((currentCardNo % 13 == 1) ? ++playerAces : 0);

		// If the player's score is greater than the max hand value we reduce it by 10 for each Ace they have or declare them bust if they have none
		while (playerTotal > maxHandValue)
		{
			if (playerAces > 0)
			{
				--playerAces;
				playerTotal -= 10;
			}
			else
			{
				std::cout << "You're bust!\n";
				playState = 2;
			}
		}

		// If the player is bust we skip to the end
		if (playState != 2)
		{
			// The player is asked whether they want to hit and plays until they stick or go bust
			int hit{ 1 };
			while (hit == 1)
			{
				std::cout << "Would you like to hit? ";
				hit = getInput();
				std::cout << '\n';
				if (hit == 1)
				{
					currentCardNo = dealCard(cardsInPlay);
					std::cout << "Your next card is the ";
					cardName(currentCardNo);
					std::cout << '\n';
					playerTotal += cardValue(currentCardNo);
					((currentCardNo % 13 == 1) ? ++playerAces : 0);
					if (playerTotal > maxHandValue)
					{
						// The player's score is over the max hand value and they have no aces so they are bust
						if (playerAces == 0)
						{
							std::cout << "You're bust!\n";
							playState = 2;
							break;
						}
						// The player's score is over the max hand value, but they have an ace worth 11 which can be reduced to 1 so they are not bust
						else
						{
							--playerAces;
							playerTotal -= 10;
						}
					}
				}
			}
		}

		// If the player is bust we skip to the end
		if (playState != 2)
		{
			// Now the dealer plays
			currentCardNo = dealCard(cardsInPlay);
			std::cout << "The dealer's first card is the ";
			cardName(currentCardNo);
			std::cout << '\n';
			dealerTotal += cardValue(currentCardNo);
			((currentCardNo % 13 == 1) ? ++dealerAces : 0);

			// The second card is dealt to the player
			currentCardNo = dealCard(cardsInPlay);
			std::cout << "The dealer's second card is the ";
			cardName(currentCardNo);
			std::cout << '\n';
			dealerTotal += cardValue(currentCardNo);
			((currentCardNo % 13 == 1) ? ++dealerAces : 0);
			//Deal with two aces case - if playerTotal > maxHandValue the player must have two aces so we reduce it by 11 and "delete" one ace
			if (dealerTotal > maxHandValue)
			{
				--dealerAces;
				dealerTotal -= 10;
			}

			int d_hit{ 1 };
			while (d_hit == 1)
			{
				(dealerTotal > 16 ? d_hit = 0 : d_hit = 1);
				if (d_hit == 1)
				{
					currentCardNo = dealCard(cardsInPlay);
					std::cout << "The dealer hits." << '\n';
					std::cout << "The dealer draws the ";
					cardName(currentCardNo);
					std::cout << '\n';
					dealerTotal += cardValue(currentCardNo);
					((currentCardNo % 13 == 1) ? ++dealerAces : 0);
					if (dealerTotal > maxHandValue)
					{
						// The dealer's score is over the max hand value and they have no aces so they are bust
						if (dealerAces == 0)
						{
							std::cout << "The dealer is bust, you win! \n";
							playState = 2;
						}
						// The dealer's score is over the max hand value, but they have an ace worth 11 which can be reduced to 1 so they are not bust
						else
						{
							--dealerAces;
							dealerTotal -= 10;
						}
					}
				}
			}

			if (playState != 2)
			{
				std::cout << "The dealer sticks. \n";

				// The point totals are evaluated and a winner decided
				if (playerTotal > dealerTotal)
					std::cout << "You win! \n";
				else if (dealerTotal > playerTotal)
					std::cout << "The dealer wins. \n";
				else
					std::cout << "It's a draw. \n";
			}

		}

		// The player is asked whether they want to play again
		std::cout << "Would you like to play again?" << '\n';
		playState = getInput();
		std::cout << '\n';
	}

	return 0;
}