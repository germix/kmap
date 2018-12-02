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
#ifndef LANGUAGES_H
#define LANGUAGES_H
#include <QObject>
#include <QTranslator>

class QMenu;
class QAction;

class Languages : public QObject
{
	Q_OBJECT
	QString		baseName;
	QString		languagesPath;
	QString		currentLanguage;
	QTranslator qtTranslator;
	QTranslator	appTranslator;
public:
    Languages();
	~Languages();
public:
	void load(QString language);
	void init(QMenu* menu, QString basePath, QString baseName, QString initialLocale);
	QString language() const { return currentLanguage; }
private:
	void loadTranslator(QTranslator* t, const QString& fname);
private slots:
	void actionTriggered(QAction* action);
};

#endif // LANGUAGES_H
