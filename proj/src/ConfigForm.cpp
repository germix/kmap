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
#include "ConfigForm.h"
#include "ui_ConfigForm.h"

#include <QSettings>

ConfigForm::ConfigForm(int typeOfSolution, int numberOfVariables, QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::ConfigForm)
{
	ui->setupUi(this);
	ui->cboTypeOfSolution->setCurrentIndex(typeOfSolution);
	ui->spinBoxNumberOfVariables->setValue(numberOfVariables);

	connect(ui->cboTypeOfSolution, SIGNAL(currentIndexChanged(int)), this, SLOT(slotComboBox_currentIndexChanged(int)));
	connect(ui->spinBoxNumberOfVariables, SIGNAL(valueChanged(int)), this, SLOT(slotSpinBox_valueChanged(int)));
}
ConfigForm::~ConfigForm()
{
	delete ui;
}
void ConfigForm::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		switch(e->type())
		{
			case QEvent::LanguageChange:
				ui->retranslateUi(this);
			default:
				break;
		}
	}
	QWidget::changeEvent(e);
}
void ConfigForm::slotSpinBox_valueChanged(int value)
{
	emit onNumberOfVariablesChanged(value);
}
void ConfigForm::slotComboBox_currentIndexChanged(int index)
{
	emit onTypeOfSolutionChanged(index);
}
