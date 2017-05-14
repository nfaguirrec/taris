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
#include "HyperSurface.h"

#include <iomanip>
using namespace std ;

#include <GTL/components.h>
#include "TarisApplication.h"

HyperSurface::HyperSurface( string name )
{
	this->name = name ;
}

HyperSurface::~HyperSurface()
{
	delete mole ;
}

/**
 * 
 * @param filename 
 */
void HyperSurface::load( const string& filename )
{
	mole = new Mol( filename.c_str() ) ;
	
	if( TarisApplication::Instance()->getDebugLevel() >= 1 )
		cerr << "Cargando archivo " << filename << " ... " ;
	
	read_gcube( mole, filename.c_str() ) ;
	
	if( TarisApplication::Instance()->getDebugLevel() >= 1 )
		cerr << "OK" << endl ;
}

Surface HyperSurface::getIsosurface( double cutoff, bool debug )
{
	Surface ouput ;
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
		cerr << endl ;
		cerr << "**************************************************************" << endl ;
		cerr << endl ;
		cerr << "                ALGORITMO DE TRIANGULARIZACION                " << endl ;
		cerr << "                ------------------------------                " << endl ;
		cerr << endl ;
	}
	
	list<Surfdot> dotList ;
	list<Tria> triList ;
	
	mole->cutoff = cutoff ;
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 )
		cerr << "Triangularizando superficie ... " ;
	
	/* ***************************************************************
	* Esta es la función de molekel que genera la lista de triangulos
	* y de pubntos
	*/
	
	cubes( mole, dotList, triList ) ;
		
	if( TarisApplication::Instance()->getDebugLevel() >= 2 )
		cerr << "OK" << endl ;
	
	TriangleVector ltri ;
	PointVector lpo ;
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
		cerr << "Reconstruyendo puntos ... " ;
	}
	
	int i=0 ;
	for( list<Surfdot>::iterator it = dotList.begin(); it != dotList.end(); it++, i++ ){
		lpo.push_back( Point( i, (*it).v[0], (*it).v[1], (*it).v[2] ) ) ;
	}
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
		cerr << "OK" << endl ;
			
		cerr << "Reconstruyendo triangulos ... " ;
	}
	
	i=0 ;
	for( list<Tria>::iterator it = triList.begin(); it != triList.end(); it++, i++ )
		ltri.push_back( Triangle( i, lpo[it->p1], lpo[it->p2], lpo[it->p3] ) ) ;
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
		cerr << "OK" << endl ;
		cerr << endl ;
		cerr << "**************************************************************" << endl ;
		cerr << endl ;
	}
	
	/******************************************
	* Se le asignan a la superficie la lista
	* de puntos, de triangulos
	*/
	ouput.setPoints( lpo ) ;
	ouput.setTriangles( ltri ) ;
	
	return ouput ;
}


Tree HyperSurface::buildAreaTree( double cutoffBegin, double cutoffEnd, double stepSize )
{
	if( cutoffBegin < 0.0 ){
		
		Tree tree ;
		
		Surface s ;  // Esta es una superficie temporal
		
		/***********************************************
		* En el trancurso de construcción del arbol
		* se debe tener el conjunto de componentes
		* a un valor de potencial x y almismo tiempo
		* al un valor de potencial x + stepSize,
		* para en un siguiente procedimiento verificar
		* la contenencia de unas con respecto a otras
		* y de esta manera crear las aristas. De igual
		* manera los nodos que representan
		*/
		SurfacesVector currentSurfaces ;
		SurfacesVector nextSurfaces ;
		
		map<int, node> currentNodeMap ;
		map<int, node> nextNodeMap ;
		//**********************************************
		
		//-----------13------------------24----------------
		cout << " +-------------+------------------------+" << endl ;
		cout << " |  Potential  |  Number of Components  |" << endl ;
		cout << " +-------------+------------------------+" << endl ;
		
		/**************************************************************
		* Se inicializa la primera fila de nodos, partiendo de las
		* componentes calculadas al valor de potencial cuttoffBegin
		*/
		s = this->getIsosurface( cutoffBegin ) ;
		currentSurfaces = s.computeComponents() ;
			
		
		int i = 0 ;
		for( SurfacesVector::iterator it1 = currentSurfaces.begin(); it1 != currentSurfaces.end(); it1++, i++ ){
						
			/*****************************************************
			* El weightValue ahora depende tanto del potencial como
			* del area superficial de las componentes
			*/
			node n = tree.newNode( NodeWeight( cutoffBegin, it1->computeArea() ) ) ;
			currentNodeMap[ i ] = n ;
			
		}
		
		cout.precision(5) ;
		cout.setf( ios::fixed ) ;
		if(currentSurfaces.size() != 0){
			cout << " | " << setw(11) << cutoffBegin << " | " << setw(22)<< currentSurfaces.size() << " |" << endl ;
		}else{
			cout << " | " << setw(11) << cutoffBegin << " | " << setw(22)<< currentSurfaces.size() << " |" << endl ;
		}
		
		
		
		for( double potential = cutoffBegin + stepSize; potential < cutoffEnd + 0.0005 ; potential += stepSize ){
			
			s = this->getIsosurface( potential ) ;
			nextSurfaces = s.computeComponents() ;
			
			nextNodeMap.clear() ;
			
			i = 0 ;
			for( SurfacesVector::iterator it2 = nextSurfaces.begin(); it2 != nextSurfaces.end(); it2++, i++ ){
				
				/*****************************************************
				* El weightValue ahora depende tanto del potencial como
				* del area superficial de las componentes
				*/
				node n = tree.newNode( NodeWeight( potential, it2->computeArea() ) ) ;
				nextNodeMap[ i ] = n ;
			}
			
			cout << " | " << setw(11) << potential << " | " << setw(22)<< nextSurfaces.size() << " |" << endl ;
				
			
			// Aca se inicializan los parametros a y b que se le introducen a la funcion isContainedIn
			int a = 0, b = 0 ;
			
			i = 0 ;
			
			// Este primer ciclo while se asegura de que ningun nodo quede sin papa
			SurfacesVector::iterator it1 = currentSurfaces.begin();
			while( it1 != currentSurfaces.end() ){
				
				
				// Este segundo siclo while se asegura de que ningun nodo quede con mas de dos papas
				int j = 0 ;
				SurfacesVector::iterator it2 = nextSurfaces.begin();
				while(  it2 != nextSurfaces.end() && currentNodeMap[i].outdeg() < 1){
					
					if( (it1->isContainedIn( *it2, a , b ) ) ){
						
						tree.new_edge( currentNodeMap[i], nextNodeMap[j] ) ;
					}
					
					it2++ ;
					j++ ;
					
				}
				
				if(currentNodeMap[i].outdeg() == 0){
					a++ ;
					b++ ;
				}else{
					it1++ ;
					i++ ;
				}
				
			}
			
			currentSurfaces = nextSurfaces ;
			currentNodeMap = nextNodeMap ;
			
		}
		
		/**************************************************************
		* Se crea el nodo raiz
		*/
		
		node n = tree.newNode( NodeWeight(0.0, true) ) ;
		
		i = 0 ;
		for( SurfacesVector::iterator it2 = currentSurfaces.begin(); it2 != currentSurfaces.end(); it2++, i++ ){
			
			tree.new_edge( currentNodeMap[i], n ) ;
			
		}
		//**************************************************************
		
		cout << " +-------------+------------------------+" << endl ;
		cout << endl ;
		
		
		/****************************************************************
		* Se define el atributo deepestNoNodes para todos los nodos
		*/
		
		node child ;
		node parent ;
	
		for( Tree::node_iterator it = tree.nodes_begin(); it != tree.nodes_end(); it++ ){

			if( it->indeg() == 0 ){

				parent = *it ;
			 
				int i = 0 ;
			
				while( (tree.inf( parent ).isRoot() == false )  && ( tree.inf( parent ).getDeepestNoNodes() < i ) ){
					tree.inf( parent ).setDeepestNoNodes( i ) ;
					parent = parent.out_edges_begin()->opposite( parent ) ;
					i++ ;
					if( (tree.inf( parent ).isRoot() == true )  && ( tree.inf( parent ).getDeepestNoNodes() < i ) ){
						tree.inf( parent ).setDeepestNoNodes( i ) ;	
					}
				
				}

				
							
			}

		}

		
		
		/***************************************************************
		* Ahora hay que crear el arbol, pero quitando redundacias, es
		* decir nodos con un solo hijo, excepto el nodo raiz y sus hijos
		*/
		
			
		vector<node> nodesToErase ;
		vector<node> nodesTMP ;
			
		for( Tree::node_iterator it = tree.nodes_begin(); it != tree.nodes_end(); it++ ){
			if( ( it->indeg() != 1 ) && ( !tree.inf(*it).isRoot() ) ){
				nodesTMP.push_back( *it ) ;
			}
		}
			
		for( vector<node>::iterator it = nodesTMP.begin(); it != nodesTMP.end(); it++ ){
				
			child = *it ;
			parent = *it ;
				
			/****************************************************
			* Esta parte se asegura de que no se borren los nodos
			* hijos del raiz, es decir lo que se consideraria como las
			* raices de las distintas ramas que conformarían el arbol. Esto
			* se hace para restar peso a los componentes que aparecen en
			* potenciales altos, que son pequeños y que no se unen a nada
			*/
			while( (parent.out_edges_begin()->opposite( parent ).indeg() == 1 ) && 
						     ( !tree.inf(parent.out_edges_begin()->opposite( parent ).out_edges_begin()->
						     opposite( parent.out_edges_begin()->opposite( parent ) ) ).isRoot() )){
				
				
				parent = parent.out_edges_begin()->opposite( parent ) ;
				nodesToErase.push_back( parent ) ;
			}
				
			if( child != parent ){
				tree.new_edge( child, parent.out_edges_begin()->opposite( parent ) ) ;
				tree.inf(child).setDeepestNoNodes(tree.inf(parent).getDeepestNoNodes()) ;
			}
		}
			
		/*
		* Finalmente eliminamos los nodos que se han rotulado como para ser borrados
		* que fueron almacenados en la lista
		*/
		for( vector<node>::iterator it = nodesToErase.begin(); it != nodesToErase.end(); it++ )
			tree.removeNode( *it ) ;
			
		
		// Por omisión el arbol es numerado en postOrder
		tree.postOrderRay() ;
		
		return tree ;
		
	}else{
		
		Tree tree ;
		
		Surface s ;  // Esta es una superficie temporal
		
		/***********************************************
		* En el trancurso de construcción del arbol
		* se debe tener el conjunto de componentes
		* a un valor de potencial x y almismo tiempo
		* al un valor de potencial x + stepSize,
		* para en un siguiente procedimiento verificar
		* la contenencia de unas con respecto a otras
		* y de esta manera crear las aristas. De igual
		* manera los nodos que representan
		*/
		SurfacesVector currentSurfaces ;
		SurfacesVector nextSurfaces ;
		
		map<int, node> currentNodeMap ;
		map<int, node> nextNodeMap ;
		//**********************************************
		
		cout << " -------------------------------------------" << endl ;
		cout << "      Potential  |  Number of Components    " << endl ;
		cout << " -------------------------------------------" << endl ;
		
		/**************************************************************
		* Se inicializa la primera fila de nodos, partiendo de las
		* componentes calculadas al valor de potencial cuttoffBegin
		*/
		s = this->getIsosurface( cutoffBegin ) ;
		currentSurfaces = s.computeComponents() ;
			
		
		int i = 0 ;
		for( SurfacesVector::iterator it1 = currentSurfaces.begin(); it1 != currentSurfaces.end(); it1++, i++ ){
			
			/*****************************************************
			* El weightValue ahora depende tanto del potencial como
			* del area superficial de las componentes
			*/
			node n = tree.newNode( NodeWeight( cutoffBegin, it1->computeArea() ) ) ;
			currentNodeMap[ i ] = n ;
			
		}
		
		if(currentSurfaces.size() != 0){
			cout << "\t" << cutoffBegin << "\t\t" << currentSurfaces.size() << endl ;
		}else{
			cout << "\t" << cutoffBegin << "\t\t" << currentSurfaces.size() << endl ;
		}
		
		
		
		for( double potential = cutoffBegin - stepSize; potential > cutoffEnd - 0.0005 ; potential -= stepSize ){
			
			s = this->getIsosurface( potential ) ;
			nextSurfaces = s.computeComponents() ;
			
			nextNodeMap.clear() ;
			
			i = 0 ;
			for( SurfacesVector::iterator it2 = nextSurfaces.begin(); it2 != nextSurfaces.end(); it2++, i++ ){
				
				/*****************************************************
				* El weightValue ahora depende tanto del potencial como
				* del area superficial de las componentes
				*/
				node n = tree.newNode( NodeWeight( potential, it2->computeArea() ) ) ;
				nextNodeMap[ i ] = n ;
			}
			
			
			cout << "\t" << potential << "\t\t" << nextSurfaces.size() << endl ;//********************************
				
			
			// Aca se inicializan los parametros a y b que se le introducen a la funcion isContainedIn
			int a = 0, b = 0 ;
			
			i = 0 ;
			
			// Este primer ciclo while se asegura de que ningun nodo quede sin papa
			SurfacesVector::iterator it1 = currentSurfaces.begin();
			while( it1 != currentSurfaces.end() ){
				
				
				// Este segundo siclo while se asegura de que ningun nodo quede con mas de dos papas
				int j = 0 ;
				SurfacesVector::iterator it2 = nextSurfaces.begin();
				while(  it2 != nextSurfaces.end() && currentNodeMap[i].outdeg() < 1){
					
					if( (it1->isContainedIn( *it2, a , b ) ) ){
						
						tree.new_edge( currentNodeMap[i], nextNodeMap[j] ) ;
					}
					
					it2++ ;
					j++ ;
					
				}
				
				if(currentNodeMap[i].outdeg() == 0){
					a++ ;
					b++ ;
				}else{
					it1++ ;
					i++ ;
				}
				
			}
			
			currentSurfaces = nextSurfaces ;
			currentNodeMap = nextNodeMap ;
			
		}
		
		/**************************************************************
		* Se crea el nodo raiz
		*/
		
		node n = tree.newNode( NodeWeight(0.0, true) ) ;
		
		i = 0 ;
		for( SurfacesVector::iterator it2 = currentSurfaces.begin(); it2 != currentSurfaces.end(); it2++, i++ ){
			
			tree.new_edge( currentNodeMap[i], n ) ;
			
		}
		//**************************************************************
		
		cout << " ----------------------------------------" << endl ;
		cout << endl ;
		
		
		/****************************************************************
		* Se define el atributo deepestNoNodes para todos los nodos
		*/
		
		node child ;
		node parent ;
	
		for( Tree::node_iterator it = tree.nodes_begin(); it != tree.nodes_end(); it++ ){

			if( it->indeg() == 0 ){

				parent = *it ;
			 
				int i = 0 ;
			
				while( (tree.inf( parent ).isRoot() == false )  && ( tree.inf( parent ).getDeepestNoNodes() < i ) ){
					tree.inf( parent ).setDeepestNoNodes( i ) ;
					parent = parent.out_edges_begin()->opposite( parent ) ;
					i++ ;
					if( (tree.inf( parent ).isRoot() == true )  && ( tree.inf( parent ).getDeepestNoNodes() < i ) ){
						tree.inf( parent ).setDeepestNoNodes( i ) ;	
					}
				
				}

				
							
			}

		}

		
		
		/***************************************************************
		* Ahora hay que crear el arbol, pero quitando redundacias, es
		* decir nodos con un solo hijo, excepto el nodo raiz y sus hijos
		*/
		
			
		vector<node> nodesToErase ;
		vector<node> nodesTMP ;
			
		for( Tree::node_iterator it = tree.nodes_begin(); it != tree.nodes_end(); it++ ){
			if( ( it->indeg() != 1 ) && ( !tree.inf(*it).isRoot() ) ){
				nodesTMP.push_back( *it ) ;
			}
		}
			
		for( vector<node>::iterator it = nodesTMP.begin(); it != nodesTMP.end(); it++ ){
				
			child = *it ;
			parent = *it ;
				
			/****************************************************
			* Esta parte se asegura de que no se borren los nodos
			* hijos del raiz, es decir lo que se consideraria como las
			* raices de las distintas ramas que conformarían el arbol. Esto
			* se hace para restar peso a los componentes que aparecen en
			* potenciales altos, que son pequeños y que no se unen a nada
			*/
			while( (parent.out_edges_begin()->opposite( parent ).indeg() == 1 ) && 
						     ( !tree.inf(parent.out_edges_begin()->opposite( parent ).out_edges_begin()->
						     opposite( parent.out_edges_begin()->opposite( parent ) ) ).isRoot() )){
				
				
				parent = parent.out_edges_begin()->opposite( parent ) ;
				nodesToErase.push_back( parent ) ;
						     }
				
						     if( child != parent ){
							     tree.new_edge( child, parent.out_edges_begin()->opposite( parent ) ) ;
							     tree.inf(child).setDeepestNoNodes(tree.inf(parent).getDeepestNoNodes()) ;
						     }
		}
			
		/*
		* Finalmente eliminamos los nodos que se han rotulado como para ser borrados
		* que fueron almacenados en la lista
		*/
		for( vector<node>::iterator it = nodesToErase.begin(); it != nodesToErase.end(); it++ )
			tree.removeNode( *it ) ;
			
		
		// Por omisión el arbol es numerado en postOrder
		tree.postOrderRay() ;
		
		return tree ;
	}
}
