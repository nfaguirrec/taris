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
#include "Triangle.h"

#include <set>
#include <iostream>
using namespace std ;

Triangle::Triangle()
{
	this->id = -1 ;
	this->p1 = Point() ;
	this->p2 = Point() ;
	this->p3 = Point() ;
	this->processed = false ;
}

Triangle::Triangle( int id, Point p1, Point p2, Point p3 )
{
	this->id = id ;
	this->p1 = p1 ;
	this->p2 = p2 ;
	this->p3 = p3 ;
	this->processed = false ;
}

Triangle::Triangle( const Triangle& tr )
{
	this->id = tr.getId() ;
	this->p1 = tr.getPoint1() ;
	this->p2 = tr.getPoint2() ;
	this->p3 = tr.getPoint3() ;
	this->processed = tr.isProcessed() ;
}

bool Triangle::operator == ( const Triangle& tr )
{
	if( ( p1 == tr.getPoint1() ) && ( p2 == tr.getPoint2() ) && ( p3 == tr.getPoint3() ) )
		return true ;
	
	return false ;
}

bool operator < ( const Triangle& tr1, const Triangle& tr2 ){
	if( ( tr1.getId() < tr2.getId() ) )
		return true ;
	
	return false ;
}

Triangle::~Triangle()
{
}

void Triangle::setId( int id ){
	this->id = id ;
}

void Triangle::setPoint1( Point p1 )
{
	this->p1 = p1 ;
}

void Triangle::setPoint2( Point p2 )
{
	this->p2 = p2 ;
}

void Triangle::setPoint3( Point p3 )
{
	this->p3 = p3 ;
}

int Triangle::getId() const
{
	return id ;
}

Point Triangle::getPoint1() const
{
	return p1 ;
}

Point Triangle::getPoint2() const
{
	return p2 ;
}

Point Triangle::getPoint3() const
{
	return p3 ;
}

bool Triangle::isProcessed() const
{
	return processed;
}

void Triangle::setProcessed( bool processed )
{
	this->processed = processed;
}

/**
 * Verifica que el punto p está contenido en el triangulo
 * comparando solo sus ids
 * @param p Punto a ser verificado
 * @return true si el punto está contenido en el triángulo o de lo contrario false
 */
bool Triangle::contains( Point p )
{
	if( p.getId() == p1.getId() )
		return true ;
	else if( p.getId() == p2.getId() )
		return true ;
	else if( p.getId() == p3.getId() )
		return true ;
	else
		return false ;
}

/**
 * Verifica que el punto con id pointId está contenido en el triangulo
 * comparando los ids de los puntos que la componen
 * @param pointId id del punto a ser verificado
 * @return true si el punto con id pointId está contenido en el triángulo o de lo contrario false
 */
bool Triangle::contains( int pointId )
{
	if( pointId == p1.getId() )
		return true ;
	else if( pointId == p2.getId() )
		return true ;
	else if( pointId == p3.getId() )
		return true ;
	else
		return false ;
}
