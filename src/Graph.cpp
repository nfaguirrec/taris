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

/**
 * @brief Constructor
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::Graph() : graph()
{
	nodeMap = map< Node, NODE_TYPE >() ;
	edgeMap = map< Edge, EDGE_TYPE >() ;
}

template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::Graph( const Graph& g )
{
	this->copy( g ) ;
}

template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::Graph( const Graph& g, const list<Node>& listNode )
{
	node_map<Node> copy(g, node());
	
	for( list<Node>::const_iterator it = listNode.begin(); it != listNode.end(); ++it)
	{
		Node tmp = new_node();
		copy[*it] = tmp ;
		nodeMap[ tmp ] = g.inf(*it) ;
 
// 		copy[*it] = newNode( g.inf( *it ) );
	}
	
	for( list<Node>::const_iterator it = listNode.begin(); it != listNode.end(); ++it){
		
		node::out_edges_iterator e_it, e_end;
		
		for (e_it = (*it).out_edges_begin(), e_end = (*it).out_edges_end(); e_it != e_end; ++e_it) {
			
			if ( copy[e_it->target()] != node() ) {
				
				new_edge( copy[e_it->source()], copy[e_it->target()] );
				
			}
			
		}
	}
	
// 	nodeMap = g.getNodeMap() ;
// 	edgeMap = g.getEdgeMap() ;
}

template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
void Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::operator = ( const Graph& g )
{
	this->copy( g ) ;
}

template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
void Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::copy( const Graph& g )
{
	node_map<node> copy(g, node());

	for( graph::node_iterator it = g.nodes_begin(); it != g.nodes_end(); ++it)
	{
		Node tmp = new_node();
		copy[*it] = tmp ;
		nodeMap[ tmp ] = g.inf(*it) ;
	}
	
	for( graph::node_iterator it = g.nodes_begin(); it != g.nodes_end(); ++it){
		
		node::out_edges_iterator e_it, e_end;
		
		for (e_it = (*it).out_edges_begin(), e_end = (*it).out_edges_end(); e_it != e_end; ++e_it) {
			
			if ( copy[e_it->target()] != node() ) {
				
				new_edge( copy[e_it->source()], copy[e_it->target()] );
				
			}
			
		}
	}
	
// 	nodeMap = g.getNodeMap() ;
// 	edgeMap = g.getEdgeMap() ;
}

/**
 * @brief Destructor
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::~Graph()
{
	nodeMap.clear() ;
	edgeMap.clear() ;
}

/**
 * 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
void Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::assign( const GRAPH_TYPE& x )
{
	this->x = x ;
}

/**
 * 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
const GRAPH_TYPE& Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::inf()
{
	return x ;
}

/**
 * 
 * @param n 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
void Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::removeNode( Node n )
{
	if( !n.is_hidden() ){
		for( typename Node::inout_edges_iterator it = n.inout_edges_begin(); it != n.inout_edges_end(); ++it ){
			edgeMap.erase(*it) ;
		}
	}else{
		cout << "Warning!!!: Not implement, remove node hidden" << endl ;
	}
	
	del_node( n ) ;
	nodeMap.erase( n ) ;
}

/**
 * 
 * @param x 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
Node Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::newNode( const NODE_TYPE& x )
{
	Node n = new_node() ;
	nodeMap[n] = x ;
	return n ;
}

/**
 * 
 * @param n 
 * @param x 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
void Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::assign( Node n, const NODE_TYPE& x )
{
	nodeMap[n] = x ;
}

/**
 * 
 * @param n 
 * @return 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
NODE_TYPE& Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::inf( Node n )
{
	return nodeMap[n] ;
}

/**
 * 
 * @param n 
 * @return 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
		NODE_TYPE Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::inf( Node n ) const
{
	return nodeMap.find(n)->second ;
}

/**
 * 
 * @param source 
 * @param target 
 * @param x 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
Edge Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::newEdge( Node source, Node target, const EDGE_TYPE& x )
{
	Edge e = new_edge( source, target ) ;
	edgeMap[e] = x ;
	return e ;
}

/**
 * 
 * @param n 
 * @param x 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
void Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::assign( Edge n, const EDGE_TYPE& x )
{
	edgeMap[n] = x ;
}

/**
 * 
 * @param n 
 * @return 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
EDGE_TYPE& Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::inf( Edge n )
{
	return edgeMap[n] ;
}

/**
 * 
 * @param n 
 */
template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
void Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::removeEdge( Edge e )
{
}

template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
map< Node, NODE_TYPE > Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::getNodeMap() const
{
	return nodeMap ;
}

template < class NODE_TYPE, class EDGE_TYPE, class GRAPH_TYPE >
map< Edge, EDGE_TYPE > Graph< NODE_TYPE, EDGE_TYPE, GRAPH_TYPE >::getEdgeMap() const
{
	return edgeMap ;
}

