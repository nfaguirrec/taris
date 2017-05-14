/***************************************************************************
 *   TARIS                                                                 *
 *   Tree Analysis and Representation of Isopotential Surfaces             *
 *   http://taris.sourceforge.net                                          *
 *                                                                         *
 *   Universidad Nacional de Colombia                                      *
 *   http://www.unal.edu.co                                                *
 ***************************************************************************/

/*=========================================================================

  Project:   Molekel

  Copyright (c) CSCS - Swiss National Supercomputing Centre.
  You may use modify and and distribute this code freely providing this
  copyright notice appears on all copies of source code and an
  acknowledgment appears with any substantial usage of the code.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

=========================================================================*/

#ifndef MOLEKEL_H
#define MOLEKEL_H

#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <math.h>

#include "patterns.h"

using namespace std ;

#define MEMBLOCK 1024

#define BOHR         0.529177249
#define _1_BOHR      1.88972599

typedef struct CUTPLANE { float alpha, beta, a[3];
	int npts, ntri;
	float dd;
	float (**plane_point)[7];
	unsigned int (*tri)[3][2];
} Cutplane;

typedef struct { int persp          ; /* global */
	int background     ;
	int both_signs     ;
	int fullscreen     ;
	int addsurf        ;
	int depthcue       ;
	int smoothline     ;
	int manip_all      ;
	int independent    ;
	int rc_stereo      ;
	int texture        ;
	int use_track      ;
	int high_qual      ;
	int singlebuf      ;
	int acbuf          ;
	int use_suffix     ;
	int activ_logo     ;
	int lbl_on_top     ;
	int no_measure_lbl ;
	int solid_arrow    ;
	int keepbonds      ;
	int superimpose    ;
	int showfixed      ;
	int tgl_idle       ;
	int render         ;
	int coord_sys      ;
	int wire_model     ;/* templates for attributes*/
	int sticks         ;/* per molecule */
	int ball_and_stick ;
	int spacefill      ;
	int suppress_H     ;
	int labels         ;
	int atm_char       ;
	int atm_spin       ;
	int transparent    ;
	int box_on         ;
	int multiple_bonds ;
	int got_macufile   ;
	int cubeplanes     ;
	int draw_contour   ;
	int outline        ;
	int ribbon         ;
	int main_chain     ;
	int residues       ;
	int bond_col       ;
	int h_bond         ;
	int show_freq_arrow;
	int show_dipole         ;
	int surf_transp    ; /* templates for attributes*/
	int surf_clip      ; /* per surface */
	int chickenwire    ;
	int flatshade      ;
	int dot_surf       ;
} Bit;

typedef struct      { float x1, x2, y1, y2, z1, z2, cubesize;
	short nx, ny, nz;
	short flag;
} BoxM;

/* structures for macu */

typedef struct { int ix, iy, iz;
	int weight, index;
} mcPoint; 

typedef struct { float v[3]; float n[3];}   Surfdot;
typedef struct { unsigned int p1, p2, p3; } Tria; 

typedef struct { float  **value;
	mcPoint  **point; } mcPlane;
	typedef struct { float xmin, xmax, ymin, ymax, zmin, zmax;
		int   nx, ny, nz, len;
} MacuHeader;
	
typedef struct { float **value, vmin, vmax;
	float p1[3], p2[3], p3[3];
	short d1, d2;
} _2D_grid;

class Mol { 
	public:
		Mol(const char *name);
		~Mol();
	
	public:
		char *filename;
// 		Surface     *firstsurf;
		BoxM         box;
		int         plane_iz, plane_iy, plane_ix;
		
		Cutplane *plane;

		float ***cube_value, cubemin, cubemax, cutoff;
		string cube_name;
		int got_macufile   ;
};

void *alloc_3D(int nx, int ny, int nz, size_t size) ;
void free_3D(Mol *mp) ;

class Vector { 
	public:
		float x, y, z; 
		Vector() : x(0), y(0), z(0) { }
		Vector(float X, float Y, float Z) { x=X; y=Y; z=Z; }
} ;

void free_plane(mcPlane *p) ;
mcPlane *plane_alloc(int nx, int ny) ;
int init_cubes(Mol *mol) ;
int alloc_tridot(void) ;
void shift_planes(Mol *mol) ;
void rectangle(void) ;
void do_points(Mol *mol) ;
void do_cubes(Mol *mol) ;
float interpolate(Mol * mol, float c1, float v1, float c2, float v2) ;
int addPoint(mcPoint *p, float *coord,  int *np) ;
void triangulate(unsigned char lbl, unsigned int *e) ;
void init_cutplane(Mol *mp) ;
void set_cutplane(Mol *mp) ;
void tri_cutplane(Mol *mp) ;
float interpolate_cutplane_value(Mol *mp, float p[3]) ;
void vec3mat(float a[3], float m[3][3], float b[3]) ;
int add_cutplane_tri(Mol *mp, unsigned int tri[3][2]) ;
void cubes(Mol *mol, list<Surfdot>& dotList, list<Tria>& triList ) ;
int alloc_tridot(void) ;
void shift_planes(Mol *mol) ;
void rectangle(void) ;
void do_points(Mol *mol) ;
void do_cubes(Mol *mol) ;
float interpolate(Mol * mol, float c1, float v1, float c2, float v2) ;
int addPoint(mcPoint *p, float *coord,  int *np) ;
void triangulate(unsigned char lbl, unsigned int *e) ;
void read_gcube(Mol *mol,const char *fname ) ;
void init_cutplane(Mol *mp) ;
void set_cutplane(Mol *mp) ;
void tri_cutplane(Mol *mp) ;
float interpolate_cutplane_value(Mol *mp, float p[3]) ;
void vec3mat(float a[3], float m[3][3], float b[3]) ;
int add_cutplane_tri(Mol *mp, unsigned int tri[3][2]) ;

Surfdot* get_dot() ;
Tria* get_tri() ;
int get_npts() ;
int get_ntri() ;

#endif

