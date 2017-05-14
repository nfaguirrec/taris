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
#include "NodeWeight.h"

NodeWeight::NodeWeight()
{
	root = false ;
	keyroot = false ;
	externalId = -1 ;
	potentialValue = INFINITY ;
	weightValue = INFINITY ;
	leftmost = -1 ;
	deepestNoNodes = -1 ;
}

// NodeWeight::NodeWeight( const NodeWeight& nw )
// {
// 	this->keyroot = nw.isKeyroot() ;
// 	this->externalId = nw.getExternalId() ;
// 	this->weightValue = nw.getWeightValue() ;
// 	this->leftmost = nw.getLeftmost() ;
// }

NodeWeight::NodeWeight( double weightValue, bool root )
{
	this->root = root ;
	keyroot = false ;
	externalId = -1 ;
	potentialValue = INFINITY ;
	this->weightValue = weightValue ;
	leftmost = -1 ;
	deepestNoNodes = -1 ;
}

NodeWeight::NodeWeight( double weightValue, bool root, int deepestNoNodes )
{
	this->root = root ;
	keyroot = false ;
	externalId = -1 ;
	potentialValue = INFINITY ;
	this->weightValue = weightValue ;
	leftmost = -1 ;
	this->deepestNoNodes = deepestNoNodes ;
}

NodeWeight::NodeWeight( double potentialValue, double weightValue ){
	this->root = false ;
	keyroot = false ;
	externalId = -1 ;
	this->potentialValue = potentialValue ;
	this->weightValue = weightValue ;
	leftmost = -1 ;
	deepestNoNodes = -1 ;
}

NodeWeight::NodeWeight( double potentialValue, double weightValue, bool root, int deepestNoNodes ){
	this->root = root ;
	keyroot = false ;
	externalId = -1 ;
	this->potentialValue = potentialValue ;
	this->weightValue = weightValue ;
	leftmost = -1 ;
	this->deepestNoNodes = deepestNoNodes ;
}

// bool operator == ( const NodeWeight& nw1, const NodeWeight& nw2 )
// {
// 	if ( ( nw1.keyroot == nw2.keyroot ) && ( nw1.externalId == nw2.externalId ) &&
// 	       (nw1.weightValue == nw2.weightValue ) && ( nw1.leftmost == nw2.leftmost ) )
// 		return true ;
// 	else
// 		return false ;
// }
// 
// bool operator < (const NodeWeight& nw1, const NodeWeight& nw2)
// {
// 	if( nw1.getExternalId() < nw2.getExternalId() )
// 		return true ;
// 	else
// 		return false ;
// }

NodeWeight::~NodeWeight()
{
}

int NodeWeight::getExternalId() const
{
	return externalId;
}

void NodeWeight::setExternalId(const int& value)
{
	externalId = value;
}

bool NodeWeight::isKeyroot() const
{
	return keyroot;
}

void NodeWeight::setKeyroot(bool value)
{
	keyroot = value;
}

int NodeWeight::getLeftmost() const
{
	return leftmost;
}

void NodeWeight::setLeftmost(const int& value)
{
	leftmost = value;
}

double NodeWeight::getPotentialValue() const
{
	return potentialValue;
}

void NodeWeight::setPotentialValue( const double& potentialValue)
{
	this->potentialValue = potentialValue ;
}

double NodeWeight::getWeightValue() const
{
	return weightValue;
}

void NodeWeight::setWeightValue( const double& weightValue)
{
	this->weightValue = weightValue ;
}

void NodeWeight::setRoot( bool root )
{
	this->root = root ;
}

bool NodeWeight::isRoot() const
{
	return root ;
}

int NodeWeight::getDeepestNoNodes() const
{
	return deepestNoNodes ;
}

void NodeWeight::setDeepestNoNodes( const int& value )
{
	deepestNoNodes = value;
}
