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
#include "KarnaughMapWidget.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include "KarnaughMapModel.h"
#include "KarnaughMapDelegate.h"
#include <QAction>
#include <QMenu>
#include <QHeaderView>
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QEvent>

#include "ConfigForm.h"
#include "SolutionWidget.h"

KarnaughMapWidget::KarnaughMapWidget(int typeOfSolution, int numberOfVariables, BooleanFunction* bf, KarnaughMap* km) : func(bf), kmap(km)
{
	connect(km, SIGNAL(onSolved(bool)), this, SLOT(slotKarnaughMap_solved(bool)));

	//
	// Tabla
	//
	tableView = new QTableView();
	tableView->setModel(model = new KarnaughMapModel(bf, *kmap));
	tableView->setItemDelegate(delegate = new KarnaughMapDelegate(this, *kmap));

	tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
#if (QT_VERSION_MAJOR == 4)
	tableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#else
	tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif
	tableView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotTableWidget_customContextMenuRequested(QPoint)));

	//
	// Acciones
	//
	actionSetTo1 = newAction(tr("Set to 1"));
	actionSetTo0 = newAction(tr("Set to 0"));
	actionSetToDontCare = newAction(tr("Set to \"don\'t care\""));

	//
	// Formulario de configuración
	//
	ConfigForm* configForm = new ConfigForm(typeOfSolution, numberOfVariables);
	connect(configForm, SIGNAL(onTypeOfSolutionChanged(int)), this, SLOT(slotConfig_typeOfSolutionChanged(int)));
	connect(configForm, SIGNAL(onNumberOfVariablesChanged(int)), this, SLOT(slotConfig_numberOfVariablesChanged(int)));

	//
	// Layout
	//
	QVBoxLayout* vbox = new QVBoxLayout();
	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget(configForm);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->setSpacing(0);
	vbox->addLayout(hbox);
	vbox->addWidget(tableView);
	setLayout(vbox);

	// ...
	tableView->setFocus();
}
KarnaughMapWidget::~KarnaughMapWidget()
{
}
void KarnaughMapWidget::setShowZeros(bool show)
{
	model->setShowZeros(show);
}
void KarnaughMapWidget::setShowCellAddress(bool show)
{
	delegate->setShowCellAddress(show);
	model->reset();
}
QAction* KarnaughMapWidget::newAction(const QString& label)
{
	QAction* action = new QAction(label, this);
	connect(action, SIGNAL(triggered()), this, SLOT(slotAction()));
	return action;
}
void KarnaughMapWidget::slotAction()
{
	QAction* action = qobject_cast<QAction*>(sender());
	QModelIndexList indexes = tableView->selectionModel()->selectedIndexes();

	if(action == actionSetTo0)
	{
		foreach(const QModelIndex& index, indexes)
			func->setValue(kmap->getMapValue(index.column(), index.row()), 0);
	}
	else if(action == actionSetTo1)
	{
		foreach(const QModelIndex& index, indexes)
			func->setValue(kmap->getMapValue(index.column(), index.row()), 1);
	}
	else if(action == actionSetToDontCare)
	{
		foreach(const QModelIndex& index, indexes)
			func->setValue(kmap->getMapValue(index.column(), index.row()), 2);
	}
}
void KarnaughMapWidget::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		if(e->type() == QEvent::LanguageChange)
		{
			actionSetTo1->setText(tr("Set to 1"));
			actionSetTo0->setText(tr("Set to 0"));
			actionSetToDontCare->setText(tr("Set to \"don\'t care\""));
		}
	}
	QWidget::changeEvent(e);
}
void KarnaughMapWidget::slotTableWidget_customContextMenuRequested(const QPoint& pos)
{
	QModelIndex index = tableView->indexAt(pos);
	if(index.isValid())
	{
		QMenu menu;

		menu.addAction(actionSetTo1);
		menu.addAction(actionSetTo0);
		menu.addAction(actionSetToDontCare);

		menu.exec(QCursor::pos());
	}
}
void KarnaughMapWidget::slotKarnaughMap_solved(bool sop)
{
	Q_UNUSED(sop);
	model->reset();
}
void KarnaughMapWidget::slotConfig_typeOfSolutionChanged(int type)
{
	emit onTypeOfSolutionChanged(type);
}
void KarnaughMapWidget::slotConfig_numberOfVariablesChanged(int number)
{
	emit onNumberOfVariablesChanged(number);
}



