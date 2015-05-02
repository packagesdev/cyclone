#ifndef __CYCLONE__
#define __CYCLONE__

class cyclone;
class particle;


#define PIx2 6.28318530718f
#define PI 3.14159265359f
#define wide 200
#define high 200

// Global variables

// Parameters edited in the dialog box

class cyclone;

class particle
{
	private:
	
		cyclone *_cyclone;
		int _size;
	
		float r, g, b;
		float xyz[3], lastxyz[3];
		float width;
		float step;
		float spinAngle;
	
		void init();
	
	public:
		
		particle(cyclone * inCyclone,int inSize);
		virtual ~particle(){};
	
		void update(int inComplexity,int inSpeed,bool inStretch,float *inFact);
};

class cyclone
{
	private:
	
		int _particles_count;
		particle **_particles;
	
		float **targetxyz;
	
		float **oldxyz;
		float *targetWidth;
	
		float *oldWidth;
		float targethsl[3];
	
		float oldhsl[3];
		int **xyzChange;
		int **widthChange;
		int hslChange[2];

	public:
	
		int _speed;
		int _complexity;
	
		bool _showCurves;
		bool _stretch;
	
		float *width;
		float **xyz;
		float hsl[3];
	
        cyclone(int inInComplexity,int inParticlesCount,int inParticleSize,int inSpeed,bool inStretch,bool inShowCurves);
        virtual ~cyclone();
	
        void update(float *inFact);
};



class scene
{
	private:
	
		cyclone **cyclones;
	
	public:
	
		int cyclones_count;
		int cyclone_complexity;
		int particles_count;
		int particle_size;
		int speed;
		
		bool stretch;
		bool showCurves;
	
		float fact[13];
	
		scene();
		virtual ~scene();
		
		void create();
		void resize(int inWidth,int inHeight);
		void draw();
};

#endif