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
#include "Tree.h"

#include "TarisApplication.h"

#include <algorithm>
#include <iomanip>
using namespace std ;

Tree::Tree():
Graph<NodeWeight>()
{
	tree = true ;
}

Tree::Tree( const Tree& t )
{
	this->tree = true ;
	this->copy( t ) ;
}

bool Tree::operator == ( const Tree& t ){
	return areSubtreesEqual( *this, this->getRoot(), t, t.getRoot() ) ;
}

Tree::~Tree()
{
}

bool Tree::isTree()
{
	return tree ;
}

Node Tree::getRoot() const
{
	Node root ;
	
	// Se busca el nodo raiz
	for( Tree::node_iterator it = this->nodes_begin(); it != this->nodes_end(); it++ ){
		if( this->inf(*it).isRoot() ){
			root = *it ;
		}
	}
	
	return root ;
}

/**
 * Busca un nodo discriminandolo por su externalId
 * @param id 
 * @return 
 */
Node Tree::getNode( int externalId ) const
{
	Node n ;
	
	// Se busca el nodo de interes
	for( Tree::node_iterator it = this->nodes_begin(); it != this->nodes_end(); it++ ){
		if( this->inf(*it).getExternalId() == externalId ){
			n = *it ;
		}
	}
	
	return n ;
}

NodeWeight Tree::getNodeInf( int externalId ) const
{
	return this->inf( this->getNode( externalId ) ) ;
}


void Tree::postOrderRay()
{
	Node root ;
	
	// Se busca el nodo raiz
	for( Tree::node_iterator it = this->nodes_begin(); it != this->nodes_end(); it++ ){
		if( this->inf(*it).isRoot() ){
			root = *it ;
		}
	}
	
	int counter = 1 ;
	postOrderRay( root, counter, true ) ;
}


/**
 * El costo de insertarun nodo es la suma de todos los pesos de los nodos
 * generados a partir de el
 * @param n 
 * @return 
 */
double Tree::insertSubtreeCost( Node n ) const {
	double cost = 0.0 ;
	
	insertSubtreeCost( n, cost ) ;
	
	return cost ;
}

/**
 * El costo de remover un nodo, corresponde a valor de su peso
 * @param n 
 * @return 
 */
double Tree::deleteSubtreeCost( Node n ) const {
	
// 	return inf( n ).getWeightValue() ;
	double cost = 0.0 ;
	
	insertSubtreeCost( n, cost ) ;
	
	return cost ;

	
}

/**
 * Esta es una funci贸n autorrecurrente que se ancarga de comparar los subarboles generados desde los nodos n1 y n2 respectivamente
 * @param n1 Nodo inicial perteneciente al Arbol 1 ( this )
 * @param t2 Arbol contra el cual se comparar谩
 * @param n2 Nodo inicial perteneciente al Arbol 2 ( t2 )
 * @return true si son igulaes false de otra manera
 */
bool Tree::areSubtreesEqual( const Tree& t1, Node n1, const Tree& t2, Node n2 )
{
	if( n1 == node() ){
		
		if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
			///@todo Aca hay que lanzar una excepci贸n en lugar de retornar false en caso de que se tengan nodos invalidos
			cerr << "Warning from: bool Tree::equalTrees( Node n1, const Tree& t2, Node n2 )" << endl ;
			cerr << "\tNodo " ;
			cerr << "[" << t1.inf( n1 ).getExternalId() << "] ";
			cerr << "del arbol 1 invalido" << endl ;
		}
		
		return false ;
		
	}else if( n2 == node() ){
		
		if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
			cerr << "Warning from: bool Tree::equalTrees( Node n1, const Tree& t2, Node n2 )" << endl ;
			cerr << "\tNodo " ;
			cerr << "[" << t2.inf( n2 ).getExternalId() << "] ";
			cerr << "del arbol 2 invalido" << endl ;
		}
		
		return false ;
		
	}else if( fabs( t1.inf( n1 ).getWeightValue() - t2.inf( n2 ).getWeightValue() ) > TarisApplication::Instance()->getDoubleThresholdComparison() ){
		
		if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
			cerr << "Warning from: bool Tree::equalTrees( Node n1, const Tree& t2, Node n2 )" << endl ;
			cerr << "\tDistintos valores de pesos para los nodos: " ;
			cerr << "[" << t1.inf( n1 ).getExternalId() << "]";
			cerr << ", " ;
			cerr << "[" << t2.inf( n2 ).getExternalId() << "]" << endl ;
			cerr << "Valor de la diferencia = " << fabs( t1.inf( n1 ).getWeightValue() - t2.inf( n2 ).getWeightValue() ) << endl ;
		}
		
		return false ;
		
	}else if( n1.indeg() != n2.indeg() ){
			
		if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
			cerr << "Warning from: bool Tree::equalTrees( Node n1, const Tree& t2, Node n2 )" << endl ;
			cerr << "\tDistinto numero de hijos para los nodos: " ;
			cerr << "[" << t1.inf( n1 ).getExternalId() << "]";
			cerr << ", " ;
			cerr << "[" << t2.inf( n2 ).getExternalId() << "]" << endl ;
		}
		
		return false ;
		
	}else{
		map<int, Node> children1 ;
		map<int, Node> children2 ;
		
		for( node::in_edges_iterator it1 = n1.in_edges_begin(); it1 != n1.in_edges_end() ; it1++ ){
			children1[ t1.inf( it1->opposite( n1 ) ).getExternalId() ] = it1->opposite( n1 ) ;
		}
		
		for( node::in_edges_iterator it2 = n2.in_edges_begin(); it2 != n2.in_edges_end() ; it2++ ){
			children2[ t2.inf( it2->opposite( n2 ) ).getExternalId() ] = it2->opposite( n2 ) ;
		}
		
		map<int, Node>::iterator it1 = children1.begin() ;
		map<int, Node>::iterator it2 = children2.begin() ;
		
		if( children1.size() > 0 && children2.size() > 0 ){
			
			for( ; it1 != children1.end() || it2 != children2.end() ; it1++, it2++ ){
				
				if( !areSubtreesEqual( t1, children1[ it1->first ], t2, children2[ it2->first ] ) ){
				
					return false ;
				
				}
				
			}
			
		}
		
		return true ;
		
	}
}

/**
 * Esta funci贸n calcula el costo de intercambiar dos ramas cuyos origenes son n11 y n12 para el arbol original
 * @param n11 
 * @param n12 
 * @param n21 
 * @param n22 
 * @return 
 */
double Tree::swapSubtrees( const Tree& t1, Node n11, Node n12, const Tree& t2, Node n21, Node n22 )
{
	///@todo Hay que agregar la secci贸n de depuraci贸n que verifica que los nodos n11 y n12 sean hermanos y a su vez que n21 y n22 tambien lo sean
	if( n11 == node() ){
		
		if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
			cerr << "Warning from: double Tree::swapSubtrees( const Tree& t1, Node n11, Node n12, const Tree& t2, Node n21, Node n22 )" << endl ;
			cerr << "\tNodo " ;
			cerr << "[" << t1.inf( n11 ).getExternalId() << "] ";
			cerr << "del arbol 1 invalido" << endl ;
		}
		
		return INFINITY ;
		
	}else if( n12 == node() ){
		
		if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
			cerr << "Warning from: double Tree::swapSubtrees( const Tree& t1, Node n11, Node n12, const Tree& t2, Node n21, Node n22 )" << endl ;
			cerr << "\tNodo " ;
			cerr << "[" << t1.inf( n12 ).getExternalId() << "] ";
			cerr << "del arbol 1 invalido" << endl ;
		}
		
		return INFINITY ;
		
	}else if( n21 == node() ){
		
		if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
			cerr << "Warning from: double Tree::swapSubtrees( const Tree& t1, Node n11, Node n12, const Tree& t2, Node n21, Node n22 )" << endl ;
			cerr << "\tNodo " ;
			cerr << "[" << t2.inf( n21 ).getExternalId() << "] ";
			cerr << "del arbol 2 invalido" << endl ;
		}
		
		return INFINITY ;
		
	}else if( n22 == node() ){
		
		if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
			cerr << "Warning from: double Tree::swapSubtrees( const Tree& t1, Node n11, Node n12, const Tree& t2, Node n21, Node n22 )" << endl ;
			cerr << "\tNodo " ;
			cerr << "[" << t2.inf( n22 ).getExternalId() << "] ";
			cerr << "del arbol 2 invalido" << endl ;
		}
			
		return INFINITY ;
		
	// ( n11.outdeg() == 1 ) es equivalente a preguntar que el nodo tiene padres ?
	}else if( ( n11.outdeg() == 1 ) && ( n21.outdeg() == 1 ) && areSubtreesEqual( t1, n11, t2, n22 ) && areSubtreesEqual( t1, n12, t2, n21 ) ){
		
		return 0.0 ;
		
	}else{
		
		return INFINITY ;
		
	}
}

/**
 *
 * @param n
 * @param counter
 */



void Tree::postOrderRay( Node n, int& counter, bool makeKeyroot  )
{
	
	vector<double> childrenOrder ;
	map<double, Node> children ;
	
	NodeWeight weight = this->inf( n ) ;
	weight.setLeftmost( counter ) ;
	weight.setKeyroot( makeKeyroot ) ;

	
	for( Node::in_edges_iterator it1 = n.in_edges_begin(); it1 != n.in_edges_end(); it1++ ){
		
		
		double order =  -(this->inf( it1->opposite( n ) ).getDeepestNoNodes() + 1 ) ;
		int i = 1 ;
		
		if(childrenOrder.size() != 0){
			
			for( vector<double>::iterator it2 = childrenOrder.begin(); it2 != childrenOrder.end(); it2++){
				if( *it2 == order ){
					order = order + i*1e-6 ;
					i++ ;
				}
				
			}
		}
		
		childrenOrder.push_back( order) ;
		children[ order ] = it1->opposite( n ) ;

	}
	
	if( childrenOrder.size() != 0 ){
		
		sort( childrenOrder.begin(), childrenOrder.end() ) ;
		
		for( vector<double>::iterator it = childrenOrder.begin(); it != childrenOrder.end(); it++ ){
			postOrderRay( children[*it], counter, ( it != childrenOrder.begin() ) ) ;

		}
	}
		
	weight.setExternalId( counter ) ;
	
	this->assign( n, weight ) ;
	counter++ ;
}


void Tree::insertSubtreeCost( Node n, double& cost ) const
{
	map<double, Node> children ;
	
	double weight = this->inf( n ).getWeightValue() ;
	
	cost += weight ;
	
	for( Node::in_edges_iterator it = n.in_edges_begin(); it != n.in_edges_end(); it++ ){
		
		Node child = it->opposite( n ) ;
		
		insertSubtreeCost( child, cost ) ;
		
	}
}

double Tree::distance( const Tree& t1, const Tree& t2 )
{
	double** forestDistance = new double*[ t1.number_of_nodes() + 1 ];
	for (int i = 0; i <= t1.number_of_nodes(); i++){
		forestDistance[i] = new double[ t2.number_of_nodes() + 1 ] ;
	}

	double** distance = new double*[ t1.number_of_nodes() ];
	for (int i = 0; i< t1.number_of_nodes(); i++){
		distance[i] = new double[ t2.number_of_nodes() ] ;
	}
	
	for (int i = 0; i <= t1.number_of_nodes(); i++){
		for (int j = 0; j <= t2.number_of_nodes(); j++){
			forestDistance[i][j] = INFINITY ;
		}
	}
	
	distance[ t1.number_of_nodes() - 1 ][ t2.number_of_nodes() - 1 ] = 0.0 ;
	
	for (int x = 1; x <= t1.number_of_nodes(); x++){
		
		if( t1.getNodeInf( x ).isKeyroot() ){
			
			for (int y = 1; y <= t2.number_of_nodes(); y++){
		
				if( t2.getNodeInf( y ).isKeyroot() ){
					
					forestDistance[ t1.getNodeInf(x).getLeftmost() - 1 ][ t2.getNodeInf(y).getLeftmost() - 1 ] = 0.0 ;
					
					for( int i = t1.getNodeInf(x).getLeftmost(); i <= x; i++ ){
						
						forestDistance[ i ][ t2.getNodeInf(y).getLeftmost() - 1 ] =
									forestDistance[ t1.getNodeInf(i).getLeftmost() - 1 ][ t2.getNodeInf(y).getLeftmost() - 1 ] +
									t1.deleteSubtreeCost( t1.getNode( i ) ) ;
						
					}
					
					for( int j = t2.getNodeInf(y).getLeftmost(); j <= y; j++ ){
						
						forestDistance[ t1.getNodeInf(x).getLeftmost() - 1 ][ j ] =
									forestDistance[ t1.getNodeInf(x).getLeftmost() - 1 ][ t2.getNodeInf(j).getLeftmost() - 1 ] +
									t2.insertSubtreeCost( t2.getNode( j ) ) ;
						
					}
					
					for( int i = t1.getNodeInf(x).getLeftmost(); i <= x; i++ ){
						
						for( int j = t2.getNodeInf(y).getLeftmost(); j <= y; j++ ){
							
							/************************************************************
							* Se agregan las cosas en el vector, solo con el motivo de
							* encontrar el minimo valor
							*/
							vector<double> vec ;
							
							vec.push_back( forestDistance[i-1][j] + GAMMA( t1.getNodeInf(i).getWeightValue(), 0.0 ) ) ;
							vec.push_back( forestDistance[i][j-1] + GAMMA( 0.0, t2.getNodeInf(j).getWeightValue() ) ) ;
							vec.push_back( forestDistance[ t1.getNodeInf(i).getLeftmost() - 1 ][j] + t1.deleteSubtreeCost( t1.getNode(i) ) ) ;
							vec.push_back( forestDistance[i][ t2.getNodeInf(j).getLeftmost() - 1 ] + t2.insertSubtreeCost( t2.getNode(j) ) ) ;
							
							forestDistance[i][j] = *min_element( vec.begin(), vec.end() ) ;
// 							cout << "El minimo es: " <<  forestDistance[i][j] << endl ;
							//***********************************************************
							
							if( t1.getNodeInf( i ).getLeftmost() == t1.getNodeInf( x ).getLeftmost() &&
								t2.getNodeInf( j ).getLeftmost() == t2.getNodeInf( y ).getLeftmost() ){
								
								/************************************************************
								* Se agregan las cosas en el vector, solo con el motivo de
								* encontrar el minimo valor
								*/
								vector<double> vec2 ;
							
								vec2.push_back( forestDistance[i][j] ) ;
								vec2.push_back( forestDistance[i-1][j-1] + GAMMA( t1.getNodeInf(i).getWeightValue(), t2.getNodeInf(j).getWeightValue() ) ) ;
							
								forestDistance[i][j] = *min_element( vec2.begin(), vec2.end() ) ;
								distance[i-1][j-1] = forestDistance[i][j] ;
								//***********************************************************
								
							}else{
								
								/************************************************************
								* Se agregan las cosas en el vector, solo con el motivo de
								* encontrar el minimo valor
								*/
								vector<double> vec2 ;
							
								vec2.push_back( forestDistance[i][j] ) ;
								vec2.push_back( forestDistance[t1.getNodeInf( i ).getLeftmost()-1][t2.getNodeInf( j ).getLeftmost()-1] +
										distance[i-1][j-1] ) ;
							
								forestDistance[i][j] = *min_element( vec2.begin(), vec2.end() ) ;
								//***********************************************************
								
								if(
									t1.getNodeInf( i ).isKeyroot() && 
									( t1.getNode( i ).outdeg() != 0 ) &&
									t2.getNodeInf( j ).isKeyroot() && 
									( t2.getNode( j ).outdeg() != 0 )
								){
									
									/************************************************************
									* Se agregan las cosas en el vector, solo con el motivo de
									* encontrar el minimo valor
									*/
									vector<double> vec3 ;
							
									
									vec3.push_back( forestDistance[i][j] ) ;
									vec3.push_back( forestDistance[t1.getNodeInf( t1.getNodeInf( i ).getLeftmost()-1 ).getLeftmost() - 1]
										[t2.getNodeInf(t2.getNodeInf( j ).getLeftmost()-1 ).getLeftmost() - 1] +
										swapSubtrees( t1, t1.getNode( i ),
											t1.getNode(t1.getNodeInf( i ).getLeftmost()-1 ), 
											t2, t2.getNode( j ), 
											t2.getNode(t2.getNodeInf(j).getLeftmost()-1)) ) ;
							
									forestDistance[i][j] = *min_element( vec3.begin(), vec3.end() ) ;
									//***********************************************************
									
								}
								
							}
							
						}
						
					}
					
				}
			}
			
		}
		
	}
	
	return distance[ t1.number_of_nodes() - 1 ][ t2.number_of_nodes() - 1 ] ;
}

void Tree::print( const Tree& t, ostream& os )
{
	map< double, vector<Node> > nodesValues ;
	vector<Node> nodes ;
	
	for( Tree::node_iterator it = t.nodes_begin(); it != t.nodes_end(); it++ ){
		
		nodes.clear() ;
		
		nodes.push_back( *it ) ;
		
		for( Tree::node_iterator it2 = t.nodes_begin(); it2 != t.nodes_end(); it2++ ){
			
			if( ( fabs( t.inf(*it).getPotentialValue() - t.inf(*it2).getPotentialValue() ) < 1e-6 ) && ( it2 != it ) ){
				
				nodes.push_back( *it2 ) ;
				
			}
			
		}
		
		nodesValues[ t.inf(*it).getPotentialValue() ] = nodes ;
		
	}
	
	os << "---------------" << endl ;
	os << " Levels" << endl ;
	os << "---------------" << endl ;
	os << endl ;
	os << " <Potential>     <Nodes>" << endl ;
	os << endl ;
	
	for( map< double, vector<Node> >::reverse_iterator it = nodesValues.rbegin() ; it != nodesValues.rend(); it++ ){
// 	for( map< double, vector<Node> >::iterator it = nodesValues.begin() ; it != nodesValues.end(); it++ ){
		os.setf(ios::fixed) ;
		os.precision(5) ;
		os << setw(10) << it->first << ":      " ;
		
		for( vector<Node>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++ ){
			os << "[" << t.inf(*it2).getExternalId() << "] " ;
		}
		
		os << endl ;
	}
	
	os << endl ;
	
	os << "---------------" << endl ;
	os << " Connections" << endl ;
	os << "---------------" << endl ;
	os << endl ;

	Node n;
	Edge out;
	string conn;

	if (t.is_directed())
		conn = "-->";
	else
		conn = "<-->";

	for( Tree::node_iterator itn = t.nodes_begin(); itn != t.nodes_end(); itn++ ){
		os << "[" << t.inf(*itn).getExternalId() << "]" << ":: ";

		for( Node::adj_edges_iterator  ite = itn->adj_edges_begin(); ite != itn->adj_edges_end(); ite++ ){
			os << conn << "[" << t.inf( itn->opposite(*ite) ).getExternalId() << "]" ;
		}

		os << endl;
	}
	
	os << endl ;
}

void Tree::save_graph_info_handler( ostream* os ) const
{
	(*os) << "tree " << 1 << endl ;
}

void Tree::save_node_info_handler(ostream* os, node n ) const
{
	(*os) << "label \"" << inf( n ).getExternalId() << "\"" << endl ;
	
	(*os) << "weight [" << endl ;
	(*os) << "keyroot " <<  ( inf( n ).isKeyroot() ? 1 : 0 ) << endl ;
	(*os) << "root " << ( inf( n ).isRoot() ? 1 : 0 ) << endl ;
	(*os) << "externalId " << inf( n ).getExternalId() << endl ;
	
	if( inf( n ).getPotentialValue() != INFINITY )
		(*os) << "potentialValue " << inf( n ).getPotentialValue() << endl ;
	
	if( inf( n ).getWeightValue() != INFINITY )
		(*os) << "weightValue " << inf( n ).getWeightValue() << endl ;
	(*os) << "leftmost " << inf( n ).getLeftmost() << endl ;
	(*os) << "deepestNoNodes " << inf( n ).getDeepestNoNodes() << endl ;
	(*os) << "]" << endl ;
}

void Tree::load_graph_info_handler( GML_pair* list_graph )
{
	while ( list_graph ) {
		if ( strcmp( "tree", list_graph->key ) == 0 ) {
			tree = ( ( list_graph->value.integer == 1 ) ? true : false ) ;
			
			if( !tree ){
				cerr << "### Error ### : This graph is not a TARIS::Tree" << endl ;
				exit(-1) ;
			}
		}
		
		list_graph = list_graph->next;
	}
}

void Tree::load_node_info_handler( node n, GML_pair* list_node )
{
	while ( list_node ) {
		
		if( strcmp( "label", list_node->key ) == 0 ){
			// En este caso no interesa, solo es para visualizaci髇
		}
			
		if( strcmp( "weight", list_node->key ) == 0 ){
			
			GML_pair* list_weight = list_node->value.list;
			
			while ( list_weight ) {
				
				if ( strcmp( "keyroot", list_weight->key ) == 0 ) {
					inf( n ).setKeyroot( ( ( list_weight->value.integer == 1 ) ? true : false ) ) ;
				}
				
				if ( strcmp( "root", list_weight->key ) == 0 ) {
					inf( n ).setRoot( ( ( list_weight->value.integer == 1 ) ? true : false ) ) ;
				}
				
				if ( strcmp( "externalId", list_weight->key ) == 0 ) {
					inf( n ).setExternalId( list_weight->value.integer ) ;
				}
				
				if ( strcmp( "potentialValue", list_weight->key ) == 0 ) {
					inf( n ).setPotentialValue( list_weight->value.floating ) ;
				}
				
				if ( strcmp( "weightValue", list_weight->key ) == 0 ) {
					inf( n ).setWeightValue( list_weight->value.floating ) ;
				}
				
				if ( strcmp( "leftmost", list_weight->key ) == 0 ) {
					inf( n ).setLeftmost( list_weight->value.integer ) ;
				}
				
				if ( strcmp( "deepestNoNodes", list_weight->key ) == 0 ) {
					inf( n ).setDeepestNoNodes( list_weight->value.integer ) ;
				}
				
				list_weight = list_weight->next;
				
			}
			
			delete list_weight ;
			
		}
		
		list_node = list_node->next;
	}
}
