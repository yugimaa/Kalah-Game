# kalah
Kalah is a modern variation in the ancient Mancala family of games. I created this kalah games with one and two player modes. 

Design:
For this project, I mostly used arrays to implement the Kalah game. Additionally, I also used stacks to implement the smart player function. For the interface/display of the Kalah game itself, I used an array to represent the holes and the pots. To make the implementation of the sow function easier, I formatted each hole with the corresponding array as shown below. 

 1 2 3 4 5 6
0            0
 6 5 4 3 2 1

For the Board class, I added a code for conversion (int arrayHole = (holes() + 1) - hole;) every time a south hole was called. This allowed for the implementation of other classes to be easy despite having a reversed hole order. This ordering of the holes also allowed me to implement the sow function using recursion as the array numbers decreased counterclockwise. I used a recursive helper function that was called through sow, which correctly used the passed parameter to sow beans into place, ignoring the opponents pot. 

As for the Game class, I created a private bool function called gameOver(); that compares the amount of beans in the holes on both sides to see if any moves are possible. If not, the function returns true. This was useful in implementing the other public functions that were reliant on knowing whether or not the game was over. Because it is a boolean function, I was able to use it as a parameter for if statements in status(); and move();. 

Finally, for the Player class, I used multiple while loops to get the correct input for each move. This was especially useful for the HumanPlayer subclass as it allowed the program to repeatedly ask for a new input if a wrong input was provided. For the BadPlayer subclass, a random number generator (based on time) was used to select holes by random. Again, a while  loop was used to make sure the randomly generated hole number had at least one bean. No private functions were used for the implementation of HumanPlayer and BadPlayer. 


SmartPlayer
For the SmartPlayer implementation, I used stacks in an attempt to implement the chooseMove function. Although I initially started off using the pseudo code provided in the specs, I could not figure out how to implement it in the given way. Because of this, I attempted to implement it a different way following the tree logic described in the spec. 

I used three helper functions to implement the chooseMove function. The chooseMove function itself, checks for the number of moves that the player can make from the given board. Each possible hole is added to the moves stack using a for loop. I then used two dynamically allocated arrays to hold each move and the strength value of each move by popping the moves stack and passing the value to the chooseMoveHelper function. 

The chooseMoveHelper function returns the strength of the move through recursively calling itself until it reaches the specified depth or no more move is possible. The strength of each move was determined as the difference between the two pots (larger number is stronger). Two more private functions, findLargestValue and findSmallestValue were used to find the strongest/weakest move, depending on who called the function. If called by an opponent, the function returns the weakest (for the SmartPlayer) move and if called by SmartPlayer, returns the strongest move, eventually resulting in one number being returned. Finally, the findLargestValue is called in the chooseMove function to determine what the strongest move is. 
