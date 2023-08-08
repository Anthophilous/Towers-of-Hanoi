#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

//CS 471 Fall 2022
//By Joshua White 
//Tested on Empress

//Peg Class
	//Has Pegs, Disks
class Pegs
{
public:
	//=======Variables==============
	vector<int> pegs[3];	//Pegs Stored as Array (Size 3) of Integer Vectors

	//=======Functions==============

	//Moves a Disk from a Source to Destination
	void moveDisk(int srcPeg, int dstPeg)
	{
		if (!pegs[srcPeg].empty())
		{
			int disk = pegs[srcPeg].back();

			pegs[srcPeg].pop_back();

			pegs[dstPeg].push_back(disk);
		}
	}

	//Returns the Value/Size of the Top Disk of a Peg
	int top(int srcPeg)
	{
		if (pegs[srcPeg].empty())
		{
			return 4;
		}
		else
		{
			return pegs[srcPeg].back();
		}
	}

	//Cout All 3 Pegs
	void displayTowers()
	{
		for (int i = 0; i < 3; i++)
		{
			displayPeg(i);
		}
		cout << endl;
	}

	//Cout A Whole Peg
	void displayPeg(int srcPeg)
	{
		cout << "[ ";
		for (int i = 0; i < pegs[srcPeg].size(); i++)
		{
			cout << pegs[srcPeg].at(i) << " ";
		}
		cout << "]";
	}
};

//State Class
	//has it's Towers	
	//has Towers of Previous State
	//has G,H,F
class State
{
public:
	//=====================Variables=============================
	Pegs towers;
	Pegs cameFrom;
	int g;
	int h;
	int f;
};

//This Vector is the Frontier 
vector<State> frontier;

//This Vector holds previous States
vector<State> beenThere;

//Reference Pegs
Pegs emptySet;

//=====================Declarations==================================
bool compareStates(Pegs ourPegs, Pegs theirPegs);
bool comparePegs(vector<int> thisPeg, vector<int> thatPeg);
bool validPeg(vector<int> peg);

//=====================Utility Functions=============================

//Adds a State to beenThere
void addToBeenThere(State x) 
{ 
	beenThere.push_back(x); 
}

//Adds a State to the Frontier
void addToFrontier(State x) 
{ 
	frontier.push_back(x); 
}

//Removes a State from Frontier
void removeFromFrontier(State x)
{
	vector<State> newfrontier;

	for (int k = 0; k < frontier.size(); k++)
	{
		if (!compareStates(frontier[k].towers, x.towers))
		{
			newfrontier.push_back(frontier[k]);
		}
	}
	frontier = newfrontier;
}

//=====================Helper Functions=============================

//Returns True if A Peg is [ L, M, S ]
bool isGoal(vector<int> pegToCheck)
{
	bool isTheGoal = false;
	vector<int> goalVector;
	goalVector.push_back(3);
	goalVector.push_back(2);
	goalVector.push_back(1);

	if (comparePegs(pegToCheck, goalVector))
	{
		isTheGoal = true;
	}

	return isTheGoal;
}

//Returns True if Two States have the Same Exact Towers
bool compareStates(Pegs ourPegs, Pegs theirPegs)
{
	bool areSameTowers = true;
	for (int i = 0; i < 3; i++)
	{
		if (comparePegs(ourPegs.pegs[i], theirPegs.pegs[i]) == false)
		{
			areSameTowers = false;
			break;
		}
	}
	return areSameTowers;
}

//Returns true if the Pegs are the Same (Same meaning same disks in same order)
bool comparePegs(vector<int> thisPeg, vector<int> thatPeg)
{
	bool areSame = true;

	if (thisPeg.size() != thatPeg.size())		//If one is a different size than the other( or if one is empty and other isn't), return false
	{
		areSame = false;
	}
	else
	{
		for (int i = 0; i < thisPeg.size(); i++)
		{
			if (thisPeg.at(i) != thatPeg.at(i))
			{
				areSame = false;
				break;
			}
		}
	}

	return areSame;
}

//Returns true if all three Pegs are Valid (Valid Meaning Larger Disk not on top of Smaller Disk OR Peg is empty)
bool validState(Pegs thesePegs)
{
	bool isValidState = true;
	for (int i = 0; i < 3; i++)
	{
		if (!validPeg(thesePegs.pegs[i]))
		{
			isValidState = false;
			break;
		}
	}
	return isValidState;
}

//Returns true if the input peg is Valid (Valid Meaning Larger Disk not on top of Smaller Disk OR Peg is empty)
bool validPeg(vector<int> peg)
{
	bool isValid = true;

	for (int i = 1; i < peg.size(); i++)
	{
		if (peg.at(i-1) < peg.at(i))
		{
			isValid = false;
			break;
		}
	}

	return isValid;
}

//=====================At The Very End=============================

//void tracePath
void tracePath(State goal)
{
	cout << endl;
	goal.towers.displayTowers();

	if (!compareStates(goal.cameFrom, emptySet))
	{
		int index = 0;
		for (int i = 0; i < beenThere.size(); i++)
		{
			if (compareStates(goal.cameFrom, beenThere.at(i).towers))
			{
				index = i;
				break;
			}
		}
		tracePath(beenThere.at(index));
	}
	else
	{
		return;
	}
}

//=====================Frontier Functions=============================

//Returns True if next is in Frontier
bool inFrontier(State next)
{
	for (int i = 0; i < frontier.size(); i++)
	{
		if (compareStates(next.towers, frontier.at(i).towers))
		{
			if (next.f < frontier.at(i).f)
			{
				cout << "Improvement to Node in Frontier" << endl;
				removeFromFrontier(next); //Might Need to Revise THis ONe
				addToFrontier(next);
			}
			return true;
		}
	}
	return false;
}

//Returns True if next has occured already
bool haveBeenThere(State next)
{
	bool bt = false;

	for (int i = 0; i < beenThere.size(); i++)
	{
		if (compareStates(next.towers, beenThere.at(i).towers))
		{
			bt = true;
		}
	}

	return bt;
}

//Returns True if a State is Valid
bool safeToAdd(State next)
{
	return validState(next.towers);
}

//Generates a State
void generate(State next, int option)
{
	cout << "Considering the Move ";

	switch (option) 
	{
	case 1:
		cout << "'Disk on Peg 1 to Disk on Peg 2'!" << endl;
		break;
	case 2:
		cout << "'Disk on Peg 1 to Disk on Peg 3'!" << endl;
		break;
	case 3:
		cout << "'Disk on Peg 2 to Disk on Peg 1'!" << endl;
		break;
	case 4:
		cout << "'Disk on Peg 2 to Disk on Peg 3'!" << endl;
		break;
	case 5:
		cout << "'Disk on Peg 3 to Disk on Peg 1'!" << endl;
		break;
	case 6:
		cout << "'Disk on Peg 3 to Disk on Peg 2'!" << endl;
		break;
	}

	if (isGoal(next.towers.pegs[2]))
	{
		cout << "Reached Goal State: [ ], [ ], [ 3, 2, 1 ]" << endl;
		cout << "Displaying Path From Goal to Initial (Reverse Order) " << endl;
		tracePath(next);
		exit(0);
	}	//End of Program

	//If been there before, do not add to frontier.
	bool bt = haveBeenThere(next);

	//If unsafe/invalid, do not add to frontier.
	bool isSafe = safeToAdd(next);

	//Compute h and f for next
	//g was set by generateAll
	int h = 3;

	//check for [L,M,S]
	for (int i = 0; i < next.towers.pegs[2].size(); i++)
	{
		if (next.towers.pegs[2].at(i) == 3 - i)
		{
			h--;
		}
	}

	next.h = h;
	next.f = next.g + next.h;

	if (!inFrontier(next) && bt == false && isSafe == true)
	{
		addToFrontier(next);
	}	
}

// Generate all new states from current (called from main)
void generateAll(State current)
{
	State next;

	//Each next will be modified from current for ease
	current.g = current.g + 1; //Updates g to be used for next

	//Storing the parent so that we can produce the solution path
	current.cameFrom = current.towers;

	//Check All 6 Possibilites

	//Option 1: Move Top Disk of Peg 1 to Peg 2  
	next = current;

	if (current.towers.top(0) < current.towers.top(1))
	{
		next.towers.moveDisk(0, 1);
		generate(next, 1);
	}

	//Option 2: Move Top Disk of Peg 1 to Peg 3
	next = current;

	if (current.towers.top(0) < current.towers.top(2))
	{
		next.towers.moveDisk(0, 2);
		generate(next, 2);
	}
	//Option 3: Move Top Disk of Peg 2 to Peg 1
	next = current;

	if (current.towers.top(1) < current.towers.top(0))
	{
		next.towers.moveDisk(1, 0);
		generate(next, 3);
	}
	
	//Option 4: Move Top Disk of Peg 2 to Peg 3
	next = current;

	if (current.towers.top(1) < current.towers.top(2))
	{
		next.towers.moveDisk(1, 2);
		generate(next, 4);
	}

	//Option 5: Move Top Disk of Peg 3 to Peg 1
	next = current;

	if (current.towers.top(2) < current.towers.top(0))
	{
		next.towers.moveDisk(2, 0);
		generate(next, 5);
	}

	//Option 6: Move Top Disk of Peg 3 to Peg 2
	next = current;

	if (current.towers.top(2) < current.towers.top(1))
	{
		next.towers.moveDisk(2, 1);
		generate(next, 6);
	}
}

//Find the best f state out of the frontier and return it (called from main)
State bestofFrontier()
{
	int currentMax = frontier.at(0).f;
	int indexTracker = 0;
	for (int i = 1; i < frontier.size(); i++)
	{
		if (frontier.at(i).f > currentMax)
		{
			currentMax = frontier.at(i).f;
			indexTracker = i;
		}
	}
	return frontier.at(indexTracker);
}

// Display the states in the frontier (called from main)
void displayFrontier()
{
	for (int k = 0; k < frontier.size(); k++)
	{
		frontier[k].towers.displayTowers();
		cout << "g = " << frontier[k].g << " ";
		cout << "h = " << frontier[k].h << " ";
		cout << "f = " << frontier[k].f << endl;
	}
}

//=========================Main==================================

int main()
{
	Pegs initialPegs;
	initialPegs.pegs[0].push_back(3);
	initialPegs.pegs[0].push_back(2); 
	initialPegs.pegs[0].push_back(1);
	
	State current = { initialPegs,emptySet,0, 3, 3 };
	addToFrontier(current);

	char ans = 'x';
	while (ans != 'n')
	{
		removeFromFrontier(current);
		addToBeenThere(current);
		cout << ">>>Expand:" << endl;
		current.towers.displayTowers();
		cout << endl;
		generateAll(current); // new states are added to frontier
		cout << "Frontier is:" << endl;
		displayFrontier();
		current = bestofFrontier(); // pick the best state out of frontier
		cin >> ans;
	} 
}