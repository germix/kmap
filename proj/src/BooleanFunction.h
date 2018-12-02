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
#ifndef BOOLEANFUNCTION_H
#define BOOLEANFUNCTION_H
#include <QObject>

class BooleanFunction : public QObject
{
	Q_OBJECT
	int					size;
	int*				values;
	unsigned int		numberOfVariables;
public:
	BooleanFunction(int vars);
	~BooleanFunction();
public:
	int getValue(int addr) const;
	void setValue(int addr, int value);
	int getNumberOfVariables() const;
	void setNumberOfVariables(unsigned int vars);
signals:
	void onSizeChanged(int vars);
	void onValueChanged(int addr, int value);
};

#endif // BOOLEANFUNCTION_H

