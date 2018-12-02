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
#include "TruthTableWidget.h"
#include <math.h>
#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
#include <QHeaderView>
#include "TruthTableModel.h"

#include <QEvent>
#include <QHeaderView>

TruthTableWidget::TruthTableWidget(BooleanFunction* bf)
{
	tableView = new QTableView();
	QVBoxLayout* l = new QVBoxLayout();
	l->setContentsMargins(0, 0, 0, 0);
	l->addWidget(tableView);
	setLayout(l);

	// ...
	func = bf;

	//
	// Tabla
	//
	tableView->setFont(QFont("Arial", 7));
	tableView->setModel(model = new TruthTableModel(bf));
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
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
}
TruthTableWidget::~TruthTableWidget()
{
}
void TruthTableWidget::setShowZeros(bool show)
{
	model->setShowZeros(show);
}
QAction* TruthTableWidget::newAction(const QString& label)
{
	QAction* action = new QAction(label, this);
	connect(action, SIGNAL(triggered()), this, SLOT(slotAction()));
	return action;
}
void TruthTableWidget::changeEvent(QEvent* e)
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
void TruthTableWidget::slotAction()
{
#if 1
	int value = 0;
	QAction* action = (QAction*)sender();

	if(action == actionSetTo0)
	{
		value = 0;
	}
	else if(action == actionSetTo1)
	{
		value = 1;
	}
	else if(action == actionSetToDontCare)
	{
		value = 2;
	}
	QItemSelectionModel* sel = tableView->selectionModel();
	QModelIndexList      indexes = sel->selectedIndexes();

	for(QModelIndexList::ConstIterator it = indexes.constBegin(); it != indexes.constEnd(); it++)
	{
		func->setValue((*it).row(), value);
	}
#else
	QAction* action = qobject_cast<QAction*>(sender());
	QModelIndex index = tableView->currentIndex();

	if(action == actionSetTo0)
	{
		func->setValue(index.row(), 0);
	}
	else if(action == actionSetTo1)
	{
		func->setValue(index.row(), 1);
	}
	else if(action == actionSetToDontCare)
	{
		func->setValue(index.row(), 2);
	}
#endif
}
void TruthTableWidget::slotTableWidget_customContextMenuRequested(const QPoint& pos)
{
#if 1
	Q_UNUSED(pos);
	QMenu menu;

	menu.addAction(actionSetTo1);
	menu.addAction(actionSetTo0);
	menu.addAction(actionSetToDontCare);

	menu.exec(QCursor::pos());
#else
	QModelIndex index = tableView->indexAt(pos);
	if(index.isValid())
	{
		qDebug("index.column()=%d", index.column());
		if(index.column() == tableView->model()->columnCount()-1)
		{
			QMenu menu;

			menu.addAction(actionSetTo1);
			menu.addAction(actionSetTo0);
			menu.addAction(actionSetToDontCare);

			menu.exec(QCursor::pos());
		}
	}
#endif
}



