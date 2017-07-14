/*
 * Copyright (C) 2002  Terence M. Welsh
 *
 * Cyclone is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Cyclone is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


// Cyclone screen saver

#include "Cyclone.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "rgbhsl.h"
#include <stdlib.h>

// useful random functions
static inline int myRandi(int x)
{
	return (rand() % x);
}

static inline float myRandf(float x)
{
	return(float(rand() * x) / float(RAND_MAX));
}


// useful vector functions

static float normalize(float *xyz)
{
	float length = float(sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]));
	if(length == 0.0)
		return(0.0);
	xyz[0] /= length;
	xyz[1] /= length;
	xyz[2] /= length;
	return(length);
}

static float dot(float *xyz1, float *xyz2)
{
	return(xyz1[0] * xyz2[0] + xyz1[1] * xyz2[1] + xyz1[2] * xyz2[2]);
}

static void cross(float *xyz1, float *xyz2, float *xyzOut)
{
	xyzOut[0] = xyz1[1] * xyz2[2] - xyz2[1] * xyz1[2];
	xyzOut[1] = xyz1[2] * xyz2[0] - xyz2[2] * xyz1[0];
	xyzOut[2] = xyz1[0] * xyz2[1] - xyz2[0] * xyz1[1];
}


// useful factorial function
static int factorial(int x)
{
    int returnval = 1;

    if(x == 0)
	{
        return(1);
	}
	else
	{
        do
		{
            returnval *= x;
            x -= 1;
        }
        while(x!=0);
    }
    return(returnval);
}

#pragma mark -

cyclone::cyclone(int inInComplexity,int inParticlesCount,int inParticleSize,int inSpeed,bool inStretch,bool inShowCurves)
{
	_complexity=inInComplexity;
	_speed=inSpeed;
	
	_stretch=inStretch;
	_showCurves=inShowCurves;
	
	_particles_count=inParticlesCount;
	
	_particles = new particle*[_particles_count];
	
	for(int i=0; i<_particles_count; i++)
		_particles[i] = new particle(this,inParticleSize);
	
	// Initialize position stuff
	targetxyz = new float*[_complexity+3];
	xyz = new float*[_complexity+3];
	oldxyz = new float*[_complexity+3];
	for(int i=0; i<int(_complexity)+3; i++)
	{
		targetxyz[i] = new float[3];
		xyz[i] = new float[3];
		oldxyz[i] = new float[3];
	}
	xyz[_complexity+2][0] = myRandf(float(wide*2)) - float(wide);
	xyz[_complexity+2][1] = float(high);
	xyz[_complexity+2][2] = myRandf(float(wide*2)) - float(wide);
	xyz[_complexity+1][0] = xyz[_complexity+2][0];
	xyz[_complexity+1][1] = myRandf(float(high / 3)) + float(high / 4);
	xyz[_complexity+1][2] = xyz[_complexity+2][2];
    
	for(int i=_complexity; i>1; i--)
    {
		xyz[i][0] = xyz[i+1][0] + myRandf(float(wide)) - float(wide / 2);
		xyz[i][1] = myRandf(float(high * 2)) - float(high);
		xyz[i][2] = xyz[i+1][2] + myRandf(float(wide)) - float(wide / 2);
	}
    
	xyz[1][0] = xyz[2][0] + myRandf(float(wide / 2)) - float(wide / 4);
	xyz[1][1] = -myRandf(float(high / 2)) - float(high / 4);
	xyz[1][2] = xyz[2][2] + myRandf(float(wide / 2)) - float(wide / 4);
	xyz[0][0] = xyz[1][0] + myRandf(float(wide / 8)) - float(wide / 16);
	xyz[0][1] = float(-high);
	xyz[0][2] = xyz[1][2] + myRandf(float(wide / 8)) - float(wide / 16);
	// Initialize width stuff
	targetWidth = new float[_complexity+3];
	width = new float[_complexity+3];
	oldWidth = new float[_complexity+3];
	width[_complexity+2] = myRandf(175.0f) + 75.0f;
	width[_complexity+1] = myRandf(60.0f) + 15.0f;
	for(int i=_complexity; i>1; i--)
		width[i] = myRandf(25.0f) + 15.0f;
	width[1] = myRandf(25.0f) + 5.0f;
	width[1] = myRandf(15.0f) + 5.0f;
	// Initialize transition stuff
	xyzChange = new int*[_complexity + 3];
	widthChange = new int*[_complexity + 3];
    
	for(int i=0; i<(_complexity+3); i++)
    {
		xyzChange[i] = new int[2];	// 0 = step   1 = total steps
		widthChange[i] = new int[2];
		xyzChange[i][0] = 0;
		xyzChange[i][1] = 0;
		widthChange[i][0] = 0;
		widthChange[i][1] = 0;
	}
    
	// Initialize color stuff
    
	oldhsl[0] = myRandf(1.0f);
	oldhsl[1] = myRandf(1.0f);
	oldhsl[2] = 0.0f;
	targethsl[0] = myRandf(1.0f);
	targethsl[1] = myRandf(1.0f);
	targethsl[2] = 1.0f;
	hslChange[0] = 0;
	hslChange[1] = 300;
}

cyclone::~cyclone()
{
    int i;
    
	if (_particles!=NULL)
	{
		for(i=0;i<_particles_count;i++)
			delete (_particles[i]);
		
		free(_particles);
		_particles=NULL;
	}


	if (targetxyz!=NULL)
    {
        for(i=0;i<_complexity;i++)
			free(targetxyz[i]);
        
        free(targetxyz);
    }
    
    if (xyz!=NULL)
    {
        for(i=0;i<_complexity;i++)
			free(xyz[i]);
        
        free(xyz);
    }
    
    if (oldxyz!=NULL)
    {
        for(i=0;i<_complexity;i++)
			free(oldxyz[i]);
        
        free(oldxyz);
    }
    
    if (targetWidth!=NULL)
        free(targetWidth);
    
    if (width!=NULL)
		free(width);
    
    if (oldWidth!=NULL)
		free(oldWidth);
    
    if (xyzChange!=NULL)
    {
        for(i=0;i<(_complexity+3);i++)
			free(xyzChange[i]);
        
        free(xyzChange);
    }
    
    if (widthChange!=NULL)
    {
        for(i=0;i<(_complexity+3);i++)
			free(widthChange[i]);
        
        free(widthChange);
    }
}

void cyclone::update(float * inFact)
{
	float between;
	float diff;
	int direction;
	float point[3];

	// update cyclone's path
	int temp = _complexity + 2;
	if(xyzChange[temp][0] == xyzChange[temp][1])
    {
		oldxyz[temp][0] = xyz[temp][0];
		oldxyz[temp][1] = xyz[temp][1];
		oldxyz[temp][2] = xyz[temp][2];
		targetxyz[temp][0] = myRandf(float(wide*2)) - float(wide);
		targetxyz[temp][1] = float(high);
		targetxyz[temp][2] = myRandf(float(wide*2)) - float(wide);
		xyzChange[temp][0] = 0;
		xyzChange[temp][1] = myRandi(10000 / _speed) + 5000 / _speed;
	}
    
	temp = _complexity + 1;
    
	if(xyzChange[temp][0] == xyzChange[temp][1])
    {
		oldxyz[temp][0] = xyz[temp][0];
		oldxyz[temp][1] = xyz[temp][1];
		oldxyz[temp][2] = xyz[temp][2];
		targetxyz[temp][0] = xyz[temp+1][0];
		targetxyz[temp][1] = myRandf(float(high / 3)) + float(high / 4);
		targetxyz[temp][2] = xyz[temp+1][2];
		xyzChange[temp][0] = 0;
		xyzChange[temp][1] = myRandi(7000 / _speed) + 5000 / _speed;
	}
    
	for(int i=_complexity; i>1; i--)
    {
		if(xyzChange[i][0] == xyzChange[i][1])
        {
			oldxyz[i][0] = xyz[i][0];
			oldxyz[i][1] = xyz[i][1];
			oldxyz[i][2] = xyz[i][2];
            
			targetxyz[i][0] = targetxyz[i+1][0] + (targetxyz[i+1][0] - targetxyz[i+2][0]) / 2.0f + myRandf(float(wide / 2)) - float(wide / 4);
			targetxyz[i][1] = (targetxyz[i+1][1] + targetxyz[i-1][1]) / 2.0f + myRandf(float(high / 8)) - float(high / 16);
			targetxyz[i][2] = targetxyz[i+1][2] + (targetxyz[i+1][2] - targetxyz[i+2][2]) / 2.0f + myRandf(float(wide / 2)) - float(wide / 4);
            
			if(targetxyz[i][1] > high)
				targetxyz[i][1] = high;
			if(targetxyz[i][1] < -high)
				targetxyz[i][1] = -high;
			xyzChange[i][0] = 0;
			xyzChange[i][1] = myRandi(5000 / _speed) + 3000 / _speed;
		}
	}
    
	if(xyzChange[1][0] == xyzChange[1][1])
    {
		oldxyz[1][0] = xyz[1][0];
		oldxyz[1][1] = xyz[1][1];
		oldxyz[1][2] = xyz[1][2];
		targetxyz[1][0] = targetxyz[2][0] + myRandf(float(wide / 2)) - float(wide / 4);
		targetxyz[1][1] = -myRandf(float(high / 2)) - float(high / 4);
		targetxyz[1][2] = targetxyz[2][2] + myRandf(float(wide / 2)) - float(wide / 4);
		xyzChange[1][0] = 0;
		xyzChange[1][1] = myRandi(4000 / _speed) + 2000 / _speed;
	}
    
	if(xyzChange[0][0] == xyzChange[0][1])
    {
		oldxyz[0][0] = xyz[0][0];
		oldxyz[0][1] = xyz[0][1];
		oldxyz[0][2] = xyz[0][2];
		targetxyz[0][0] = xyz[1][0] + myRandf(float(wide / 8)) - float(wide / 16);
		targetxyz[0][1] = float(-high);
		targetxyz[0][2] = xyz[1][2] + myRandf(float(wide / 8)) - float(wide / 16);
		xyzChange[0][0] = 0;
		xyzChange[0][1] = myRandi(3000 / _speed) + 1500 / _speed;
	}
    
	for(int i=0; i<(_complexity+3); i++)
    {
		between = float(xyzChange[i][0]) / float(xyzChange[i][1]) * PIx2;
		between = (1.0f - float(cos(between))) / 2.0f; 
		xyz[i][0] = ((targetxyz[i][0] - oldxyz[i][0]) * between) + oldxyz[i][0];
		xyz[i][1] = ((targetxyz[i][1] - oldxyz[i][1]) * between) + oldxyz[i][1];
		xyz[i][2] = ((targetxyz[i][2] - oldxyz[i][2]) * between) + oldxyz[i][2];
		xyzChange[i][0] ++;
	}

	// Update cyclone's widths
    
	temp = _complexity + 2;
    
	if(widthChange[temp][0] == widthChange[temp][1])
    {
		oldWidth[temp] = width[temp];
		targetWidth[temp] = myRandf(225.0f) + 75.0f;
		widthChange[temp][0] = 0;
		widthChange[temp][1] = myRandi(5000 / _speed) + 5000 / _speed;
	}
    
	temp = _complexity + 1;
    
	if(widthChange[temp][0] == widthChange[temp][1])
    {
		oldWidth[temp] = width[temp];
		targetWidth[temp] = myRandf(80.0f) + 15.0f;
		widthChange[temp][0] = 0;
		widthChange[temp][1] = myRandi(5000 / _speed) + 5000 / _speed;
	}
    
	for(int i=_complexity; i>1; i--)
    {
		if(widthChange[i][0] == widthChange[i][1])
        {
			oldWidth[i] = width[i];
			targetWidth[i] = myRandf(25.0f) + 15.0f;
			widthChange[i][0] = 0;
			widthChange[i][1] = myRandi(5000 / _speed) + 4000 / _speed;
		}
	}
    
	if(widthChange[1][0] == widthChange[1][1])
    {
		oldWidth[1] = width[1];
		targetWidth[1] = myRandf(25.0f) + 5.0f;
		widthChange[1][0] = 0;
		widthChange[1][1] = myRandi(5000 / _speed) + 3000 / _speed;
	}
    
	if(widthChange[0][0] == widthChange[0][1]){
		oldWidth[0] = width[0];
		targetWidth[0] = myRandf(15.0f) + 5.0f;
		widthChange[0][0] = 0;
		widthChange[0][1] = myRandi(5000 / _speed) + 2000 / _speed;
	}
	for(int i=0; i<(_complexity+3); i++){
		between = float(widthChange[i][0]) / float(widthChange[i][1]);
		width[i] = ((targetWidth[i] - oldWidth[i]) * between) + oldWidth[i];
		widthChange[i][0] ++;
	}

	// Update cyclones color
    
	if(hslChange[0] == hslChange[1])
    {
		oldhsl[0] = hsl[0];
		oldhsl[1] = hsl[1];
		oldhsl[2] = hsl[2];
		targethsl[0] = myRandf(1.0f);
		targethsl[1] = myRandf(1.0f);
		targethsl[2] = myRandf(1.0f) + 0.5f;
		if(targethsl[2] > 1.0f)
			targethsl[2] = 1.0f;
		hslChange[0] = 0;
		hslChange[1] = myRandi(1900) + 100;
	}
    
	between = float(hslChange[0]) / float(hslChange[1]);
	diff = targethsl[0] - oldhsl[0];
	direction = 0;
	if((targethsl[0] > oldhsl[0] && diff > 0.5f) || (targethsl[0] < oldhsl[0] && diff < -0.5f))
		if(diff > 0.5f)
			direction = 1;
	hslTween(oldhsl[0], oldhsl[1], oldhsl[2],
			targethsl[0], targethsl[1], targethsl[2], between, direction,
			hsl[0], hsl[1], hsl[2]);
	hslChange[0] ++;

	if(_showCurves==true)
    {
		glDisable(GL_LIGHTING);
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for(float step=0.0; step<1.0; step+=0.02f)
        {
			point[0] = point[1] = point[2] = 0.0f;
            
			for(int i=0; i<(_complexity+3); i++)
            {
				float blend = inFact[_complexity+2] / (inFact[i]
					* inFact[_complexity+2-i]) * pow(step, float(i))
					* pow((1.0f - step), float(_complexity+2-i));
				point[0] += xyz[i][0] * blend;
				point[1] += xyz[i][1] * blend;
				point[2] += xyz[i][2] * blend;
			}
			glVertex3fv(point);
		}
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for(int i=0; i<(_complexity+3); i++)
			glVertex3fv(&xyz[i][0]);
		glEnd();
		glEnable(GL_LIGHTING);
	}
	
	for(int i=0; i<_particles_count; i++)
	{
		_particles[i]->update(_complexity,_speed,_stretch,inFact);
	}
}

#pragma mark -

particle::particle(cyclone * inCyclone,int inSize)
{
	_cyclone = inCyclone;
	
	_size=inSize;
	
	init();
}

void particle::init()
{
	width = myRandf(0.8f) + 0.2f;
	step = 0.0f;
	spinAngle = myRandf(360);
	hsl2rgb(_cyclone->hsl[0], _cyclone->hsl[1], _cyclone->hsl[2], r, g, b);
}

void particle::update(int inComplexity,int inSpeed,bool inStretch,float *inFact)
{
	float newStep;
	float newSpinAngle;
	float cyWidth;
	float between;
	float dir[3];
	float crossVec[3];
	float tiltAngle;
	float up[3] = {0.0f, 1.0f, 0.0f};
    
	lastxyz[0] = xyz[0];
	lastxyz[1] = xyz[1];
	lastxyz[2] = xyz[2];
	if(step > 1.0f)
		init();
	xyz[0] = xyz[1] = xyz[2] = 0.0f;
    
	for(int i=0; i<(inComplexity+3); i++)
    {
		float blend = inFact[inComplexity+2] / (inFact[i]
			* inFact[inComplexity+2-i]) * pow(step, float(i))
			* pow((1.0f - step), float(inComplexity+2-i));
		xyz[0] += _cyclone->xyz[i][0] * blend;
		xyz[1] += _cyclone->xyz[i][1] * blend;
		xyz[2] += _cyclone->xyz[i][2] * blend;
	}
    
	dir[0] = dir[1] = dir[2] = 0.0f;
    
	for(int i=0; i<(inComplexity+3); i++)
    {
		float blend = inFact[inComplexity+2] / (inFact[i]
			* inFact[inComplexity+2-i]) * pow(step - 0.01f, float(i))
			* pow((1.0f - (step - 0.01f)), float(inComplexity+2-i));
		dir[0] += _cyclone->xyz[i][0] * blend;
		dir[1] += _cyclone->xyz[i][1] * blend;
		dir[2] += _cyclone->xyz[i][2] * blend;
	}
    
	dir[0] = xyz[0] - dir[0];
	dir[1] = xyz[1] - dir[1];
	dir[2] = xyz[2] - dir[2];
	normalize(dir);
	cross(dir, up, crossVec);
	tiltAngle = -acos(dot(dir, up)) * 180.0f / PI;
	int i = int(step * (float(inComplexity) + 2.0f));
	if(i >= (inComplexity + 2))
		i = inComplexity + 1;
	between = (step - (float(i) / float(inComplexity + 2))) * float(inComplexity + 2);
	cyWidth = _cyclone->width[i] * (1.0f - between) + _cyclone->width[i+1] * (between);
	newStep = (0.005f * float(inSpeed)) / (width * width * cyWidth);
	step += newStep;
	newSpinAngle = (40.0f * float(inSpeed)) / (width * cyWidth);
	spinAngle += newSpinAngle;
    
	float scale=0;
	
	if(inStretch==true)
    {
		scale = width * cyWidth * newSpinAngle * 0.02f;
		float temp = cyWidth * 2.0f / float(_size);
		if(scale > temp)
			scale = temp;
		if(scale < 1.0f)
			scale = 1.0f;
	}
	glColor3f(r, g, b);
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(xyz[0], xyz[1], xyz[2]);
		glRotatef(tiltAngle, crossVec[0], crossVec[1], crossVec[2]);
		glRotatef(spinAngle, 0, 1, 0);
		glTranslatef(width * cyWidth, 0, 0);
		if(inStretch==true)
			glScalef(1.0f, 1.0f, scale);
		glCallList(1);
	glPopMatrix();
}

#pragma mark -

scene::scene()
{
	srand((unsigned)time(NULL));
	rand(); rand(); rand(); rand(); rand(); rand(); rand(); rand(); rand(); rand();
	
	for(int i=0; i<13; i++)
		fact[i] = float(factorial(i));
}

scene::~scene()
{
	if (cyclones!=NULL)
	{
		for(int i=0; i<cyclones_count; i++)
			delete(cyclones[i]);
		
		delete(cyclones);
		
		cyclones=NULL;
	}
}

void scene::create()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (glIsList(1))
		glDeleteLists(1,1);
	
	glNewList(1, GL_COMPILE);
	GLUquadricObj *qobj = gluNewQuadric();
	gluSphere(qobj, float(this->particle_size) / 4.0f, 3, 2);
	gluDeleteQuadric(qobj);
	glEndList();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float ambient[4] = {0.25f, 0.25f, 0.25f, 0.0f};
	float diffuse[4] = {1.0f, 1.0f, 1.0f, 0.0f};
	float specular[4] = {1.0f, 1.0f, 1.0f, 0.0f};
	float position[4] = {float(wide * 2), -float(high), float(wide * 2), 0.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColor3f(0.7f, 0.7f, 0.7f);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	// Initialize cyclones and their particles
	
	cyclones = new cyclone*[cyclones_count];
	
	for(int i=0; i<cyclones_count; i++)
		cyclones[i] = new cyclone(cyclone_complexity,particles_count,particle_size,speed,stretch,showCurves);
}

void scene::resize(int inWidth,int inHeight)
{
	glViewport(0,0,inWidth,inHeight);
	
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, float(inWidth) / float(inHeight), 50, 3000);
	
	if(!myRandi(500))
	{  // Easter egg view
		glRotatef(90, 1, 0, 0);
		glTranslatef(0.0f, -(wide * 2), 0.0f);
	}
	else
	{	// Normal view
		glTranslatef(0.0f, 0.0f, -(wide * 2));
	}
}

void scene::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for(int i=0; i<cyclones_count; i++)
		cyclones[i]->update(fact);
}
