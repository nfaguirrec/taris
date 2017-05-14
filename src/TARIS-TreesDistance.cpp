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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

#include "HyperSurface.h"
#include "Surface.h"
#include "TarisApplication.h"
#include "Tree.h"

namespace Programs{

/**
 *  @brief Programa calcula la distancia entre dos árboles generados con TARIS-BuildTree
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class TARIS_TreesDistance{
	public:
		static string usage()
		{
			system("clear") ;
			return
			"=================================\n"
			"TARIS-TreesDistance (" __DATE__ ")\n"
			"=================================\n"
			"\n"
			"Returns the distance between two trees that have been previously generated\n"
			"with the TARIS-BuildTree program\n"
			"\n"
			"Syntax:\n"
			"   $ TARIS-TreesDistance file1 file2\n"
			"\n"
			"Required parameters:\n"
			"   file1\n"
			"      This is the name of the first tree in gml format\n"
			"   file2\n"
			"      This is the name of the second tree in gml format\n"
			"\n"
			"For example:\n"
			"\n"
			"   $ TARIS-BuildTree -c file1.cube -o file1.gml\n"
			"   $ TARIS-BuildTree -c file2.cube -o file2.gml\n"
			"   $ TARIS-TreesDistance file1.gml file2.gml\n"
			"\n"
			"Authors:\n"
			"   Nestor F. Aguirre, Ray M. Marin and Edgar E. Daza\n"
			"   Universidad Nacional de Colombia\n"
			;
		}
		
		static int main( int argc, char **argv )
		{
			
			if( argc == 3 ){
				
				string gmlFileName1 = argv[1] ;
				string gmlFileName2 = argv[2] ;
				
				Tree tree1 ;
				tree1.load( gmlFileName1 ) ;
				
				Tree tree2 ;
				tree2.load( gmlFileName2 ) ;
				
				cout << "DISTANCE IS: " << Tree::distance( tree1, tree2 ) << endl ;
				
			}else{
				cout << usage() << endl ;
			}
			
			return EXIT_SUCCESS;
		}
};

}

int main( int argc, char **argv )
{
	return Programs::TARIS_TreesDistance::main( argc, argv ) ;
}
