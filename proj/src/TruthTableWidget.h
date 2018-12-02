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
#ifndef TRUTHTABLEWIDGET_H
#define TRUTHTABLEWIDGET_H
#include <QWidget>
#include <QTableWidget>
#include "BooleanFunction.h"

class TruthTableModel;

class TruthTableWidget : public QWidget
{
	Q_OBJECT
	BooleanFunction*	func;
	QTableView*			tableView;
	QAction*			actionSetTo1;
	QAction*			actionSetTo0;
	QAction*			actionSetToDontCare;

	TruthTableModel*	model;
public:
	TruthTableWidget(BooleanFunction* bf);
	~TruthTableWidget();
public:
	void setShowZeros(bool show);
private:
	QAction* newAction(const QString& label);
private:
	void changeEvent(QEvent* e);
private slots:
	void slotAction();
	void slotTableWidget_customContextMenuRequested(const QPoint& pos);
};

#endif // TRUTHTABLEWIDGET_H
