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
#ifndef GRAPH_H
#define GRAPH_H

#include <map>
using namespace std ;

#include <GTL/graph.h>

#define Node node
#define Edge edge

/**
 *  @brief Clase de tipo <b>"Template"</b> que permite crear grafos ponderados
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
template < class NODE_TYPE = int, class EDGE_TYPE = int, class GRAPH_TYPE = int >
class Graph : public graph
{
	public:
		Graph();
		Graph( const Graph& g );
		Graph( const Graph& g, const list<Node>& listNode );
		void operator = ( const Graph& g );
		void copy( const Graph& g ) ;
		~Graph();
		
		void assign( const GRAPH_TYPE& x ) ;
		const GRAPH_TYPE& inf() ;
		
		virtual Node newNode( const NODE_TYPE& x ) ;
		void assign( Node n, const NODE_TYPE& x ) ;
		NODE_TYPE& inf( Node n ) ;
		NODE_TYPE inf( Node n ) const ;
		virtual void removeNode( Node n ) ;
		
		virtual Edge newEdge( Node source, Node target, const EDGE_TYPE& x = 0 ) ;
		void assign( Edge n, const EDGE_TYPE& x ) ;
		EDGE_TYPE& inf( Edge n ) ;
		EDGE_TYPE inf( Edge n ) const;
		virtual void removeEdge( Edge e ) ;
		
		map< Node, NODE_TYPE > getNodeMap() const ;
		map< Edge, EDGE_TYPE > getEdgeMap() const ;
		
	protected:
		map< Node, NODE_TYPE > nodeMap ;
		map< Edge, EDGE_TYPE > edgeMap ;
		
	private:
		GRAPH_TYPE x ;
};

#include "Graph.cpp"

#endif
