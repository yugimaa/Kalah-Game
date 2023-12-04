#ifndef Game_h
#define Game_h

#include"Board.h"
#include"Player.h"

class Game
{
public:
	Game(const Board& b, Player* south, Player* north);
	~Game();
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move(Side s);
	void play();
	int beans(Side s, int hole) const;


	//tesing
	void test();

private:
	Board* m_board;
	std::string m_southplayer;
	std::string m_northplayer;

	Player* southPlayer;
	Player* northPlayer;
	
	Side endSide;
	int endHole;

	bool over = true; //default is true, setting it to false is easier. 
	Side winner;
	bool hasWinner = false;

	bool gameOver() const; //checks for game over


};


#endif // !Game_h
