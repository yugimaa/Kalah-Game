#include"Player.h"
#include<iostream>
#include<stack>

using namespace std;

//base class
Player::Player(std::string name)
{
	m_name = name;
}

std::string Player::name() const
{
	return m_name;
}

bool Player::isInteractive() const
{
	return false; //default is false
}

Player::~Player()
{

}

//human player
HumanPlayer::HumanPlayer(string name) : Player(name)
{
}

bool HumanPlayer::isInteractive() const
{
	return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	bool hasMoves = false; //default is false
	
	for (int i = 1; i <= b.holes(); i++) //if there is at least one move, set has moves to true
	{
		if (b.beans(s, i) > 0)
		{
			hasMoves = true; //if there is at least one hole with beans, returns
			break;
		}
	}

	if (hasMoves == false) //if no moves, returns -1
	{
		return -1;
	}


	int input;
	cout << "Choose a hole, " << name() << ": ";
	cin >> input;

	cin.ignore(100, '\n'); //clear buffer

	if (input <= 0 || input > b.holes())
	{
		while (input <= 0 || input > b.holes())
		{
			cout << "The hole number must be from 1 to " << b.holes() << endl;
			cout << "Choose a hole, " << name() << ": ";
			cin >> input;
		}
	}
	return input;
}

//bad player
BadPlayer::BadPlayer(string name) : Player(name)
{
}

bool BadPlayer::isInteractive() const
{
	return false;
}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
	bool hasMoves = false; //default is false

	for (int i = 1; i <= b.holes(); i++) //if there is at least one move, set has moves to true
	{
		if (b.beans(s, i) != 0)
		{
			hasMoves = true; //if there is at least one hole with beans, returns
			break;
		}
	}

	if (hasMoves == false) //if no moves, returns -1
	{
		return -1;
	}

	srand(time(0)); //seeds as random time (every time program starts)

	int min = 1;
	int max = b.holes();

	int move = min + rand() % (max - min + 1);

	if (b.beans(s, move) == 0) //if move is zero
	{
		while (b.beans(s, move) == 0)
		{
			move = min + rand() % (max - min + 1);
		}
	}
	return move;
}


//smart player
SmartPlayer::SmartPlayer(string name) : Player(name)
{
}

bool SmartPlayer::isInteractive() const
{
	return false;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
	Board copy = b; // Create a copy of the original board

	bool hasMoves = false; //default is false

	for (int i = 1; i <= b.holes(); i++) //if there is at least one move, set has moves to true
	{
		if (b.beans(s, i) > 0)
		{
			hasMoves = true; //if there is at least one hole with beans, returns
			break;
		}
	}

	if (hasMoves == false) //if no moves, returns -1
	{
		return -1;
	}

	stack<int> moves;

	for (int i = 1; i <= b.holes(); i++)
	{
		if (b.beans(s, i) != 0)
		{
			moves.push(i); //push possible moves
		}
	}

	int size = moves.size();

	int* strengthArray = new int[size];
	int* moveArray = new int[size];

	for (int i = 0; i < size; i++)
	{
		int move = moves.top();
		moves.pop();
		moveArray[i] = move;
		strengthArray[i] = chooseMoveHelper(copy, s, move, 0, s);
	}

	//find strongest move (from strengthArray) and return the corresponding move
	int largestPos = 0; // Assume the first element is the largest
	int largest = strengthArray[0];

	// Iterate over the remaining elements
	for (int i = 1; i < size; i++)
	{
		if (strengthArray[i] > largest)
		{
			largest = strengthArray[i]; // Update the largest value
			largestPos = i; // Update the largest position
		}
	}
	
	int move = moveArray[largestPos];
	delete [] moveArray;
	delete [] strengthArray;

	return move;
}

int SmartPlayer::chooseMoveHelper(Board& copy, Side s, int move, int depth, Side calledby) const //this returns strength of move. 
{
	Side endSide, otherSide;
	int endHole;

	//determine the other side
	if (s == 0) //if north
	{
		otherSide = SOUTH;
	}
	else if (s == 1) //is south
	{
		otherSide = NORTH;
	}

	//create stack of other possible moves
	stack<int> possibleMoves;

	//first sow move
	copy.sow(s, move, endSide, endHole);

	//push every possible move for other side onto stack
	for (int i = 1; i <= copy.holes(); i++)
	{
		if (copy.beans(otherSide, i) > 0) //if there is a bean (possible move)
		{
			possibleMoves.push(i);
		}
	}

	//base cases
	if (depth >= 5)
	{
		int moveStrength = copy.beans(s, POT) - copy.beans(otherSide, POT);
		return moveStrength;
	}
	else if (possibleMoves.empty())
	{
		int moveStrength = copy.beans(s, POT) - copy.beans(otherSide, POT);
		return moveStrength;
	}


	if (s == calledby)
	{
		stack<int> strengthStack;
		while (!possibleMoves.empty()) //for every possible move, call itself to get strength
		{
			int move = possibleMoves.top();
			possibleMoves.pop();
			strengthStack.push(chooseMoveHelper(copy, otherSide, move, depth + 1, calledby));
		}
		return findLargestValue(strengthStack);
	}
	else if (s != calledby)
	{
		stack<int> strengthStack;
		while (!possibleMoves.empty()) //for every possible move, call itself to get strength
		{
			int move = possibleMoves.top();
			possibleMoves.pop();
			strengthStack.push(chooseMoveHelper(copy, otherSide, move, depth + 1, calledby));
		}
		return findSmallestValue(strengthStack);
	}
	return -1;
}

int SmartPlayer::findLargestValue(std::stack<int>& possibleMoves) const 
{
	int largest = possibleMoves.top();  // Assume the top element is the largest

	// Iterate through the stack to find the largest value


	stack<int> tempStack = possibleMoves; // Create a temporary copy of the stack
	while (!tempStack.empty()) {
		int current = tempStack.top();
		if (current > largest) {
			largest = current;
		}
		tempStack.pop();
	}

	return largest;
}

int SmartPlayer::findSmallestValue(std::stack<int>& possibleMoves) const
{
	int smallest = possibleMoves.top();  // Assume the top element is the largest

	// Iterate through the stack to find the largest value


	stack<int> tempStack = possibleMoves; // Create a temporary copy of the stack
	while (!tempStack.empty()) {
		int current = tempStack.top();
		if (current < smallest) {
			smallest = current;
		}
		tempStack.pop();
	}

	return smallest;
}
