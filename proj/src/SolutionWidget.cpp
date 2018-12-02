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
#include "SolutionWidget.h"
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QString>
#include <list>
#include <QComboBox>
#include <QGridLayout>

#include "SolutionItemDelegate.h"

SolutionWidget::SolutionWidget(KarnaughMap* km) : kmap(km)
{
	//
	// TreeWidget de los resultados
	//
	treeWidget = new QTreeWidget();
	treeWidget->setHeaderHidden(true);
	treeWidget->setItemDelegate(new SolutionItemDelegate());
//	treeWidget->setFont(QFont("Courier New", 10));
//	treeWidget->setRootIsDecorated(false);
	treeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList() << "X = 0"));

	//
	// Generar layout
	//
	QGridLayout* l = new QGridLayout();
	l->setContentsMargins(0, 0, 0, 0);
	l->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Maximum), 2, 0);
	l->addWidget(treeWidget, 0, 1, 3, 1);
	setLayout(l);

	// ...
	connect(kmap, SIGNAL(onSolved(bool)), this, SLOT(slotKarnaughMap_solved(bool)));
}
SolutionWidget::~SolutionWidget()
{
}
void SolutionWidget::slotKarnaughMap_solved(bool sop)
{
	treeWidget->clear();
	// ...
	std::list<KarnaughNode> solutionsList = kmap->getSolutions();
	QString s = "X = ";
	QTreeWidgetItem* item = new QTreeWidgetItem();
	if(sop)
	{
		for(std::list<KarnaughNode>::const_iterator iter = solutionsList.begin(); iter != solutionsList.end(); iter++)
		{
			QString b; // A, B, C, D
			QString a; // All the rest
			char chr = 'A';
			for(int i = 0; i < iter->values.size(); i++)
			{
#if 0
				if(iter->values[i] == 1)
				{
					a.append(QString().sprintf("%c", chr));
				}
				if(iter->values[i] == 0)
				{
					a.append("|");
					a.append(QString().sprintf("%c", chr));
				}
				chr++;
#else
				int apos=kmap->getNumberOfVariables()-4;
				if(apos<0) apos=0;
				int bpos=kmap->getNumberOfVariables()-3;
				if(bpos<1) bpos=1;
				int cpos=kmap->getNumberOfVariables()-2;
				if(cpos<2) cpos=2;
				int dpos=kmap->getNumberOfVariables()-1;
				if(dpos<3) dpos=3;

				if((i!=apos) && (i!=bpos) && (i!=cpos) && (i!=dpos))
				{
					if(iter->values[i] == 1)
					{
						a.append(QString().sprintf("%c", 68+(kmap->getNumberOfVariables()-4-i)));
					}
					if(iter->values[i] == 0)
					{
						a.append("|");
						a.append(QString().sprintf("%c", 68+(kmap->getNumberOfVariables()-4-i)));
					}
				}
				if(i==apos)
				{
					if(iter->values[i]==1) b += "A";
					if(iter->values[i]==0) b += "|A";
				}
				if(i==bpos)
				{
					if(iter->values[i]==1) b += "B";
					if(iter->values[i]==0) b += "|B";
				}
				if(i==cpos)
				{
					if(iter->values[i]==1) b += "C";
					if(iter->values[i]==0) b += "|C";
				}
				if(i==dpos)
				{
					if(iter->values[i]==1) b += "D";
					if(iter->values[i]==0) b += "|D";
				}
#endif
			}
			b=b+a;

			if(b.isEmpty()) b="1";
			s+=b;

			if(iter != (--solutionsList.end())) s.append(" + ");

			if(b != "1")
				item->addChild(new QTreeWidgetItem(QStringList() << b));
		}
		if(s=="X = ") s="X = 0";
	}
	else
	{
		for(std::list<KarnaughNode>::const_iterator iter = solutionsList.begin(); iter != solutionsList.end(); iter++)
		{
			QString b= "(";
			QString a;
			char chr = 'A';
			for(int i = 0; i < iter->values.size(); i++)
			{
#if 0
				if(iter->values[i] == 0)
				{
					a += QString().sprintf("%c+", chr);
				}
				if(iter->values[i] == 1)
				{
					a += "|";
					a += QString().sprintf("%c+", chr);
				}
				chr++;
#else
				int apos=kmap->getNumberOfVariables()-4;
				if(apos<0) apos=0;
				int bpos=kmap->getNumberOfVariables()-3;
				if(bpos<1) bpos=1;
				int cpos=kmap->getNumberOfVariables()-2;
				if(cpos<2) cpos=2;
				int dpos=kmap->getNumberOfVariables()-1;
				if(dpos<3) dpos=3;

				if((i!=apos) && (i!=bpos) && (i!=cpos) && (i!=dpos))
				{
					if(iter->values[i]==0)
					{
						a += QString().sprintf("%c+", 68+(kmap->getNumberOfVariables()-4-i));
					}
					if(iter->values[i]==1)
					{
						a += "|";
						a += QString().sprintf("%c+", 68+(kmap->getNumberOfVariables()-4-i));
					}
				}

				if(i==apos)
				{
					if(iter->values[i]==0) b += "A+";
					if(iter->values[i]==1) b += "|A+";
				}
				if(i==bpos)
				{
					if(iter->values[i]==0) b += "B+";
					if(iter->values[i]==1) b += "|B+";
				}
				if(i==cpos)
				{
					if(iter->values[i]==0) b += "C+";
					if(iter->values[i]==1) b += "|C+";
				}
				if(i==dpos)
				{
					if(iter->values[i]==0) b += "D+";
					if(iter->values[i]==1) b += "|D+";
				}
#endif
			}
			b=b+a;
			if(b.isEmpty()) b = "0";
			else b[b.size()-1] = ')';
			s+=b;

			if(b != ")" && b != "1")
				item->addChild(new QTreeWidgetItem(QStringList() << b));
		}
		if(s == "X = ") s = "X = 1";
		if(s == "X = )") s = "X = 0";
	}
	item->setText(0, s);
	treeWidget->addTopLevelItem(item);
	item->setExpanded(true);
}


