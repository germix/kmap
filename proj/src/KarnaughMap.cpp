/***************************************************************************
 *   Copyright (C) 2005 by Robert Kovacevic                                *
 *   robert.kovacevic@etfos.hr                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "KarnaughMap.h"
#include <math.h>
//#include <iostream>

#include "gray.h"
#include "BooleanFunction.h"

KarnaughMap::KarnaughMap()
{
}
KarnaughMap::~KarnaughMap()
{

}
void KarnaughMap::setup(int n)
{
	//
	// Establecer número de variables
	//
	numberOfVariables = n;
	numberOfDCares = 0;

	//
	// Determinar el ancho y largo a partir del número de variables
	//
#if 1
	width = (unsigned int)pow(2, ceil((double)n/2));
	height = (unsigned int)pow(2, floor((double)n/2));
#else
	width = (unsigned int)pow(2, floor((double)n/2));
	height = (unsigned int)pow(2, ceil((double)n/2));
#endif

	// Fill map with 0s and clear the list of KarnaughNodes
	reset();
	
	//
	// Fill map kmapValues with values that each cell in the map
	// has. Look here for rules:
	// http://www.allaboutcircuits.com/vol_4/chpt_8/3.html
	//
	if(numberOfVariables > 2)
	{
		for(unsigned int i = 0; i < height; i++)
		{
			for(unsigned int j = 0; j < width; j++)
			{
				// Set every 4x4 block's first 4 vars to gray code
				if(width >= 4)
					kmapValues[std::make_pair(j, i)] = GrayEncode(j%4+((i%4)*4));
				else
					kmapValues[std::make_pair(j, i)] = GrayEncode(j%4+((i%4)*width));

				// Combine 4x4 blocks into map (5th and higher variables)
				kmapValues[std::make_pair(j, i)] += 16*(j/4+((i/4)*(width/4)));
			}
		}
	}
	else
	{
		if(numberOfVariables == 2)
		{
			kmapValues[std::make_pair(0, 0)] = 0;
			kmapValues[std::make_pair(1, 0)] = 1;
			kmapValues[std::make_pair(0, 1)] = 2;
			kmapValues[std::make_pair(1, 1)] = 3;
		}
		if(numberOfVariables == 1)
		{
			kmapValues[std::make_pair(0, 0)] = 0;
			kmapValues[std::make_pair(1, 0)] = 1;
		}
	}
	clearColorSolutions();
}

void KarnaughMap::reset()
{
	//
	// Fills map with zeros and deletes all nodes from the solution list
	//
	for(unsigned int i = 0; i < height; i++)
		for(unsigned int j = 0; j < width; j++)
			set(j, i, 0);
	blocks.clear();
}
void KarnaughMap::solve(BooleanFunction* func, bool solveSOP)
{
	clearColorSolutions();
	for(unsigned int y = 0; y < height; y++)
	{
		for(unsigned int x = 0; x < width; x++)
		{
			if(solveSOP)
			{
				if(func->getValue(getMapValue(x, y)) == 1)		set(x, y, 1);
				else if(func->getValue(getMapValue(x, y)) == 0)	set(x, y, 0);
				else if(func->getValue(getMapValue(x, y)) == 2)	set(x, y, 2);
			}
			else
			{
				if(func->getValue(getMapValue(x, y)) == 1)		set(x, y, 0);
				else if(func->getValue(getMapValue(x, y)) == 0)	set(x, y, 1);
				else if(func->getValue(getMapValue(x, y)) == 2)	set(x, y, 2);
			}
		}
	}
	//
	// Iterates through all possible ways that 'Don't cares' can be
	// arranged, and finds one with fewest number of nodes in the solution
	// (bestc). If there are more ways that give the same number of nodes
	// in the solution choose the one with biggest nodes (bestsc)
	//
	
	std::list <KarnaughNode> best;
	int bestc = -1;
	unsigned int bestsc = 0;
	for(unsigned int i = 0; i < (unsigned int)pow(2, numberOfDCares); i++)
	{
		std::vector<bool> b;
		unsigned int j = i;
		while(j > 0)
		{
			b.insert(b.begin(), (bool)(j%2));
			j = j/2;
		}
		for(j = b.size(); j < numberOfDCares; j++)
		{
			b.insert(b.begin(), 0);
		}
		blocks.clear();
		
		unsigned int c = 0;
		for(unsigned int k = 0; k < height; k++)
		{
			for(unsigned int l = 0; l < width; l++)
			{
				if(kmapDCare[std::make_pair(l, k)] == 1)
				{
					if(b[c] == 1)
						kmap[std::make_pair(l, k)] = 1;
					else
						kmap[std::make_pair(l, k)] = 0;
					c++;
				}
			}
		}
		
		solve2();
		
		if((bestc == -1) || (blocks.size() <= bestc))
		{
			unsigned int sc = 0;
			for(std::list <KarnaughNode>::const_iterator iter = blocks.begin(); iter != blocks.end(); iter++)
			{
				for(int i = 0; i < iter->values.size(); i++)
				{
					if(iter->values[i] == 2)
						sc++;
				}
			}
			
			if((bestc == -1) || (blocks.size() < bestc))
			{
				best = blocks;
				bestc = best.size();
				bestsc = sc;
			}
			else
			{
				if(sc > bestsc)
				{
					best = blocks;
					bestc = best.size();
					bestsc = sc;
				}
			}
		}
	}
	blocks = best;

	//
	// Establecer colores
	//
	for(std::list<KarnaughNode>::const_iterator iter = blocks.begin(); iter != blocks.end(); iter++)
	{
		addColorSolution(iter->values);
	}
	//
	// Notificar
	//
	emit onSolved(solveSOP);
}

void KarnaughMap::solve2()
{
	//
	// Check for special case that all cells in the map are the same
	//
	bool a = true;
	for(unsigned int i = 0; i < height; i++)
	{
		if(a == false)
			break;
		for(unsigned int j = 0; j < width; j++)
		{
			if(kmap[std::make_pair(j, i)] != kmap[std::make_pair(0, 0)])
			{
				a = false;
				break;
			}
		}
	}
	if(a)
	{
		//
		// Clear the list so that all those nodes with one item are deleted
		//
		blocks.clear();
		
		// If there are only zeros in the map there's nothing to solve
		if(kmap[std::make_pair(0, 0)] == 0)
		{
			return;
		}
		else
		{
			// If there are only ones, solution is one element as big as the map
			KarnaughNode n;
			n.numberOfItems = width*height;
			for(unsigned int j = 0; j < numberOfVariables; j++)
				n.values.insert(n.values.begin(), 2);
			blocks.push_back(n);
			return;
		}
	}
	//
	// Put all blocks with 1 element in list
	//
	for(unsigned int i = 0; i < height; i++)
	{
		for(unsigned int j = 0; j < width; j++)
		{
			if(kmap[std::make_pair(j, i)] == 1)
			{
				KarnaughNode n;
				n.numberOfItems = 1;
				n.flag = 0;
				n.values = getMapBoolValue(j, i);
				blocks.push_back(n);
			}
		}
	}
	//
	// Joining blocks into blocks with 2^i elements
	//
	for(int i = 1; i <= (int)log2(width*height); i++)
	{
		//
		// Check every block with every other block and see if they can be joined into a bigger block
		//
		for(std::list<KarnaughNode>::iterator a = blocks.begin(); a != blocks.end(); a++)
		{
			for(std::list<KarnaughNode>::iterator b = blocks.begin(); b != blocks.end(); b++)
			{
				if((a->numberOfItems == (int)pow(2, i-1)) && (b->numberOfItems == (int)pow(2, i-1)) )
				{
					unsigned int x = isJoinable(a->values, b->values);
					if(x > 0)
					{
						//
						// If they can be joined make a new block with 2 in the place
						// of the one bit where they a and b are different
						//
						KarnaughNode n;
						n.numberOfItems=a->numberOfItems*2;
						n.flag=0;
						for(int i = 0; i < a->values.size(); i++)
						{
							if(i!=(x-1))
								n.values.insert(n.values.end(), a->values[i]);
							else
								n.values.insert(n.values.end(), 2);
						}
						
						// Mark that a node is part of a larger node
						a->flag = true;
						b->flag = true;
						
						// Check if that block already exists in the list
						bool exist = false;
						for(std::list<KarnaughNode>::const_iterator c = blocks.begin(); c != blocks.end(); c++)
						{
							if(n.values == c->values)
								exist = true;
						}
						if(!exist)
							blocks.push_back(n);
					}
				}
			}
		}
	}
	//
	// Deletes nodes that are cointained in larger nodes
	//
	for(std::list<KarnaughNode>::iterator a = blocks.begin(); a != blocks.end(); a++)
	{
		if(a->flag)
		{
			blocks.erase(a);
			a--;
		}
	}
	//
	// Deletes unneeded nodes. Draws a temp map with all nodes but one
	// and if that map is same as the main map, node that wasn't drawn can be deleted
	//
	std::map<std::pair<unsigned int, unsigned int>, unsigned short int> temp;
	
	for(std::list<KarnaughNode>::iterator a = blocks.begin(); a != blocks.end(); a++)
	{	
		for(unsigned int i = 0; i < height; i++)
			for(unsigned int j = 0; j < width; j++)
				temp[std::make_pair(j, i)] = 0;
		
		for(std::list<KarnaughNode>::iterator b = blocks.begin(); b != blocks.end(); b++)
		{
			if(a != b)
			{
				for(unsigned int i = 0; i < height; i++)
				{
					for(unsigned int j = 0; j < width; j++)
					{
						if(isAtCell(j, i, b->values))
							temp[std::make_pair(j, i)] = 1;
					}
				}
			}
		}
		
		bool del = true;
		for(unsigned int i = 0; i < height; i++)
		{
			for(unsigned int j = 0; j < width; j++)
			{
				if(temp[std::make_pair(j, i)] != kmap[std::make_pair(j, i)])
				{
					del = false;
					break;
				}
			}
			if(!del)
				break;
		}
		if(del)
		{
			a = blocks.erase(a);
			a--;
		}
	}
}

bool KarnaughMap::isAtCell(int x, int y, const std::vector<unsigned short int>& a)
{
	std::vector<unsigned short int> b = getMapBoolValue(x, y);
	for(int i = 0; i < a.size(); i++)
	{
		if((a[i] != b[i]) && (a[i] != 2))
			return 0;
	}
	return 1;
}
std::vector<unsigned short int> KarnaughMap::getMapBoolValue(unsigned int x, unsigned int y)
{
	std::vector<unsigned short int> b;
	unsigned int i = getMapValue(x, y);
	while(i > 0)
	{
		b.insert(b.begin(), i%2);
		i = i/2;
	}
	for(int j = b.size(); j < numberOfVariables; j++)
	{
		b.insert(b.begin(), 0);
	}
	return b;
}
//
// Checks if 2 karnaugh nodes with values a and b are joinable (only differ in one bit),
// and if they are returns (place where they differ + 1), otherwise returns 0
//
unsigned int KarnaughMap::isJoinable(const std::vector<unsigned short int>& a, const std::vector<unsigned short int>& b)
{
	int x;
	int c = 0;
	for(unsigned int i = 0; i < a.size(); i++)
	{
		if(a[i] != b[i])
		{
			c++;
			x = i;
		}
	}
	if(c == 1)
		return x+1;
	return 0;
}
void KarnaughMap::set(unsigned int x, unsigned int y, unsigned short int value)
{
	kmap[std::make_pair(x, y)] = value;
	if(value == 2) 
	{
		kmapDCare[std::make_pair(x, y)] = 1;
		numberOfDCares++;
	}
	else
	{
		kmapDCare[std::make_pair(x, y)] = 0;
	}
}
unsigned short int KarnaughMap::get(unsigned int x, unsigned int y)
{
	if(!kmapDCare[std::make_pair(x,y)])
		return kmap[std::make_pair(x,y)];
	return 2;
}

unsigned int KarnaughMap::getMapValue(unsigned int x, unsigned int y)
{
	return kmapValues[std::make_pair(x,y)];
}

unsigned int KarnaughMap::getWidth() const
{
	return width;
}

unsigned int KarnaughMap::getHeight() const
{
	return height;
}

std::list<KarnaughNode> KarnaughMap::getSolutions() const
{
	return blocks;
}

unsigned int KarnaughMap::getNumberOfVariables() const
{
	return numberOfVariables;
}
QColor KarnaughMap::getBackground(unsigned int col, unsigned int row) const
{
	return kmapBackgrounds[std::make_pair(col, row)];
}
void KarnaughMap::addColorSolution(const std::vector<unsigned short int>& sol)
{
	for(unsigned int i = 0; i < height; i++)
	{
		for(unsigned int j = 0; j < width; j++)
		{
			std::vector<unsigned short int> m = getMapBoolValue(j, i);
			bool is = true;
			for(unsigned int k = 0; k < sol.size(); k++)
			{
				if((sol[k] != m[k]) && (sol[k]!=2))
					is = false;
			}
			if(is)
			{
				QColor c = kmapBackgrounds[std::make_pair(j, i)];
				kmapBackgrounds[std::make_pair(j, i)] = QColor(c.red()-40, c.green()-30, c.blue());
			}
		}
	}
}
void KarnaughMap::clearColorSolutions()
{
	for(unsigned int i = 0; i < height; i++)
	{
		for(unsigned int j = 0; j < width; j++)
		{
			kmapBackgrounds[std::make_pair(j, i)] = Qt::white;
		}
	}
}
