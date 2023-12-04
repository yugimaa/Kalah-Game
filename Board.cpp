#include"Board.h"
#include"Side.h"
#include<iostream>
using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole)
{

	//sets m_holes as specified
	m_holes = nHoles;

	//set each pots as 0
	m_northPot = 0;
	m_southPot = 0;

	//sets each hole as indicated
	m_beaninhole = nInitialBeansPerHole;

	//sets two sets of array for the holes
	northHole = new int[nHoles + 1];
	southHole = new int[nHoles + 1];


	//sets the pot (array 0) of each side as 0;
	northHole[0] = m_northPot;
	southHole[0] = m_southPot;

	//sets each hole with specified amount of beans 
	for (int i = 1; i <= nHoles; i++)
	{
		northHole[i] = m_beaninhole;
		southHole[i] = m_beaninhole;
	}
}

// Deep copy constructor
Board::Board(const Board& other) {
	m_holes = other.m_holes;
	m_beaninhole = other.m_beaninhole;
	m_northPot = other.m_northPot;
	m_southPot = other.m_southPot;

	// Allocate new memory for northHole and southHole arrays
	northHole = new int[m_holes + 1];
	southHole = new int[m_holes + 1];

	// Copy the contents from the source object to the new memory
	for (int i = 0; i <= m_holes; i++) {
		northHole[i] = other.northHole[i];
		southHole[i] = other.southHole[i];
	}
}

// Destructor
Board::~Board() 
{
	delete[] northHole;
	delete[] southHole;
}

int Board::holes() const
{
	return m_holes;
}

int Board::beans(Side s, int hole) const
{
	if (hole > m_holes || hole < 0) //checks if valid
	{
		return -1;
	}
	else
	{
		if (s == 0) //north
		{
			return northHole[hole];
		}
		else if (s == 1) //south
		{
			if (hole == 0)
			{
				return southHole[0];
			}
			else
			{
				int arrayHole = (holes() + 1) - hole;
				return southHole[arrayHole];
			}
		}
	}
	return -1;
}

int Board::beansInPlay(Side s) const
{
	int numBeans = 0;
	if (s == 0)//north
	{
		for (int i = 1; i <= m_holes; i++)
		{
			int beanInHole = northHole[i];
			numBeans += beanInHole;
		}
	}
	else if (s == 1) //south
	{
		for (int i = 1; i <= m_holes; i++)
		{
			int beanInHole = southHole[i];
			numBeans += beanInHole;
		}
	}
	return numBeans;
}

int Board::totalBeans() const
{
	int numBeans = 0;
	for (int i = 0; i <= m_holes; i++)
	{
		int beanInHole = northHole[i];
		numBeans += beanInHole;
	}
	for (int i = 0; i <= m_holes; i++)
	{
		int beanInHole = southHole[i];
		numBeans += beanInHole;
	}
	return numBeans;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)  //converts to array holes. can pass player hole
{
	if ((s == 0 || s == 1) && (0 < hole && hole <= holes())) //if s is valid and hole is valid, and there are beans in hole
	{
		if (s == 0) //NORTH
		{
			if (beans(s, hole) == 0)
			{
				return false;
			}
			else
			{
				int holding = beans(s, hole); //"Hold" the beans
				northHole[hole] = 0;
				sowHelper(s, hole, holding, s, endSide, endHole);
			}
		}
		else if (s == 1) //SOUTH
		{
			//check if correct hole has no beans
			if (beans(s, hole) == 0)
			{
				return false;
			}
			else
			{
				int holding = beans(s, hole); //"Hold" the beans

				int arrayHole = (holes() + 1) - hole;

				southHole[arrayHole] = 0;
				sowHelper(s, arrayHole, holding, s, endSide, endHole);
			}
		}
		//cout << endSide << ", " << endHole << endl;
		return true;
	}
	else
	{
		return false;
	}
} 

void Board::sowHelper(Side s, int hole, int holding, Side calledby, Side& endSide, int& endHole) //called by sow so takes in arrayHole. Returns player hole
{

	if (holding == 0)
	{
		if (s == 1) //south //this parameter is important, reverts back to player hole
		{
			int newHole = holes() - (hole - 1);
			hole = newHole;
		}

		endHole = hole;  // Update endHole with the current hole
		endSide = s;     // Update endSide with the current side

		return;
	}

	hole--; //increments hole
	if (s == 0) //NORTH
	{
		if (hole == 0) //if reached pot
		{
			if (s == calledby) //same side as the caller 
			{
				northHole[hole] = northHole[hole] + 1;


				if (holding - 1 == 0) //edge case, if last bean is in pot, dont flip side (?)
				{
					sowHelper(s, hole, holding - 1, calledby, endSide, endHole); //switch sides and recusion
				}
				else
				{
					hole = holes() + 1;
					sowHelper(SOUTH, hole, holding - 1, calledby, endSide, endHole); //switch sides and recusion
				}
			}
			else //other side of caller, skip
			{
				hole = holes() + 1; //reset hole to amount of holes
				sowHelper(SOUTH, hole, holding, calledby, endSide, endHole); //switch sides and recusion
			}
		}
		else
		{
			//if not pot
			northHole[hole] = northHole[hole] + 1; //increase bean in hole by 1
			sowHelper(s, hole, holding - 1, calledby, endSide, endHole); //call again for next hole
		}
	}

	else if (s == 1) //SOUTH
	{
		if (hole == 0) //if reached pot
		{
			if (s == calledby) //same side as the caller 
			{
				southHole[hole] = southHole[hole] + 1;
				hole = holes() + 1;
				if (holding - 1 == 0) //edge case, if last bean is in pot, dont flip side (?)
				{
					sowHelper(s, hole, holding - 1, calledby, endSide, endHole); //switch sides and recusion
				}
				else
				{
					sowHelper(NORTH, hole, holding - 1, calledby, endSide, endHole); //switch sides and recusion
				}
			}
			else //other side of caller, skip
			{
				hole = holes() + 1; //reset hole to amount of holes
				sowHelper(NORTH, hole, holding, calledby, endSide, endHole); //switch sides and recusion
			}
		}
		else
		{
			//if not pot
			southHole[hole] = southHole[hole] + 1; //increase bean in hole by 1
			sowHelper(s, hole, holding - 1, calledby, endSide, endHole); //call again for next hole
		}
	}

}

bool Board::moveToPot(Side s, int hole, Side potOwner) //takes in player hole
{
	if (hole < 0 || hole>holes()) //invalid hole
	{
		return false;
	}
	else
	{
		int holding;

		if (s == 0) //NORTH
		{
			//hold beans and clear hole
			holding = northHole[hole];
			northHole[hole] = 0;

			//set the hole that captured it to zero 
			//int arrayHole = (holes() + 1) - hole;
			//capture = southHole[arrayHole];
			//southHole[arrayHole] = 0;
		}
		else if (s == 1) //SOUTH
		{
			//reverse the hole number
			//flip the input to set proper index
			int arrayHole = (holes() + 1) - hole;


			//hold beans and clear hole
			holding = southHole[arrayHole];
			southHole[arrayHole] = 0;

			//capture = northHole[hole];
			//northHole[hole] = 0;
		}

		//sort into pot
		if (potOwner == 0) //pot owner is North player
		{
			northHole[0] += holding;
		}
		else if (potOwner == 1) //pot owner is South player
		{
			southHole[0] += holding;
		}
		return true;
	}
}

bool Board::setBeans(Side s, int hole, int beans) //takes in player hole
{
	if (hole > holes() || hole < 0)
	{
		return false;
	}
	else if (beans < 0)
	{
		return false;
	}
	else
	{
		if (s == 0) // north
		{
			northHole[hole] = beans;
		}
		else if (s == 1) //south
		{
			if (hole == 0)
			{
				southHole[0] = beans;
			}
			else
			{
				int arrayHole = holes() - (hole - 1);
				southHole[arrayHole] = beans;
			}

		}
		return true;
	}
}