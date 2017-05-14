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
#ifndef TARISAPPLICATION_H
#define TARISAPPLICATION_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
using namespace std ;

#define uint unsigned int

/**
 *  @brief Clase de tipo <b>"Singleton"</b> que almacena todas los parámetros globales de la aplicación
 *
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> 2007-03-18
 */
class TarisApplication{
	static TarisApplication* instance ;
	
	protected:
		TarisApplication();
		~TarisApplication();
		
	public:
		static TarisApplication* Instance() ;
		
		uint getDebugLevel() const ;
		void setDebugLevel( uint debugLevel ) ;
		double getDoubleThresholdComparison() const ;
		void setDoubleThresholdComparison( double doubleThresholdComparison ) ;
		
		static char* extractParameter( int argc, char **argv, const char *param, const char *def = ""  ) ;
		
	private:
		uint debugLevel ;
		double doubleThresholdComparison ;
};

#endif
