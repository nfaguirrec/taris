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
#ifndef TREE_H
#define TREE_H

#include <GTL/graph.h>

#include "Graph.h"
#include "NodeWeight.h"

#define MAX(X,Y) (X) > (Y) ? : (X) : (Y)
#define MIN(X,Y) (X) < (Y) ? : (X) : (Y)
#define GAMMA(p, q) fabs(p-q)
#define GAMMA1(p, q) (fabs(p-q))>(1e-6) ?  1.0 : 0.0

/**
 *  @brief Permite crear arboles, además de ofrecer algunos algoritmos básicos como el de organización en post-order
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class Tree : public Graph<NodeWeight>
{
	public:
		Tree() ;
		Tree( const Tree& t ) ;
		bool operator == ( const Tree& t ) ;
		~Tree() ;
		
		bool isTree() ;
		
		Node getRoot() const ;
		Node getNode( int externalId ) const ;
		NodeWeight getNodeInf( int externalId ) const ;
		
		void postOrder() ;
		void postOrderRay() ;
		void postOrderComplete() ;
		
		double insertSubtreeCost( Node n ) const ;
		double deleteSubtreeCost( Node n ) const ;
		
		///@todo Cambiar subtree por Branch
		static bool areSubtreesEqual( const Tree& t1, Node n1, const Tree& t2, Node n2 ) ;
		static double swapSubtrees( const Tree& t1, Node n11, Node n12, const Tree& t2, Node n21, Node n22 ) ;
		static double distance( const Tree& t1, const Tree& t2 ) ;
		static void print( const Tree& t, ostream& os ) ;
		
	protected:
		virtual void save_graph_info_handler( ostream* os ) const ;
		virtual void save_node_info_handler( ostream* os, node n ) const ;
		
		virtual void load_graph_info_handler( GML_pair* list_graph ) ;
		virtual void load_node_info_handler( node n, GML_pair* list_node ) ;
		
	private:
		void postOrder( Node n, int& counter, bool makeKeyroot ) ;
		void postOrderRay( Node n, int& counter, bool makeKeyroot ) ;
		void postOrderComplete( Node n, int& counter, bool makeKeyroot ) ;
		void insertSubtreeCost( Node n, double& cost ) const ;
// 		friend ostream& operator << ( ostream& os, const Tree& e ) ;
		
	private:
		bool tree ;
};

#endif
