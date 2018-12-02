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

#ifndef KARNAUGH_MAP_H
#define KARNAUGH_MAP_H

#include <vector>
#include <list>
#include <map>
#include <utility>


class BooleanFunction;

struct KarnaughNode
{
	bool							flag;
	std::vector<unsigned short int>	values;
	unsigned int					numberOfItems;
};

#include <QColor>
#include <QObject>

class KarnaughMap : public QObject
{
	Q_OBJECT

	std::map<std::pair<unsigned int, unsigned int>, unsigned short int>	kmap;
	std::map<std::pair<unsigned int, unsigned int>, unsigned int>		kmapValues;
	std::map<std::pair<unsigned int, unsigned int>, bool>				kmapDCare;
	std::list<KarnaughNode>												blocks;
	unsigned int														numberOfDCares;
	unsigned int														numberOfVariables;
	unsigned int														width;
	unsigned int														height;
	mutable std::map<std::pair<unsigned int, unsigned int>, QColor>		kmapBackgrounds;
public:
	KarnaughMap();
	~KarnaughMap();
public:
	void solve(BooleanFunction* func, bool solveSOP);
signals:
	void onSolved(bool sumOfProducts);
public:
	void setup(int n);
	unsigned int getMapValue(unsigned int x, unsigned int y);
	std::vector<unsigned short int> getMapBoolValue(unsigned int x, unsigned int y);
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getNumberOfVariables() const;
	std::list<KarnaughNode> getSolutions() const;
public:
	QColor getBackground(unsigned int col, unsigned int row) const;
private:
	void reset();
	unsigned short int get(unsigned int x, unsigned int y);
	void set(unsigned int x, unsigned int y, unsigned short int value);
	void solve2();
	bool isAtCell(int x, int y, const std::vector<unsigned short int>& a);
	unsigned int isJoinable(const std::vector<unsigned short>& a, const std::vector<unsigned short>& b);
private:
	void addColorSolution(const std::vector<unsigned short int>& sol);
	void clearColorSolutions();
};

#endif // KARNAUGH_MAP_H
