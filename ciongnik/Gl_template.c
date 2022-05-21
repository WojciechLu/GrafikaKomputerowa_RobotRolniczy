// Gl_template.c
//Wy³šczanie b³êdów przed "fopen"
#define  _CRT_SECURE_NO_WARNINGS



// £adowanie bibliotek:

#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
#endif

typedef enum { FALSE, TRUE } bool;


// Ustalanie trybu tekstowego:
#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include "resource.h"           // About box resource identifiers.
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14
time_t t;


//tekstury






// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = "GL Template";
static HINSTANCE hInstance;

// Rotation amounts
static GLfloat xRot = 40.0f;
static GLfloat yRot = 90.0f;

int pozycjaX = 0;
int pozycjaX2 = 1200;
int pozycjaX3 = 600;
int pozycjaZ = 0;
int pozycjaZ2 = 0;

int pozycjaXdrzew = 1200;
int pozycjaXdrzew2 = 0;

int pozycjaZdrzewa1 = 220; 
int pozycjaZdrzewa2 = 240; 
int pozycjaZdrzewa3 = 260; 
int pozycjaZdrzewa4 = 340; 
int pozycjaZdrzewa5 = 210; 
int pozycjaZdrzewa6 = 270;
int pozycjaZdrzewa11 = 280;
int pozycjaZdrzewa22 = 260;
int pozycjaZdrzewa33 = 240;
int pozycjaZdrzewa44 = 200;
int pozycjaZdrzewa55 = 270;
int pozycjaZdrzewa66 = 240;

int pozycjaXkamieni = 1000;
int pozycjaXkamieni2 = -200;

int pozycjaZkamienia1 = 300;
int pozycjaZkamienia2 = 300;
int pozycjaZkamienia3 = 300;
int pozycjaZkamienia4 = 300;
int pozycjaZkamienia5 = 300;
int pozycjaZkamienia6 = 300;
int pozycjaZkamienia11 = 300;
int pozycjaZkamienia22 = 300;
int pozycjaZkamienia33 = 300;
int pozycjaZkamienia44 = 300;
int pozycjaZkamienia55 = 300;
int pozycjaZkamienia66 = 300;

//int pozycjaZdzew = rand() % 100 + 200;

static GLsizei lastHeight;
static GLsizei lastWidth;

// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nag³ówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[5];			// obiekt tekstury


// Declaration for Window procedure
LRESULT CALLBACK WndProc(   HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam);

// Dialog procedure for about box
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam);

// Set Pixel Format function - forward declaration
void SetDCPixelFormat(HDC hDC);

// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
	{
	float length;
	
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
	{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
	}



// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
	{
	GLfloat nRange = 300.0f;
	GLfloat fAspect;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;
		
	fAspect=(GLfloat)w/(GLfloat)h;
	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Establish clipping volume (left, right, bottom, top, near, far)
  //  if (w <= h) 
		//glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange*200, nRange*200);
  //  else 
		//glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange*200, nRange*200);
	
	// Establish perspective: 
	
	gluPerspective(60.0f,fAspect,1.0,4000);
	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	}



// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
	{
	// Light values and coordinates
	//GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	//GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	//glEnable(GL_LIGHTING);

	// Setup and enable light 0
	//glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMateriali(GL_FRONT,GL_SHININESS,128);


	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	// Black brush
	glColor3f(0.0,0.0,0.0);
	}

void skrzynka(void)
{
	glColor3d(1.0, 1.0, 1.0);


	glEnable(GL_TEXTURE_2D); // W³¹cz teksturowanie

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glTexCoord2d(1.0, 1.0); glVertex3d(25, 25, 25);
	glTexCoord2d(0.0, 1.0); glVertex3d(-25, 25, 25);
	glTexCoord2d(0.0, 0.0); glVertex3d(-25, -25, 25);
	glTexCoord2d(1.0, 0.0); glVertex3d(25, -25, 25);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glNormal3d(1, 0, 0);
	glTexCoord2d(1.0, 1.0); glVertex3d(25, 25, 25);
	glTexCoord2d(1.0, 0.0); glVertex3d(25, -25, 25);
	glTexCoord2d(0.0, 0.0); glVertex3d(25, -25, -25);
	glTexCoord2d(0.0, 1.0); glVertex3d(25, 25, -25);
	glEnd();

	glDisable(GL_TEXTURE_2D); // Wy³¹cz teksturowanie



	glBegin(GL_QUADS);
	glNormal3d(0, 0, -1);
	glVertex3d(25, 25, -25);
	glVertex3d(25, -25, -25);
	glVertex3d(-25, -25, -25);
	glVertex3d(-25, 25, -25);

	glNormal3d(-1, 0, 0);
	glVertex3d(-25, 25, -25);
	glVertex3d(-25, -25, -25);
	glVertex3d(-25, -25, 25);
	glVertex3d(-25, 25, 25);

	glNormal3d(0, 1, 0);
	glVertex3d(25, 25, 25);
	glVertex3d(25, 25, -25);
	glVertex3d(-25, 25, -25);
	glVertex3d(-25, 25, 25);

	glNormal3d(0, -1, 0);
	glVertex3d(25, -25, 25);
	glVertex3d(-25, -25, 25);
	glVertex3d(-25, -25, -25);
	glVertex3d(25, -25, -25);
	glEnd();
}

void walec01(void)
{
GLUquadricObj*obj;
obj=gluNewQuadric();
gluQuadricNormals(obj,GLU_SMOOTH);
glColor3d(1,0,0);
glPushMatrix();
gluCylinder(obj,20,20,30,15,7);
gluCylinder(obj,0,20,0,15,7);
glTranslated(0,0,60);
glRotated(180.0,0,1,0);
gluCylinder(obj,0,20,30,15,7);
glPopMatrix();
}

void kula(void)
{
	GLUquadricObj*obj;
	obj=gluNewQuadric();
	gluQuadricTexture(obj,GL_TRUE);
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3d(1.0,0.8,0.8);
	glEnable(GL_TEXTURE_2D);
	gluSphere(obj,40,15,7);
	glDisable(GL_TEXTURE_2D);
}




// LoadBitmapFile
// opis: ³aduje mapê bitow¹ z pliku i zwraca jej adres.
//       Wype³nia strukturê nag³ówka.
//	 Nie obs³uguje map 8-bitowych.
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wskaŸnik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag³ówek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk³adowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nag³ówek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nag³ówek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wskaŸnik pozycji pliku na pocz¹tku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pamiêæ buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy uda³o siê przydzieliæ pamiêæ
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy dane zosta³y wczytane
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia miejscami sk³adowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wskaŸnik bufora zawieraj¹cego wczytany obraz
	fclose(filePtr);
	return bitmapImage;
}
void walecZ(int zmienna_x, int zmienna_y, int zmienna_z, int r, int h) {
	double x, y, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0 + zmienna_x, 0 + zmienna_y, 0 + zmienna_z);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r * sin(alpha);
		y = r * cos(alpha);
		glVertex3d(x + zmienna_x, y + zmienna_y, 0 + zmienna_z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r * sin(alpha);
		y = r * cos(alpha);
		glVertex3d(x + zmienna_x, y + zmienna_y, 0 + zmienna_z);
		glVertex3d(x + zmienna_x, y + zmienna_y, h + zmienna_z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0 + zmienna_x, 0 + zmienna_y, h + zmienna_z);
	for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
	{
		x = r * sin(alpha);
		y = r * cos(alpha);
		glVertex3d(x + zmienna_x, y + zmienna_y, h + zmienna_z);
	}
	glEnd();
}

void walecY(int zmienna_x, int zmienna_y, int zmienna_z, int r, int h) {
	double x, z, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0 + zmienna_x, 0 + zmienna_y, 0 + zmienna_z);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r * sin(alpha);
		z = r * cos(alpha);
		glVertex3d(x + zmienna_x, 0 + zmienna_y, z + zmienna_z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r * sin(alpha);
		z = r * cos(alpha);
		glVertex3d(x + zmienna_x, 0 + zmienna_y, z + zmienna_z);
		glVertex3d(x + zmienna_x, h + zmienna_y, z + zmienna_z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0 + zmienna_x, h + zmienna_y, 0 + zmienna_z);
	for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
	{
		x = r * sin(alpha);
		z = r * cos(alpha);
		glVertex3d(x + zmienna_x, h + zmienna_y, z + zmienna_z);
	}
	glEnd();
}

void walecX(int zmienna_x, int zmienna_y, int zmienna_z, int r, int h) {
	double y, z, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0 + zmienna_x, 0 + zmienna_y, 0 + zmienna_z);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		y = r * sin(alpha);
		z = r * cos(alpha);
		glVertex3d(0 + zmienna_x, y + zmienna_y, z + zmienna_z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		y = r * sin(alpha);
		z = r * cos(alpha);
		glVertex3d(0 + zmienna_x, y + zmienna_y, z + zmienna_z);
		glVertex3d(h + zmienna_x, y + zmienna_y, z + zmienna_z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(h + zmienna_x, 0 + zmienna_y, 0 + zmienna_z);
	for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
	{
		y = r * sin(alpha);
		z = r * cos(alpha);
		glVertex3d(h + zmienna_x, y + zmienna_y, z + zmienna_z);
	}
	glEnd();
}

void ko³a(int r, int h) {
	glColor3d(1, 0.0, 0.0);
	walecZ(40, 0, 0, r, h);

	glColor3d(0.2, 0.2, 0.2);
	walecZ(40, 0, -1, r*0.7, h+2);

	glColor3d(0, 1.0, 0);
	walecZ(0, 0, 40, r, h);

	glColor3d(0.2, 0.2, 0.2);
	walecZ(0, 0, 39, r * 0.7, h + 2);

	glColor3d(0, 0.0, 1);
	walecZ(40, 0, 40, r, h);

	glColor3d(0.2, 0.2, 0.2);
	walecZ(40, 0, 39, r * 0.7, h + 2);

	glColor3d(0, 0, 0);
	walecZ(0, 0, 0, r, h);

	glColor3d(0.2, 0.2, 0.2);
	walecZ(0, 0, -1, r * 0.7, h + 2);
}

void os() {
	int a = 5 / 2;
	glColor3d(0.3, 0.3, 0.3);
	walecZ(40, 0, a, 1, 40);
	walecZ(0, 0, a, 1, 40);

	glColor3d(0.5, 0.5, 0.5);
	walecX(0, 0, (40 + 5 - 10) / 2, 1, 40);
	walecX(0, 0, (40 + 5 + 10 + 1) / 2, 1, 40);
	walecY(0, 0, (40 + 5) / 2, 1, 10);
	walecY(40, 0, (40 + 5) / 2, 1, 10);
}

void prostokatZ(int x, int y, int z, int a, int b) {

	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		GLfloat sa[3] = { x,y,z };
		GLfloat sb[3] = { x + a,y,z };
		GLfloat sc[3] = { x,y + b,z };
		GLfloat sd[3] = { x + a,y + b,z };
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glNormal3d(0, 0, 1);
		glBegin(GL_TRIANGLE_STRIP);

		glTexCoord2d(1.0, 1.0);
		glVertex3fv(sa);
		glTexCoord2d(1.0, .0);
		glVertex3fv(sb);
		glTexCoord2d(0.0, 1.0);
		glVertex3fv(sc);
		glTexCoord2d(0.0, 0.0);
		glVertex3fv(sd);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

void prostokatX(int x, int y, int z, int a, int b) {

	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		GLfloat sa[3] = { x,y,z };
		GLfloat sb[3] = { x ,y,z + a };
		GLfloat sc[3] = { x,y + b,z };
		GLfloat sd[3] = { x ,y + b,z + a };
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glNormal3d(0, 0, 1);
		glBegin(GL_TRIANGLE_STRIP);

		glTexCoord2d(1.0, 1.0);
		glVertex3fv(sa);
		glTexCoord2d(1.0, .0);
		glVertex3fv(sb);
		glTexCoord2d(0.0, 1.0);
		glVertex3fv(sc);
		glTexCoord2d(0.0, 0.0);
		glVertex3fv(sd);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

void prostokatY(int x, int y, int z, int a, int b) {

	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		GLfloat sa[3] = { x,y,z };
		GLfloat sb[3] = { x ,y,z + b };
		GLfloat sc[3] = { x + a,y ,z };
		GLfloat sd[3] = { x + a ,y ,z + b };
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glNormal3d(0, 0, 1);
		glBegin(GL_TRIANGLE_STRIP);

		glTexCoord2d(1.0, 1.0);
		glVertex3fv(sa);
		glTexCoord2d(1.0, .0);
		glVertex3fv(sb);
		glTexCoord2d(0.0, 1.0);
		glVertex3fv(sc);
		glTexCoord2d(0.0, 0.0);
		glVertex3fv(sd);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

void szescian(int x, int y, int z, int a, int b, int c)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		// Parametry wierzcholkow

		GLfloat sa[3] = { x,y,z };
		GLfloat sb[3] = { x + a,y,z };
		GLfloat sc[3] = { x + a,y + b,z };
		GLfloat sd[3] = { x,y + b,z };
		GLfloat se[3] = { x,y,z - c };
		GLfloat sf[3] = { x + a,y,z - c };
		GLfloat sg[3] = { x + a,y + b,z - c };
		GLfloat sh[3] = { x,y + b,z - c };


		//Sciany skladowe
		//glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sd);
		glVertex3fv(sc);
		glEnd();

		//glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(sc);
		glVertex3fv(sg);
		glEnd();

		//glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3fv(sf);
		glVertex3fv(se);
		glVertex3fv(sg);
		glVertex3fv(sh);
		glEnd();

		//glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3fv(se);
		glVertex3fv(sa);
		glVertex3fv(sh);
		glVertex3fv(sd);
		glEnd();

		//glColor3f(0.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3fv(sd);
		glVertex3fv(sc);
		glVertex3fv(sh);
		glVertex3fv(sg);
		glEnd();

		//glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(se);
		glVertex3fv(sf);
		glEnd();
	}
}

void wachlarz(int x, int y, int z, int r)
{
	double pi = 3.14;
	GLfloat sa[3] = { r + x, r + y, z };
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	for (double alpha = 0; alpha <= pi / 2; alpha += pi / 20)
	{
		glVertex3d(r * cos(alpha) + x, r * sin(alpha) + y, z);
	}
	glEnd();

	GLfloat sb[3] = { -r + x, r + y, z };
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	for (double alpha = pi / 2; alpha <= pi; alpha += pi / 20)
	{
		glVertex3d(r * cos(alpha) + x, r * sin(alpha) + y, z);
	}
	glEnd();
}

void front_kloca(int x, int y, int z) {

	//glColor3f(1.0f, 1.0f, 0.0f);
	prostokatZ(-10 - 8 + x, 0 + y, 0 + z, 10, 25);
	prostokatZ(40 + 8 + x, 0 + y, 0 + z, 10, 25);

	//glColor3f(0.0f, 1.0f, 0.0f);
	prostokatZ(-18 + x, 8 + y, 0 + z, 76, 20);
	//glColor3f(1.0f, 0.0f, 1.0f);
	prostokatZ(8 + x, 0 + y, 0 + z, 24, 60);
	//glColor3f(0,0,0);
	prostokatZ(-5 + x, 28 + y, 0 + z, 50, 32);

	wachlarz(0 + x, 0 + y, 0 + z, 8);
	wachlarz(40 + x, 0 + y, 0 + z, 8);
}

void bok_kloca(int x, int y, int z) {
	prostokatX(0 + x, 0 + y, 0 + z, 45, 28);
	prostokatX(76 + x, 0 + y, 0 + z, 45, 28);
	prostokatX(13 + x, 28 + y, 0 + z, 45, 32);
	prostokatX(63 + x, 28 + y, 0 + z, 45, 32);
}

void gora_kloca(int x, int y, int z) {
	
	prostokatY(-18 + x, 28 + y, 0 + z, 13, 45);
	prostokatY(-18 +13 +50 + x, 28 + y, 0 + z, 13, 45);
	prostokatY(-5 + x, 60 + y, 0 + z, 50, 45);
}

void most(int zmienna_x, int zmienna_y, int zmienna_z, int r, int h) {
	double x, y, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_STRIP);
	for (alpha = PI; alpha <= 2*PI; alpha += PI / 8.0)
	{
		y = -r * sin(alpha);
		x = r * cos(alpha);
		glVertex3d(x + zmienna_x, y + zmienna_y, 0 + zmienna_z);
		glVertex3d(x + zmienna_x, y + zmienna_y , h + zmienna_z);
	}
	glEnd();
}

void dol_kloca(int x, int y, int z) {
	prostokatY(0 +x, 0 + y, 0 +z, 10, 45);
	most(18 + x,-2 + y,0 + z,8,45);
	most(58 + x, -2 + y, 0 + z, 8, 45);
	prostokatY(26 + x, 0 + y, 0 + z, 24, 45);
	prostokatY(66 + x, 0 + y, 0 + z, 10, 45);
}

void kloc(int x, int y, int z) {

	glColor3d(1, 0, 0);
	front_kloca(0 + x, 0 + y, 0 + z);
	front_kloca(0 + x, 0 + y, 40 + 5 + z);

	glColor3d(1, 0, 0);
	bok_kloca(-18 + x,0 + y,0 + z);

	glColor3d(0.9, 0.9, 0.9);
	gora_kloca(0 + x, 0 + y, 0 + z);

	dol_kloca(-18 + x, 3 + y, 0 + z);

}

void sprzêt() {
	
	glColor3d(0.3, 0.3, 0.3);
	walecZ(73, 10, 10, 2, 25);


	//glColor3d(0.5, 0.5, 0.5);
	walecX(55, 10, 35, 2, 20);
	walecX(55, 10, 10, 2, 20);
	glColor3d(0.8, 0.8, 0.8);
	walecX(73, 10, 10, 1, 12);
	walecX(73, 10, 22.5, 1, 12);
	walecX(73, 10, 35, 1, 12);
	glColor3d(0.54, 0.27, 0.07);
	walecZ(88, 6, 7, 8, 2);
	walecZ(88, 6, 19.5, 8, 2);
	walecZ(88, 6, 32, 8, 2);
}

void pudlo(int x, int y, int z) {
	glColor3d(1, 1, 1);
	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		GLfloat xa[3] = { -18 - 50 - 20 + x,18 + y,z };
		GLfloat xb[3] = { -18 - 50 - 20 + x ,18 + y,z + 45 };
		GLfloat xc[3] = { -18 - 50 - 20 + x + 50,18 + y ,z };
		GLfloat xd[3] = { -18 - 50 - 20 + x + 50 ,18 + y,z + 45 };
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glNormal3d(0, 0, 1);
		glBegin(GL_TRIANGLE_STRIP);
		
		glTexCoord2d(1.0, 1.0); 
		glVertex3fv(xa);
		glTexCoord2d(1.0, 0.0);
		glVertex3fv(xb);
		glTexCoord2d(0.0, 1.0);
		glVertex3fv(xc);
		glTexCoord2d(0.0, 0.0); 
		glVertex3fv(xd);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D); 
	
	glColor3d(0.1, 0.1, 0.1);
	szescian(-18 - 50 - 20 + x, 4 + y, 1 + z, 51, 15, 2);
	szescian(-18 - 50 - 20 + x, 4 + y, 1 + 45 + z, 51, 15, 2);

	glColor3d(0.1, 0.1, 0.1);
	szescian(-18 - 20 -1 + x, 4 + y, 45 + z, 2, 15, 45);
	szescian(-18 - 50 - 20 + x, 4 + y, 45 + z, 2, 15, 45);

	glColor3d(0.5, 0.2, 0.2);
	walecZ(-18 - 1 - 50, 4, -5, 8, 4);
	walecZ(-18 - 1 - 50, 4, 45+1, 8, 4);
}

void przyczepa(int x, int y, int z) {

	glColor3d(0.1, 0.1, 0.1);
	walecX(-38+x, 10+y, 35+z, 2, 20);
	walecX(-38+x, 10+y, 10+z, 2, 20);

	pudlo(0, 0, 0); 

}

void ciongnik(void) {
	ko³a(5, 5); //8 figur
	os(); //6 figur
	kloc(0,-2,0);
	sprzêt(); //9 figur
	przyczepa(0, 0, 0); //9 figury
}

void walec(double r, double h)
{
	double x, y, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0.8, 0.0, 0);
	glVertex3d(0, 0, 0);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r*sin(alpha);
		y = r*cos(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r*sin(alpha);
		y = r* cos(alpha);
		glVertex3d(x, y, 0);
		glVertex3d(x, y, h);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, h);
	for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
	{
		x = r*sin(alpha);
		y = r*cos(alpha);
		glVertex3d(x, y, h);
	}
	glEnd();
}
void ramie(double r1, double r2, double h, double d)
{
	double PI = 3.14, alpha, x, y;
	glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.8, 0.0, 0);
		glVertex3d(0, 0, 0);
		for (alpha = PI; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = r1*sin(alpha);
			y = r1*cos(alpha);
			glVertex3d(x, y, 0);
		}
	glEnd();

	glBegin(GL_QUAD_STRIP);
		for (alpha = 0; alpha >= -PI; alpha -= PI / 8.0)
		{
			x = r1*sin(alpha);
			y = r1* cos(alpha);
			glVertex3d(x, y, h);
			glVertex3d(x, y, 0);
		}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		glVertex3d(0, 0, h);
		for (alpha = 0; alpha >= -PI; alpha -= PI / 8.0)
		{
			x = r1*sin(alpha);
			y = r1*cos(alpha);
			glVertex3d(x, y, h);
		}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.8, 0.0, 0);
		//glVertex3d(d,r2,0);
		//glVertex3d(d, r2, h);
		for (alpha = 0; alpha <= PI; alpha += PI / 8.0)
		{
			x = d + r2 * sin(alpha);
			y = d + r2 * cos(alpha);
			glVertex3d(x, y, 0);
		}
	glEnd();

	glBegin(GL_QUAD_STRIP);
		//glVertex3d(d, r2, 0);
		for (alpha = 0; alpha <= PI; alpha += PI / 8.0)
		{
			x = d+ r2*sin(alpha);
			y = d +r2* cos(alpha);
			glVertex3d(x, y, h);
			glVertex3d(x, y, 0);
		}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		//glVertex3d(d, r2, h);
		for (alpha = 0; alpha <= PI; alpha += PI / 8.0)
		{
			x = d +r2*sin(alpha);
			y = d +r2*cos(alpha);
			glVertex3d(x, y, h);
		}
	glEnd();
}

void pobocze(int pozX, int pozY,int rozmX, int rozmZ) {
	glColor3d(0.7, 0.7, 0.7);
	szescian(pozX, -5, pozY, rozmX, 0, rozmZ);
}

void droga(int pozX, int pozY, int rozmX, int rozmZ, int r, int g, int b) {
	glColor3d(r, g, b);
	szescian(pozX, -5, pozY, rozmX, 0, rozmZ);
}

void pasZieleni(int pozX, int pozY, int rozmX, int rozmZ) {
	glColor3d(0, 0.8, 0);
	szescian(pozX, -5, pozY, rozmX, 0, rozmZ);
}

void pasy(int pozX, int pozZ, int rozmX, int rozmZ, int przesuniecieX) {
	for (int i = 0; i < rozmX; i += 200) {
		glPushMatrix();
		glTranslatef(przesuniecieX, 0, 0);
		glColor3d(1, 1, 1);
		szescian(pozX+i, -3, pozZ, 100, 0, rozmZ);
		glPopMatrix(); //20 takich 
	}
}

void liscie(int r, int slices, int stacks) {
	GLUquadricObj* obj;
	obj = gluNewQuadric();
	gluQuadricTexture(obj, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3d(1.0, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	gluSphere(obj, r, slices, stacks);
	glDisable(GL_TEXTURE_2D);
}

void drzewo(int x, int y, int z, int h) {
	glColor3d(0.54, 0.27, 0.074);
	walecY(x, y, z, 10, h);
	glPushMatrix();
	glTranslatef(x, y + h, z);
	liscie(25, 8, 5);
	glPopMatrix();
}

void kamien(int x, int y, int z, int r, int slices, int stacks) {
	glColor3d(0.9, 0.9, 0.9);
	glPushMatrix();
	glTranslatef(x, y, z);
	GLUquadricObj* obj;
	obj = gluNewQuadric();
	gluQuadricTexture(obj, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3d(1.0, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	gluSphere(obj, r, slices, stacks);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void przeszkoda(int pozX, int pozZ) {

	glColor3d(0.7, 0.7, 0.7);
	glPushMatrix();
	glTranslated(pozX, 2, pozZ - 5);
	glRotated(50, 1, 0, 0);
	glTranslated(-pozX, -2, -pozZ + 5);
	walecY(pozX, 2, pozZ - 5, 5, 70);
	glPopMatrix();

	glPushMatrix();
	glTranslated(pozX + 25, 2, pozZ + 20);
	glRotated(50, 0, 0, 1);
	glTranslated(-pozX - 25, -2, -pozZ - 20);
	walecY(pozX + 25, 2, pozZ + 20, 5, 70);
	glPopMatrix();

	glPushMatrix();
	glTranslated(pozX, 2 + 45, pozZ - 7);
	glRotated(130, 1, 0, 0);
	glTranslated(-pozX, -2 - 45, -pozZ + 7);
	walecY(pozX, 2 + 45, pozZ - 7, 5, 70);
	glPopMatrix();
}
//(0, 0, 2000);
//jeden pasek to 200jednostek
void mapa(int pozX, int pozZ, int dlugosc) {
	int szerokoscZieleni = 500;
	pasZieleni(-1000+pozX, 203 + szerokoscZieleni + pozZ, dlugosc, szerokoscZieleni);
	pobocze(-1000 + pozX,203 + pozZ, dlugosc,30);

	droga(-1000 + pozX,173 + pozZ, dlugosc,100,0,0,0);
	droga(-1000 + pozX, 73 + pozZ, dlugosc, 100, 0, 0, 0);
	droga(-1000 + pozX, -27 + pozZ, dlugosc, 100, 0, 0, 0);

	pobocze(-1000 + pozX, -127 + pozZ, dlugosc, 30);

	pasZieleni(-1000 + pozX, -157 + pozZ, dlugosc, szerokoscZieleni);

	pasy(-400 + pozX, 78 + pozZ, dlugosc + 200, 10, pozycjaX);
	pasy(-400 + pozX, -22 + pozZ, dlugosc +200, 10, pozycjaX);

	//drzewa

	drzewo(0 + pozycjaXdrzew2, 0, pozycjaZdrzewa3, 70);
	drzewo(100 + pozycjaXdrzew2, 0, -pozycjaZdrzewa4, 70);
	drzewo(300 + pozycjaXdrzew2, 0, pozycjaZdrzewa1, 70);
	drzewo(400 + pozycjaXdrzew2, 0, -pozycjaZdrzewa6, 70);
	drzewo(600 + pozycjaXdrzew2, 0, pozycjaZdrzewa2, 70);
	drzewo(700 + pozycjaXdrzew2, 0, -pozycjaZdrzewa5, 70);

	//kamienie
	kamien(-50 + pozycjaXkamieni, -10, pozycjaZkamienia1, 20, 20, 20);
	kamien(100 + pozycjaXkamieni, -10, -pozycjaZkamienia2, 20, 20, 20);
	kamien(350 + pozycjaXkamieni, -10, pozycjaZkamienia3, 20, 20, 20);
	kamien(440 + pozycjaXkamieni, -10, -pozycjaZkamienia4, 20, 20, 20);
	kamien(660 + pozycjaXkamieni, -10, pozycjaZkamienia5, 20, 20, 20);
	kamien(790 + pozycjaXkamieni, -10, -pozycjaZkamienia6, 20, 20, 20);

	//drzewa i kamienie o 1200 jednostek dalej
	//drzewa2

	drzewo(0 + pozycjaXdrzew, 0, pozycjaZdrzewa11, 70);
	drzewo(100 + pozycjaXdrzew, 0, -pozycjaZdrzewa22, 70);
	drzewo(300 + pozycjaXdrzew, 0, pozycjaZdrzewa33, 70);
	drzewo(400 + pozycjaXdrzew, 0, -pozycjaZdrzewa44, 70);
	drzewo(600 + pozycjaXdrzew, 0, pozycjaZdrzewa55, 70);
	drzewo(700 + pozycjaXdrzew, 0, -pozycjaZdrzewa66, 70);

	pasy(-800 + pozX, 78 + pozZ, dlugosc + 200, 10, pozycjaX2);
	pasy(-800 + pozX, -22 + pozZ, dlugosc + 200, 10, pozycjaX2);

	//kamienie
	kamien(50 + pozycjaXkamieni2, -10, pozycjaZkamienia11, 20, 20, 20);
	kamien(150 + pozycjaXkamieni2, -10, -pozycjaZkamienia22, 20, 20, 20);
	kamien(350 + pozycjaXkamieni2, -10, pozycjaZkamienia33, 20, 20, 20);
	kamien(450 + pozycjaXkamieni2, -10, -pozycjaZkamienia44, 20, 20, 20);
	kamien(650 + pozycjaXkamieni2, -10, pozycjaZkamienia55, 20, 20, 20);
	kamien(750 + pozycjaXkamieni2, -10, -pozycjaZkamienia66, 20, 20, 20);
  
	przeszkoda(pozycjaX3, pozycjaZ2);
}

void uklad()
{
	//X
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex3f(-10, 0, 0);
	glVertex3f(10, 0, 0);
	glEnd();
	//>
	glBegin(GL_LINE_STRIP);
	glVertex3f(9.5, 0.5, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(9.5, -0.5, 0);
	glEnd();
	//x
	glBegin(GL_LINES);
	glVertex3f(10.5, -0.5, 0);
	glVertex3f(11, -1.25, 0);
	glVertex3f(10.5, -1.25, 0);
	glVertex3f(11, -0.5, 0);


	//Y
	glBegin(GL_LINES);
	glVertex3f(0, -10, 0);
	glVertex3f(0, 10, 0);
	glEnd();
	//^
	glBegin(GL_LINE_STRIP);
	glVertex3f(-0.5, 9.5, 0);
	glVertex3f(0, 10, 0);
	glVertex3f(0.5, 9.5, 0);
	glEnd();
	//y
	glBegin(GL_LINE_STRIP);
	glVertex3f(-1.25, 10, 0);
	glVertex3f(-1, 9.625, 0);
	glVertex3f(-0.75, 10, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-1, 9.625, 0);
	glVertex3f(-1, 9.250, 0);
	glEnd();

	//Z
	glBegin(GL_LINES);
	glVertex3f(0, 0, -10);
	glVertex3f(0, 0, 10);
	glEnd();
	//>
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0.5, 9.5);
	glVertex3f(0, 0, 10);
	glVertex3f(0, -0.5, 9.5);
	glEnd();
	//z
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, -0.5, 11);
	glVertex3f(0, -0.5, 10.5);
	glVertex3f(0, -1.25, 11);
	glVertex3f(0, -1.25, 10.5);
	glEnd();
}
// Called to draw scene
void RenderScene(void)
	{
	//float normal[3];	// Storeage for calculated surface normal

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////
	//szescian();
	
	//Sposób na odróŸnienie "przedniej" i "tylniej" œciany wielok¹ta:
	glPolygonMode(GL_BACK,GL_LINE);

	glPushMatrix();
	glTranslatef(1000, -300, -25);

		glPushMatrix();
		glTranslatef(-800,0,pozycjaZ);
			ciongnik();
		glPopMatrix();
		mapa(0, 0,2000);
	glPopMatrix();
	//skrzynka();
	//kula();


	//uklad();
	//walec(40, 40);
	//szescian();
	//Uzyskanie siatki:
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	//Wyrysowanie prostokata:
	//glRectd(-10.0,-10.0,20.0,20.0);
		
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Flush drawing commands
	glFlush();
	}


// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
	{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
	}



// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
	{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;
								// Range for each color entry (7,7,and 3)


	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;	

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;

	// Loop through all the palette entries
	for(i = 0; i < nColors; i++)
		{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
		}
		

	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC,hRetPal,FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
	}


// Entry point of all Windows programs
int APIENTRY WinMain(   HINSTANCE       hInst,
						HINSTANCE       hPrevInstance,
						LPSTR           lpCmdLine,
						int                     nCmdShow)
	{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style                        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc          = (WNDPROC) WndProc;
	wc.cbClsExtra           = 0;
	wc.cbWndExtra           = 0;
	wc.hInstance            = hInstance;
	wc.hIcon                        = NULL;
	wc.hCursor                      = LoadCursor(NULL, IDC_ARROW);
	
	// No need for background brush for OpenGL window
	wc.hbrBackground        = NULL;         
	
	wc.lpszMenuName         = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName        = lpszAppName;

	// Register the window class
	if(RegisterClass(&wc) == 0)
		return FALSE;


	// Create the main application window
	hWnd = CreateWindow(
				lpszAppName,
				lpszAppName,
				
				// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	
				// Window position and size
				10, 10,
				600, 800,
				NULL,
				NULL,
				hInstance,
				NULL);

	// If window was not created, quit
	if(hWnd == NULL)
		return FALSE;


	// Display the window
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);

	// Process application messages until the application closes
	while( GetMessage(&msg, NULL, 0, 0))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	return msg.wParam;
	}




// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(       HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam)
	{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	
	switch (message)
		{
		case WM_TIMER:
			switch (wParam) {
				case 1:
					if (pozycjaX <= -1200) 
						pozycjaX = 600;
					if (pozycjaX2 <= -1200)
						pozycjaX2 = 600;
					if (pozycjaXdrzew <= -1600) {
						pozycjaXdrzew = 800;
						pozycjaZdrzewa11 = (rand() % 250) + 200;
						pozycjaZdrzewa22 = (rand() % 250) + 200;
						pozycjaZdrzewa33 = (rand() % 250) + 200;
						pozycjaZdrzewa44 = (rand() % 250) + 200;
						pozycjaZdrzewa55 = (rand() % 250) + 200;
						pozycjaZdrzewa66 = (rand() % 250) + 200;

					}
						
					if (pozycjaXdrzew2 <= -1600) {
						pozycjaXdrzew2 = 800;
						pozycjaZdrzewa1 = (rand() % 250) + 200;
						pozycjaZdrzewa2 = (rand() % 250) + 200;
						pozycjaZdrzewa3 = (rand() % 250) + 200;
						pozycjaZdrzewa4 = (rand() % 250) + 200;
						pozycjaZdrzewa5 = (rand() % 250) + 200;
						pozycjaZdrzewa6 = (rand() % 250) + 200;

					}
					if (pozycjaXkamieni <= -1600) {
						pozycjaXkamieni = 800;
						pozycjaZkamienia1 = (rand() % 150) + 250;
						pozycjaZkamienia2 = (rand() % 150) + 250;
						pozycjaZkamienia3 = (rand() % 150) + 250;
						pozycjaZkamienia4 = (rand() % 150) + 250;
						pozycjaZkamienia5 = (rand() % 150) + 250;
						pozycjaZkamienia6 = (rand() % 150) + 250;
					}
					if (pozycjaXkamieni2 <= -1600) {
						pozycjaXkamieni2 = 800;
						pozycjaZkamienia11 = (rand() % 150) + 250;
						pozycjaZkamienia22 = (rand() % 150) + 250;
						pozycjaZkamienia33 = (rand() % 150) + 250;
						pozycjaZkamienia44 = (rand() % 150) + 250;
						pozycjaZkamienia55 = (rand() % 150) + 250;
						pozycjaZkamienia66 = (rand() % 150) + 250;
					}
				
					pozycjaX-=20;
					pozycjaX2-=20;
					pozycjaXdrzew -= 20;
					pozycjaXdrzew2 -= 20;
					pozycjaXkamieni -= 20;
					pozycjaXkamieni2 -= 20;

					InvalidateRect(hWnd, NULL, FALSE);

					//if (pozycjaX3 < 0 && pozycjaZ == 0) {
					//	//pozycjaZ += 50;
					//	pozycjaX -= 1;
					//}
					break;
			}
			break;

		// Window creation, setup for OpenGL
		case WM_CREATE:
			SetTimer(hWnd, 1, 5, NULL);
			// Store the device context
			hDC = GetDC(hWnd);              

			// Select the pixel format
			SetDCPixelFormat(hDC);          

			// Create palette if needed
			hPalette = GetOpenGLPalette(hDC);

			// Create the rendering context and make it current
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			SetupRC();
			glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			

			// ³aduje pierwszy obraz tekstury:
			bitmapData = LoadBitmapFile("car.bmp", &bitmapInfoHeader);

			glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			// tworzy obraz tekstury
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
				bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

			if (bitmapData)
				free(bitmapData);

			// ³aduje drugi obraz tekstury:

			bitmapData = LoadBitmapFile("weed.bmp", &bitmapInfoHeader);
			glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			// tworzy obraz tekstury
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
				bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

			if (bitmapData)
				free(bitmapData);

			bitmapData = LoadBitmapFile("kupakamieni.bmp", &bitmapInfoHeader);
			glBindTexture(GL_TEXTURE_2D, texture[2]);       // aktywuje obiekt tekstury

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			// tworzy obraz tekstury
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
				bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

			if (bitmapData)
				free(bitmapData);

			bitmapData = LoadBitmapFile("liscie.bmp", &bitmapInfoHeader);
			glBindTexture(GL_TEXTURE_2D, texture[3]);       // aktywuje obiekt tekstury

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			// tworzy obraz tekstury
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
				bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

			if (bitmapData)
				free(bitmapData);

			// ustalenie sposobu mieszania tekstury z t³em
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			break;

		// Window is being destroyed, cleanup
		case WM_DESTROY:
			// Deselect the current rendering context and delete it
			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);

			// Delete the palette if it was created
			if(hPalette != NULL)
				DeleteObject(hPalette);

			// Tell the application to terminate after the window
			// is gone.
			PostQuitMessage(0);
			break;

		// Window is resized.
		case WM_SIZE:
			// Call our function which modifies the clipping
			// volume and viewport
			ChangeSize(LOWORD(lParam), HIWORD(lParam));
			break;


		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
		case WM_PAINT:
			{
			// Call OpenGL drawing code
			RenderScene();

			SwapBuffers(hDC);

			// Validate the newly painted client area
			ValidateRect(hWnd,NULL);
			}
			break;

		// Windows is telling the application that it may modify
		// the system palette.  This message in essance asks the 
		// application for a new palette.
		case WM_QUERYNEWPALETTE:
			// If the palette was created.
			if(hPalette)
				{
				int nRet;

				// Selects the palette into the current device context
				SelectPalette(hDC, hPalette, FALSE);

				// Map entries from the currently selected palette to
				// the system palette.  The return value is the number 
				// of palette entries modified.
				nRet = RealizePalette(hDC);

				// Repaint, forces remap of palette in current window
				InvalidateRect(hWnd,NULL,FALSE);

				return nRet;
				}
			break;

	
		// This window may set the palette, even though it is not the 
		// currently active window.
		case WM_PALETTECHANGED:
			// Don't do anything if the palette does not exist, or if
			// this is the window that changed the palette.
			if((hPalette != NULL) && ((HWND)wParam != hWnd))
				{
				// Select the palette into the device context
				SelectPalette(hDC,hPalette,FALSE);

				// Map entries to system palette
				RealizePalette(hDC);
				
				// Remap the current colors to the newly realized palette
				UpdateColors(hDC);
				return 0;
				}
			break;

		// Key press, check for arrow keys to do cube rotation.
		case WM_KEYDOWN:
			{
			if(wParam == VK_UP)
				xRot-= 5.0f;

			if(wParam == VK_DOWN)
				xRot += 5.0f;

			if(wParam == VK_LEFT)
				yRot -= 5.0f;

			if(wParam == VK_RIGHT)
				yRot += 5.0f;

			if (wParam == 87) {
				//pozycjaX += 10;
			}

			if (wParam == 83) {
				//pozycjaX -= 10;
			}
				
			if (wParam == 68) { //D
				pozycjaZ += 100;
				if (pozycjaZ > 100) pozycjaZ = -100;
			}

			if (wParam == 65) { //A
				pozycjaZ -= 100;
				if (pozycjaZ < -100) pozycjaZ = 100;
			}

			xRot = (const int)xRot % 360;
			yRot = (const int)yRot % 360;

			InvalidateRect(hWnd,NULL,FALSE);
			}
			break;

		// A menu command
		case WM_COMMAND:
			{
			switch(LOWORD(wParam))
				{
				// Exit the program
				case ID_FILE_EXIT:
					DestroyWindow(hWnd);
					break;

				// Display the about box
				case ID_HELP_ABOUT:
					DialogBox (hInstance,
						MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
						hWnd,
						AboutDlgProc);
					break;
				}
			}
			break;


	default:   // Passes it on if unproccessed
	    return (DefWindowProc(hWnd, message, wParam, lParam));

	}

    return (0L);
	}




// Dialog procedure.
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
	{
	
    switch (message)
	{
		// Initialize the dialog box
	    case WM_INITDIALOG:
			{
			int i;
			GLenum glError;

			// glGetString demo
			SetDlgItemText(hDlg,IDC_OPENGL_VENDOR,glGetString(GL_VENDOR));
			SetDlgItemText(hDlg,IDC_OPENGL_RENDERER,glGetString(GL_RENDERER));
			SetDlgItemText(hDlg,IDC_OPENGL_VERSION,glGetString(GL_VERSION));
			SetDlgItemText(hDlg,IDC_OPENGL_EXTENSIONS,glGetString(GL_EXTENSIONS));

			// gluGetString demo
			SetDlgItemText(hDlg,IDC_GLU_VERSION,gluGetString(GLU_VERSION));
			SetDlgItemText(hDlg,IDC_GLU_EXTENSIONS,gluGetString(GLU_EXTENSIONS));


			// Display any recent error messages
			i = 0;
			do {
				glError = glGetError();
				SetDlgItemText(hDlg,IDC_ERROR1+i,gluErrorString(glError));
				i++;
				}
			while(i < 6 && glError != GL_NO_ERROR);


			return (TRUE);
			}
			break;

		// Process command messages
	    case WM_COMMAND:      
			{
			// Validate and Make the changes
			if(LOWORD(wParam) == IDOK)
				EndDialog(hDlg,TRUE);
		    }
			break;

		// Closed from sysbox
		case WM_CLOSE:
			EndDialog(hDlg,TRUE);
			break;
		}

	return FALSE;
	}