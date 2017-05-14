/***************************************************************************
 *   Copyright (C) 2007 by Universidad Nacional de Colombia                *
 *   http://www.unal.edu.co                                                *
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
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"

#define TriangleList list<Triangle>
#define TriangleVector vector<Triangle>

/**
 *  @brief Crea un triángulo, compuesto por tres puntos ( Point )
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class Triangle{
	public:
		Triangle();
		Triangle( int id, Point p1, Point p2, Point p3 );
		Triangle( const Triangle& tr );
		bool operator == ( const Triangle& p ) ;
		friend bool operator < ( const Triangle& tr1, const Triangle& tr2 ) ;
		~Triangle();
		
		void setId( int id ) ;
		void setPoint1( Point p1 ) ;
		void setPoint2( Point p2 ) ;
		void setPoint3( Point p3 ) ;
		
		int getId() const ;
		Point getPoint1() const ;
		Point getPoint2() const ;
		Point getPoint3() const ;
		
		void setProcessed( bool processed );
		bool isProcessed() const;
		
		bool contains( Point p ) ;
		bool contains( int pointId ) ;
		
	private:
		int id ;
		Point p1 ;
		Point p2 ;
		Point p3 ;
		bool processed ;
};

#endif
