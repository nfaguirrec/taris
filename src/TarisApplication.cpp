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
#include "TarisApplication.h"

TarisApplication::TarisApplication()
{
	debugLevel = 0 ;
	doubleThresholdComparison = 1e-6 ;
}

TarisApplication::~TarisApplication()
{
}

TarisApplication* TarisApplication::instance = NULL ;
TarisApplication* TarisApplication::Instance()
{
	if( instance == NULL )
		instance = new TarisApplication() ;
	return instance ;
}

uint TarisApplication::getDebugLevel() const
{
	return debugLevel ;
}

void TarisApplication::setDebugLevel( uint debugLevel )
{
	this->debugLevel = debugLevel ;
}

double TarisApplication::getDoubleThresholdComparison() const
{
	return doubleThresholdComparison ;
}

void TarisApplication::setDoubleThresholdComparison( double doubleThresholdComparison )
{
	this->doubleThresholdComparison = doubleThresholdComparison ;
}

/**
 * Retorna el valor del parametro param, si se da un valor para def el parametro es tomado como opcional
 * @param argc valor obtenido de la función main
 * @param argv valor obtenido de la función main
 * @param param El parametro buscado
 * @param required true si el parametro es requerido
 * @param def valor por omisión del parámetro
 * @return valor del parametro
 */
char* TarisApplication::extractParameter( int argc, char **argv, const char *param, const char *def )
{
	int numberOfParameters = argc - 1 ;
	bool in = false ;
	
	for( int i=0; i < numberOfParameters; i++ ){
		if ( strcmp( argv[i+1], param ) == 0 ){
			if ( argv[i+2] != 0 ){
				in = true ;
				return argv[i+2] ;
			}else{
				cerr << "Can't find asked option " << param << endl ;
				exit(-1) ;
			}
		}
	}
	
	if( strlen( def ) != 0 ){
		if( in == false ) // Retorna el valor por omisión
			return (char*)def ;
	}else{
		cerr << "Parameter " << param << " is required" << endl ;
		exit(-1) ;
	}
}

