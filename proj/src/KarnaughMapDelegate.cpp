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
#include "KarnaughMapDelegate.h"
#include "KarnaughMapModel.h"
#include <QPainter>
#include "KarnaughMap.h"
#include "gray.h"

KarnaughMapDelegate::KarnaughMapDelegate(QObject* parent, KarnaughMap& ___kmap) : QItemDelegate(parent), kmap(___kmap)
{
	showCellAddress = true;
}
KarnaughMapDelegate::~KarnaughMapDelegate()
{
}
void KarnaughMapDelegate::setShowCellAddress(bool show)
{
	showCellAddress = show;
}
void KarnaughMapDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QItemDelegate::paint(painter, option, index);
	if(showCellAddress)
	{
		QFont font("sans", 7);
		QFont oldFont = painter->font();

		painter->setFont(font);


		unsigned int addr = kmap.getMapValue(index.column(), index.row());

		QFontMetrics fm(font);
		QString txt = QString().sprintf("%d", addr);
		int     txtw = fm.width(txt);
		int     txth = fm.height();

		if(!(option.state & QStyle::State_Selected))
			painter->setPen(Qt::gray);
		else
			painter->setPen(Qt::darkGray);
		painter->drawText(option.rect.right() - txtw - 1, option.rect.bottom() - txth - 1, txt);

		painter->setFont(oldFont);
	}
}
QSize KarnaughMapDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QItemDelegate::sizeHint(option, index);
}
unsigned int KarnaughMapDelegate::getMapValue(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const
{
	unsigned int r;

	if(width>2)
	{
		if(width>=4)
			r=GrayEncode(x%4+((y%4)*4));
		else
			r=GrayEncode(x%4+((y%4)*width));
		r+=16*(x/4+((y/4)*(width/4)));

		return r;
	}
	else
	{
		if(height==2)
		{
			if( (x==0) && (y==0) ) return 0;
			if( (x==1) && (y==0) ) return 1;
			if( (x==0) && (y==1) ) return 2;
			if( (x==1) && (y==1) ) return 3;
		}
		if(height==1)
		{
			if( (x==0) && (y==0) ) return 0;
			if( (x==1) && (y==0) ) return 1;
		}
	}
	return -1; // avoid warning
}
