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
#ifndef NODEWEIGHT_H
#define NODEWEIGHT_H

#include <math.h>

/**
 *  @brief Valores de ponderación para los arboles
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 *
 *  @todo Hay que separar los atributos involucrados en algoritmos de arboles, de los verdaderos valores de ponderación
 */
class NodeWeight{
	public:
		NodeWeight();
// 		NodeWeight( const NodeWeight& nw );
		NodeWeight( double weightValue, bool root = false );
		NodeWeight( double weightValue, bool root, int deepestNoNodes ) ;
		NodeWeight( double potentialValue, double weightValue );
		NodeWeight( double potentialValue, double weightValue, bool root, int deepestNoNodes ) ;
// 		friend bool operator == ( const NodeWeight& nw1, const NodeWeight& nw2 ) ;
// 		friend bool operator < (const NodeWeight& nw1, const NodeWeight& nw2);
		~NodeWeight();
		
		void setExternalId(const int& value);
		int getExternalId() const;
		void setKeyroot(bool value);
		bool isKeyroot() const;
		void setLeftmost(const int& value);
		int getLeftmost() const;
		void setPotentialValue( const double& value);
		double getPotentialValue() const;
		void setWeightValue( const double& value);
		double getWeightValue() const;

		void setRoot( bool root ) ;
		bool isRoot() const ;
		void setDeepestNoNodes(const int& value);
		int getDeepestNoNodes() const;
		
	private:
		bool keyroot ;		///< Este valor es true si el nodo tiene nodos hermanos a la izquierda
		bool root ;		///< Flag que asegura que es nodo asociado es la raiz de un arbol
		int externalId ;	///< Valor de numeración para implementar algoritmo de postorden transversal
		double potentialValue ;	///< Valor de doble presición con el cual se va a ponderar el nodo
		double weightValue ;	///< Valor de doble presición con el cual se va a ponderar el nodo
		int leftmost ;		///< El externalId del nodo descendiente sin hijos que se encuentra mas a la izquierda
		int deepestNoNodes ;    ///< Este valor corresponde a la maxima distancia grafoteorica que puede existir entre un nodo 
					/// y un descendiente suyo
};

#endif
