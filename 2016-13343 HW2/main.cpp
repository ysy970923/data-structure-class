#include <iostream>
#include <random>
#include <string>
#include <fstream>

using namespace std;

//file to print output
string filePath = "test.txt";
ofstream writeFile(filePath.data());

//enum class for 4 card shapes
enum class Shape { DIAMOND, CLOVER, SPADE, HEART };

//Card class : class representing card by shape and number of the card
class Card {
public:
	Card();
	Card(Shape shape, int number);
	bool isKing();
	bool sameShapeMinusOneIs(Card& card);
	bool differentColorPlusOneIs(Card& card);
	string cardInfoString();

private:
	int number;
	Shape shape;
};

//if the card is not set by card data, make the card number -1 to show the card does not have proper data
Card::Card()
{
	this->number = -1;
	this->shape = Shape::SPADE;
}

//card set by card data(shape, number)
Card::Card(Shape shape, int number)
{
	this->number = number;
	this->shape = shape;
}

//check if the card number is King
//used for solitaire game: only King can be moved to empty playing pile
bool Card::isKing() 
{
	return this->number == 13;
}

//check if the card comparing is same shape but 1 smaller number than this card
//used for solitaire game: only when the output pile's top card is same shape but 1 smaller than this card, than the this card can be moved to output pile
//example: if cardA.sameShapeMinusOneIs(cardB) == true;
//				means, cardA.number - 1 == cardB.number && same shape
//				--> cardA can move to output pile where cardB is
bool Card::sameShapeMinusOneIs(Card& card)
{
	return (this->number - 1 == card.number) && (this->shape == card.shape);
}

//check if the card comparing is different color but 1 bigger number than this card
//used for solitaire game: only when the playing pile's top card is different color but 1 bigger than this card, than the this card can be moved to playing pile
//example: if cardA.differentColorPlusOneIs(cardB) == true;
//				means, cardA.number + 1 == cardB.number && different color(diamond, heart: red // clover, spade: black)
//				--> cardA can move to playing pile where cardB is
bool Card::differentColorPlusOneIs(Card& card)
{
	bool returnVal;
	if (card.shape == Shape::SPADE || card.shape == Shape::CLOVER) {
		returnVal = this->shape == Shape::DIAMOND || this->shape == Shape::HEART;
	}
	else {
		returnVal = this->shape == Shape::SPADE || this->shape == Shape::CLOVER;
	}
	returnVal = returnVal && (this->number + 1 == card.number);
	return returnVal;
}

//return card's information in string
//format: Shape number
//if number is alphabet(A,J,Q,K), string contains alphabet
//example: Spade A
string Card::cardInfoString() {
	string numberString;
	if (2 <= this->number && this->number <= 10)
		numberString = to_string(this->number);
	else if (this->number == 1)
		numberString = "A";
	else if (this->number == 11)
		numberString = "J";
	else if (this->number == 12)
		numberString = "Q";
	else if (this->number == 13)
		numberString = "K";
	else
		numberString = "invalid Number";

	if (this->shape == Shape::SPADE) {
		return " Spade " + numberString + " ";
	}
	else if (this->shape == Shape::DIAMOND) {
		return " Diamond " + numberString + " ";
	}
	else if (this->shape == Shape::HEART) {
		return " Heart " + numberString + " ";
	}
	else {
		return " Clover " + numberString + " ";
	}
}


//CardNode class: for CardPile class, CardNode has data of Card(card) and pointer of the CardNode(link) below this Card
class CardNode {
public:
	friend class CardPile;
	CardNode(Card& card, CardNode* link = 0):card(card),link(link)
	{
	}
private:
	Card card;
	CardNode* link;
};


//CardPile class: Stack data structure(First in First out) & LinkedList function
//has Stack's basic functions: isEmpty, Top, Push, Pop
//has linkedList functions: elementByIndex ==> used when finding playing pile's card to move to other playing pile in solitaire, compare the card data without popping it
class CardPile {
public:
	CardPile();

	~CardPile();
	
	bool isEmpty() const;

	Card& Top() const;

	void Push(Card card);

	void Pop();

	Card& elementByIndex(int index) const;

	string cardPileString();

private:
	CardNode* top;
};

//constructor to make empty card pile
CardPile::CardPile()
{
	top = 0;
}

//destructor
//delete all the nodes from top to bottom
CardPile::~CardPile()
{
	CardNode* delNode = top;
	CardNode* nextDelNode;
	//delete until delNode hits bottom
	while (delNode != 0) {
		nextDelNode = delNode->link;
		delete delNode;
		delNode = nextDelNode;
	}
}

//return true when CardPile is empty
bool CardPile::isEmpty() const{
	return top == 0;
}

//return Top card data
Card& CardPile::Top() const {
	return top->card;
}

//place card on Top
void CardPile::Push(Card card) {
	//make new card top and old top new card top's link
	top = new CardNode(card, top);
}

//delete card on Top
void CardPile::Pop() {
	//make the second card top and delete the old top one
	CardNode* delNode = top;
	top = top->link;
	delete delNode;
}

//get the card data in card pile's index place(top index == 0)
Card& CardPile::elementByIndex(int index) const{
	//iterate from top node to below card index times
	CardNode* cardNode = top;
	for (int i = 0; i < index; i++) {
		cardNode = cardNode->link;
	}
	return cardNode->card;
}

//return card pile's information string
//shows each card information from top to bottom
//example: (top) ->Spade A->Spade 2-> (bottom)
string CardPile::cardPileString() {
	string returnString;
	returnString = "(top) ->";
	CardNode* printingNode = top;
	while (printingNode != 0) {
		returnString += printingNode->card.cardInfoString() + "->";
		printingNode = printingNode->link;
	}
	returnString += " (bottom)";
	return returnString;
}

//Solitaire game class
class Solitaire {

//variables
//	cardPiles: 7 playing piles, 1 waste pile, 1 stock, 4 output piles, all dynamic allocated pointer to invoke card node's correct deletion
//	howManyCardsOpen: size 7 array for how many cards are open on each playing piles
//	gameScore: check game score, start from -52, +5 for each card sent to output piles
//	shapes: size 4 array representing each output pile's shape
private:
	CardPile* playingPiles;
	CardPile* wastePile;
	CardPile* stock;
	CardPile* outputPiles;
	int howManyCardsOpen[7];
	int gameScore;
	Shape shapes[4] = { Shape::SPADE, Shape::DIAMOND, Shape::HEART, Shape::CLOVER };

	void shuffle(Card* deck, int length);

	void initialize();

	void moveCard(CardPile& from, CardPile& to);

	void moveCardsBetweenPlayingPiles(int fromIndex, int toIndex, int howMany);

	void tapStock();

	bool sendToOutputPileIfCan();

	bool wasteToPlayingIfCan();

	bool playingToPlayingIfCan(int* lastplayingToPlaying);

	void printGameField();

public:
	Solitaire();

	~Solitaire();

	int play();
};

//constructor
//dynamic allocation for each card pile to empty card pile
//open one card for each playing piles
//set gamescore to -52
Solitaire::Solitaire() {
	playingPiles = new CardPile[7];
	outputPiles = new CardPile[4];
	wastePile = new CardPile();
	stock = new CardPile();
	//open one card for each playing piles
	for (int i = 0; i < 7; i++) {
		howManyCardsOpen[i] = 1;
	}
	gameScore = -52;
}

//destructor
//delete all card piles --> invoke the card pile destructor which will delete all node
Solitaire::~Solitaire() {
	delete[] playingPiles;
	delete[] outputPiles;
	delete wastePile;
	delete stock;

}

//function for randomly shuffling the deck
//used when initalizing the game field
//swaps randomly two card from the deck deck size times
void Solitaire::shuffle(Card* deck, int length) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, length - 1);
	Card temp;
	int shuffleCard1;
	int shuffleCard2;
	for (int i = 0; i < length; i++) {
		shuffleCard1 = dis(gen);
		shuffleCard2 = dis(gen);
		temp = deck[shuffleCard1];
		deck[shuffleCard1] = deck[shuffleCard2];
		deck[shuffleCard2] = temp;
	}
}

//initialize game
//make empty card piles by deleting and reallocating the card piles
//shuffle card deck and distribute to each card pile
void Solitaire::initialize() {
	//make all card piles empty by deleting and reallocating
	delete[] playingPiles;
	delete[] outputPiles;
	delete wastePile;
	delete stock;
	playingPiles = new CardPile[7];
	outputPiles = new CardPile[4];
	wastePile = new CardPile();
	stock = new CardPile();

	//before placing cards make size 52 deck
	Card cardDeck[52];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			cardDeck[i * 13 + j] = Card(shapes[i], j + 1);
		}
	}
	//shuffle card deck
	shuffle(cardDeck, 52);
	//place card in order to playing piles and stock
	//playing piles have 1~7 cards in order and stock has 24 cards left
	int k = 0;
	for (int j = 0; j < 7; j++) {
		for (int i = 0; i < (j + 1); i++) {
			playingPiles[j].Push(cardDeck[k]);
			k++;
		}
	}
	for (int i = 0; i < 24; i++) {
		stock->Push(cardDeck[k]);
		k++;
	}
	//open on card for each playing piles
	for (int i = 0; i < 7; i++) {
		howManyCardsOpen[i] = 1;
	}
	//initialize game score to -52
	gameScore = -52;
}

//move card from one card pile to other card pile
void Solitaire::moveCard(CardPile& from, CardPile& to) {
	//pop top card from 'from' card pile and push to 'to' card pile
	Card topCard = from.Top();
	from.Pop();
	to.Push(topCard);
}

//function for moving multiple cards between playing piles
//playingPile[fromIndex] to playingPile[toIndex], move 'howMany' cards
void Solitaire::moveCardsBetweenPlayingPiles(int fromIndex, int toIndex, int howMany) {
	//log when this move is made: example) playing #1 to playing #2 (Spade 2, Heart A)
	string moveInfoString = "playing #" + to_string(fromIndex) + " to playing #" + to_string(toIndex) + " (";
	//to move multiple cards in same order, pop and push cards one by one to temp card pile than pop and push cards from temp card pile one by one
	CardPile temp = CardPile();
	for (int i = 0; i < howMany; i++) {
		moveCard(playingPiles[fromIndex], temp);
	}
	for (int i = 0; i < howMany; i++) {
		//add card information moving to information string
		moveInfoString += temp.Top().cardInfoString();
		if (i != howMany - 1)moveInfoString += ", ";
		moveCard(temp, playingPiles[toIndex]);
	}
	//since all moved cards were open cards, we have to add and minus the number of open cards in each playing piles
	howManyCardsOpen[fromIndex] -= howMany;
	howManyCardsOpen[toIndex] += howMany;
	//when all open cards in one card pile are moved, if the pile is not empty open one top card, if empty leave to 0
	if (howManyCardsOpen[fromIndex] == 0 && !playingPiles[fromIndex].isEmpty())
		howManyCardsOpen[fromIndex] = 1;
	moveInfoString += ")";
	//log on file
	writeFile << moveInfoString + "\n";
}

//function for tapping the stock
// move top card from stock to waste
void Solitaire::tapStock() {
	//log when this move is made: example) stock to waste (Spade A)
	string moveInfoString = "stock to waste (";
	moveInfoString += stock->Top().cardInfoString();
	moveCard(*stock, *wastePile);
	moveInfoString += ")";
	writeFile << moveInfoString + "\n";
}

//return true if move is made
//send from waste or playing pile to output file if can
//can move when output pile's top card is same shape but one number less
//check waste pile first, then check playing piles from left to right
//if moved to output pile, add 5 to game score
//log if move made
bool Solitaire::sendToOutputPileIfCan() {
	string moveInfoString = "";
	for (int i = 0; i < 4; i++) {
		//if output pile is empty make the top card as like if it's number is 0 to compare and move As: Spade A to empty card
		//if not empty set output pile's top card data
		Card outPutPileTop;
		if (outputPiles[i].isEmpty()) outPutPileTop = Card(shapes[i], 0);
		else outPutPileTop = outputPiles[i].Top();

		//if not empty move waste pile to output pile if can
		//if output pile's top card is same shape but one number less, move
		//when moved, make log: example) waste to output #1 (Spade A)
		//add 5 points to game score
		if (!wastePile->isEmpty())
			if (wastePile->Top().sameShapeMinusOneIs(outPutPileTop)) {
				moveInfoString += "waste to output #" + to_string(i) + " (";
				moveInfoString += wastePile->Top().cardInfoString();
				moveInfoString += ")";
				writeFile << moveInfoString + "\n";
				moveCard(*wastePile, outputPiles[i]);
				gameScore += 5;
				return true;

			}

		//if not empty move playing pile to output pile if can
		//if output pile's top card is same shape but one number less, move
		//when moved, make log: example) playing #1 to output #1 (Spade A)
		//add 5 points to game score
		for (int j = 0; j < 7; j++) {
			if (!playingPiles[j].isEmpty())
				if (playingPiles[j].Top().sameShapeMinusOneIs(outPutPileTop)) {
					moveInfoString += "playing #" + to_string(j) + " to output #" + to_string(i) + " (";
					moveInfoString += playingPiles[j].Top().cardInfoString();
					moveInfoString += ")";
					writeFile << moveInfoString + "\n";
					moveCard(playingPiles[j], outputPiles[i]);
					howManyCardsOpen[j]--;
					//when all open cards in card pile are moved, if the pile is not empty open one top card, if empty leave to 0
					if (howManyCardsOpen[j] == 0 && !playingPiles[j].isEmpty())
						howManyCardsOpen[j] = 1;
					gameScore += 5;
					return true;
				}
		}

	}
	return false;
}

//return true if move is made
//send from waste to playing pile(leftmost) if can
//can move when playing pile's top card is different color but one number bigger
//log if move made: example) waste to playing #1 (Spade A)
bool Solitaire::wasteToPlayingIfCan() {
	string moveInfoString = "";
	//if waste pile is empty, tap stock and begin
	//if waste and stock is all empty, no move can be made further
	if (wastePile->isEmpty()) {
		if (stock->isEmpty())
			return false;
		tapStock();
	}

	//if can, move from waste pile to playing pile
	//if waste pile top card is King, find empty playing pile then move
	//if moved add one to howManyCardsOpen because moved card is open too
	Card wastePileTop = wastePile->Top();
	for (int i = 0; i < 7; i++) {
		if (playingPiles[i].isEmpty()) {
			if (wastePileTop.isKing()) {
				moveInfoString += "waste to playing #" + to_string(i) + " (";
				moveInfoString += wastePileTop.cardInfoString();
				moveInfoString += ")";
				moveCard(*wastePile, playingPiles[i]);
				howManyCardsOpen[i]++;
				return true;
			}
		}
		else if (wastePileTop.differentColorPlusOneIs(playingPiles[i].Top())) {
			moveInfoString += "waste to playing #" + to_string(i) + " (";
			moveInfoString += wastePileTop.cardInfoString();
			moveInfoString += ")";
			writeFile << moveInfoString + "\n";
			moveCard(*wastePile, playingPiles[i]);
			howManyCardsOpen[i]++;
			return true;
		}
	}
	return false;
}

//return true if move is made
//find stack of multiple cards and move between playing piles if can
//to avoid infinite loop: make lastPlayingToPlaying array remember the move making and compare next time the playing to playing move is made
//	check if the move is the reverse move of lastMove and don't move if it is
//log if move made: example) playing #2 to playing #1 (Spade A, Heart 2)
bool Solitaire::playingToPlayingIfCan(int* lastplayingToPlaying) {

	//check all open cards from left to right playing piles
	//King can move to empty playing pile
	//cards can move below the card which is different color but one bigger(Spade A can move below Heart 2)
	//if a card can move all the cards below has to move with the card
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < howManyCardsOpen[i]; j++) {
			Card cardChecking = playingPiles[i].elementByIndex(j);
			for (int k = 0; k < 7; k++) {
				if (k != i) {
					if (playingPiles[k].isEmpty()) {
						if (cardChecking.isKing()) {
							//check if this move is a reverse move of the move made last time
							if (lastplayingToPlaying[0] == k && lastplayingToPlaying[1] == i) {
								return false;
							}
							//move j+1 cards from playing pile i to playing pile j
							moveCardsBetweenPlayingPiles(i, k, j + 1);
							//update lastplayingToPlaying remeber the move
							lastplayingToPlaying[0] = i;
							lastplayingToPlaying[1] = k;
							return true;
						}
					}
					else if ((k != i) && (cardChecking.differentColorPlusOneIs(playingPiles[k].Top()))) {
						//check if this move is a reverse move of the move made last time
						if (lastplayingToPlaying[0] == k && lastplayingToPlaying[1] == i) {
							return false;
						}
						//move j+1 cards from playing pile i to playing pile j
						moveCardsBetweenPlayingPiles(i, k, j + 1);
						//update lastplayingToPlaying remeber the move
						lastplayingToPlaying[0] = i;
						lastplayingToPlaying[1] = k;
						return true;
					}

				}

			}
		}
	}
	return false;
}


//function for printing log of game field
//how to read is in the README.txt file
void Solitaire::printGameField() {
	string printString = "\n-----------------------------------------------------------------\n";
	printString += "stock pile: ";
	printString += stock->cardPileString() + "\n\n";
	printString += "waste pile: ";
	printString += wastePile->cardPileString() + "\n\n";
	for (int i = 0; i < 4; i++) {
		printString += "output pile " + to_string(i) + ": ";
		printString += outputPiles[i].cardPileString() + "\n";
	}
	printString += "\n";
	for (int i = 0; i < 7; i++) {
		printString += "playing pile " + to_string(i) + ": ";
		printString += playingPiles[i].cardPileString() + "\n";
		printString += "how many open cards: ";
		printString += to_string(howManyCardsOpen[i]) + "\n";
	}
	printString += "-----------------------------------------------------------------\n\n";
	writeFile << printString;
}


//playing solitaire game, return game score
//initialize game field -> print the game field to output file
//	-> send from waste or playing piles to output pile if can
//	-> send from waste pile to playing piles if can
//	-> send from playing to playing piles card stack if can
//-> repeat the 3 functions, if any move is made, start from sendToOutputPileIfCan
//-> if no move is made for all 3 functions, tap the stock and repeat again, if the stock is empty game is over
//-> print the game field done to output file
int Solitaire::play() {
	initialize();
	printGameField();

	//checking if any move is made for starting from the first step again
	bool isMoveMade = true;
	//to avoid infinite loop: array to remember the last move for playing to playing moves
	int lastPlayingToPlaying[2] = {};

	//repeat until break(empty stock & no move to make)
	while (true) {
		//if any move is made, start from here
		if (isMoveMade) {
			isMoveMade = false;
			isMoveMade = sendToOutputPileIfCan();
		}

		//if any move is made, pass below and start from sendToOutputIfCan again
		if (!isMoveMade)
			isMoveMade = wasteToPlayingIfCan();

		//if any move is made from top 2 functions, the last move is not playing to playing, so make lastPlayingToPlaying[0] = -1 to avoid mischeck
		if (isMoveMade)
			lastPlayingToPlaying[0] = -1;

		//if any move is made, pass below and start from sendToOutputIfCan again
		if (!isMoveMade)
			isMoveMade = playingToPlayingIfCan(lastPlayingToPlaying);

		//if no move were made until here, check stock and tap if not empty, if empty, stop game
		if (!isMoveMade)
		{
			if (stock->isEmpty()) break;
			tapStock();
			isMoveMade = true;
		}
	}
	printGameField();
	writeFile << "game score is " + to_string(gameScore) +"points\n";
	if (gameScore == 208)
		writeFile << "you win";
	else
		writeFile << "you lose";
	return gameScore;
}

int  main() {
	//play solitaire
	Solitaire solitaire = Solitaire();
	solitaire.play();
	//test.txt will be made, containing game data
	return 0;
}
