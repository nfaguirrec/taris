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
#ifndef HYPERSURFACE_H
#define HYPERSURFACE_H

#include <string>
#include <list>
#include <vector>
using namespace std ;

#include "molekel.h"

#include "Surface.h"
#include "Triangle.h"

#include "Tree.h"

/**
 *  @brief Permite cargar archivos .cube y contriur iso-superficies
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class HyperSurface{
	public:
		HyperSurface( string name = "" );
		~HyperSurface();
		
		void load( const string& filename ) ;
		Surface getIsosurface( double cutoff, bool debug = false ) ;
		Tree buildAreaIMTree( double cutoffBegin, double cutoffEnd, double stepSize ) ;
		Tree buildAreaIATree( double cutoffBegin, double cutoffEnd, double stepSize ) ;
		Tree buildIMTree( double cutoffBegin, double cutoffEnd, double stepSize ) ;
		Tree buildIATree( double cutoffBegin, double cutoffEnd, double stepSize ) ;
		Tree buildAreaTree( double cutoffBegin, double cutoffEnd, double stepSize ) ;
		Tree buildCompleteTree( double cutoffBegin, double cutoffEnd, double stepSize ) ;
		Tree buildMayorBranch( double cutoffBegin, double cutoffEnd, double stepSize ) ;
		Tree buildWithoutMayorBranch( double cutoffBegin, double cutoffEnd, double stepSize ) ;
		
	private:
		string name ;
		Mol* mole ;
};

#endif

