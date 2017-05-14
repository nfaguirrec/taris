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
#include <string>
#include <cstdlib>
#include <vector>
using namespace std;

#include "HyperSurface.h"
#include "Surface.h"
#include "TarisApplication.h"
#include "Tree.h"

namespace Programs{

/**
 *  @brief Programa que genera archivos OOGL de isosuperficies desde archivos CUBE
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class TARIS_BuildIsosurface{
	public:
		static string usage()
		{
			system("clear") ;
			return
			"===================================\n"
			"TARIS-BuildIsosurface (" __DATE__ ")\n"
			"===================================\n"
			"\n"
			"Produces files in OOGL format containing the information that allows \n"
			"to visualize an isopotential surface for a given cutoff, from the .cube\n"
			"file. The OOGL files can be visualized with the \"geomview\" program:\n"
			"(http://www.geom.uiuc.edu/software/geomview/)\n"
			"\n"
			"Syntax:\n"
			"   $ TARIS-BuildIsosurface -c file [PARAMETERS]\n"
			"\n"
			"Required parameters:\n"
			"   -c   file\n"
			"           This is the name of the cube file containing the electrostatic\n"
			"           potential data\n"
			"\n"
			"Optional parameters:\n"
			"   -b   cutoff\n"
			"           Potential value for the surface calculation potential data\n"
			"           (default=-0.07)\n"
			"   -o   output\n"
			"           Name of the oogl output file\n"
			"           (default=screen)\n"
			"\n"
			"For example:\n"
			"   $ TARIS-BuildIsosurface -c file.cube -b -0.1 -o file.oogl\n"
			"   $ geomview -nopanels file.oogl\n"
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
				double cutoff = atof( TarisApplication::extractParameter( argc, argv, "-b", "-0.07" ) ) ;
				string outputFileName = TarisApplication::extractParameter( argc, argv, "-o", "screen" ) ;
				
				HyperSurface hs ;
				hs.load( cubeFileName ) ;
				
				Surface output = hs.getIsosurface( cutoff ) ;
				
				if( outputFileName == "screen" ){
					output.printOogl( cout ) ;
				}else{
					ofstream file( outputFileName.c_str() ) ;
					output.printOogl( file ) ;
					file.close() ;
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
	return Programs::TARIS_BuildIsosurface::main( argc, argv ) ;
}
