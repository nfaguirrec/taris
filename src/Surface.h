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
#ifndef SURFACE_H
#define SURFACE_H

#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std ;

#include "Point.h"
#include "Triangle.h"

#define SurfacesVector vector<Surface>

/**
 *  @brief Crea una superficie, la cual esta compuesta por puntos y triángulos
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class Surface : pair< PointVector, TriangleVector >{
	public:
		Surface();
		Surface( const vector<Point>& lpo, const vector<Triangle>& ltri );
		Surface( const Surface& surf );
		~Surface();
		
		PointVector getPoints() const ;
		TriangleVector getTriangles() const ;
		
		void setPoints( const PointVector& pv ) ;
		void setTriangles( const TriangleVector& tv ) ;
		
		void saveOogl( string filename ) ;
		void printOogl( ostream& os ) ;
		SurfacesVector computeComponents() ;
		bool contains( Surface s ) ;
		bool isContainedIn( Surface s , int a, int b) ;
		
		double computeArea() ;
// 		double getArea() ;

	private:
// 		double area ;
};

#endif
