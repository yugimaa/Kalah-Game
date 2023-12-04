#ifndef Player_h
#define Player_h
#include<string>
#include<stack>

#include"Board.h"

class Player
{
public:
	Player(std::string name);
	std::string name() const;
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual ~Player();
private:
	std::string m_name;
};


class HumanPlayer : public Player
{
public:
	HumanPlayer(std::string name);
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player
{
public:
	BadPlayer(std::string name);
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer : public Player
{
public: 
	SmartPlayer(std::string name);
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const;
	
private:
	int chooseMoveHelper(Board& copy, Side s, int move, int depth, Side calledby) const;
	int move;
	int moveStrength;

	int bestHole, value;
	void bestMove(Board& copy, Side s, int depth, int& bestHole, int& value) const;

	int findLargestValue(std::stack<int>& possibleMoves) const;
	int findSmallestValue(std::stack<int>& possibleMoves) const;

	bool hasMoves(Side s, const Board& b) const;
};

#endif // !Player_h
