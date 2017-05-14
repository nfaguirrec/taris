/*=========================================================================

  Project:   Molekel

  Copyright (c) CSCS - Swiss National Supercomputing Centre.
  You may use modify and and distribute this code freely providing this
  copyright notice appears on all copies of source code and an
  acknowledgment appears with any substantial usage of the code.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

=========================================================================*/

#include "molekel.h"

/*----------------------------------------*/
Bit bit = {1,  /* persp          */
	1,  /* background     */
	0,  /* both_signs     */
	0,  /* fullscreen     */
	0,  /* addsurf        */
	0,  /* depthcue       */
	0,  /* smoothline     */
	0,  /* manip_all      */
	1,  /* independent    */
	0,  /* rc_stereo      */
	1,  /* texture        */
	1,  /* use_track      */
	1,  /* high_qual      */
	0,  /* singlebuf      */
	0,  /* acbuf          */
	1,  /* use_suffix     */
	1,  /* activ_logo     */
	1,  /* lbl_on_top     */
	0,  /* no_measure_lbl */
	1,  /* solid_arrow    */
	1,  /* keepbonds      */
	0,  /* superimpose    */
	1,  /* showfixed      */
	0,  /* tgl_idle       */
	0,  /* render         */
	0,  /* coord_sys      */
	0,  /* wire_model     */
	1,  /* sticks         */
	0,  /* ball_and_stick */
	0,  /* spacefill      */
	0,  /* suppress_H     */
	0,  /* labels         */
	0,  /* atm_char       */
	0,  /* atm_spin       */
	0,  /* transparent    */
	0,  /* box_on         */
	1,  /* multiple_bonds */
	0,  /* got_macufile   */
	0,  /* cubeplanes     */
	0,  /* draw_contour   */
	0,  /* outline        */
	0,  /* ribbon         */
	1,  /* main_chain     */
	0,  /* residues       */
	0,  /* bond_col       */
	0,  /* h_bond         */
	0,  /* freq_arrow     */
	0,  /* show dipole    */
	0,  /* surf_transp    */
	0,  /* surf_clip      */
	0,  /* chickenwire    */
	0,  /* flatshade      */
	0,  /* dot_surf       */
};
/*----------------------------------------*/

/*----------------------------------------*/
/* Globals */
float cubemin = 0, cubemax = 0;
Mol *project_mol;
/*----------------------------------------*/
static Surfdot *dot;
static Tria *tri;

int nMacuPlanes;
//char  macu_name[30];

MacuHeader    cubehead;
_2D_grid      projection = {NULL};

static float         xmin, xmax, ymin, ymax, zmin, zmax;
static float         za, zb, dx, dy, dz, invdx, invdy, invdz;
static int           nx, ny, nz, iz;
static int           dotmem, trimem;
static int           npts, ntri, surface_number = 0;
float                rectcol[] = {1.0, 1.0, 0.0, 1.0};
static tri_pattern   **pattern;
static unsigned char *npat;
unsigned char        **ccase;
static mcPlane         *A, *B, *C;

FILE *fpt;
static long previous_line = 0, preprevious = 0, preprepre = 0;
static char line[256];

Mol::Mol(const char *name)
{
	
	this->plane_iz = this->plane_iy = this->plane_ix = 0;
	
	if (name)
		this->filename  = strdup(name);
	else
		this->filename  = NULL;
	
	this->plane        = NULL;
	
	this->got_macufile   = 0;
	this->cube_value     = NULL;
	this->cubemin = this->cubemax = this->cutoff = 0;
	
}

void *alloc_3D(int nx, int ny, int nz, size_t size)
{
	void ***temp;
	void **pointerarray;
	char *array;
	int  _2D;
	register int i;

	if(_2D = !nz) nz = 1;
	if((array = (char*) malloc(nx*ny*nz*size)) == NULL) return NULL;
	/* array will hold the data */
	if((pointerarray = (void**) malloc(ny*nz*sizeof(char *))) == NULL) return NULL;
	/* pointerarray will hold the pointers to the rows of data */
	for(i=0; i<ny*nz; i++) pointerarray[i] = array + i * nx * size;
	if (ny*nz==0) pointerarray[0] = array;

	if(_2D) return pointerarray;

	if((temp = (void***) malloc(nz*sizeof(char **))) == NULL) return NULL;
	/* temp will hold the pointers to the planes */
	for(i=0; i<nz; i++)    temp[i] = pointerarray + i * ny;

	return temp;
}

void free_3D(Mol *mp)
{
	if(!mp) return;

	mp->got_macufile = 0;
	if(mp->cube_value){
		free(mp->cube_value[0][0]);
		free(mp->cube_value[0]);
		free(mp->cube_value);
		mp->cube_value = NULL;
	}

	mp->cubemin = mp->cubemax = mp->cutoff = 0;

	cubemin = cubemax = 0;
}

Mol::~Mol() 
{	
	if (this->cube_value)    free_3D(this);
	if (this->filename)      free(this->filename);
}

void free_plane(mcPlane *p)
{
	free(p->point[0]);
	free(p->point);
	free(p);
}

mcPlane *plane_alloc(int nx, int ny)
{
	mcPoint  *pointarray, *pp;
	mcPlane  *temp;
	int i;

	if((temp = (mcPlane*) malloc(sizeof(mcPlane))) == NULL) return NULL;

	if((pointarray = (mcPoint*) malloc(nx*ny*sizeof(mcPoint))) == NULL) return NULL;
	if((temp->point = (mcPoint**) malloc(ny*sizeof(mcPoint *))) == NULL) return NULL;

	for(i=0; i<ny; i++){
		temp->point[i]  = pointarray + i*nx;
	}

	for(i=0, pp = pointarray; i<nx*ny; i++, pp++){
		pp->weight = pp->index = 0;
		pp->ix = pp->iy = pp->iz = 0;
	}

	return temp;
}

void cubes( Mol *mol, list<Surfdot>& dotList, list<Tria>& triList )
{
	char str[50];
	int  npass;
	Vector ***gradientCube;
	
	dot = NULL;
	tri = NULL;
	
	xmin = mol->box.x1;
	xmax = mol->box.x2;
	ymin = mol->box.y1;
	ymax = mol->box.y2;
	zmin = mol->box.z1;
	zmax = mol->box.z2;
	nx =  mol->box.nx;
	ny =  mol->box.ny;
	nz =  mol->box.nz;
	
	if(init_cubes(mol)) return;
	
	npass = (bit.both_signs && mol->cutoff) ? 2 : 1;
	
	dx = (xmax-xmin)/((float)nx-1.);
	dy = (ymax-ymin)/((float)ny-1.);
	dz = (zmax-zmin)/((float)nz-1.);
	
	invdx = 0.5/dx;
	invdy = 0.5/dy;
	invdz = 0.5/dz;

	if(mol->cutoff >= 0) {
		pattern = pat_B;
		npat    = npat_B;
	}else {
		pattern = pat_A;
		npat    = npat_A;
	}
	
	ntri = npts = 0;
	if(!alloc_tridot()) return;
	zb = zmin - dz;
	memset(ccase[0], 0, (nx+1)*(ny+1));
   
	if(mol->cutoff >= 0) {
		pattern = pat_B;
		npat    = npat_B;
	}else {
		pattern = pat_A;
		npat    = npat_A;
	}
	
	ntri = npts = 0;
	if(!alloc_tridot()) return;
	zb = zmin - dz;
	memset(ccase[0], 0, (nx+1)*(ny+1));
	
		 
	for(iz = 0; iz < nz; iz++){
		shift_planes(mol);
		do_points(mol);
		do_cubes(mol);
	}
		
	for( int i = 0; i < npts; i++, dot++ ){
		dotList.push_back( *dot ) ;
	}
	
	for( int i = 0; i < ntri; i++, tri++ ){
		triList.push_back( *tri ) ;
	}
}

int init_cubes(Mol *mol)
{
	static const char *errmsg[] =  { "Marching Cubes :",
		"load a .macu-file first",
		"mol->cutoff below minimal value!",
		"mol->cutoff above maximal value!",
		"can't allocate enough memory for plane A\n",
		"can't allocate enough memory for plane B\n",
		"can't allocate enough memory for plane C\n" 
	};
	char str[100];

	if(!mol->got_macufile){
		sprintf(str, "%s\n%s", errmsg[0], errmsg[1]); 
// 		showinfobox(str);
		return 1;
	}
	if(mol->cutoff < mol->cubemin){
		sprintf(str, "%s\n%s", errmsg[0], errmsg[2]); 
// 		showinfobox(str);
		return 2;
	}
	if(mol->cutoff >\
		  ((bit.both_signs && (-1*mol->cubemin) > mol->cubemax) ?\
		  (-1*mol->cubemin) : mol->cubemax)){
		sprintf(str, "%s\n%s", errmsg[0], errmsg[3]); 
// 		showinfobox(str);
		return 3;
		  }
		  if((A = plane_alloc(nx, ny)) == NULL){
			  sprintf(str, "%s\n%s", errmsg[0], errmsg[4]); 
// 			  showinfobox(str);
			  return 4;
		  }
		  if((B = plane_alloc(nx, ny)) == NULL){
			  sprintf(str, "%s\n%s", errmsg[0], errmsg[5]); 
// 			  showinfobox(str);
			  return 5;
		  }
		  if((C = plane_alloc(nx, ny)) == NULL){
			  sprintf(str, "%s\n%s", errmsg[0], errmsg[6]); 
// 			  showinfobox(str);
			  return 6;
		  }

		  return 0;
}

int alloc_tridot(void)
{
	if((dot = (Surfdot*) malloc(MEMBLOCK * sizeof(Surfdot))) == NULL){
// 		showinfobox("can't allocate enough memory (dot)");
		return 0;
	}
	dotmem = MEMBLOCK;
	if((tri = (Tria*) malloc(2 * MEMBLOCK * sizeof(Tria))) == NULL){
// 		showinfobox("can't allocate enough memory (tri)");
		return 0;
	}
	trimem = 2 * MEMBLOCK;

	return 1;
}

/*
 *   shift the three actual planes one plane upwards in the box
 */
void shift_planes(Mol *mol)
{
	register short ix, iy;
	mcPlane *temp;
	mcPoint *pt;
	
	temp = A;
	A = B;
	B = C;
	C = temp;
	
	for(iy=0; iy<ny+1; iy++){
		for(ix=0; ix<nx+1; ix++) 
			ccase[iy][ix] >>= 4;
	}
		
	for(iy=0; iy<ny; iy++){
		for(ix=0, pt = C->point[iy]; ix<nx; ix++, pt++) {
			pt->weight = pt->index = pt->ix = pt->iy = pt->iz = 0;
		}
	}
	
	C->value = mol->cube_value[iz+1];
	B->value = mol->cube_value[iz];
	if(iz) A->value = mol->cube_value[iz-1];
	
	za = zb;
	zb += dz;
	rectangle();
}

void rectangle(void)
{
	float corner[4][3];
	register short i;
	
	corner[0][0] = xmin; corner[0][1] = ymin; corner[0][2] = zb;
	corner[1][0] = xmax; corner[1][1] = ymin; corner[1][2] = zb;
	corner[2][0] = xmax; corner[2][1] = ymax; corner[2][2] = zb;
	corner[3][0] = xmin; corner[3][1] = ymax; corner[3][2] = zb;
	
// 	glDisable(GL_LIGHTING);
// 	glColor4fv(rectcol);
// 	glBegin(GL_LINE_LOOP);
// 	for(i=0; i<4; i++) glVertex3fv(corner[i]);
// 	glEnd();
// 	glEnable(GL_LIGHTING);
}

/*
 *  determine the surface-points
 *     as the intersection of the contour with the grid
 *     using linear interpolation for position and for the surface-normal
 */
void do_points(Mol *mol)
{
	register short ix, iy;
	float x, y, value;
	float coord[3], difference1, difference2;
	int index;
	
	for(iy=0; iy<ny; iy++){
		y = ymin + iy*dy;
		
		for(ix=0; ix<nx; ix++){
			x = xmin + ix*dx;
			
			value =  B->value[iy][ix];
			
			if(ix<(nx-1)){
				if((value >= mol->cutoff) ^ (B->value[iy][ix+1] >= mol->cutoff)) {
					coord[0] = interpolate(mol, x, value, x+dx, B->value[iy][ix+1]);
					coord[1] = y;
					coord[2] = zb;
					
					difference1 = fabsf(value - mol->cutoff);
					difference2 = fabsf(B->value[iy][ix+1] - mol->cutoff);
					
					if(difference1 < difference2) {
						index = addPoint(&B->point[iy][ix], coord, &npts);
					}else {
						index = addPoint(&B->point[iy][ix+1], coord, &npts);
					}
					
					B->point[iy][ix].ix = index;
				}
			}
			
			if(iy<(ny-1)){
				if((value >= mol->cutoff) ^ (B->value[iy+1][ix] >= mol->cutoff)) {
					coord[0] = x;
					coord[1] = interpolate(mol, y, value, y+dy, B->value[iy+1][ix]);
					coord[2] = zb;
					
					difference1 = fabsf(value - mol->cutoff);
					difference2 = fabsf(B->value[iy+1][ix] - mol->cutoff);
					
					if(difference1 < difference2) {
						index = addPoint(&B->point[iy][ix], coord, &npts);
					}else {
						index = addPoint(&B->point[iy+1][ix], coord, &npts);
					}
					
					B->point[iy][ix].iy = index;
				}
			}
			
			if (iz){
				if((value >= mol->cutoff) ^ (A->value[iy][ix] >= mol->cutoff)) {
					coord[0] = x;
					coord[1] = y;
					coord[2] = interpolate(mol, zb, value, za, A->value[iy][ix]);
					difference1 = fabsf(value - mol->cutoff);
					difference2 = fabsf(A->value[iy][ix] - mol->cutoff);
					
					if(difference1 < difference2) {
						index = addPoint(&B->point[iy][ix], coord, &npts);
					}else {
						index = addPoint(&A->point[iy][ix], coord, &npts);
					}
					
					B->point[iy][ix].iz = index;
				}
			}
		
			if(6*(npts + 3) > dotmem){
				if((dot  = (Surfdot*) realloc(dot,(dotmem + MEMBLOCK) * sizeof(float))) == NULL){
					fprintf(stderr, " Can't reallocate enough memory ");
					return;
				}
				dotmem += MEMBLOCK;
			}
		}
	}
}

/*
 *  Determine the high-order bits of the cube-label,
 *  then triangulate the cubes
 */
void do_cubes(Mol *mol)
{
	register short ix, iy;
	unsigned int e[12];
   
	for(iy=0; iy<ny; iy++){
		for(ix=0; ix<nx; ix++){
			if(B->value[iy][ix] >= mol->cutoff){
				ccase[iy+1][ix+1] |=  16;
				ccase[iy+1][ix]   |=  32;
				ccase[iy][ix+1]   |= 128;
				ccase[iy][ix]     |=  64;
			}
		}
	}

	if(!iz) return;

	for(iy=1; iy<ny; iy++){
		for(ix=1; ix<nx; ix++){
			if((ccase[iy][ix]) && (ccase[iy][ix] != 255)){
				e[0]  = A->point[iy-1][ix-1].ix;
				e[1]  = A->point[iy-1][ix].iy;
				e[2]  = A->point[iy][ix-1].ix;
				e[3]  = A->point[iy-1][ix-1].iy;
				e[4]  = B->point[iy-1][ix-1].ix; 
				e[5]  = B->point[iy-1][ix].iy;
				e[6]  = B->point[iy][ix-1].ix;
				e[7]  = B->point[iy-1][ix-1].iy;
				e[8]  = B->point[iy-1][ix-1].iz;
				e[9]  = B->point[iy-1][ix].iz;
				e[10] = B->point[iy][ix].iz;
				e[11] = B->point[iy][ix-1].iz;

				triangulate(ccase[iy][ix], e);
			}
		}
	}
}

/*
 *  linear inerpolation of the grid-points 1 and 2
 *  (coordinate c, vector a, value v) to get the normal at the surface-point
 *  returns the coordinate of the surface-point
 */
float interpolate(Mol * mol, float c1, float v1,
		  float c2, float v2)
{
	float r;

	r = (mol->cutoff-v1)/(v2-v1);
/*
	dot[npts].n[0] = a1.x + (a2.x - a1.x)*r;
	dot[npts].n[1] = a1.y + (a2.y - a1.y)*r;
	dot[npts].n[2] = a1.z + (a2.z - a1.z)*r;
*/
	return(c1 + (c2-c1)*r);
}


int addPoint(mcPoint *p, float *coord,  int *np)
{
	if(!p->weight) {
		p->index = *np;
		*np += 1;
		dot[p->index].v[0] = coord[0];
		dot[p->index].v[1] = coord[1];
		dot[p->index].v[2] = coord[2];
		dot[p->index].n[0] = 0.0;
		dot[p->index].n[1] = 0.0;
		dot[p->index].n[2] = 1.0;
		p->weight = 1;
	}else {
		dot[p->index].v[0] = ((float)p->weight * dot[p->index].v[0] + coord[0])/(float)(p->weight + 1);
		dot[p->index].v[1] = ((float)p->weight * dot[p->index].v[1] + coord[1])/(float)(p->weight + 1);
		dot[p->index].v[2] = ((float)p->weight * dot[p->index].v[2] + coord[2])/(float)(p->weight + 1);
		p->weight += 1;
	}
	
	return p->index;
}

void triangulate(unsigned char lbl, unsigned int *e)
{
	register int i;
	int pt[3];
	tri_pattern *tp;
	
	tp = pattern[lbl];
	
	for (i=0; i<npat[lbl]; i++, tp++){
		pt[0] = e[tp->edge0];
		pt[1] = e[tp->edge1];
		pt[2] = e[tp->edge2];
		
		if((pt[0] != pt[1]) && (pt[0] != pt[2]) && (pt[1] != pt[2])) {
		
			tri[ntri].p1 = pt[0];
			tri[ntri].p2 = pt[1];
			tri[ntri].p3 = pt[2];
		
			ntri++;
		}
	}
	
	if((ntri+6) > trimem){
		if((tri = (Tria*) realloc(tri, (trimem + 2*MEMBLOCK)*sizeof(Tria))) == NULL){
			fprintf(stderr, " Can't reallocate enough memory ");
			return;
		}
		
		trimem += 2 * MEMBLOCK;
	}
}

void read_gcube(Mol *mol,const char *fname )
{
	FILE *fp;
	BoxM *bp;
	register float *p;
	register int h, i, j, k;
	static char str[50];
	const char *s;
	float sum, dv;

	int isorb = 0, natoms, dimx, dimy, dimz;
	int norb, ijnk1;
	float xi, yi, zi, xf, yf, zf, xinc, yinc, zinc;
	float rjnk1, rjnk2, rjnk3;
	char buffer[132];
	char *bgnPtr, *endPtr;
	double value;
	
	if(!mol){
// 		logprint("readgcube: no molecule loaded");
// 		update_logs();
		return;
	}
	
	if((fp = fopen(fname, "r")) == NULL){
		sprintf(str, "can't open %s \n", fname);
// 		showinfobox(str);
		return;
	}

	fgets(buffer, 132, fp);
	fgets(buffer, 132, fp);
	fgets(buffer, 132, fp);
	sscanf(buffer, "%d%f%f%f", &natoms, &xi, &yi, &zi);
	if( natoms < 0 ) { natoms=-natoms;isorb=1;}
	fgets(buffer, 132, fp);
	sscanf(buffer, "%d%f%f%f", &dimx, &xinc, &rjnk2, &rjnk3);
	xf=(dimx-1)*xinc+xi;
	fgets(buffer, 132, fp);
	sscanf(buffer, "%d%f%f%f", &dimy, &rjnk1, &yinc, &rjnk3);
	yf=(dimy-1)*yinc+yi;
	fgets(buffer, 132, fp);
	sscanf(buffer, "%d%f%f%f", &dimz, &rjnk1, &rjnk2, &zinc);
	zf=(dimz-1)*zinc+zi;

	bp = &mol->box;
	xmin = bp->x1 = xi * BOHR;
	xmax = bp->x2 = xf * BOHR;
	ymin = bp->y1 = yi * BOHR;
	ymax = bp->y2 = yf * BOHR;
	zmin = bp->z1 = zi * BOHR;
	zmax = bp->z2 = zf * BOHR;
	nx   = bp->nx = dimx;
	ny   = bp->ny = dimy;
	nz   = bp->nz = dimz;
	nMacuPlanes = nz;

	i=0;
	while (i<natoms) {
		i=i+1;
		fgets(buffer, 132, fp);
	}

	if( isorb == 1) {
		fgets(buffer, 132, fp);
		sscanf(buffer, "%d%d", &ijnk1, &norb);
		if( ijnk1 > 1 ) { 
			fclose(fp);
			return;
		}
		sprintf(str," this is orbital number %d\n",norb);
	}


	if((s = strrchr(fname, '/')) == NULL) s = fname;
	else s++;
	mol->cube_name = s;
	sprintf(str, "xmin =  ymin =  zmin =");
	sprintf(str, "%6.2f  %6.2f  %6.2f", xmin, ymin, zmin);
	sprintf(str, "xmax =  ymax =  zmax =");
	sprintf(str, "%6.2f  %6.2f  %6.2f", xmax, ymax, zmax);
	sprintf(str, "nx = %3d  ny = %3d  nz = %3d", nx, ny, nz); 

	free_3D(mol);
	if((mol->cube_value = (float***) alloc_3D(nx, ny, nz, sizeof(float))) == NULL){
		return;
	}
	if((ccase = (unsigned char**) alloc_3D(nx+1, ny+1, 0, sizeof(char))) == NULL){
		return;
	}
	i = j = k = 0;
	while (fgets(buffer, 132, fp) != NULL) {
		bgnPtr = buffer;
		for (h = 0; h < 100; h++) {
			value = strtod(bgnPtr, &endPtr);
			if ((value == 0.0) && (endPtr == bgnPtr)) {
				continue;
			}
			mol->cube_value[k][j][i] = (float)value;
			bgnPtr = endPtr;
			if(k<nz-1) k++;
			else if(j<ny-1) {
				j++; k = 0;
			}
			else if(i<nx-1) {
				i++; j = 0; k = 0;
			}
		}
	}
	
	fclose(fp);

	p = mol->cube_value[0][0];
	cubemin = cubemax = *p;
	sum = 0;
	for(i=1; i < (nx*ny*nz); i++, p++){
		if(*p > cubemax) cubemax = *p;
		if(*p < cubemin) cubemin = *p;
		sum += *p;
	}
	if(cubemin < 0.000001 && cubemin > -0.000001) cubemin = 0.0;
	if(cubemax < 0.000001 && cubemax > -0.000001) cubemax = 0.0;
	mol->cubemin = cubemin;
	mol->cubemax = cubemax;
	mol->got_macufile = 1;
	mol->cutoff = mol->cutoff = 0.00;
	mol->plane_iz = cubehead.nz/2;
	mol->plane_iy = cubehead.ny/2;
	mol->plane_ix = cubehead.nx/2;
	init_cutplane(mol);

	dv = (xmax-xmin) * _1_BOHR / (nx-1.) *
			(ymax-ymin) * _1_BOHR / (ny-1.) *
			(zmax-zmin) * _1_BOHR / (nz-1.);
	sum *= dv;
	sprintf(str, " (a.u.) = %f", sum);
}

void init_cutplane(Mol *mp)
{
	int size, k, i, j;
	float zerox, zeroy, dd;

	if((mp->plane = (Cutplane *)malloc(sizeof(Cutplane))) ==NULL) {
// 		showinfobox("Can't allocate memory for plane");
		return;
	}
	dx = (xmax-xmin)/((float)nx-1.);
	dy = (ymax-ymin)/((float)ny-1.);
	dz = (zmax-zmin)/((float)nz-1.);
	mp->plane->a[0] = xmin + dx*(nx-1)*0.5;
	mp->plane->a[1] = ymin + dy*(ny-1)*0.5;
	mp->plane->a[2] = zmin + dz*(nz-1)*0.5;
	mp->plane->alpha = 0.0;
	mp->plane->beta = 0.0;
	dd = 0.5*sqrt(dx*dx + dy*dy + dz*dz);
	mp->plane->npts = 0;
	mp->plane->ntri = 0;
	mp->plane->tri = NULL;
	size = (int)(sqrt((xmax-xmin)*(xmax-xmin)+(ymax-ymin)*(ymax-ymin)+(zmax-zmin)*(zmax-zmin))/dd + 1);
	mp->plane->npts = size;
	if((mp->plane->plane_point = (float(**)[7])alloc_3D(size, size, 0, sizeof(float[7]))) ==NULL) {
// 		showinfobox("Can't allocate memory for plane");
		return;
	}
	zerox = mp->plane->a[0] - size/2 * dd;
	zeroy = mp->plane->a[1] - size/2 * dd;
	for(j=0; j<size; j++) {  // fill row per row
		for(i=0; i<size; i++) {
			mp->plane->plane_point[j][i][0] = zerox + i*dd;
			mp->plane->plane_point[j][i][1] = zeroy + j*dd;
		}
	}
	set_cutplane(mp);
}


void set_cutplane(Mol *mp)
{
	float m1[3][3], m2[3][3], n1[3], n2[3], n3[3];
	float alpha, beta;
	int register i, j;

	if(mp->plane->ntri) {
		free(mp->plane->tri);
		mp->plane->tri = NULL;
		mp->plane->ntri = 0;
	}
	xmin = mp->box.x1;
	xmax = mp->box.x2;
	ymin = mp->box.y1;
	ymax = mp->box.y2;
	zmin = mp->box.z1;
	zmax = mp->box.z2;
	nx =  mp->box.nx;
	ny =  mp->box.ny;
	nz =  mp->box.nz;

	dx = (xmax-xmin)/((float)nx-1.);
	dy = (ymax-ymin)/((float)ny-1.);
	dz = (zmax-zmin)/((float)nz-1.);

	alpha = mp->plane->alpha/180*M_PI;  
	beta = mp->plane->beta/180*M_PI;
	n1[0] = n1[1] = 0;
	n1[2] = 1;
	m1[0][0] = 1;
	m1[1][0] = 0;
	m1[2][0] = 0;
	m1[0][1] = 0;
	m1[1][1] = cos(beta);
	m1[2][1] = sin(beta);
	m1[0][2] = 0;
	m1[1][2] = -sin(beta);
	m1[2][2] = cos(beta);
	m2[0][0] = cos(alpha);
	m2[1][0] = 0;
	m2[2][0] = sin(alpha);
	m2[0][1] = 0;
	m2[1][1] = 1;
	m2[2][1] = 0;
	m2[0][2] = -sin(alpha);
	m2[1][2] = 0;
	m2[2][2] = cos(alpha);

	for(j=0; j<mp->plane->npts; j++) {
		for(i=0; i<mp->plane->npts; i++) {
			n1[0] = mp->plane->plane_point[j][i][0];
			n1[1] = mp->plane->plane_point[j][i][1];
			n1[2] = 0;
			vec3mat(n1, m1, n2);
			vec3mat(n2, m2, n3);
			n3[0] = n3[0] + mp->plane->a[0];
			n3[1] = n3[1] + mp->plane->a[1];
			n3[2] = n3[2] + mp->plane->a[2];
			mp->plane->plane_point[j][i][6] = 0;
			if(n3[0] >= mp->box.x1 && n3[0] <= mp->box.x2) {
				if(n3[1] >= mp->box.y1 && n3[1] <= mp->box.y2) {
					if(n3[2] >= mp->box.z1 && n3[2] <= mp->box.z2) {
						mp->plane->plane_point[j][i][2] = n3[0];
						mp->plane->plane_point[j][i][3] = n3[1];
						mp->plane->plane_point[j][i][4] = n3[2];
						mp->plane->plane_point[j][i][5] = interpolate_cutplane_value(mp, n3);
						mp->plane->plane_point[j][i][6] = 1;
					}
				}
			}
		}
	}
	tri_cutplane(mp);
}

void tri_cutplane(Mol *mp)
{
	int size, jj=0;
	int register i, j;
	unsigned int tri[3][2];

	size = mp->plane->npts;
	for(j=0; j<size-1; j++) {
//printf("j: %d\n", j);
		i = 0;
		jj = 0;
		while(i<size) {
//printf("i: %d\n", i);
			if(mp->plane->plane_point[j][i][6]) {
				tri[0][0]=j;
				tri[0][1]=i;
				i++;
				if(i<size && mp->plane->plane_point[j][i][6]) {
					tri[1][0]=j;
					tri[1][1]=i;
					while(jj < size) {
//printf("jj1: %d\n", jj);
						if(mp->plane->plane_point[j+1][jj][6]) {
							tri[2][0]=j+1;
							tri[2][1]=jj;
// add trangle
//printf("position 1\n");
							if(!add_cutplane_tri(mp, tri)) return;
							break;
						} else {
							jj++;
						}
					}
				}
				else {
					if(mp->plane->ntri == 0) i--;
				}
				while (jj<i) {
//printf("jj2: %d\n", jj);
					if(!mp->plane->plane_point[j+1][jj][6]) {
						jj++;
						continue;
					}
					tri[0][0]=j;
					tri[0][1]=i;
					tri[1][0]=j+1;
					tri[1][1]=jj;
					jj++;
					if(jj < size && mp->plane->plane_point[j+1][jj][6]) {
						tri[2][0]=j+1;
						tri[2][1]=jj;
//printf("position 2\n");
						if(!add_cutplane_tri(mp, tri)) return;
					}
				}
				if(i+1<size && !mp->plane->plane_point[j][i+1][6]) {
					jj++;
					while (jj<size) {
//printf("jj3: %d\n", jj);
						if(!mp->plane->plane_point[j+1][jj][6]) break;
						tri[0][0]=j;
						tri[0][1]=i;
						tri[1][0]=j+1;
						tri[1][1]=jj-1;
						tri[2][0]=j+1;
						tri[2][1]=jj;
//printf("position 3\n");
						if(!add_cutplane_tri(mp, tri)) return;
						jj++;
					}
				}
				if(mp->plane->ntri != 0) i--;
			}
			i++;
		}
	}
}

float interpolate_cutplane_value(Mol *mp, float p[3])
{
	int i, ix, iy, iz;
	float ddx, ddy, ddz, x, y, z;
	float p000, p100, p110, p010, p001, p101, p111, p011;
	float t[8];
	float v = 0;

	ix = (int)floor((p[0] - xmin) / dx);
	iy = (int)floor((p[1] - ymin) / dy);
	iz = (int)floor((p[2] - zmin) / dz);
	ddx = (p[0] - xmin) - dx*ix;
	ddy = (p[1] - ymin) - dy*iy;
	ddz = (p[2] - zmin) - dz*iz;
	ix = (ix<nx) ? ix : ix-1;
	iy = (iy<ny) ? iy : iy-1;
	iz = (iz<nz) ? iz : iz-1;
	x = ddx/dx;
	y = ddy/dy;
	z = ddz/dz;

	p000 = mp->cube_value[iz][iy][ix];
	p100 = mp->cube_value[iz][iy][ix+1];
	p010 = mp->cube_value[iz][iy+1][ix];
	p001 = mp->cube_value[iz+1][iy][ix];
	p110 = mp->cube_value[iz][iy+1][ix+1];
	p101 = mp->cube_value[iz+1][iy][ix+1];
	p011 = mp->cube_value[iz+1][iy+1][ix];
	p111 = mp->cube_value[iz+1][iy+1][ix+1];

	t[0] = p000*(1-x)*(1-y)*(1-z);
	t[1] = p100*x*(1-y)*(1-z);
	t[2] = p010*(1-x)*y*(1-z);
	t[3] = p001*(1-x)*(1-y)*z;
	t[4] = p101*x*(1-y)*z;
	t[5] = p011*(1-x)*y*z;
	t[6] = p110*x*y*(1-z);
	t[7] = p111*x*y*z;

	for(i=0; i<8; i++) {
		v += t[i];
	}

	return v;
}

void vec3mat(float a[3], float m[3][3], float b[3])
{
	register int i;

	for(i=0; i<3; i++){
		b[i] = a[0]*m[0][i] + a[1]*m[1][i] + a[2]*m[2][i];
	}
}

int add_cutplane_tri(Mol *mp, unsigned int tri[3][2])
{
	if(!mp->plane->tri) {
		if((mp->plane->tri = (unsigned int(*)[3][2])malloc(sizeof(unsigned int[3][2]))) == NULL) {
// 			showinfobox("Can't allocate memory for plane");
			return 0;
		}
	} else
	{
		if((mp->plane->tri = 
				  (unsigned int(*)[3][2])realloc(mp->plane->tri, (mp->plane->ntri+1)*sizeof(unsigned int[3][2]))) == NULL) {
// 			showinfobox("Can't allocate memory for plane");
			return 0;
				  }
	}
	mp->plane->tri[mp->plane->ntri][0][0] = tri[0][0];
	mp->plane->tri[mp->plane->ntri][0][1] = tri[0][1];
	mp->plane->tri[mp->plane->ntri][1][0] = tri[1][0];
	mp->plane->tri[mp->plane->ntri][1][1] = tri[1][1];
	mp->plane->tri[mp->plane->ntri][2][0] = tri[2][0];
	mp->plane->tri[mp->plane->ntri][2][1] = tri[2][1];
//printf("ntri: %d\n", mp->plane->ntri);
//printf("%d %d %d %d %d %d\n", mp->plane->tri[mp->plane->ntri][0][0], mp->plane->tri[mp->plane->ntri][0][1], mp->plane->tri[mp->plane->ntri][1][0], mp->plane->tri[mp->plane->ntri][1][1], mp->plane->tri[mp->plane->ntri][2][0], mp->plane->tri[mp->plane->ntri][2][1]);
	mp->plane->ntri++;
	return 1;
}

Surfdot* get_dot(){ return dot ; }
Tria* get_tri(){ return tri ; }
int get_npts(){ return npts ; }
int get_ntri(){ return ntri ; }
