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
#ifndef KARNAUGHMAPWIDGET_H
#define KARNAUGHMAPWIDGET_H
#include <QWidget>
#include <QTableView>
#include "KarnaughMap.h"
#include "BooleanFunction.h"

class SolutionWidget;
class KarnaughMapModel;
class KarnaughMapDelegate;

class QLabel;
class QSettings;
class QSplitter;
class QGroupBox;

class KarnaughMapWidget : public QWidget
{
	Q_OBJECT
	BooleanFunction*		func;
	KarnaughMap*			kmap;
	KarnaughMapModel*		model;
	KarnaughMapDelegate*	delegate;
	QTableView*				tableView;
	QAction*				actionSetTo1;
	QAction*				actionSetTo0;
	QAction*				actionSetToDontCare;
public:
	KarnaughMapWidget(int typeOfSolution, int numberOfVariables, BooleanFunction* bf, KarnaughMap* kmap);
	~KarnaughMapWidget();
signals:
	void onTypeOfSolutionChanged(int type);
	void onNumberOfVariablesChanged(int number);
public:
	void setShowZeros(bool show);
	void setShowCellAddress(bool show);
private:
	QAction* newAction(const QString& label);
private:
	void changeEvent(QEvent* e);
private slots:
	void slotAction();
	void slotTableWidget_customContextMenuRequested(const QPoint& pos);
	void slotKarnaughMap_solved(bool sop);

	void slotConfig_typeOfSolutionChanged(int type);
	void slotConfig_numberOfVariablesChanged(int number);
};

#endif // KARNAUGHMAPWIDGET_H
