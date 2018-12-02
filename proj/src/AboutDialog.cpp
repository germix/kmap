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

#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <QtGui>

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

	// Mostrar información
	{
		QString s;
		QFileInfo info(qApp->applicationFilePath());

		s += tr("Time") + ": ";
		s += info.lastModified().time().toString();
		s += "\n";
		s += tr("Date") + ": ";
		s += info.lastModified().date().toString();
		s += "\n";
		s += tr("Version") + ": ";
		s += QString::number(MAJOR_VERSION) + "." + QString::number(MINOR_VERSION);
		s += "\n";
		s += tr("Developer") + ": Germán Martínez";
		s += "\n";
		s += tr("Original developer") + ": Robert Kovacevic";
		ui->label->setText(s);
	}
	// Ajustar tamaño
	{
		QSize s = sizeHint();
		setMinimumSize(s);
		setMaximumSize(s);
	}
	ui->lblLink1->setTextInteractionFlags(Qt::TextBrowserInteraction);
	ui->lblLink1->setOpenExternalLinks(true);
	ui->lblLink2->setTextInteractionFlags(Qt::TextBrowserInteraction);
	ui->lblLink2->setOpenExternalLinks(true);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
