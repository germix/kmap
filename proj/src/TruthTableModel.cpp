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
#include "TruthTableModel.h"
#include <QColor>
#include <math.h>

const QColor TruthTableModel::oddColor(245, 245, 245);
const QColor TruthTableModel::evenColor(235, 235, 235);
const QColor TruthTableModel::functionColor(215, 225, 255);

TruthTableModel::TruthTableModel(BooleanFunction* bf)
{
	func = bf;
	showZeros = false;
	numberOfVariables = bf->getNumberOfVariables();

	//
	// ...
	//
	connect(func, SIGNAL(onSizeChanged(int)), this, SLOT(slotSizeChanged(int)));
	connect(func, SIGNAL(onValueChanged(int,int)), this, SLOT(slotValueChanged(int,int)));
}
TruthTableModel::~TruthTableModel()
{
}
void TruthTableModel::setValue(int addr, int value)
{
	func->setValue(addr, value);
}
int TruthTableModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return pow(2, numberOfVariables);
}
int TruthTableModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return numberOfVariables + 1;
}
QVariant TruthTableModel::data(const QModelIndex& index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		int value;

		if(index.column() == numberOfVariables)
		{
			value = func->getValue(index.row());

			if(value == 0 && !showZeros)
			{
				return "";
			}
			if(value == 2)
				return "?";
		}
		else
		{
			int i = index.row();
			int j = index.column();

			if((i%((int)pow(2, numberOfVariables-j)))<((int)pow(2, numberOfVariables-j-1)))
				value = 0;
			else
				value = 1;

		}
		return QString().sprintf("%d", value);
	}
	else if(role == Qt::BackgroundRole)
	{
		if(index.column() == numberOfVariables)
			return functionColor;
		else if(!(index.row()%2))
			return oddColor;
		else
			return evenColor;
	}
	else if(role == Qt::TextAlignmentRole)
	{
		return Qt::AlignCenter;
	}
	return QVariant();
}
QVariant TruthTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role == Qt::DisplayRole)
	{
		if(orientation == Qt::Horizontal)
		{
			if(section == numberOfVariables)
				return "f=";
			return QString().sprintf("%c", 65+section);
		}
		return QString().sprintf("%d", section);
	}
	else if(role == Qt::SizeHintRole)
	{
		/*
		QSize size = QAbstractTableModel::headerData(section, orientation, role).toSize();

		if(orientation == Qt::Horizontal)
		{
			size.setWidth(10);
			return QVariant(size);
		}
		*/
	}
	else if(role == Qt::TextAlignmentRole)
	{
		return Qt::AlignCenter;
	}
	return QVariant();
}
void TruthTableModel::slotSizeChanged(int vars)
{
	beginResetModel();
	numberOfVariables = vars;
	endResetModel();
}
void TruthTableModel::slotValueChanged(int addr, int value)
{
	Q_UNUSED(addr);
	Q_UNUSED(value);
	beginResetModel();
	endResetModel();
}
