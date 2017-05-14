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
 *  @brief Programa que genera archivos GML de arboles desde archivos CUBE
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class TARIS_BuildTree{
	public:
		static string usage()
		{
			system("clear") ;
			return
			"=============================\n"
			"TARIS-BuildTree (" __DATE__ ")\n"
			"=============================\n"
			"\n"
			"Produces files in GML format containing the information that allows \n"
			"to visualize the corresponding molecule's tree, taking as parameters \n"
			"the .cube file, the initial cutoff, the final cutoff and the step-size\n"
			"for the scan \n"
			"\n"
			"Syntax:\n"
			"   $ TARIS-BuildTree -c file [PARAMETERS]\n"
			"\n"
			"Required parameters:\n"
			"   -c   file\n"
			"           This is the name of the cube file containing the electrostatic\n"
			"           potential data\n"
			"\n"
			"Optional parameters:\n"
			"   -b   cutoffBegin\n"
			"           Initial cutoff for the scan\n"
			"           (default=-0.1)\n"
			"   -e   cutoffEnd\n"
			"           Final cutoff for the scan\n"
			"           (default=-0.07)\n"
			"   -s   stepSize\n"
			"           Step-size for the scan\n"
			"           (default=-0.005)\n"
			"   -o   output\n"
			"           Name of the gml output file\n"
			"           (default=screen)\n"
			"\n"
			"For example:\n"
			"   $ TARIS-BuildTree -c file.cube -o file.gml\n"
			"\n"
			"Authors:\n"
			"   Nestor F. Aguirre, Ray M. Marin and Edgar E. Daza\n"
			"   Universidad Nacional de Colombia\n"
			;
		}
		
		static int main( int argc, char **argv )
		{
			
			if( argc > 1 ){
				
				string cubeFileName = TarisApplication::extractParameter( argc, argv, "-c" ) ;
				double cutoffBegin = atof( TarisApplication::extractParameter( argc, argv, "-b", "-0.1" ) ) ;
				double cutoffEnd = atof( TarisApplication::extractParameter( argc, argv, "-e", "-0.07" ) ) ;
				double stepSize = atof( TarisApplication::extractParameter( argc, argv, "-s", "0.005" ) ) ;
				string outputFileName = TarisApplication::extractParameter( argc, argv, "-o", "screen" ) ;
				
				HyperSurface hs ;
				hs.load( cubeFileName ) ;
				
				Tree output = hs.buildAreaTree( cutoffBegin, cutoffEnd, stepSize ) ;
				
				Tree::print( output, cout ) ;
				
				if( outputFileName != "screen" ){
					output.save( outputFileName.c_str() ) ;
				}else{
					cout << "---------------" << endl ;
					cout << "GML FILE" << endl ;
					cout << "---------------" << endl ;
					cout << endl ;
					output.save( &cout ) ;
				}
				
			}else{
				cout << usage() << endl ;
			}

			return EXIT_SUCCESS;
		}
};

}

int main( int argc, char **argv )
{
	return Programs::TARIS_BuildTree::main( argc, argv ) ;
}
