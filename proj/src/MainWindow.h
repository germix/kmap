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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}
#include "TruthTableWidget.h"
#include "KarnaughMapWidget.h"
#include "SolutionWidget.h"

#include "KarnaughMap.h"

#include "BooleanFunction.h"

#include "Languages.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow*	ui;

	TruthTableWidget*	truthTable;
	KarnaughMapWidget*	karnaughMap;

	KarnaughMap			kmap;
	BooleanFunction*	boolFunction;

	Languages			languages;

	bool				showZeros;
	bool				showCellAddress;

	int					typeOfSolution;
	int					numberOfVariables;

	QSplitter*			splitter;

	QWidget*			solutionForm;
	QLabel*				solutionLabel;
	SolutionWidget*		solutionWidget;

	QHash<QDockWidget*, QLatin1String> docks;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
	void changeEvent(QEvent* e);
	bool eventFilter(QObject* watched, QEvent* event);
private:
	void solve();
	QDockWidget* createDockPanel(QWidget* widget, const QString& name, const QString& title, const char* rawTitle, bool visible, Qt::DockWidgetArea initialArea, Qt::DockWidgetAreas allowedAreas = Qt::AllDockWidgetAreas);
private slots:
	void slotAction();
	void slotTypeOfSolution(int type);
	void slotNumberOfVariables(int number);
	void slotBooloanFunction_sizeChanged(int vars);
	void slotBooloanFunction_valueChanged(int addr, int value);
};

#endif // MAINWINDOW_H
