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
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>
#include <QDockWidget>
#include <QSettings>

#include <QInputDialog>

#define SETTINGS_APPLICATION "KarnaughMap"
#define SETTINGS_ORGANIZATION "Germix"

#include "AboutDialog.h"

#include <QKeyEvent>
#include <QSplitter>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	// ...
	languages.init(ui->menu_Languages, "translations", "kmap", s.value("Language").toString());

	// ...
	typeOfSolution = s.value("TypeOfSolution", "0").toInt();
	numberOfVariables = s.value("NumberOfVariables", "4").toInt();

	//
	// ...
	//
	kmap.setup(numberOfVariables);
	boolFunction = new BooleanFunction(numberOfVariables);
	connect(boolFunction, SIGNAL(onSizeChanged(int)), this, SLOT(slotBooloanFunction_sizeChanged(int)));
	connect(boolFunction, SIGNAL(onValueChanged(int,int)), this, SLOT(slotBooloanFunction_valueChanged(int,int)));

	truthTable = new TruthTableWidget(boolFunction);
	karnaughMap = new KarnaughMapWidget(typeOfSolution, numberOfVariables, boolFunction, &kmap);
	solutionWidget = new SolutionWidget(&kmap);

	// ...
	solutionForm = new QWidget();
	solutionLabel = new QLabel(tr("Solution:"));
	solutionLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QVBoxLayout* vbox = new QVBoxLayout(solutionForm);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->addWidget(solutionLabel);
	vbox->addWidget(solutionWidget);

	karnaughMap->installEventFilter(this);
	connect(karnaughMap, SIGNAL(onTypeOfSolutionChanged(int)), this, SLOT(slotTypeOfSolution(int)));
	connect(karnaughMap, SIGNAL(onNumberOfVariablesChanged(int)), this, SLOT(slotNumberOfVariables(int)));

	//
	// Splitter
	//
	splitter = new QSplitter(Qt::Vertical);
	splitter->addWidget(karnaughMap);
	splitter->addWidget(solutionForm);
	setCentralWidget(splitter);
	splitter->restoreState(s.value("SplitterState").toByteArray());

	//
	// Acciones
	//
	connect(ui->actionProgramExit, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionHelpAbout, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionSettingsShowZeros, SIGNAL(triggered()), this, SLOT(slotAction()));
	connect(ui->actionSettingsShowCellAddress, SIGNAL(triggered()), this, SLOT(slotAction()));
	ui->actionSettingsShowZeros->setChecked(showCellAddress = s.value("ShowZeros").toBool());
	ui->actionSettingsShowCellAddress->setChecked(showCellAddress = s.value("ShowCellAddress").toBool());

	//
	// ...
	//
	createDockPanel(truthTable, "TruthTable", tr("Truth table"), "Truth table", true, Qt::LeftDockWidgetArea);

	//
	// Restaurar estado anterior
	//
	restoreGeometry(s.value("WindowGeometry").toByteArray());
	restoreState(s.value("WindowState").toByteArray());

	karnaughMap->setShowZeros(showZeros);
	karnaughMap->setShowCellAddress(showCellAddress);
	karnaughMap->setFocus();
}

MainWindow::~MainWindow()
{
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
	s.setValue("Language", languages.language());
	s.setValue("WindowState", saveState());
	s.setValue("WindowGeometry", saveGeometry());
	s.setValue("SplitterState", splitter->saveState());

	s.setValue("ShowZeros", showZeros);
	s.setValue("ShowCellAddress", showCellAddress);
	s.setValue("TypeOfSolution", typeOfSolution);
	s.setValue("NumberOfVariables", numberOfVariables);

	delete ui;
}

void MainWindow::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		switch(e->type())
		{
#if 1
			case QEvent::LocaleChange:
				{
					QString locale = QLocale::system().name();
					locale.truncate(locale.lastIndexOf('_'));
					languages.load(locale);
				}
				break;
#endif
			case QEvent::LanguageChange:
				ui->retranslateUi(this);
				solutionLabel->setText(tr("Solution:"));
				for(QHash<QDockWidget*, QLatin1String>::ConstIterator it = docks.constBegin(); it != docks.constEnd(); it++)
				{
					QDockWidget*  dw = it.key();
					QLatin1String title = it.value();
					dw->setWindowTitle(tr(title.latin1()));
				}
				break;
			default:
				break;
		}
	}
	QMainWindow::changeEvent(e);
}
bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent* e = (QKeyEvent*)(event);
		if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
		{
			solve();
			return true;
		}
	}
	return QMainWindow::eventFilter(watched, event);
}
void MainWindow::solve()
{
	kmap.solve(boolFunction, typeOfSolution == 0);
}
QDockWidget* MainWindow::createDockPanel(QWidget* widget, const QString& name, const QString& title, const char* rawTitle, bool visible, Qt::DockWidgetArea initialArea, Qt::DockWidgetAreas allowedAreas)
{
	//
	// Crear el dock widget
	//
	QDockWidget* dock = new QDockWidget(title, this);
	dock->setObjectName(name);
	dock->setWidget(widget);
	dock->setAllowedAreas(allowedAreas);
	if(!restoreDockWidget(dock))
	{
		dock->setVisible(visible);
		addDockWidget(initialArea, dock);
	}
	docks.insert(dock, QLatin1String(rawTitle));

	//
	// Obtener la acción y agregarla al menu
	//
	QAction* action = dock->toggleViewAction();

	ui->menu_Windows->addAction(action);

	return dock;
}
void MainWindow::slotAction()
{
	QAction* action = qobject_cast<QAction*>(sender());

	if(action == ui->actionProgramExit)
	{
		close();
	}
	else if(action == ui->actionHelpAbout)
	{
		AboutDialog().exec();
	}
	else if(action == ui->actionSettingsShowZeros)
	{
		showZeros = ui->actionSettingsShowZeros->isChecked();
		truthTable->setShowZeros(showZeros);
		karnaughMap->setShowZeros(showZeros);
	}
	else if(action == ui->actionSettingsShowCellAddress)
	{
		showCellAddress = ui->actionSettingsShowCellAddress->isChecked();
		karnaughMap->setShowCellAddress(showCellAddress);
	}
}
void MainWindow::slotTypeOfSolution(int type)
{
	typeOfSolution = type;
	solve();
}
void MainWindow::slotNumberOfVariables(int number)
{
	numberOfVariables = number;
	boolFunction->setNumberOfVariables(number);
	solve();
}
void MainWindow::slotBooloanFunction_sizeChanged(int vars)
{
	Q_UNUSED(vars);
}
void MainWindow::slotBooloanFunction_valueChanged(int addr, int value)
{
	Q_UNUSED(addr);
	Q_UNUSED(value);
	solve();
}




