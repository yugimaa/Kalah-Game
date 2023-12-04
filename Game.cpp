#include"Game.h"
#include"Board.h"
#include"Player.h"
#include<iostream>
using namespace std;


Game::Game(const Board& b, Player* south, Player* north)
{
	m_board = new Board(b); //creates a board for game with same number of holes and default 0;

	southPlayer = south;
	northPlayer = north;

	m_southplayer = south->name();
	m_northplayer = north->name();
}

Game::~Game() 
{
	delete m_board;
};

void Game::display() const
{
	cout << m_northplayer << endl;
	cout << "  ";

	for (int i = 1; i <= m_board->holes(); i++)
	{
		cout << m_board->beans(NORTH, i) << " ";
	}

	//north pot
	cout << endl << m_board->beans(NORTH, 0);

	//space
	cout << " ";
	for (int i = 0; i < m_board->holes(); i++)
	{
		cout << "  ";
	}

	//south pot
	cout << m_board->beans(SOUTH, POT) << endl;


	//south array (holes)
	cout << "  ";
	for (int i = 1; i <= m_board->holes(); i++)
	{
		cout << m_board->beans(SOUTH, i) << " ";
	}

	cout << endl << m_southplayer << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
	if (gameOver())
	{
		//cout << "Game Over." << endl;
		//compares the pots
		if (m_board->beans(SOUTH, 0) > m_board->beans(NORTH, 0)) //if south has more beans than north
		{
			hasWinner = true;
			winner = SOUTH;
		}
		else if (m_board->beans(SOUTH, 0) < m_board->beans(NORTH, 0)) //if north has more beans than south
		{
			hasWinner = true;
			winner = NORTH;
		}
		else if (m_board->beans(SOUTH, 0) == m_board->beans(NORTH, 0)) //if equal
		{
			hasWinner = false;
		}
		over = true;
	}
	else
	{
		over = false;
	}
	return;
}

bool Game::move(Side s)
{
	if (s == 0) //NORTH
	{
		//north move
		int move = northPlayer->chooseMove(*m_board, NORTH);

		if (move == -1)
		{
			//if no more moves, calls game over.
			gameOver();

			cout << m_northplayer << " has no more beans to sow. " << endl;
			cout << "Sweeping remaining beans into " << m_southplayer << "'s pot. " << endl;

			return false; //no more moves so game over
		}

		//if input is invalid, choose move till valid input
		while (m_board->sow(NORTH, move, endSide, endHole) == false)
		{
			cout << "There are no beans in that hole, " << m_northplayer << endl;
			move = northPlayer->chooseMove(*m_board, NORTH);
		}

		if (northPlayer->isInteractive() == false)
		{
			cout << m_northplayer << " chooses hole " << move << ". " << endl;
		}

		//check for ending in pot
		if (endHole == 0)
		{
			display();

			if (gameOver())
			{
				cout << m_northplayer << " has no more beans to sow. " << endl;
				cout << "Sweeping remaining beans into " << m_southplayer << "'s pot. " << endl;
				return false;
			}

			cout << northPlayer->name() << " takes another turn. " << endl;
			return Game::move(NORTH);
		}

		//check if empty hole
		if (m_board->beans(endSide, endHole) == 1) //if the endhole is 0, take the opposing side into pot
		{
			if (endSide == NORTH && m_board->beans(SOUTH, endHole)) //ends in north
			{
				m_board->moveToPot(SOUTH, endHole, NORTH);
				m_board->moveToPot(NORTH, endHole, NORTH);
			}
		}

		if (gameOver())
		{
			cout << m_northplayer << " has no more beans to sow. " << endl;
			cout << "Sweeping remaining beans into " << m_southplayer << "'s pot. " << endl;
			return false;
		}


	}
	else if (s == 1) //south
	{
		int move = southPlayer->chooseMove(*m_board, SOUTH);

		if (move == -1)
		{
			gameOver();

			cout << m_southplayer << " has no more beans to sow. " << endl;
			cout << "Sweeping remaining beans into " << m_northplayer << "'s pot. " << endl;

			return false; //game oveer
		}


		while (m_board->sow(SOUTH, move, endSide, endHole) == false)
		{
			cout << "There are no beans in that hole, " << m_southplayer << endl;
			move = southPlayer->chooseMove(*m_board, SOUTH);
		}

		if (southPlayer->isInteractive() == false)
		{
			cout << m_southplayer << " chooses hole " << move << ". " << endl;
		}

		//check for ending in pot
		if (endHole == 0)
		{
			display();
			if (gameOver())
			{
				cout << m_southplayer << " has no more beans to sow. " << endl;
				cout << "Sweeping remaining beans into " << m_northplayer << "'s pot. " << endl;
				return false;
			}
			cout << southPlayer->name() << " takes another turn. " << endl;
			return Game::move(SOUTH);
		}


		if (m_board->beans(endSide, endHole) == 1) //if the endhole is 0, take the opposing side into pot
		{
			if (endSide == SOUTH && m_board->beans(NORTH, endHole)) //ends in south and opposing side has beans
			{
				m_board->moveToPot(NORTH, endHole, SOUTH);
				m_board->moveToPot(SOUTH, endHole, SOUTH);
			}
		}

		if (gameOver())
		{
			cout << m_southplayer << " has no more beans to sow. " << endl;
			cout << "Sweeping remaining beans into " << m_northplayer << "'s pot. " << endl;
			return false;
		}
	}
	//checks if the game ends or not
	display();
	return true;
	//if (gameOver())
	//{
	//	return false;
	//}
	//else
	//{
	//	return true;
	//}
}

bool Game::gameOver()  const //compares both sides and checks if they have a move
{
	bool northHasNoMoves = true; //default is true;
	bool southHasNoMoves = true;

	//check North to see if at least one hole has a bean
	for (int i = 1; i <= m_board->holes(); i++)
	{
		if (m_board->beans(NORTH, i) > 0)
		{
			northHasNoMoves = false; //if there is at least one hole with beans, returns
			break;
		}
	}

	//check South to see if at least one hole has a bean
	for (int i = 1; i <= m_board->holes(); i++)
	{
		if (m_board->beans(SOUTH, i) > 0)
		{
			southHasNoMoves = false; //if there is at least one hole with beans, returns
			break;
		}
	}

	//if one of them has no moves, move to pot
	if (southHasNoMoves == true || northHasNoMoves == true)
	{
		if (northHasNoMoves == true) //North has no moves 
		{
			//go through every south hole and put in south pot
			for (int i = 1; i <= m_board->holes(); i++)
			{
				m_board->moveToPot(SOUTH, i, SOUTH);
			}
		}
		else if (southHasNoMoves == true) //south has no moves
		{
			//go through every north hole and put in north pot
			for (int i = 1; i <= m_board->holes(); i++)
			{
				m_board->moveToPot(NORTH, i, NORTH);
			}
		}
		return true;
	}
	else //both has moves
	{
		return false;
	}
}

void Game::play()
{
	display();
	status(over, hasWinner, winner); //sets over as false (start board always passes status)

	while (over == false)
	{
		move(SOUTH);
		status(over, hasWinner, winner);
		if (over)
		{
			if (hasWinner && winner == SOUTH)
			{
				display();

				cout << "The winner is " << m_southplayer << ".";
			}
			else if (hasWinner && winner == NORTH)
			{

				display();

				cout << "The winner is " << m_northplayer << ".";
			}
			else if (!hasWinner)
			{
				display();
				cout << "It's a tie!";
			}

			break;
		}
		move(NORTH);
		status(over, hasWinner, winner);
		if (over)
		{
			if (hasWinner && winner == SOUTH)
			{

				display();

				cout << "The winner is " << m_southplayer << ".";
			}
			else if (hasWinner && winner == NORTH)
			{
				display();

				cout << "The winner is " << m_northplayer << ".";
			}
			else if (!hasWinner)
			{
				display();
				cout << "It's a tie!";
			}

			break;
		}
	}
}

int Game::beans(Side s, int hole) const
{
	if (hole > m_board->holes() || hole < 0) //checks if valid
	{
		return -1;
	}
	else
	{
		return m_board->beans(s, hole);
	}
}

void Game::test()
{
	Side endSide;
	int endHole;

	m_board->sow(SOUTH, 0, endSide, endHole);
	display();

	m_board->sow(NORTH, 2, endSide, endHole);
	display();

	m_board->sow(SOUTH, 4, endSide, endHole);
	display();

	m_board->sow(SOUTH, 6, endSide, endHole);
	display();
	m_board->sow(SOUTH, 0, endSide, endHole);
	display();

}