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
#ifndef POINT_H
#define POINT_H

#define PointList list<Point>
#define PointVector vector<Point>

/**
 *  @brief Crea un punto 3D
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class Point{
	public:
		Point();
		Point( int id, double x, double y, double z );
		Point( double x, double y, double z );
		Point( const Point& p ) ;
		void operator = ( const Point& p ) ;
		bool operator == ( const Point& p ) ;
		~Point();

		void setId( int id );
		void setX( double x );
		void setY( double y );
		void setZ( double z );
		
		int getId() const;
		double getX() const;
		double getY() const;
		double getZ() const;
			
		void setProcessed( bool processed );
		bool isProcessed() const;	
		
	private:
		double x ;
		double y ;
		double z ;
		int id ;
		bool processed ;
};

#endif
