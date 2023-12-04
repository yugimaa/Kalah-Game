#include"Board.h"
#include"Game.h"
#include"Player.h"
#include<iostream>
#include<cassert>
using namespace std;


void doBoardTests()
{
	Board b(3, 2);
	assert(b.holes() == 3);

	assert(b.totalBeans() == 12);
	assert(b.beans(SOUTH, POT) == 0);
	assert(b.beansInPlay(SOUTH) == 6);

	b.setBeans(SOUTH, 1, 1);
	b.moveToPot(SOUTH, 2, SOUTH);
	assert(b.totalBeans() == 11);

	assert(b.beans(SOUTH, 1) == 1);
	assert(b.beans(SOUTH, 2) == 0);


	assert(b.beans(SOUTH, POT) == 2);
	assert(b.beansInPlay(SOUTH) == 3);
	Side es;
	int eh;
	b.sow(SOUTH, 3, es, eh);
	assert(es == NORTH && eh == 3 && b.beans(SOUTH, 3) == 0 && b.beans(NORTH, 3) == 3 && b.beans(SOUTH, POT) == 3 && b.beansInPlay(SOUTH) == 1 && b.beansInPlay(NORTH) == 7);
}

void doPlayerTests()
{
	HumanPlayer hp("Marge");
	assert(hp.name() == "Marge" && hp.isInteractive());
	BadPlayer bp("Homer");
	assert(bp.name() == "Homer" && !bp.isInteractive());
	SmartPlayer sp("Lisa");
	assert(sp.name() == "Lisa" && !sp.isInteractive());
	Board b(3, 2);
	b.setBeans(SOUTH, 2, 0);
	cout << "=========" << endl;
	int n = hp.chooseMove(b, SOUTH);
	cout << "=========" << endl;
	assert(n == 1 || n == 3);
	n = bp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
	n = sp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
}

void doGameTests()
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Board b(3, 0);
	b.setBeans(SOUTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	Game g(b, &bp1, &bp2);
	bool over;
	bool hasWinner;
	Side winner;
	//    Homer
	//   0  1  2
	// 0         0
	//   2  0  0
	//    Bart
	g.status(over, hasWinner, winner);
	assert(!over);
	assert(g.beans(NORTH, POT) == 0);
	assert(g.beans(SOUTH, POT) == 0);
	assert(g.beans(NORTH, 1) == 0);
	assert(g.beans(NORTH, 2) == 1);
	assert(g.beans(NORTH, 3) == 2);
	assert(g.beans(SOUTH, 1) == 2);
	assert(g.beans(SOUTH, 2) == 0);
	assert(g.beans(SOUTH, 3) == 0);
	g.move(SOUTH);
	//   0  1  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over);
	assert(g.beans(NORTH, POT) == 0);
	assert(g.beans(SOUTH, POT) == 3);
	assert(g.beans(NORTH, 1) == 0);
	assert(g.beans(NORTH, 2) == 1);
	assert(g.beans(NORTH, 3) == 0);
	assert(g.beans(SOUTH, 1) == 0);
	assert(g.beans(SOUTH, 2) == 1);  
	assert(g.beans(SOUTH, 3) == 0);

	g.move(NORTH);
	//   1  0  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move(SOUTH);
	//   1  0  0
	// 0         3
	//   0  0  1
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

	g.move(NORTH);
	//   0  0  0
	// 1         4
	//   0  0  0
	g.status(over, hasWinner, winner);
	assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
	assert(hasWinner && winner == SOUTH);
}

void test()
{
	HumanPlayer player1("Human");
	BadPlayer player2("Jason");
	SmartPlayer player3("Fish");
	SmartPlayer player4("Guillermo");

	Board b1(3, 0); //creates emtpy board
	b1.setBeans(NORTH, 1, 2);
	b1.setBeans(NORTH, 2, 2);
	b1.setBeans(NORTH, 3, 2);
	b1.setBeans(SOUTH, 3, 1); //sets board as one bean on right side


	//tests end game scenairo for noninteractive  players
	Game test1(b1, &player2, &player4);
	test1.play(); //should end game with player3 winning
	Game test2(b1, &player3, &player4);
	test2.play(); //shoud end game with player 4 winning 

	//tests capture should result in two tie games
	Board b2(3, 0);
	b2.setBeans(NORTH, 3, 2);
	b2.setBeans(SOUTH, 1, 1);
	b2.setBeans(NORTH, 2, 1);
	Game test3(b2, &player2, &player4);
	test3.play();
	Game test4(b2, &player3, &player4);
	test4.play();

	//test for behavior of smartplayer (mine does not do this.. :( )
	//sets up board as shown in spec
	Board b3(6, 0);
	b3.setBeans(NORTH, POT, 22);
	b3.setBeans(SOUTH, POT, 20);
	b3.setBeans(NORTH, 2, 1);
	b3.setBeans(NORTH, 5, 2);
	b3.setBeans(SOUTH, 3, 2);
	b3.setBeans(SOUTH, 5, 1);
	Game test5(b3, &player3, &player1);
	test5.display();
	test5.play(); //this should cause smartplayer to chose hole 3 as it is the more optimal move
}


int main()
{
	Board b(6, 4);
	HumanPlayer player1("Player 1");
	HumanPlayer player2("Player 2");

	//BadPlayer player3("Jason");
	//BadPlayer player4("Guillermo");

	//SmartPlayer player5("Yugo");
	//SmartPlayer player6("Su Bingtang");

	Game g(b, &player1, &player2);
	g.play();

	//doBoardTests();
	//doPlayerTests();

	//doGameTests();
	
	
	//test();
	cout << "Passed all tests" << endl;

}