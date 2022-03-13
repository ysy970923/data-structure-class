How to read output file test.txt
OS: Windows 10
Compiler: Visual Studio

After playing the game, test.txt will be made.
format: 
----------------------------------------------------------------
gamefield print
----------------------------------------------------------------

game moves print

----------------------------------------------------------------
gamefield print
----------------------------------------------------------------
game scores print
game result print

game field print
format:
stock pile: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)

waste pile: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)

output pile 0: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
output pile 1: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
output pile 2: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
output pile 3: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)

playing pile 0: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
how many open cards: (number of open cards in playing pile 0)
playing pile 1: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
how many open cards: (number of open cards in playing pile 1)
playing pile 2: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
how many open cards: (number of open cards in playing pile 2)
playing pile 3: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
how many open cards: (number of open cards in playing pile 3)
playing pile 4: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
how many open cards: (number of open cards in playing pile 4)
playing pile 5: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
how many open cards: (number of open cards in playing pile 5)
playing pile 6: (top) -> (all cards in this pile ordered from top to bottom) -> (bottom)
how many open cards: (number of open cards in playing pile 6)

example:
stock pile: (top) -> Clover 10 -> Clover 7 -> Spade 6 -> Clover 2 -> Clover 9 -> Heart 4 -> Clover Q -> Clover K -> Heart 10 -> Spade K -> Clover 6 -> Spade 8 -> Clover A -> Spade 10 -> Spade 9 -> Diamond 8 -> Diamond 4 -> Diamond A -> Heart A -> Heart 2 -> Spade J -> Clover 5 -> Spade 4 -> Clover 3 -> (bottom)
waste pile: (top) -> (bottom)
output pile 0: (top) -> (bottom)
output pile 1: (top) -> (bottom)
output pile 2: (top) -> (bottom)
output pile 3: (top) -> (bottom)
playing pile 0: (top) -> Heart 8 -> (bottom)
how many open cards: 1
playing pile 1: (top) -> Spade 3 -> Diamond 3 -> (bottom)
how many open cards: 1
playing pile 2: (top) -> Clover J -> Spade 2 -> Diamond 9 -> (bottom)
how many open cards: 1
playing pile 3: (top) -> Diamond 10 -> Spade 7 -> Spade 5 -> Heart Q -> (bottom)
how many open cards: 1
playing pile 4: (top) -> Diamond Q -> Diamond K -> Spade Q -> Diamond 2 -> Clover 8 -> (bottom)
how many open cards: 1
playing pile 5: (top) -> Heart 7 -> Diamond 7 -> Diamond 6 -> Diamond 5 -> Heart 5 -> Heart 6 -> (bottom)
how many open cards: 1
playing pile 6: (top) -> Heart J -> Spade A -> Heart 9 -> Heart 3 -> Diamond J -> Clover 4 -> Heart K -> (bottom)
how many open cards: 1

game moves print
format: (pile where) to (pile where) ( (card shape) (card number) )
example: 
	stock to waste ( Clover 10 )
	playing #0 to playing #2 ( Clover K ,  Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
		--> if multiple cards are moved, cards will be shown in top -> bottom order

game scores print
format: game score is (gameScore)points
example: game score is -12points

game result print
format: you (win or lose)
example: you lose


The overall output file will be as below

format: 
----------------------------------------------------------------
gamefield print
----------------------------------------------------------------

all game moves print

----------------------------------------------------------------
gamefield print
----------------------------------------------------------------
game scores print
game result print


example:
test.txt
-----------------------------------------------------------------
stock pile: (top) -> Clover 10 -> Clover 7 -> Spade 6 -> Clover 2 -> Clover 9 -> Heart 4 -> Clover Q -> Clover K -> Heart 10 -> Spade K -> Clover 6 -> Spade 8 -> Clover A -> Spade 10 -> Spade 9 -> Diamond 8 -> Diamond 4 -> Diamond A -> Heart A -> Heart 2 -> Spade J -> Clover 5 -> Spade 4 -> Clover 3 -> (bottom)
waste pile: (top) -> (bottom)
output pile 0: (top) -> (bottom)
output pile 1: (top) -> (bottom)
output pile 2: (top) -> (bottom)
output pile 3: (top) -> (bottom)
playing pile 0: (top) -> Heart 8 -> (bottom)
how many open cards: 1
playing pile 1: (top) -> Spade 3 -> Diamond 3 -> (bottom)
how many open cards: 1
playing pile 2: (top) -> Clover J -> Spade 2 -> Diamond 9 -> (bottom)
how many open cards: 1
playing pile 3: (top) -> Diamond 10 -> Spade 7 -> Spade 5 -> Heart Q -> (bottom)
how many open cards: 1
playing pile 4: (top) -> Diamond Q -> Diamond K -> Spade Q -> Diamond 2 -> Clover 8 -> (bottom)
how many open cards: 1
playing pile 5: (top) -> Heart 7 -> Diamond 7 -> Diamond 6 -> Diamond 5 -> Heart 5 -> Heart 6 -> (bottom)
how many open cards: 1
playing pile 6: (top) -> Heart J -> Spade A -> Heart 9 -> Heart 3 -> Diamond J -> Clover 4 -> Heart K -> (bottom)
how many open cards: 1
-----------------------------------------------------------------

stock to waste ( Clover 10 )
waste to playing #6 ( Clover 10 )
stock to waste ( Clover 7 )
waste to playing #0 ( Clover 7 )
stock to waste ( Spade 6 )
waste to playing #5 ( Spade 6 )
stock to waste ( Clover 2 )
playing #2 to playing #4 ( Clover J )
playing #3 to playing #4 ( Diamond 10 )
stock to waste ( Clover 9 )
waste to playing #4 ( Clover 9 )
playing #0 to playing #4 ( Heart 8 ,  Clover 7 )
stock to waste ( Heart 4 )
stock to waste ( Clover Q )
stock to waste ( Clover K )
playing #4 to playing #0 ( Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
waste to playing #4 ( Clover Q )
playing #6 to playing #4 ( Heart J ,  Clover 10 )
playing #6 to output #0 ( Spade A )
playing #2 to output #0 ( Spade 2 )
playing #1 to output #0 ( Spade 3 )
playing #2 to playing #4 ( Diamond 9 )
playing #0 to playing #2 ( Clover K ,  Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
stock to waste ( Heart 10 )
stock to waste ( Spade K )
playing #2 to playing #0 ( Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
stock to waste ( Clover 6 )
stock to waste ( Spade 8 )
waste to playing #4 ( Spade 8 )
playing #0 to playing #2 ( Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
stock to waste ( Clover A )
waste to output #3 ( Clover A )
playing #2 to playing #0 ( Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
stock to waste ( Spade 10 )
stock to waste ( Spade 9 )
stock to waste ( Diamond 8 )
stock to waste ( Diamond 4 )
stock to waste ( Diamond A )
waste to output #1 ( Diamond A )
playing #0 to playing #2 ( Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
stock to waste ( Heart A )
waste to output #2 ( Heart A )
playing #2 to playing #0 ( Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
stock to waste ( Heart 2 )
waste to output #2 ( Heart 2 )
playing #0 to playing #2 ( Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
stock to waste ( Spade J )
stock to waste ( Clover 5 )
stock to waste ( Spade 4 )
waste to output #0 ( Spade 4 )
playing #2 to playing #0 ( Diamond Q ,  Clover J ,  Diamond 10 ,  Clover 9 ,  Heart 8 ,  Clover 7 )
stock to waste ( Clover 3 )

-----------------------------------------------------------------
stock pile: (top) -> (bottom)
waste pile: (top) -> Clover 3 -> Clover 5 -> Spade J -> Diamond 4 -> Diamond 8 -> Spade 9 -> Spade 10 -> Clover 6 -> Heart 10 -> Heart 4 -> Clover 2 -> (bottom)
output pile 0: (top) -> Spade 4 -> Spade 3 -> Spade 2 -> Spade A -> (bottom)
output pile 1: (top) -> Diamond A -> (bottom)
output pile 2: (top) -> Heart 2 -> Heart A -> (bottom)
output pile 3: (top) -> Clover A -> (bottom)
playing pile 0: (top) -> Clover 7 -> Heart 8 -> Clover 9 -> Diamond 10 -> Clover J -> Diamond Q -> Spade K -> (bottom)
how many open cards: 7
playing pile 1: (top) -> Diamond 3 -> (bottom)
how many open cards: 1
playing pile 2: (top) -> Clover K -> (bottom)
how many open cards: 1
playing pile 3: (top) -> Spade 7 -> Spade 5 -> Heart Q -> (bottom)
how many open cards: 1
playing pile 4: (top) -> Spade 8 -> Diamond 9 -> Clover 10 -> Heart J -> Clover Q -> Diamond K -> Spade Q -> Diamond 2 -> Clover 8 -> (bottom)
how many open cards: 6
playing pile 5: (top) -> Spade 6 -> Heart 7 -> Diamond 7 -> Diamond 6 -> Diamond 5 -> Heart 5 -> Heart 6 -> (bottom)
how many open cards: 2
playing pile 6: (top) -> Heart 9 -> Heart 3 -> Diamond J -> Clover 4 -> Heart K -> (bottom)
how many open cards: 1
-----------------------------------------------------------------

game score is -12points
you lose