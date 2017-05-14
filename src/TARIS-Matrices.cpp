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
#include <iomanip>
#include <vector>
using namespace std;

#include "HyperSurface.h"
#include "Surface.h"
#include "TarisApplication.h"
#include "Tree.h"

namespace Programs{

/**
 *  @brief Programa genera la matriz de similitud de un conjunto moléulas desde sus archivos CUBE
 *
 *  Este es el programa principal, el cual a través paráetros de la línea de comandos
 *  permite generar la matriz de similaridad para un conjunto dado de moleculas, listadas
 *  en un archivo
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class TARIS_Matrices{
	public:
		static string usage()
		{
			system("clear") ;
			return
			"============================\n"
			"TARIS-Matrices (" __DATE__ ")\n"
			"============================\n"
			"\n"
			"Produces a text file containing distance and similiraty matrices\n"
			"for a given set of molecules, taking as parameters a text file with \n"
			"the list of .cube files for each molecule, the initial cutoff, the \n"
			"final cutoff and the step-size for the scan\n"
			"\n"
			"Syntax:\n"
			"   $ TARIS-Matrices -i list [PARAMETERS]\n"
			"\n"
			"Required parameters:\n"
			"   -i   list\n"
			"           This is the name of the file containing the name of the\n"
			"           cube/gml files to be analyzed\n"
			"\n"
			"Optional parameters:\n"
			"   -t   format\n"
			"           Specifies the input data type: cube or gml files\n"
			"           (default=cube)\n"
			"   -b   cutoffBegin\n"
			"           Initial cutoff for the scan\n"
			"           (default=-0.1)\n"
			"   -e   cutoffEnd\n"
			"           Final cutoff for the scan\n"
			"           (default=-0.07)\n"
			"   -s   stepSize\n"
			"           Step-size for the scan\n"
			"           (default=-0.005)\n"
			"   -m   type\n"
			"           Type of matrix to be generated: distance, similarity or both\n"
			"           (default=both)\n"
			"   -o   output\n"
			"           Name of the output file\n"
			"           (default=screen)\n"
			"\n"
			"For example:\n"
			"   Generating matrices from .cube files\n"
			"      $ find . -name \"*.cube\" > list\n"
			"      $ TARIS-Matrices -i list -o matrices.dat\n"
			"   Generating matrices from .gml files\n"
			"      $ find . -name \"*.gml\" > list\n"
			"      $ TARIS-Matrices -i list -type gml -o matrices.dat\n"
			"\n"
			"Authors:\n"
			"    Nestor F. Aguirre, Ray M. Marin and Edgar E. Daza\n"
			"    Universidad Nacional de Colombia\n"
			;
		}
		
		static string extractFileName( const string& path )
		{
			char pathTMP[256] ;
			strcpy( pathTMP, path.c_str() ) ;
			vector<string> tokens ;
			
			char* pch = strtok( pathTMP, "/." );
			while( pch != NULL )
			{
				tokens.push_back( pch ) ;
				pch = strtok( NULL, "/." );
			}
			
			return tokens[ tokens.size() - 2 ] ;
		}
		
		static void printMatrix( double** matrix, int size, vector< string > moleculesName, ostream& os )
		{
			os.setf( ios::fixed ) ;
			os.precision(6) ;
			
			string filename ;
			int spaceForName = 15 ;
			
			for (int i = 0; i< size; i++){
				
				filename = extractFileName( moleculesName[i] ) ;
				
				if( filename.length() <= spaceForName ){
					os << setw(spaceForName) << extractFileName( moleculesName[i] ) << " :\t" ;
				}else{
					os << setw(spaceForName-3) << extractFileName( moleculesName[i] ).substr(0, spaceForName-3) << "... :\t" ;
				}
				
				for (int j = 0; j< size; j++){
					os << setw(12) << matrix[i][j] ;
				}
				os << endl ;
			}
		}
		
		static int main( int argc, char **argv )
		{
			
			if( argc > 1 ){
				
				string cubeList = TarisApplication::extractParameter( argc, argv, "-i" ) ;
				string type = TarisApplication::extractParameter( argc, argv, "-t", "cube" ) ;
				double cutoffBegin = atof( TarisApplication::extractParameter( argc, argv, "-b", "-0.1" ) ) ;
				double cutoffEnd = atof( TarisApplication::extractParameter( argc, argv, "-e", "-0.07" ) ) ;
				double stepSize = atof( TarisApplication::extractParameter( argc, argv, "-s", "0.005" ) ) ;
				string matrix = TarisApplication::extractParameter( argc, argv, "-m", "both" ) ;
				string outputFileName = TarisApplication::extractParameter( argc, argv, "-o", "screen" ) ;
				
				ifstream file( cubeList.c_str() ) ;
				
				//Este arreglo contiene los nombres de las moleculas
				vector< string > moleculesName ;
				string filenameTMP ;
	
				while( !file.eof() ) {
		
					file >> filenameTMP ;
					moleculesName.push_back( filenameTMP ) ;
				}
				
				int size = moleculesName.size() - 1 ;
				
				//Se crea la matriz de distancias
				double** distance = new double*[ size ];
				for (int i = 0; i< size; i++){
					distance[i] = new double[ size ] ; 
				}
	
				//Se crea la matriz de similitud
				double** similarity = new double*[ size ];
				for (int i = 0; i< size; i++){
					similarity[i] = new double[ size ] ; 
				}
				
				vector<Tree> treeVector ;
				
				if( type == "cube" ){
					
					HyperSurface hs ;	
					/*******************************************************
					* Se genera el arbol de cada molecula y se guarda en
					* un vector de arboles treeVector
					*/
					for (int i=0 ; i<size; i++){
						cout << "BUILDING TREE FOR: (" << i + 1 << ") " << extractFileName( moleculesName[i] ) << endl ;
						hs.load( moleculesName[i] ) ;
						treeVector.push_back( hs.buildAreaTree( cutoffBegin, cutoffEnd, stepSize ) ) ;
					}
					
				}else if( type == "gml" ){
					
// 					Tree treeTMP ;
					
					/*******************************************************
					* Se carga el arbol de cada molecula y se guarda en
					* un vector de arboles treeVector
					*/
					for (int i=0 ; i<size; i++){
						Tree treeTMP ;
						treeTMP.load( moleculesName[i] ) ;
						treeVector.push_back( treeTMP ) ;
// 						treeTMP.clear() ;
					}
					
				}else{
					cout << "Ilegal value for parameter -type; please use \"cube\" or \"gml\"" << endl ;
					exit(-1) ;
				}
				
				cout << endl ;
				cout << "GENERATING DISTANCE MATRIX ... "  ;
			
				double max = 0.0 ;
			
				// Se calcula la matriz de distancia
				for (int i = 0; i< size; i++){
		
					if ( i < size-1 ) {
			
						for (int j = i+1; j< size; j++){
				
							distance[i][j] = Tree::distance( treeVector[i], treeVector[j] ) ; 
							distance[j][i] = distance[i][j] ;
							if(distance[i][j] > max){
								max = distance[i][j] ;
							}
						}
					}
				}
				
				cout << "OK" << endl ;
				cout << "GENERATING SIMILARITY PERCENTAGE MATRIX ... "  ;
				
				// Se calcula la matriz de similitud
				if (max >= 1e-6){
					for (int i = 0; i< size; i++){
						for (int j = 0; j< size; j++){
							similarity[i][j] = ( 1-(distance[i][j] / max)) *100.0 ; 
						}
					}
				}else{
					for (int i = 0; i< size; i++){
						for (int j = 0; j< size; j++){
							similarity[i][j] = 100.0 ; 
						}
					}
				}
				
				cout << "OK" << endl ;
				cout << endl ;
				
				if( outputFileName.compare("screen") == 0  ){
					
					if( matrix.compare("distance") == 0  || matrix.compare("both") == 0 ){
						cout << "DISTANCE MATRIX 'D'" << endl ;
						cout << "-------------------" << endl ;
	
						printMatrix( distance, size, moleculesName, cout ) ;
						cout << endl ;
						cout << "THE MAXIMUM DISTANCE IS: " << max << endl ;
						cout << endl ;
					}
					
					if( matrix.compare("similarity") == 0 || matrix.compare("both") == 0 ){
						cout << "SIMILARITY PERCENTAGE MATRIX 'S'" << endl ;
						cout << "--------------------------------" << endl ;
	
						printMatrix( similarity, size, moleculesName, cout ) ;
					}

				}else{
					ofstream file( outputFileName.c_str() ) ;
					
					if( matrix.compare("distance") == 0  || matrix.compare("both") == 0 ){
						file << "DISTANCE MATRIX 'D'" << endl ;
						file << "-------------------" << endl ;
	
						printMatrix( distance, size, moleculesName, file ) ;
						file << endl ;
						file << "THE MAXIMUM DISTANCE IS: " << max << endl ;
						file << endl ;
					}
					
					if( matrix.compare("similarity") == 0 || matrix.compare("both") == 0 ){
						file << "SIMILARITY PERCENTAGE MATRIX 'S'" << endl ;
						file << "--------------------------------" << endl ;
	
						printMatrix( similarity, size, moleculesName, file ) ;
					}
					
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
	return Programs::TARIS_Matrices::main( argc, argv ) ;
}
