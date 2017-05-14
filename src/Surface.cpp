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
#include "Surface.h"

#include <fstream>
#include <set>
#include <math.h>

#define PI 3.14159265

#include <GTL/components.h>

#include "TarisApplication.h"

Surface::Surface()
{
}

Surface::Surface( const vector<Point>& lpo, const vector<Triangle>& ltri ):
pair< vector<Point>, vector<Triangle> >( lpo, ltri )
{
}

Surface::Surface( const Surface& surf ):
pair< vector<Point>, vector<Triangle> >( surf.first, surf.second )
{
}

Surface::~Surface()
{
}

PointVector Surface::getPoints() const
{
	return this->first ;
}

TriangleVector Surface::getTriangles() const
{
	return this->second ;
}

void Surface::setPoints( const PointVector& pv )
{
	this->first = pv ;
}

void Surface::setTriangles( const TriangleVector& tv )
{
	this->second = tv ;
}

inline double distance2( double x1, double y1, double z1, double x2, double y2, double z2 )
{
	if( ( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2) ) <= 0 )
		cerr << "Error: Distancia euclidiana menor a cero !!!" << endl ;
	return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2) ;
}


double Surface::computeArea(){

	double area = 0.0 ;

	for( TriangleVector::iterator it = this->second.begin(); it != this->second.end(); it++ ){
		
		// Cuadrado de las distancias entre los puntos del triángulo
		double a  = sqrt( distance2( it->getPoint1().getX(), it->getPoint1().getY(), it->getPoint1().getZ(),
				  it->getPoint2().getX(), it->getPoint2().getY(), it->getPoint2().getZ() ) ) ;
		
		double b  = sqrt( distance2( it->getPoint1().getX(), it->getPoint1().getY(), it->getPoint1().getZ(),
				  it->getPoint3().getX(), it->getPoint3().getY(), it->getPoint3().getZ() ) ) ;
		
		double c  = sqrt( distance2( it->getPoint2().getX(), it->getPoint2().getY(), it->getPoint2().getZ(),
				  it->getPoint3().getX(), it->getPoint3().getY(), it->getPoint3().getZ() ) );
		
		area += 0.25*sqrt( (a + (b + c))*(c - (a - b))*(c + (a - b))*(a + (b - c)) ) ;
	}
	
	return area ;
}

/**
 * 
 * @param filename
 * 
 */
void Surface::printOogl( ostream& os )
{
	if( TarisApplication::Instance()->getDebugLevel() >= 1 )
		cout << "Saving file in oogl format ... " ;

	os << "OFF" << endl ;
	os << first.size() << "   " << second.size() << endl ;
		
	map<int, int> ids ;
	int i=0 ;
	for( PointVector::iterator it = first.begin(); it != first.end(); it++, i++ ){
		os << it->getX() << "  " << it->getY() << "  " << it->getZ() << endl ;
		ids[it->getId()] = i ;
	}
	
	for( TriangleVector::iterator it = second.begin(); it != second.end(); it++ ){
		os << "3 " ;
		os << ids[ it->getPoint1().getId() ] << "  " ;
		os << ids[ it->getPoint2().getId() ] << "  " ;
		os << ids[ it->getPoint3().getId() ] << endl ;
	}
	
	if( TarisApplication::Instance()->getDebugLevel() >= 1 )
		cerr << "OK" << endl ;
}

/**
 * @return 
 * @warning El algoritmo de components de GTL siempre retorna false, pero no impide que éste funcione
 */
SurfacesVector Surface::computeComponents()
{
	SurfacesVector output ;
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
		cerr << endl ;
		cerr << "**************************************************************" << endl ;
		cerr << endl ;
		cerr << "        ALGORITMO DE BUSQUEDA DE COMPONENTES CONEXAS        " << endl ;
		cerr << "        --------------------------------------------        " << endl ;
		cerr << endl ;
		cerr << endl ;
		cerr << "  Construyendo grafo de superficie ... " ;
	}
	
	graph g ;
	map<int, node> nodeId ;

        /***********************************************************
	* Construir el grafo asociado a la superficie
	*/
	int i=0 ;
	for( PointVector::iterator it = first.begin(); it != first.end(); it++, i++ ){
		node n = g.new_node() ;
		nodeId[i] = n ;
	}

        /***********************************************************
	* Se construyen las aristas del grafo y al mismo tiempo
	* la lista de triangulos
	*/
	for( TriangleVector::iterator it = second.begin(); it != second.end(); it++ ){
		g.new_edge( nodeId[it->getPoint1().getId()], nodeId[it->getPoint2().getId()] ) ;
		g.new_edge( nodeId[it->getPoint2().getId()], nodeId[it->getPoint3().getId()] ) ;
		g.new_edge( nodeId[it->getPoint3().getId()], nodeId[it->getPoint1().getId()] ) ;
	}
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
		cerr << "OK" << endl ;
	
		cerr << endl ;
		cerr << "  Busqueda de componentes: " << endl ;
		cerr << "  -------------------------" << endl ;
		cerr << endl ;
	}
	
	components algol ;
	
        /***********************************************************
	* En este paso hay un problema, el algoritmo de check
	* siempre retorna false, así que hay que ver las 
	* propiedades del grafo construido
	*/
	if( TarisApplication::Instance()->getDebugLevel() >= 2 )
		cerr << "  Verificando condiciones del algoritmo ... " ;
	
	if (algol.check(g) != algorithm::GTL_OK){
		if( TarisApplication::Instance()->getDebugLevel() >= 2 )
			cerr << "OK" << endl ;
	}else{
		if( TarisApplication::Instance()->getDebugLevel() >= 2 )
			cerr << "Falló!!!!!!" << endl ;
	}
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 )
		cerr << "  Ejecutando el algoritmo ... " ;
	
	if (algol.run(g) == dfs::GTL_OK){
		if( TarisApplication::Instance()->getDebugLevel() >= 2 )
			cerr << "OK" << endl ;
	}else{
		if( TarisApplication::Instance()->getDebugLevel() >= 2 )
			cerr << "Falló!!!!!!" << endl ;
	}
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 )
		cerr << "  Reconstruyendo componentes ... " ;
	/**************************************************
	* Este mapa se usa para no repetir triangulos 
	* durante la creación de cada subsuperficie
	* el value es solamente un valor dummy
	*/
	set<Triangle> setTriangleTMP ;
		
		
	for( components::component_iterator it2 = algol.components_begin(); it2 != algol.components_end(); it2++ ){
		
		/**************************************************
		* Solo se seleccionan los componentes con más
		* de 10 puntos, sino el arbol queda con mucho ruido
		* HAY QUE BUSCAR LA MANERA DE QUE ESTE PARAMETRO SEA
		* MODIFICABLE POR EL USUARIO!!!!!!!
		*/
		/// @todo HAY QUE BUSCAR LA MANERA DE QUE ESTE PARAMETRO SEA MODIFICABLE POR EL USUARIO
		if( it2->first.size() > 10 ){
			Surface outputSurfaceTMP ;
			
			for( list<node>::iterator it3 = it2->first.begin(); it3 != it2->first.end(); it3++ ){
				
				/************************************************************
				* Hay que ter cuidado en esta linea ya que se esta
				* suponiendo que la numeración en el vector coincide cona la
				* numeració en el grafo
				*/
				outputSurfaceTMP.first.push_back( first[it3->id()] ) ;
				
				for( TriangleVector::iterator it1 =  second.begin(); it1 != second.end(); it1++ ){
					
					if( it1->contains( it3->id() ) ){
						setTriangleTMP.insert( *it1 ) ;
					}
				
				}
			}
			
			/****************************************************************
			* Esta parte es para llenar la lista de triágulos de la
			* subsuperficie creada donde ya han sido filtrados los triágulos
			* repetidos gracias al mapa mapTriangleTMP
			*/
			for( set<Triangle>::iterator it4 = setTriangleTMP.begin(); it4 != setTriangleTMP.end(); it4++ )
				outputSurfaceTMP.second.push_back( *it4 ) ;
			
			/******************************************************************
			* Se le asignan lista de superficies la que ha sido encontrada
			*/
			output.push_back( outputSurfaceTMP ) ;
			
			setTriangleTMP.clear() ;
		}
		
	}
	
	if( TarisApplication::Instance()->getDebugLevel() >= 2 ){
		cerr << "OK" << endl ;
		
		cerr << "  Número de componentes conexas encontradas = " << output.size() << endl ;
		cerr << endl ;
		cerr << "    Breve descripción: " << endl ;
		cerr << endl ;
		cerr << "       COMPONENTE\tPUNTOS\tTRIANGULOS   " << endl ;
		cerr << "       -----------------------------------" << endl ;
		for( SurfacesVector::iterator it = output.begin(); it != output.end(); it++ ){
			cerr << "    \t" << i << "\t\t" << it->first.size() << "\t" << it->second.size() <<  endl ;
		}
		
		cerr << endl ;
		cerr << "**************************************************************" << endl ;
		cerr << endl ;
	}
	
	return output ;
}

/**
 * El segundo y tercer parametro (0 y 0)obedecen al formato requerido por la funcion "isContainedIn"
 * @return Si una superficie esta contenida en otra
 */
bool Surface::contains( Surface s )
{
	return s.isContainedIn( *this , 0, 0) ;
}

/**
 * Calcula la distancia euclideana al cuadrado entre los puntos ( x1, y1, z1 ) y ( x2, y2, z2 )
 * @internal
 * @return El valor de la distancia euclideana al cuadrado 
 */


/**
 * ESTA ES LA FUNCION MAS IMPORTANTE DEL PROGRAMA. DETERMINA CUANDO UNA SUPERFICIE ESTA DENTRO DE OTRA.
 * 
 * s = es la grande
 * this = es la pequeña
 *
 @param a Es el numero del triangulo que se empleara en la superficie "this" para encontrar los valores promedio.
 @param b Es el numero del triangulo que se empleara en la superficie "s" para encontrar los valores promedio.
*/

bool Surface::isContainedIn( Surface s , int a, int b)
{
	int i = 0 ;
	int insideVector[3] = {0, 0, 0} ;
	bool inside = false ;
	
	for( TriangleVector::iterator it = s.second.begin(); it != s.second.end(); it++ ){
		
		/* **********************************************************************
		* PromInternal son las coordenas del punto de la superficie interna
		* que servira para trazar la recta entre superficies. PromExternal 
		* corresponde a al otro punto de la recta definido dobre la superfice 
		* de afuera
		*/

		
		// Aqui entra el parametro a
		double xPromInternal = ( this->second[a].getPoint1().getX() +
					this->second[a].getPoint2().getX() +
					this->second[a].getPoint3().getX() )/3.0 ;
		
		double yPromInternal = ( this->second[a].getPoint1().getY() +
					this->second[a].getPoint2().getY() +
					this->second[a].getPoint3().getY() )/3.0 ;
		
		double zPromInternal = ( this->second[a].getPoint1().getZ() +
					this->second[a].getPoint2().getZ() +
					this->second[a].getPoint3().getZ() )/3.0 ;
		
		// Aqui entra el parametro b
		double xPromExternal = ( s.second[b].getPoint1().getX() +
					s.second[b].getPoint2().getX() +
					s.second[b].getPoint3().getX() )/3.0 ;
		
		double yPromExternal = ( s.second[b].getPoint1().getY() +
					s.second[b].getPoint2().getY() +
					s.second[b].getPoint3().getY() )/3.0 ;
		
		double zPromExternal = ( s.second[b].getPoint1().getZ() +
					s.second[b].getPoint2().getZ() +
					s.second[b].getPoint3().getZ() )/3.0 ;
		
		
		/* **************************************************************
		 * Este ciclo es para evitar que xPromExternal-xPromInternal 
		 * sea igual a cero  y evitar que mas adelante quede una
		 * division por cero
		*/
		
		int j = a+1 ; 
		while( fabs(xPromExternal-xPromInternal) < 1e-6 ){
			
			xPromInternal = ( this->second[j].getPoint1().getX() +
					this->second[j].getPoint2().getX() +
					this->second[j].getPoint3().getX() )/3.0 ;
		
			yPromInternal = ( this->second[j].getPoint1().getY() +
					this->second[j].getPoint2().getY() +
					this->second[j].getPoint3().getY() )/3.0 ;
		
			zPromInternal = ( this->second[j].getPoint1().getZ() +
					this->second[j].getPoint2().getZ() +
					this->second[j].getPoint3().getZ() )/3.0 ;
			
			j++ ;
				
		}
		
		/* **********************************************************************
		* A continuación se definen los parametros a, b, c y d que determinan
		* la ecuación del plano definido por los puntos point1, point2 y point3
		*/
		
		double a = ( it->getPoint2().getY() - it->getPoint1().getY() )*( it->getPoint3().getZ() - it->getPoint1().getZ() )-
			   ( it->getPoint2().getZ() - it->getPoint1().getZ() )*( it->getPoint3().getY() - it->getPoint1().getY() ) ;
		
		double b = ( it->getPoint2().getZ() - it->getPoint1().getZ() )*( it->getPoint3().getX() - it->getPoint1().getX() )-
			   ( it->getPoint2().getX() - it->getPoint1().getX() )*( it->getPoint3().getZ() - it->getPoint1().getZ() ) ;
		
		double c = ( it->getPoint2().getX() - it->getPoint1().getX() )*( it->getPoint3().getY() - it->getPoint1().getY() )-
			   ( it->getPoint2().getY() - it->getPoint1().getY() )*( it->getPoint3().getX() - it->getPoint1().getX() ) ;
		
		double d = a*it->getPoint1().getX()+b*it->getPoint1().getY()+c*it->getPoint1().getZ() ;
		
		
		/* **********************************************************************
		* A continuación se calculan las coordenadas x, y, z que definen
		* al punto "p" de intersección del plano con la recta trazada entre las dos
		* superficies
		*/
		
		double x = (d-
				b*
				(
					(xPromInternal*yPromInternal-xPromInternal*yPromExternal)
					/
					( xPromExternal-xPromInternal ) 
					+
					yPromInternal
				)
					-
				c*
				(
					(xPromInternal*zPromInternal-xPromInternal*zPromExternal)
					/
					( xPromExternal-xPromInternal )
					+
					zPromInternal
				)
				
			   )
			/
			(
				a+b*( yPromExternal - yPromInternal )
				/
				( xPromExternal - xPromInternal )
				+
				c*( zPromExternal - zPromInternal )
				/
				( xPromExternal - xPromInternal )
			) ;
		
		
		
		double y = ( x - xPromInternal )*( yPromExternal - yPromInternal )/
			   ( xPromExternal - xPromInternal ) + yPromInternal;
		
		double z = ( x - xPromInternal )*( zPromExternal - zPromInternal )/
			   ( xPromExternal - xPromInternal ) + zPromInternal;
		
		
		
		// Cuadrado de la distancia del punto p a los puntos que forman el triángulo
		double dp1  = distance2( x, y, z, it->getPoint1().getX(), it->getPoint1().getY(), it->getPoint1().getZ() ) ;
		double dp2  = distance2( x, y, z, it->getPoint2().getX(), it->getPoint2().getY(), it->getPoint2().getZ() ) ;
		double dp3  = distance2( x, y, z, it->getPoint3().getX(), it->getPoint3().getY(), it->getPoint3().getZ() ) ;
		
		// Cuadrado de las distancias entre los puntos del triángulo
		double d12  = distance2( it->getPoint1().getX(), it->getPoint1().getY(), it->getPoint1().getZ(),
					 it->getPoint2().getX(), it->getPoint2().getY(), it->getPoint2().getZ() ) ;
		
		double d13  = distance2( it->getPoint1().getX(), it->getPoint1().getY(), it->getPoint1().getZ(),
					 it->getPoint3().getX(), it->getPoint3().getY(), it->getPoint3().getZ() ) ;
		
		double d23  = distance2( it->getPoint2().getX(), it->getPoint2().getY(), it->getPoint2().getZ(),
					 it->getPoint3().getX(), it->getPoint3().getY(), it->getPoint3().getZ() ) ;
		
		// Angulos formados por dos puntos del triangulo y el punto "p"
		double alpha_12 = acos( ( dp1 + dp2 - d12 )/( 2.0*sqrt(dp1*dp2) ) )*180.0/PI ;
		double alpha_13 = acos( ( dp1 + dp3 - d13 )/( 2.0*sqrt(dp1*dp3) ) )*180.0/PI ;
		double alpha_23 = acos( ( dp2 + dp3 - d23 )/( 2.0*sqrt(dp2*dp3) ) )*180.0/PI ;
		
		
		double totalAlpha = alpha_12 + alpha_13 + alpha_23 ;
		
		/* **************************************************************
		* Este if determina si el punto de intereccion "p" se encuentra por 
		* dentro o por fuera del triangulo, lo cual se sabe con la suma de los 
		* angulos internos. Si totalAlpha es igual a 360 ...
		*/
		
		if( ( fabs( totalAlpha - 360.0 ) < 1e-6 ) ){
			if( !inside ){
				insideVector[0] = ( (xPromExternal-xPromInternal)>=0 ? 1:-1 ) ;
				insideVector[1] = ( (yPromExternal-yPromInternal)>=0 ? 1:-1 ) ;
				insideVector[2] = ( (zPromExternal-zPromInternal)>=0 ? 1:-1 ) ;
				inside = true ;
				i++ ;
			}else{
				int insideVector2[3] ;
				
				insideVector2[0] = ( (x-xPromInternal)>=0 ? 1:-1 ) ;
				insideVector2[1] = ( (y-yPromInternal)>=0 ? 1:-1 ) ;
				insideVector2[2] = ( (z-zPromInternal)>=0 ? 1:-1 ) ;
				
				if( ( insideVector[0] == insideVector2[0] ) && 
					( insideVector[1] == insideVector2[1] ) && 
						( insideVector[2] == insideVector2[2] )){
					i++ ;
				}
			}
		}
	}
	
	return ( ( i%2 == 0 ) ? false : true )  ;
}


