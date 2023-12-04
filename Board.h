#ifndef Board_h
#define Board_h

#include"Side.h"

class Board
{
public:
	Board(int nHoles, int nInitialBeansPerHole);

	//own copy construtcor and destructor
	Board(const Board& other);
	~Board();


	int holes() const;
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const;
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);


private:
	int m_northPot;
	int m_southPot;
	int m_holes;
	int m_beaninhole;
	int* northHole; //array for north hole
	int* southHole; //array for south hole
	void sowHelper(Side s, int hole, int holding, Side calledby, Side& endSide, int& endHole);



};





#endif // !Board.h
