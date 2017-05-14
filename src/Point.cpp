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
#include "Point.h"

Point::Point()
{
	this->id = -1 ;
	this->x = 0.0 ;
	this->y = 0.0 ;
	this->z = 0.0 ;
	processed = false ;
}

Point::Point( int id, double x, double y, double z )
{
	this->id = id ;
	this->x = x ;
	this->y = y ;
	this->z = z ;
	processed = false ;
}

Point::Point( double x, double y, double z )
{
	this->id = -1 ;
	this->x = x ;
	this->y = y ;
	this->z = z ;
	processed = false ;
}

Point::Point( const Point& p )
{
	this->id = p.getId() ;
	this->x = p.getX() ;
	this->y = p.getY() ;
	this->z = p.getZ() ;
	this->processed = p.isProcessed() ;
}

void Point::operator = ( const Point& p )
{
	this->id = p.getId() ;
	this->x = p.getX() ;
	this->y = p.getY() ;
	this->z = p.getZ() ;
	this->processed = p.isProcessed() ;
}

/**
 * Dos puntos son igules si y solo si sus ids son iguales
 * @param p Punto a ser comparado
 * @return true si son iguales o de lo contrario false
 */
bool Point::operator == ( const Point& p )
{
	return ( this->id == p.getId() ) ? true : false ;
}

Point::~Point()
{
}

int Point::getId() const
{
	return id;
}

void Point::setId( int id )
{
	this->id = id;
}

double Point::getX() const
{
	return x;
}

void Point::setX( double x )
{
	this->x = x;
}

double Point::getY() const
{
	return y;
}

void Point::setY( double y )
{
	this->y = y;
}

double Point::getZ() const
{
	return z;
}

void Point::setZ( double z )
{
	this->z = z;
}


bool Point::isProcessed() const
{
	return processed;
}


void Point::setProcessed( bool processed )
{
	this->processed = processed;
}


