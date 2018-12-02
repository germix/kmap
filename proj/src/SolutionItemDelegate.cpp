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
#include "SolutionItemDelegate.h"
#include <QPainter>
#include <QApplication>

SolutionItemDelegate::SolutionItemDelegate()
{
}
SolutionItemDelegate::~SolutionItemDelegate()
{
}

void SolutionItemDelegate::paint(QPainter *painter,
		   const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString s;
	QString txt = index.data(Qt::DisplayRole).toString();

	const QFontMetrics& fm = option.fontMetrics;
	{
		QStyle* style = (option.widget ? option.widget->style() : qApp->style());
		QStyleOptionViewItem opt = option;
		initStyleOption(&opt, index);

		// Guardar contexto
		painter->save();

		// Pintar el fondo
		style->proxy()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, option.widget);

		// Pintar el texto
		{
			enum
			{
				STATE_NORMAL,
				STATE_NOT,
			};
			int state = STATE_NORMAL;
			int currentX = 0;
			struct LINE
			{
				int x;
				int width;
			};
			QVector<LINE> notList;

			for(int i = 0; i < txt.size(); i++)
			{
				QChar chr = txt.at(i);
				if(chr == '|')
				{
					state = STATE_NOT;
				}
				else
				{
					if(state == STATE_NOT)
					{
						LINE l;
						l.x = currentX;
						l.width = fm.width(chr);
						currentX += l.width;

						notList.append(l);
					}
					else
					{
						currentX += fm.width(chr);
					}
					s += chr;
					state = STATE_NORMAL;
				}
			}
			opt.rect.adjust(2, 2, -2, -2);
			painter->drawText(opt.rect, Qt::AlignLeft|Qt::AlignVCenter, s);
			for(int i = 0; i < notList.size(); i++)
			{
				const LINE& line = notList.at(i);

				painter->fillRect(QRect(opt.rect.x() + line.x + 1, opt.rect.y()+1, line.width - 2, 1), Qt::black);
			}
		}

		// Restaurar contexto
		painter->restore();
	}
}

