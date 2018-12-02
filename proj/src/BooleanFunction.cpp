/***************************************************************************
 *   Copyright (C) 2018 by Germán Martínez                                 *
 *   germix.software@gmail.com                                             *
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
#include "BooleanFunction.h"
#include <math.h>

BooleanFunction::BooleanFunction(int vars)
{
	size = 0;
	values = 0;
	numberOfVariables = 0;
	setNumberOfVariables(vars);
}
BooleanFunction::~BooleanFunction()
{
	if(values != NULL)
		delete [] values;
}
int BooleanFunction::getValue(int addr) const
{
	return values[addr];
}
void BooleanFunction::setValue(int addr, int value)
{
	if(addr >= 0 && addr < size)
	{
		if(values[addr] != value)
		{
			values[addr] = value;
			// ...
			emit onValueChanged(addr, value);
		}
	}
}
int BooleanFunction::getNumberOfVariables() const
{
	return numberOfVariables;
}
void BooleanFunction::setNumberOfVariables(unsigned int vars)
{
	Q_ASSERT(vars >= 0);

	if(vars != numberOfVariables)
	{
		numberOfVariables = vars;

		if(values != NULL)
			delete [] values;

		size = pow(2, numberOfVariables);
		values = new int[size];
		memset(values, 0, size * sizeof(int));

		emit onSizeChanged(numberOfVariables);
	}
}


