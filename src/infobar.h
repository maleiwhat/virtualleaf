/*
 *
 *  $Id$
 *
 *  This file is part of the Virtual Leaf.
 *
 *  The Virtual Leaf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The Virtual Leaf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the Virtual Leaf.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2010 Roeland Merks.
 *
 */


#ifndef _INFOBAR_H_
#define _INFOBAR_H_


#include <q3mainwindow.h>
#include <QLabel>
#include <QBoxLayout>

class InfoBar : public Q3DockWindow {

	Q_OBJECT
public:

	InfoBar(void) : Q3DockWindow() { 
		
		virtleaf = new QLabel();
		SetText("undefined");
		
		setHorizontalStretchable(true);
		//dockwindow->boxLayout()->addWidget(viblab);//,Qt::AlignLeft);
		boxLayout()->addStretch();
		boxLayout()->addWidget(virtleaf);//, Qt::AlignHCenter);
		boxLayout()->addStretch();
	
		//dockwindow->boxLayout()->addWidget(psblab);//, Qt::AlignRight);
	}
	
	void SetText(QString text) {
		virtleaf->setText(QString("<h1>The Virtual Leaf</h1>\n<center><b>Model:</b> <i>%1</i></center>").arg(text));
	}
	
private:
	//Q3DockWindow *dockwindow;
	QLabel *virtleaf;
};

#endif