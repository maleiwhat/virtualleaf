/*
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

#include <string>
#include <QGraphicsScene>
#include <QVector>
#include "canvas.h"
#include "wallitem.h"
#include "parameter.h"
#include "node.h"
#include "transporterdialog.h"

static const std::string _module_id("$Id$");

WallItem::WallItem( Wall *w, int wallnumber, QGraphicsScene *canvas )
  : QGraphicsLineItem( 0, canvas ), SimItemBase( w, canvas){

  wn = wallnumber;

  extern Parameter par;
  //Wall *w=&getWall();
  
  // Draw amount of "PIN1"
  //double val = wn==1?(w->Transporters1(1)/par.Pi_tot)*255.:(w->Transporters2(1)/par.Pi_tot)*255.;
  
  /* if (val > 255 || val < 0 ) { 
     std::cerr << "val = " << val << endl;
     if (wn == 1) {
     std::cerr << "Transporters1(1) = " << w->Transporters1(1) << endl;
     } else {
     std::cerr << "Transporters2(1) = " << w->Transporters2(1) << endl;
     }
    
     }*/
  
  
	  setColor();
  
	  // line with "PIN1"is a bit inside the cell wall
	  Vector edgevec = (*(w->N2())) - (*(w->N1()));
	  Vector perp = edgevec.Normalised().Perp2D();
	  
	  Vector offs = Cell::Offset();
	  double factor = Cell::Factor();
	  
	  Vector from = ( offs + *(w->N1()) ) * factor + (wn==1?-1:1) * par.outlinewidth * 0.5 * factor * perp;
	  Vector to = ( offs + *(w->N2()) ) *factor + (wn==1?-1:1) * par.outlinewidth * 0.5 * factor * perp;
	  
	  
	  Vector tmp_centroid = ( *(w->N2()) + *(w->N1()) )/2.;
	  Vector centroid = ( offs + tmp_centroid ) * factor;
	  
	  QString text=QString("%1").arg(w->Index());

	  /*  if (0) {
		  QGraphicsSimpleTextItem *ctext = new QGraphicsSimpleTextItem ( text, 0, canvas );
		  ctext->setPen( QPen(QColor("orange")) );
		  ctext->setZValue(20);
		  ctext->setFont( QFont( "Helvetica", par.nodenumsize, QFont::Bold) );
		  //ctext->setTextFlags(Qt::AlignCenter);
		  ctext->show();
		  ctext ->setPos(centroid.x,
						 centroid.y );
	  }*/
	  
	  setLine(( from.x ),
			  ( from.y ),
			  ( to.x ),
			  ( to.y ) );
	  setZValue(12);
  }


void WallItem::setColor(void) {

  QColor diffcolor;
  static const QColor purple("Purple");
  static const QColor blue("blue");
  
  Wall *w=&getWall();
  double tr = wn==1?w->Transporters1(1):w->Transporters2(1);
  CellBase *c = wn==1?w->C1():w->C2();
  diffcolor.setRgb( (int)( ( tr / (1 + tr) )*255.), 0, 0);
  //diffcolor.setRgb( (int)( ((wn==1?w->Transporters1(1):w->Transporters2(1)))*255.), 0, 0);
  /* if (wn==1) {
     cerr << "Transporter: " << w->Transporters1(1) << endl;
     } else {
     cerr << "Transporter: " << w->Transporters2(1) << endl;
     }*/
  
  if (w->AuxinSource() && c->BoundaryPolP()) {
    setPen( QPen(purple , par.outlinewidth) );
  } else {
    if (w->AuxinSink() && c->BoundaryPolP()) {
      setPen( QPen(blue, par.outlinewidth));
    } else {
      setPen (QPen(diffcolor, par.outlinewidth) );
    }
  }
  
//   if (c->BoundaryPolP()) {
//     setPen(QPen(QColor("red"), 20));
//   }
}

void WallItem::OnClick(QMouseEvent *e) {
  

	Wall *w=&getWall();
	cerr << "Wall ID = " << w->Index() << ", this = " << w << "\n";
	cerr << "Wall item = " << this << "\n";
	cerr << "C1 = " << w->C1()->Index() << ", C2 = " << w->C2()->Index() << endl;
	cerr << "N1 = " << w->N1()->Index() << ", N2 = " << w->N2()->Index() << endl;
	//double tr = wn==1?w->Transporters1(1):w->Transporters2(1);
	CellBase *c = wn==1?w->C1():w->C2();

	TransporterDialog dialog(w, c, wn);
	dialog.exec();

	if (e->button() == Qt::RightButton) {
		QString message;
		if (wn==1) {
			message=QString("Transporter 1 = %1, color = %2, length = %3\n").arg(w->Transporters1(1)).arg(pen().color().red()).arg(getWall().Length());
		} else {
			message=QString("Transporter 2 = %1, color = %2, length = %3\n").arg(w->Transporters2(1)).arg(pen().color().red()).arg(getWall().Length());
		}
		
		//extern MainBase *main_window;
		//((Main *)main_window)->UserMessage(message);
		
	} else {
		if (e->button() == Qt::LeftButton) {
			if (c->BoundaryPolP()) {
				w->cycleWallType();
			} else {
				if (e->modifiers() == Qt::ShiftModifier) {
					wn==1?w->setTransporters1(1,0):w->setTransporters2(1,0);					
					
				} else {
					// set high amount of PIN1
					//cerr << "Setting PIN1\n";
					wn==1?w->setTransporters1(1,10):w->setTransporters2(1,10);
				}
			}
			setColor();
			update(boundingRect());
		} 
		/* else {
			if (e->button() == Qt::MidButton && (e->modifiers == Qt::ShiftModifier & Qt::ControlModifier)) {
				// hidden feature for correcting 
			}*/
		
	
	}
}
