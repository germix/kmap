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
#include "KarnaughMapModel.h"
#include "KarnaughMap.h"

KarnaughMapModel::KarnaughMapModel(BooleanFunction* bf, KarnaughMap& ___kmap)
	: func(bf)
	, kmap(___kmap)
{
	//
	// ...
	//
	connect(func, SIGNAL(onSizeChanged(int)), this, SLOT(slotSizeChanged(int)));
	connect(func, SIGNAL(onValueChanged(int,int)), this, SLOT(slotValueChanged(int,int)));

	showZeros = false;
	slotSizeChanged(bf->getNumberOfVariables());
}
KarnaughMapModel::~KarnaughMapModel()
{
}
int KarnaughMapModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return kmap.getHeight();
}
int KarnaughMapModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return kmap.getWidth();
}
QVariant KarnaughMapModel::data(const QModelIndex& index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		unsigned int i = (unsigned int)index.row();
		unsigned int j = (unsigned int)index.column();

		unsigned int v = func->getValue(kmap.getMapValue(j, i));
		if(v == 2)
			return "?";
		if(v == 0 && !showZeros)
		{
			return "";
		}
		return QString().sprintf("%d", v);
	}
	else if(role == Qt::BackgroundRole)
	{
		return kmap.getBackground(index.column(), index.row());
	}
	else if(role == Qt::TextAlignmentRole)
	{
		return Qt::AlignCenter;
	}
	return QVariant();
}
QVariant KarnaughMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role == Qt::DisplayRole)
	{
		if(orientation == Qt::Horizontal)
			return horzHeader.at(section);
		return vertHeader.at(section);
	}
	return QVariant();
}
void KarnaughMapModel::slotSizeChanged(int vars)
{
	beginResetModel();

	kmap.setup(vars);
	vertHeader.clear();
	horzHeader.clear();

	for(unsigned int i=0; i< kmap.getHeight(); i++)
	{
		std::vector<unsigned short int> a = kmap.getMapBoolValue(0,i);
		for(unsigned int j=0; j< kmap.getWidth(); j++)
		{
			std::vector<unsigned short int> b = kmap.getMapBoolValue(j,i);
			for(unsigned int k=0; k<a.size(); k++)
			{
				if( a[k]!=b[k] ) a[k]=2;
			}
		}
		QString r;
		for(unsigned int k=0; k<a.size(); k++)
		{
			if( a[k]!=2 )
				r += QString().sprintf("%d", a[k]);
		}
		vertHeader.append(r);
	}

	for(unsigned int i = 0; i < kmap.getWidth(); i++)
	{
		std::vector<unsigned short int> a = kmap.getMapBoolValue(i,0);
		for(unsigned int j = 0; j < kmap.getHeight(); j++)
		{
			std::vector<unsigned short int> b = kmap.getMapBoolValue(i,j);
			for(unsigned int k = 0; k < a.size(); k++)
			{
				if(a[k] != b[k])
					a[k] = 2;
			}
		}
		QString r;
		for(unsigned int k = 0; k < a.size(); k++)
		{
			if(a[k] != 2)
				r += QString().sprintf("%d", a[k]);
		}
		horzHeader.append(r);
	}
	endResetModel();
}
void KarnaughMapModel::slotValueChanged(int addr, int value)
{
	Q_UNUSED(addr);
	Q_UNUSED(value);
	beginResetModel();
	endResetModel();
}

