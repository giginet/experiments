#ifndef			XGRA5_H
#define			XGRA5_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef			XGRA5
#include		<stdio.h>
#include		<stdlib.h>
#include		<string.h>
#include		<math.h>
#include		<X11/Xlib.h>
#include		<X11/Xutil.h>
#include		<X11/keysym.h>
#include		<X11/XWDFile.h>
#include		<X11/cursorfont.h>
#include		<netinet/in.h>

#define     C(x) ((char)(x))
#define     S(x) ((short)(x))
#define     I(x) ((int)(x))
#define     L(x) ((long)(x))
#define     F(x) ((float)(x))
#define     D(x) ((double)(x))
#define     UC(x) ((unsigned char)(x))
#define     US(x) ((unsigned short)(x))
#define     UI(x) ((unsigned int)(x))
#define     UL(x) ((unsigned long)(x))

#define		MAX_NUM_WIN		32
#define		MAX_NUM_COLOR	65536
#define		MAX_NUM_FONT	32

typedef     unsigned char   Uchar;
typedef     unsigned short  Ushort;
typedef     unsigned long   Ulong;
typedef     unsigned int    Uint;

char	*MallocInX( unsigned long, char*);

#endif


void	InitWindow( int );
unsigned long	OpenWindow( int );
void	CloseWindow( int );
void	SetBubWindow( int, int );
int		gWindowX( int win );
int		gWindowY( int win );
int		gWindowW( int win );
int		gWindowH( int win );
void	SetBorderWidth( int, int );
void	MngrOn( int, int);
void	BackingOn( int);
void	NamingWindow( int, char*);
void	FocusOnWindow( int);
void	SetPixel( int, int, int);
void	SetPixels( int, int*, int*, int);
void	Moveto( int, int, int);
void	Lineto( int, int, int);
void	Line( int, int, int, int, int);
void	Lines( int, int*, int*, int);
void	Rectangle( int, int, int, int, int);
void	Rectangles( int, int*, int*, int, int, int);
void	PutRectangle( int, int, int, int, int);
void	PutRectangles( int, int*, int*, int, int, int);
void	PutPolygon( int, int*, int*, int);
void	Circle( int, int, int, int);
void	PutCircle( int, int, int, int);
void    Ellipse( int, int, int, int, int);
void    PutEllipse( int, int, int, int, int);
void	Arc( int, int, int, int, double, double );
void	PutArc( int, int, int, int, double, double );
void	SetLineWidth( int);
void	SetLineStyle( int, int, int, int);
void	ClearBlock( int, int, int, int, int);
void	ClearWindow( int);
int		SetFont( char* );
void	OutText( int, int, int, char*);
void	PutText( int, int, int, char*);
void	ClearText( int, int, int, int);
int		HitText( int, int, int, int, int, int);
void	InputText( int, int, int, char*);
int		SetWindowColor( int, char* );
void	SetWindowColorN( int, int );
int		SetBorderColor( int, char* );
void	SetBorderColorN( int, int );
int		SetColor( char* );
int		SetColorInX( char* );
void	SetColorN( int );
void	SetOrgColor( int );
void	SetGray( int );
int		InitColorPixel( char* );
void	InitDefaultColor( void );
unsigned long GetColorPxl( char* );
int		KPress( int);
int		BPress( int);
int		Button_press( int, int*, int*);
int		WindowIO( int*, int*, int*, int*, int*);
int		WindowExpose( int);
void	RaiseWindow( int);
void	SetSize( int, int, int);
void	SetPosition( int, int, int);
void	MoveWindow( int);
void	SetCursor( int, int);
void	SetPointer( int, int, int);
void	CreateImage( int, int, int);
void	PutImage( int);
void	SetImageData( int, char*);
void	SetImageColor( int, char*);
void	EndWindow( void);


void	WriteImage(int win, FILE *fp);
void	WritePixmap(int win, FILE *fp);
void	WriteDummyPixmap(FILE *fp);
void	CreatePixmap(int win, int w, int h);
void	CreateDummyPixmap(int w, int h);
void	RectanglePixmap(int win, int x, int y, int w, int h);
void	PutRectanglePixmap(int win, int x, int y, int w, int h);
void	PutRectangleDummyPixmap(int x, int y, int w, int h);
void	CirclePixmap(int win, int x, int y, int r);
void	PutCirclePixmap(int win, int x, int y, int r);
void	CopyPixmap2Window(int win, int w, int h);
void	CompulsoryFlush(void);
void	SetPixelPixmap(int win, int x, int y);


void	SaveImageInXWD(int win, char* filename);

#ifdef __cplusplus
}
#endif

#endif
