#define			XGRA5
//#include		"/export/yoshii/xlib/xgra5-3.h"
#include		"xgra5-3.h"



/*****************************************************************************/

int				WinNum;
Display			*Xd;
Window			*Xw, Xwdummy;
Window			*Xpw;
Pixmap			*Xp, Xpdummy;;
Colormap		Xcm;

GC				Xgc;

Cursor			*Xcu;
XImage			**Ximg;
int				*Xcx, *Xcy;

int				*WPX, *WPY, WPDummyX, WPDummyY;
Uint			*WSW, *WSH, WSDummyW, WSDummyH;

int				ColorNum;
Ulong			ColorPxlv[MAX_NUM_COLOR];
Ulong			*GrayLvl;
Ulong			Xblk, Xwht;
Ulong			*Xpx;

XFontStruct		*Xfnt[MAX_NUM_FONT];
int				FontNum;
int				Ta, Td, Tw, Th, Tr, Tl, Tp;

int				*Xbacking;

char			*ChkOpen;
char			*Xwmng;
char			*ColorName[MAX_NUM_COLOR];
char			*FontName[MAX_NUM_FONT];
char			**WindowName;


/*****************************************************************************/

void
InitWindow( int win )
{
  int		i;
  int		Xs;

  WinNum = win;

  Xd  = XOpenDisplay(NULL);
  Xs  = DefaultScreen( Xd );
  Xcm = DefaultColormap( Xd, Xs );

  ColorNum = 0;
  FontNum = 0;
  Xgc == NULL;

  Xblk = BlackPixel(Xd, 0);
  Xwht = WhitePixel(Xd, 0);

  Xw  = (Window*)MallocInX( UL(sizeof(Window)*WinNum), "Xw" );
  Xpw = (Window*)MallocInX( UL(sizeof(Window)*WinNum), "Xpw" );
	Xp = (Pixmap *)MallocInX(UL(sizeof(Pixmap) * WinNum), "Xp");
  Xcu = (Cursor*)MallocInX( UL(sizeof(Cursor)*WinNum), "Xcu" );
  WPX = (int*)MallocInX( UL(sizeof(int)*WinNum), "WPX" );
  WPY = (int*)MallocInX( UL(sizeof(int)*WinNum), "WPY" );
  WSW = (Uint*)MallocInX( UL(sizeof(Uint)*WinNum), "WSW" );
  WSH = (Uint*)MallocInX( UL(sizeof(Uint)*WinNum), "WSH" );
  Xcx = (int*)MallocInX( UL(sizeof(int)*WinNum), "Xcx" );
  Xcy = (int*)MallocInX( UL(sizeof(int)*WinNum), "Xcy" );
  Xwmng = (char*)MallocInX( UL(sizeof(char)*WinNum), "wmng" );
  Xbacking = (int*)MallocInX( UL(sizeof(int)*WinNum), "Xbacking" );
  WindowName =(char**)MallocInX( UL(sizeof(char*)*WinNum),"winname");
  Ximg = (XImage**)MallocInX( UL(sizeof(XImage*)*WinNum), "img" );
  ChkOpen = (char*)MallocInX( UL(sizeof(char)*WinNum), "chkOpen" );

  for( i = 0; i < WinNum; i++ ){
	Xw[i] = 0;
	Xpw[i] = RootWindow( Xd, Xs );
//	Xp[i] = 0;
	WPX[i] = I(0);
	WPY[i] = I(0);
	WSW[i] = UI(640);
	WSH[i] = UI(400);
	Xcx[i] = I(0);
	Xcy[i] = I(0);
	Xwmng[i] = 1;
	Xbacking[i] = 0;
	WindowName[i] = NULL;
	Ximg[i] = NULL;
	Xcu[i] = XCreateFontCursor(Xd, XC_top_left_arrow);
	ChkOpen[i] = 0;
  }

  for( i = 0; i < MAX_NUM_COLOR; i++ ){
	ColorPxlv[i] = (unsigned long)1; /* Black */
	ColorName[i] = NULL;
  }
}
/**********************************************************************/
Ulong
OpenWindow( int win )
{
  Ulong		vmsk;
  XGCValues	val;
  XEvent		ev;
  XSizeHints	Xsize;
  XSetWindowAttributes    Xatt;

  if( Xw[win] == 0 ){
	if( Xpw[win] == 0 ){
	  fprintf(stderr,"No parent window\n");
	  exit(1);
	}

	Xw[win] = XCreateSimpleWindow(Xd, Xpw[win],
								  WPX[win], WPY[win],
								  WSW[win], WSH[win],
								  UI(2), Xblk, Xwht);

	if( Xwmng[win] == 0 ){
	  Xatt.override_redirect = True;
	  XChangeWindowAttributes(Xd,Xw[win], CWOverrideRedirect, &Xatt);
	}
	else{
	  Xsize.flags = USPosition | USSize;
	  Xsize.width = WSW[win];
	  Xsize.height = WSH[win];
	  Xsize.x = WPX[win];
	  Xsize.y = WPY[win];
	  XSetNormalHints( Xd, Xw[win], &Xsize );
	}

	if( Xbacking[win] == 1 ){
	  Xatt.backing_store = Always;
	  XChangeWindowAttributes(Xd,Xw[win], CWBackingStore, &Xatt);
	}

	if( WindowName[win] != NULL ){
	  XStoreName( Xd, Xw[win], WindowName[win] );
	}

	XSelectInput(Xd, Xw[win],
				 ExposureMask|
				 ButtonPressMask|
				 ButtonReleaseMask|
				 ButtonMotionMask|
				 KeyPressMask|
				 StructureNotifyMask);

	XDefineCursor( Xd, Xw[win], Xcu[win] );

	if( Xgc == NULL ){
      Xgc = XCreateGC(Xd,Xw[win],0,0);
	  SetFont( "8x16" );
	  SetLineWidth( 1 );
	  XSetForeground( Xd, Xgc, Xblk );
	}
	XFlush(Xd);
  }

  if( ChkOpen[win] == 0 ){
	XMapWindow(Xd, Xw[win]);
	XFlush(Xd);
	while( 1 ){
	  XNextEvent( Xd, &ev );
	  XFlush(Xd);
	  if( ev.type == MapNotify ){
		if( ev.xany.window == Xw[win] ){
		  break;
		}
	  }
	}
	ChkOpen[win] = 1;
  }
  
  return( (Ulong)Xw[win] );
}

/**********************************************************************/
void
CloseWindow( int win )
{
  WPX[win] = gWindowX( win );
  WPY[win] = gWindowX( win );
  WSW[win] = UI(gWindowW( win ));
  WSH[win] = UI(gWindowH( win ));

  XUnmapWindow(Xd, Xw[win]);
  XFlush(Xd);
  ChkOpen[win] = 0;
}
/**********************************************************************/
void
SetSubWindow( int parent, int child )
{
  Xpw[ child ] = Xw[ parent ];
}
/**********************************************************************/
int
gWindowX( int win )
{
  Window	r;
  int		x, y;
  unsigned int		w, h, bw, d;
  
  XGetGeometry( Xd, Xw[win], &r, &x, &y, &w, &h, &bw, &d );
  XFlush(Xd);
  return x;
}
int
gWindowY( int win )
{
  Window	r;
  int		x, y;
  unsigned int		w, h, bw, d;
  
  XGetGeometry( Xd, Xw[win], &r, &x, &y, &w, &h, &bw, &d );
  XFlush(Xd);
  return y;
}
int
gWindowW( int win )
{
  Window	r;
  int		x, y;
  unsigned int		w, h, bw, d;
  
  XGetGeometry( Xd, Xw[win], &r, &x, &y, &w, &h, &bw, &d );
  XFlush(Xd);
  return (int)w;
}
int
gWindowH( int win )
{
  Window	r;
  int		x, y;
  unsigned int		w, h, bw, d;
  
  XGetGeometry( Xd, Xw[win], &r, &x, &y, &w, &h, &bw, &d );
  XFlush(Xd);
  return (int)h;
}

void
SetBorderWidth( int win, int width )
{
  XSetWindowBorderWidth( Xd, Xw[win], (unsigned int)width );
  XFlush(Xd);
}
/**********************************************************************/
void
MngrOn( int win, int s )
{
  Xwmng[win] = (s == 0) ? 0 : 1;
}
/**********************************************************************/
void
BackingOn( int win )
{
	Xbacking[win] = 1;
}
/**********************************************************************/
void
NamingWindow( int win, char* str )
{
  if( WindowName[win] != NULL ) free( WindowName[win] );
  WindowName[win] = (char*)MallocInX( (strlen(str)+2), "WindName" );
  strcpy( WindowName[win], str );
  if( Xw[win] != 0 )
	XStoreName( Xd, Xw[win], str );
  XFlush(Xd);
}
/**********************************************************************/
void
SetPixel( int win, int x, int y )
{
	XDrawPoint(Xd, Xw[win], Xgc, x, y);
	XFlush(Xd);
	Xcx[win] = x;
	Xcy[win] = y;
}
/**********************************************************************/
void
SetPixels( int win, int *points_x, int *points_y, int pn )
{
XPoint	*p;
int	i;
	p = (XPoint*)MallocInX( UL(sizeof(XPoint)*pn), "XPoint");
	for( i = 0; i < pn; i++){
	  p[i].x = points_x[i];
	  p[i].y = points_y[i];
	}
	XDrawPoints(Xd, Xw[win], Xgc, p, pn, CoordModeOrigin);
	XFlush(Xd);
	free( p );
	Xcx[win] = points_x[pn-1];
	Xcy[win] = points_y[pn-1];
}
/**********************************************************************/
void
Moveto( int win, int x, int y )
{
	Xcx[win] = x;
	Xcy[win] = y;
}
/**********************************************************************/
void
Lineto( int win, int x, int y )
{
	XDrawLine(Xd, Xw[win], Xgc, Xcx[win], Xcy[win], x, y);
	XFlush(Xd);
	Xcx[win] = x;
	Xcy[win] = y;
}
/*********************************************************************/
void
Line( int win, int x1, int y1, int x2, int y2 )
{
	XDrawLine(Xd, Xw[win], Xgc, x1, y1, x2, y2);
	XFlush(Xd);
}
/**********************************************************************/
void
Lines( int win, int *points_x, int *points_y, int pn )
{
XPoint	*p;
int	i;

	p = (XPoint*)MallocInX( UL(sizeof(XPoint)*pn), "XPoint");
	for( i = 0; i < pn; i++){
	  p[i].x = points_x[i];
	  p[i].y = points_y[i];
	}
	XDrawLines(Xd, Xw[win], Xgc, p, pn, CoordModeOrigin);
	XFlush(Xd);
	free( p );
}
/**********************************************************************/
void
Rectangle( int win, int x, int y, int w, int h )
{
	XDrawRectangle(Xd, Xw[win], Xgc, x, y, UI(w), UI(h));
	XFlush(Xd);
}
/**********************************************************************/
void
Rectangles( int win, int *points_x, int *points_y, int w, int h, int rn )
{
XRectangle	*rect;
int		i;

	rect=(XRectangle*)MallocInX(UL(sizeof(XRectangle)*rn),"XRectangle");
	for( i = 0; i < rn; i++){
	  rect[i].x = points_x[i];
	  rect[i].y = points_y[i];
	  rect[i].width = UI(w);
	  rect[i].height = UI(h);
	}
	XDrawRectangles(Xd, Xw[win], Xgc, rect, rn);
	XFlush(Xd);
	free( rect );
}
/**********************************************************************/
void
PutRectangle( int win, int x, int y, int w, int h )
{
	XFillRectangle(Xd, Xw[win], Xgc, x, y, UI(w), UI(h));
	XFlush(Xd);
}
/**********************************************************************/
void
PutRectangles( int win, int *points_x, int *points_y, int w, int h, int rn )
{
XRectangle	*rect;
int		i;

	rect=(XRectangle*)MallocInX(UL(sizeof(XRectangle)*rn),"XRectangle");
	for( i = 0; i < rn; i++){
	  rect[i].x = points_x[i];
	  rect[i].y = points_y[i];
	  rect[i].width = UI(w);
	  rect[i].height = UI(h);
	}
	XFillRectangles(Xd, Xw[win], Xgc, rect, rn);
	XFlush(Xd);
	free( rect );
}
/**********************************************************************/
void
PutPolygon( int win, int *points_x, int *points_y, int pn )
{
XPoint	*p;
int	i;
	
	p = (XPoint*)MallocInX( UL(sizeof(XPoint)*pn), "XPoint" );
	for( i = 0; i < pn; i++){
	  p[i].x = points_x[i];
	  p[i].y = points_y[i];
	}
	XFillPolygon(Xd, Xw[win], Xgc, p, pn, Convex, CoordModeOrigin);
	XFlush( Xd );
	free( p );
}
/**********************************************************************/
void
Circle( int win, int x, int y, int r )
{
	XDrawArc(Xd, Xw[win], Xgc, x-r, y-r, UI(r*2), UI(r*2),
													I(0),I(360*64));
	XFlush(Xd);
}
/**********************************************************************/
void
PutCircle( int win, int x, int y, int r )
{
	XFillArc(Xd, Xw[win], Xgc, x-r, y-r, UI(r*2), UI(r*2),
													I(0),I(360*64));
	XFlush(Xd);
}
/**********************************************************************/
void
Ellipse( int win, int x, int y, int xr, int yr )
{
	XDrawArc(Xd, Xw[win], Xgc, x-xr, y-yr, UI(xr*2), UI(yr*2),
													I(0),I(360*64));
	XFlush(Xd);
}
/**********************************************************************/
void
PutEllipse( int win, int x, int y, int xr, int yr )
{
	XFillArc(Xd, Xw[win], Xgc, x-xr, y-yr, UI(xr*2), UI(yr*2),
													I(0),I(360*64));
	XFlush(Xd);
}
/**********************************************************************/
void
Arc( int win, int x, int y, int r, double a1, double a2 )
{
  int		b1, b2;

  b1 = (int)(a1 * 64.0) + (int)(a1 * 64.0)%64;
  b2 = (int)(a2 * 64.0) + (int)(a2 * 64.0)%64;
  XDrawArc(Xd, Xw[win], Xgc, x-r, y-r, UI(r*2), UI(r*2), b1, b2 );
  XFlush(Xd);
}
/**********************************************************************/
void
PutArc( int win, int x, int y, int r, double a1, double a2 )
{
  int		b1, b2;

  b1 = (int)(a1 * 64.0) + (int)(a1 * 64.0)%64;
  b2 = (int)(a2 * 64.0) + (int)(a2 * 64.0)%64;
  XFillArc(Xd, Xw[win], Xgc, x-r, y-r, UI(r*2), UI(r*2), b1, b2 );
  XFlush(Xd);
}
/**********************************************************************/
void
SetLineWidth( int lw )
{
  XSetLineAttributes(Xd,Xgc,UI(lw),LineSolid,CapButt,JoinMiter);
  XFlush(Xd);
}
/**********************************************************************/
void
SetLineStyle( int lw, int ls, int cs, int js )
{
  XSetLineAttributes(Xd, Xgc, UI(lw), ls, cs, js);
  XFlush(Xd);
  /*
	lw : line width
	ls : line style
	cs : cap style
	js : joint syle
	*/
}
/**********************************************************************/
void
ClearBlock( int win, int x, int y, int w, int h )
{
  XClearArea(Xd, Xw[win], x, y, UI(w), UI(h), True);
  XFlush(Xd);
}
/**********************************************************************/
void
ClearWindow( int win )
{
  XClearWindow( Xd, Xw[win] );
}
/**********************************************************************/
int
SetFont( char *fnt_name )
{
  int			i = 0;

  if( fnt_name != NULL ){
	for( i = 0; i < FontNum; i++ ){
	  if( strcmp( FontName[i], fnt_name ) == 0 ) break;
	}
	if( i == MAX_NUM_FONT ) return 0;
	else if( i == FontNum ){
	  Xfnt[i] = XLoadQueryFont( Xd, fnt_name );
	  if( Xfnt[i] == 0 ) return 0;
	  FontName[i] = (char*)MallocInX( (strlen(fnt_name)+2), "fntname" );
	  strcpy( FontName[i], fnt_name );
	  FontNum++;
	}
  }

  XSetFont(Xd, Xgc, Xfnt[i]->fid );

  Tl = (int)Xfnt[i]->max_bounds.lbearing;
  Tr = (int)Xfnt[i]->max_bounds.rbearing;
  Ta = (int)Xfnt[i]->max_bounds.ascent;
  Td = (int)Xfnt[i]->max_bounds.descent;
  Tw = (int)Xfnt[i]->max_bounds.width;
  Th = (int)Xfnt[i]->max_bounds.ascent + Xfnt[i]->max_bounds.descent;
  Tp = Th+2;

  XFlush(Xd);
  return 1;
}
/**********************************************************************/
void
OutText( int win, int x, int y, char* s )
{
  XDrawString(Xd, Xw[win], Xgc, x, y, s, I(strlen(s)));
  XFlush(Xd);
}
/**********************************************************************/
void
PutText( int win, int r, int c, char* s )
{
  XDrawString(Xd,Xw[win],Xgc,(++c)*Tw,(++r)*(Tp),s,I(strlen(s)));
  XFlush(Xd);
}
/**********************************************************************/
void
ClearText( int win, int r, int c, int len )
{
  ClearBlock(win, (++c)*Tw-Tl, (++r)*(Tp)-Ta-1, Tl+Tw*(--len)+Tr, Tp );
  XFlush(Xd);
}
/**********************************************************************/
int
HitText( int x, int y, int sr, int sc, int r, int c )
{
  int			sx, sy, ex, ey;

  sx = Tw * (++sc);
  sy = Tp * sr;
  ex = ( c <= 0 ) ? (1024) : (sx + (Tw * c));
  ey = ( r <= 0 ) ? (1024) : (sy + (Tp * r));
  
  if( sx < x && x < ex && sy < y && y < ey ) return 1;

  return 0;
}
/**********************************************************************/
void
InputText( int win, int r, int c, char* s )
{
  int		p, isend, x, y;
  XEvent	ev;
  XKeyEvent kev;
  KeySym	key;
  char		str[20];
  int		w, h, dir, asc, des;
  XCharStruct		v;

/*
Time	time;
	XSetInputFocus( Xd, Xw[win], RevertToPointerRoot, time );
*/
  p = 0;
  isend = False;
  while( 1 ){
	XNextEvent( Xd, &ev );
	if( ev.type == KeyPress && ev.xany.window == Xw[win] ){
	  if( XLookupString( &kev, str, 20, &key, 0 ) == 1 );
	  switch( key ){
	  case XK_Return :
		isend = True;
		break;
	  case XK_BackSpace :
		p--;
		if( p < 0 ) p = 0;
		break;
	  case XK_Shift_R :
	  case XK_Shift_L :
		break;
		default :
		  if( !IsCursorKey( key ) && !IsFunctionKey( key ) ){
			s[ p++ ] = str[0];
		  }
	  }
	  s[ p ] = 0x0;
	  ClearText( win, r, c, p+1 );
	  PutText( win, r, c, s );
	  if( isend ) break;
	}
  }
  /*
	XSetInputFocus( Xd, PointerRoot, RevertToPointerRoot, time );
	*/
}
/**********************************************************************/
int
SetWindowColor( int win, char* cname )
{
  int		rtn;

  if( (rtn = SetColorInX( cname )) == -1 ) return -1;

  XSetWindowBackground( Xd, Xw[win], ColorPxlv[ rtn ] );
  XFlush(Xd);
  return rtn;
}
/**********************************************************************/
void
SetWindowColorN( int win, int num )
{
  if( num < ColorNum ){
	XSetWindowBackground( Xd, Xw[win], ColorPxlv[num] );
	XFlush(Xd);
  }
}
/**********************************************************************/
int
SetBorderColor( int win, char* cname )
{
  int		rtn;

  if( (rtn = SetColorInX( cname )) == -1 ) return -1;

  XSetWindowBorder( Xd, Xw[win], ColorPxlv[ rtn ] );
  XFlush(Xd);
  return rtn;
}
/**********************************************************************/
void
SetBorderColorN( int win, int num )
{
  if( num < ColorNum ){
	XSetWindowBorder( Xd, Xw[win], ColorPxlv[num] );
	XFlush(Xd);
  }
}
/**********************************************************************/
int
SetColor( char *cname )
{
  int		rtn;

  if( (rtn = SetColorInX( cname )) == -1 ) return -1;
  XSetForeground(Xd, Xgc, ColorPxlv[ rtn ] );
  XFlush(Xd);
  return rtn;
}
/**********************************************************************/
int
SetColorInX( char *cname )
{
  int		i;

  for( i = 0; i < ColorNum; i++ ){
	if( strcmp( ColorName[i], cname ) == 0 ) break;
  }
  if( i == MAX_NUM_COLOR ) return -1;
  else if( i == ColorNum ){
	ColorPxlv[i] = GetColorPxl( cname );
	ColorName[i] = (char*)MallocInX( (strlen(cname)+2), "colorname" );
	strcpy( ColorName[ i ], cname );
	ColorNum++;
  }
  return i;
}
/**********************************************************************/
void
SetColorN( int num )
{
  if( num < ColorNum ){
	XSetForeground(Xd, Xgc, ColorPxlv[num] );
	XFlush(Xd);
  }
}
/**********************************************************************/
void
SetGray( int lvl )
{
	XSetForeground(Xd, Xgc, GrayLvl[lvl+32]);
	XFlush(Xd);
}
/**********************************************************************/
void
SetOrgColor( int number )
{
	XSetForeground(Xd, Xgc, Xpx[number]);
	XFlush(Xd);
}

/**********************************************************************/
void
InitDefaultColor()
{
  int			i;
  char		str[10];
  XColor		c0, c1;

  GrayLvl = (Ulong*)MallocInX( UL(sizeof(Ulong)*160), "GrayLvl" );
  GrayLvl[ 27 ] = GetColorPxl( "aliceblue" );
  GrayLvl[ 28 ] = GetColorPxl( "blue" );
  GrayLvl[ 29 ] = GetColorPxl( "red" );
  GrayLvl[ 30 ] = GetColorPxl( "blue" );
  GrayLvl[ 31 ] = GetColorPxl( "LightSkyBlue" );

  for( i = 0; i <= 100; i++ ){
	sprintf( str, "gray%d", i );
	if( XAllocNamedColor(Xd, Xcm, str, &c0, &c1) == 0 )
	  GrayLvl[i + 32] = GrayLvl[i-1 + 32];
	else
	  GrayLvl[i + 32] = c1.pixel;
  }
  XFlush( Xd );
}
/**********************************************************************/
Ulong
GetColorPxl( char *c )
{
  XColor		c0, c1;
  Status		result;

  result = XAllocNamedColor(Xd, Xcm, c, &c0, &c1);
  if( result == 0 ) return BlackPixel(Xd,0);
  return c1.pixel;
}
/**********************************************************************/
void        ColorLoad( char* fname )
{
int         n, rgb, clr;
FILE        *fp;
XColor      *aclr;
Ulong       Xpl[1];
Ushort      *rgbfmt;

    fp = fopen( fname, "r" );
      if(fp==NULL){printf("cannot open %s",fname); exit(1); }
    {
        fscanf(fp,"%d", &clr );

        Xpx = (Ulong*)malloc( clr*sizeof(Ulong) );
          if(Xpx==NULL){printf("!!! fault: malloc Xpx\n");exit(1); }

        aclr = (XColor*)malloc( clr*sizeof(XColor) );
          if(aclr==NULL){printf("!!! fault: malloc aclr\n");exit(1); }

        rgbfmt = (Ushort*)malloc( clr*3*sizeof(Ushort) );
          if(Xpx==NULL){printf("!!! fault: malloc rgbfmt\n");exit(1); }

        for( n = 0; n < clr; n++ ){
          for( rgb = 0; rgb < 3; rgb++ ){
            fscanf(fp,"%x", &rgbfmt[ n*3 + rgb ] ); } }
    }
    fclose(fp);

    if( XAllocColorCells( Xd, Xcm, True, Xpl, 0, Xpx, clr ) );
    else{
      printf("fault alloc color cell \n"); exit(1); }

    for( n = 0; n < clr; n++ ){
      aclr[n].pixel = Xpx[n];
      aclr[n].flags = DoRed|DoGreen|DoBlue;
      aclr[n].red   = rgbfmt[ n*3 + 0 ];
      aclr[n].green = rgbfmt[ n*3 + 1 ];
      aclr[n].blue  = rgbfmt[ n*3 + 2 ];
    }

    XStoreColors( Xd, Xcm, aclr, clr );
    XFlush( Xd );
    free( aclr );
    free( rgbfmt );
}
/**********************************************************************/
int
KPress( int win )
{
  XEvent		ev;
  XKeyEvent		kev;
  KeySym		key;
  char			str[100];

  if( XCheckTypedWindowEvent(Xd,Xw[win],KeyPress,&ev) ){
	XLookupString( &kev, str, 100, &key, 0 );
	return( key );
  }
  return -1;
}
/**********************************************************************/
int
BPress( int win )
{
  XEvent		ev;

  if( XCheckTypedWindowEvent(Xd,Xw[win],ButtonPress,&ev) ){
	return( ev.xbutton.button );
  }
  if( XCheckTypedWindowEvent(Xd,Xw[win],ButtonRelease,&ev) ){
	return( -ev.xbutton.button );
  }
  return( 0 );
}
/**********************************************************************/
int
Button_press( int win, int *x, int *y )
{
  XEvent		ev;

  if( XCheckTypedWindowEvent(Xd,Xw[win],ButtonPress,&ev) ){
    *x = ev.xbutton.x;
    *y = ev.xbutton.y;
	return( ev.xbutton.button );
  }
  if( XCheckTypedWindowEvent(Xd,Xw[win],ButtonRelease,&ev) ){
    *x = ev.xbutton.x;
    *y = ev.xbutton.y;
	return( -ev.xbutton.button );
  }
  return( 0 );
}
/**********************************************************************/
int
WindowIO( int* type, int* x, int* y, int* state, int* code )
{
  int			i;
  XEvent		ev;
  
  XNextEvent( Xd, &ev );
  *type = ev.xany.type;
  *x = ev.xbutton.x;
  *y = ev.xbutton.y;
  *state = (int)ev.xkey.state;
  *code = (int)ev.xkey.keycode;
  
  for( i = 0; i < WinNum; i++ ){
	if( ev.xany.window == Xw[i] ) return( i );
  }
  return( -1 );
}
/**********************************************************************/
int
WindowExpose( int win )
{
  XEvent    e;

  if( XCheckTypedWindowEvent(Xd,Xw[win],Expose,&e ) )  return( 1 );
  return( 0 );
}
/**********************************************************************/
void		RaiseWindow( int win )
{
  XRaiseWindow( Xd, Xw[win] );
  XFlush(Xd);
}
/**********************************************************************/
void		SetSize( int win, int w, int h )
{
  XEvent		ev;

  if( UI(w) != WSW[win] || UI(h) != WSH[win] ){
	if( Xw[win] != 0 ){
	  XResizeWindow(Xd, Xw[win], UI(w), UI(h));
	  XFlush(Xd);
	  while( 1 ){
		XNextEvent( Xd, &ev );
		XFlush(Xd);
		if( ev.type == ConfigureNotify ) break;
	  }
	}
	WSW[win] = UI(w);
	WSH[win] = UI(h);
  }
}
/**********************************************************************/
void
SetPosition( int win, int x, int y )
{
  XEvent		ev;

  if( x != WPX[win] || y != WPY[win] ){
	if( Xw[win] != 0 ){
	  XMoveWindow( Xd, Xw[win], x, y );
	  XFlush( Xd );
	  while( 1 ){
		XNextEvent( Xd, &ev );
		XFlush(Xd);
		if( ev.type == ConfigureNotify ) break;
	  }
	}
	WPX[win] = x;
	WPY[win] = y;
  }
}
/**********************************************************************/
void
MoveWindow( int win )
{
  int			ox, oy;
  XEvent		ev;

  XNextEvent( Xd, &ev );
  if( ev.type == MotionNotify ){
	ox = ev.xbutton.x;
	oy = ev.xbutton.y;
	while( 1 ){
	  XNextEvent( Xd, &ev );
	  if( ev.type == ButtonRelease ){
		WPX[win] += ev.xbutton.x - ox;
		WPY[win] += ev.xbutton.y - oy;
		SetPosition( win, WPX[win], WPY[win] );
		break;
	  }
	}
  }
}
/**********************************************************************/
void
SetCursor( int win, int shapenum )
{
  XFreeCursor( Xd, Xcu[win] );
  Xcu[win] = XCreateFontCursor(Xd, shapenum);
  XDefineCursor( Xd, Xw[win], Xcu[win] );
  XFlush(Xd);
}
/**********************************************************************/
/*
void
SetCursorColor( int win, int fore, int back )
{
XColor		cf, cb, cc;

	XAllocNamedColor(Xd, Xcm, fore, &cf, &cc);
	XAllocNamedColor(Xd, Xcm, back, &cb, &cc);
	XRecolorCursor( Xd, Xcu[win], &cf, &cb );
	XFlush(Xd);
}
*/
/**********************************************************************/
void
SetPointer( int win, int x, int y )
{
  XWarpPointer( Xd, None, Xw[win], 0, 0, 0, 0, x, y );
  XFlush(Xd);
}
/**********************************************************************/
void
CreateImage( int win, int w, int h )
{
  Visual		*Xv;
  Uint			Xdp;

  Xv = DefaultVisual( Xd, DefaultScreen( Xd ) );
  Xdp = DefaultDepth( Xd, DefaultScreen( Xd ) );

  if( Ximg[win] != NULL){
	if( Ximg[win] != NULL ) free( Ximg[win]->data );
	Ximg[win]->data = NULL;
	XDestroyImage( Ximg[win] );
	Ximg[win] = NULL;
  }
  Ximg[win] = XCreateImage(Xd,Xv,Xdp,ZPixmap, 0, 0, UL(w), UL(h), 8, 0);
  if( Ximg[win] == NULL ){

	printf("!!! fault: XCreateImage\n");
	exit(1);
  }
  Ximg[win]->data = (char*)malloc( Ximg[win]->bytes_per_line*UL(h) );
  if( Ximg[win]->data == NULL ){
	printf("\nxgra5::CreateImage: not enough memory\n\n");
	exit(1);
  }
}
/**********************************************************************/
void
PutImage( int win )
{
  int		ew, eh;

  ew = Ximg[win]->width;
  eh = Ximg[win]->height;

  XPutImage( Xd, Xw[win], Xgc, Ximg[win], 0, 0, 0, 0, ew, eh );
  XFlush( Xd );
}
/**********************************************************************/
void
SetImageData( int win, char* img )
{
  long		i, item;
  char		*img_data;

  GrayLvl += 32;
  img_data = Ximg[win]->data;
  item = Ximg[win]->width * Ximg[win]->height;
  for( i = 0; i < item; i++ ){
	img_data[i] = GrayLvl[ img[i] ]; 
  }
  GrayLvl -= 32;
}
/**********************************************************************/
char*
MallocInX( unsigned long size, char* msg )
{
  char		*pntr;

  pntr = (char *)malloc( (unsigned)size );
  if( pntr == NULL ){
	fprintf( stderr, "!!! Fault: malloc- %s\n", msg);
	exit(1);
  }
  return( pntr );
}
/**********************************************************************/
void
EndWindow()
{
  int			w;


  for( w = 0; w < FontNum; w++ ){
	XFreeFont( Xd, Xfnt[w] );
  }
  for( w = 0; w < ColorNum; w++ ){
	if( ColorName[w] != NULL ) free( ColorName[w] );
  }

  XFreeColormap( Xd, Xcm );
  XFreeGC( Xd, Xgc );
  for( w = 0; w < WinNum; w++ ){
	if( Ximg[w] != NULL ){
	  free( Ximg[w]->data );
	  XDestroyImage( Ximg[w] );
	}
	if( Xw[w] != 0 ){
	  XDestroyWindow( Xd, Xw[w] );
	  XFreeCursor( Xd, Xcu[w] );
	}
	if( WindowName[w] != NULL ) free( WindowName[w] );
  }
  free( Xw );
  free( Xpw );
  free( Xcu );
  free( WPX );
  free( WPY );
  free( WSW );
  free( WSH );
  free( Xcx );
  free( Xcy );
  free( Xwmng );
  free( Xbacking );
  free( Ximg );
  free( WindowName );
  free( ChkOpen );
}





/******************************************************************************
		Save Pixmap on File (from Sato's xgra collection)
******************************************************************************/

void SaveImageInXWD( int win , char *filename )
{
  XWDFileHeader fhead;
  Visual *visual = DefaultVisual(Xd, 0);
  int ncolors = visual->map_entries;
  XImage *ximage;
  FILE *fpw;
  XColor xcolor;
  Colormap cmap = DefaultColormap( Xd, 0 );

  ximage = XGetImage( Xd , Xw[win] , 0 , 0 , WSW[win] , WSH[win] ,
             AllPlanes , ZPixmap);
  fhead.header_size = (CARD32)htonl(sizeof(fhead)+strlen(filename)+1);
  fhead.file_version = (CARD32)htonl( (CARD32)XWD_FILE_VERSION );


  fhead.pixmap_format = (CARD32)htonl(ximage->format);
  fhead.pixmap_depth = (CARD32)htonl(ximage->depth);
  fhead.pixmap_width = (CARD32)htonl(ximage->width);
  fhead.pixmap_height = (CARD32)htonl(ximage->height);
  fhead.xoffset = (CARD32)htonl(ximage->xoffset);
  fhead.byte_order = (CARD32)htonl(ximage->byte_order);
  fhead.bitmap_unit = (CARD32)htonl(ximage->bitmap_unit);
  fhead.bitmap_bit_order = (CARD32)htonl(ximage->bitmap_bit_order);
  fhead.bitmap_pad = (CARD32)htonl(ximage->bitmap_pad);
  fhead.bits_per_pixel = (CARD32)htonl(ximage->bits_per_pixel);
  fhead.bytes_per_line = (CARD32)htonl(ximage->bytes_per_line);
  fhead.visual_class = (CARD32)htonl(visual->c_class);
  fhead.red_mask = (CARD32)htonl(visual->red_mask);
  fhead.green_mask = (CARD32)htonl(visual->green_mask);
  fhead.blue_mask = (CARD32)htonl(visual->blue_mask);
  fhead.bits_per_rgb = (CARD32)htonl(visual->bits_per_rgb);
  fhead.colormap_entries = (CARD32)htonl(ncolors);
  fhead.ncolors = (CARD32)htonl(ncolors);

  fhead.window_width = (CARD32)htonl(ximage->width);
  fhead.window_height = (CARD32)htonl(ximage->height);
  fhead.window_x = (CARD32)htonl(0);
  fhead.window_y = (CARD32)htonl(0);
  fhead.window_bdrwidth = (CARD32)htonl(0);


  if( ( fpw = fopen( filename,"wb") ) == NULL ){
    printf("SaveImageInXWD : Can't open -- %s --!!\n",filename);
    exit(1);
  }

  fwrite( &fhead , sizeof(fhead) , 1 , fpw );
  fwrite( filename , strlen(filename)+1 , 1 , fpw );

  for( int i = 0 ; i < visual->map_entries ; i++ ){
    xcolor.pixel = i | i << 8 | i << 16;
    xcolor.pad = 0;
    xcolor.flags = DoRed | DoGreen | DoBlue;
    XQueryColor( Xd , cmap , &xcolor );
    fwrite( &xcolor, sizeof(xcolor), 1 , fpw );
  }
  fwrite( ximage->data , ximage->bytes_per_line * WSH[win] , 1 , fpw );
  fclose(fpw);

  XDestroyImage( ximage );


}


/******************************************************************************
		Write Pixmap on File
******************************************************************************/

void	WritePixmap(int win, FILE *fp) {

/*** beginning of exchange with Sato's xgra ***/
	
/**
	XWDFileHeader	fhead;
//	XColor		xcolor;
	XColor		*xcolor;
	Visual		*visual = DefaultVisual(Xd, 0);
	int			ncolors = visual->map_entries;
	XImage		*ximage;
	Colormap	cmap = DefaultColormap( Xd, 0 );
	
	
	ximage = XGetImage(Xd, Xp[win], 0, 0, 
				WSW[win], WSH[win], AllPlanes , ZPixmap);
	
	fhead.header_size = (CARD32)htonl(sizeof(fhead));
	fhead.file_version = (CARD32)htonl((CARD32)XWD_FILE_VERSION);
	
	fhead.pixmap_format = (CARD32)htonl(ximage->format);
	fhead.pixmap_depth = (CARD32)htonl(ximage->depth);
	fhead.pixmap_width = (CARD32)htonl(ximage->width);
	fhead.pixmap_height = (CARD32)htonl(ximage->height);
	fhead.xoffset = (CARD32)htonl(ximage->xoffset);
	fhead.byte_order = (CARD32)htonl(ximage->byte_order);
	fhead.bitmap_unit = (CARD32)htonl(ximage->bitmap_unit);
	fhead.bitmap_bit_order = (CARD32)htonl(ximage->bitmap_bit_order);
	fhead.bitmap_pad = (CARD32)htonl(ximage->bitmap_pad);
	fhead.bits_per_pixel = (CARD32)htonl(ximage->bits_per_pixel);
	fhead.bytes_per_line = (CARD32)htonl(ximage->bytes_per_line);
	fhead.visual_class = (CARD32)htonl(visual->c_class);
	fhead.red_mask = (CARD32)htonl(visual->red_mask);
	fhead.green_mask = (CARD32)htonl(visual->green_mask);
	fhead.blue_mask = (CARD32)htonl(visual->blue_mask);
	fhead.bits_per_rgb = (CARD32)htonl(visual->bits_per_rgb);
	fhead.colormap_entries = (CARD32)htonl(ncolors);
	fhead.ncolors = (CARD32)htonl(ncolors);
	
	fhead.window_width = (CARD32)htonl(ximage->width);
	fhead.window_height = (CARD32)htonl(ximage->height);
	fhead.window_x = (CARD32)htonl(0);
	fhead.window_y = (CARD32)htonl(0);
	fhead.window_bdrwidth = (CARD32)htonl(0);
	
	fwrite(&fhead, sizeof(fhead), 1, fpw);
	
	for(int i = 0; i < visual->map_entries; i++) {
		xcolor.pixel = i | i << 8 | i << 16;
		xcolor.pad = 0;
		xcolor.flags = DoRed | DoGreen | DoBlue;
		XQueryColor(Xd, cmap, &xcolor);
		fwrite(&xcolor, sizeof(xcolor), 1, fpw);
	}
	
	xcolor = (XColor *)malloc(sizeof(XColor) * ncolors);
	for (int i = 0; visual->map_entries > i; i++) {
		xcolor[i].pixel = i;
	}
	XQueryColors(Xd, DefaultColormap(Xd, 0), xcolor, ncolors);
	
	fwrite(&fhead, fhead.header_size, 1, fpw);
	fwrite(xcolor, sizeof(XColor), ncolors, fpw);
	fwrite(ximage->data, ximage->bytes_per_line * WSH[win], 1, fpw);
	
	XDestroyImage( ximage );
**/
	
	
	
/*** end of exchange with Sato's xgra ***/

	XWDFileHeader	head;
	XColor			*colors;
	Visual			*visual = DefaultVisual(Xd, 0);
	int				i, ncolors = visual->map_entries;
	XImage			*ximage = XGetImage(Xd, Xp[win], 0, 0, 
						WSW[win], WSH[win], AllPlanes, ZPixmap);
	
	
printf("01\n");
	head.header_size = sizeof(XWDFileHeader);
printf("02\n");
	head.file_version = XWD_FILE_VERSION;
printf("03\n");
	head.pixmap_format = ximage->format;
printf("04\n");
	head.pixmap_depth = ximage->depth;
printf("05\n");
	head.pixmap_width = ximage->width;
printf("06\n");
	head.pixmap_height = ximage->height;
printf("07\n");
	head.xoffset = ximage->xoffset;
printf("08\n");
	head.byte_order = ximage->byte_order;
printf("09\n");
	head.bitmap_unit = ximage->bitmap_unit;
printf("0a\n");
	head.bitmap_bit_order = ximage->bitmap_bit_order;
printf("0b\n");
	head.bitmap_pad = ximage->bitmap_pad;
printf("0c\n");
	head.bits_per_pixel = ximage->bits_per_pixel;
printf("0c\n");
	head.bytes_per_line = ximage->bytes_per_line;
printf("0d\n");
	head.visual_class = visual->c_class;
printf("0e\n");
	head.red_mask = visual->red_mask;
printf("0f\n");
	head.green_mask = visual->green_mask;
printf("0g\n");
	head.blue_mask = visual->blue_mask;
printf("0h\n");
	head.bits_per_rgb = visual->bits_per_rgb;
printf("0i\n");
	head.colormap_entries = head.ncolors = ncolors;
	
printf("11\n");
	colors = (XColor *)malloc(sizeof(XColor) * ncolors);
printf("12\n");
	for (i = 0; visual->map_entries > i; i++) {
printf("13\n");
		colors[i].pixel = i;
printf("14\n");
	}
printf("15\n");
printf("ncolors = %d\n", ncolors);
	XQueryColors(Xd, DefaultColormap(Xd, 0), colors, ncolors);
printf("16\n");
	
printf("17\n");
	fwrite(&head, head.header_size, 1, fp);
printf("18\n");
	fwrite(colors, sizeof(XColor), ncolors, fp);
printf("19\n");
	fwrite(ximage->data, ximage->bytes_per_line * WSH[win], 1, fp);
printf("1a\n");
}


/******************************************************************************
		Write Dummy Pixmap on File
******************************************************************************/

/***
void	WriteDummyPixmap(FILE *fp) {
	XWDFileHeader	head;
	XColor			*colors;
	Visual			*visual = DefaultVisual(Xd, 0);
	int				i, ncolors = visual->map_entries;
	XImage			*ximage = XGetImage(Xd, Xpdummy, 0, 0, 
						WSDummyW, WSDummyH, AllPlanes, ZPixmap);
	
	
	head.header_size = sizeof(XWDFileHeader);
	head.file_version = XWD_FILE_VERSION;
	head.pixmap_format = ximage->format;
	head.pixmap_depth = ximage->depth;
	head.pixmap_width = ximage->width;
	head.pixmap_height = ximage->height;
	head.xoffset = ximage->xoffset;
	head.byte_order = ximage->byte_order;
	head.bitmap_unit = ximage->bitmap_unit;
	head.bitmap_bit_order = ximage->bitmap_bit_order;
	head.bitmap_pad = ximage->bitmap_pad;
	head.bits_per_pixel = ximage->bits_per_pixel;
	head.bytes_per_line = ximage->bytes_per_line;
	head.visual_class = visual->class;
	head.red_mask = visual->red_mask;
	head.green_mask = visual->green_mask;
	head.blue_mask = visual->blue_mask;
	head.bits_per_rgb = visual->bits_per_rgb;
	head.colormap_entries = head.ncolors = ncolors;
	
	colors = (XColor *)malloc(sizeof(XColor) * ncolors);
	for (i = 0; visual->map_entries > i; i++) {
		colors[i].pixel = i;
	}
	XQueryColors(Xd, DefaultColormap(Xd, 0), colors, ncolors);
	
	fwrite(head, head.header_size, 1, fp);
	fwrite(colors, sizeof(XColor), ncolors, fp);
	fwrite(ximage->data, ximage->bytes_per_line * WSDummyH, 1, fp);
}
***/


/******************************************************************************
		Write Window Image on File
******************************************************************************/

/***
void	WriteImage(int win, FILE *fp) {
	XWDFileHeader	head;
	XColor			*colors;
	Visual			*visual = DefaultVisual(Xd, 0);
	int				i, ncolors = visual->map_entries;
	XImage			*ximage = XGetImage(Xd, Xw[win], 0, 0, 
						WSW[win], WSH[win], AllPlanes, ZPixmap);
	
	
	head.header_size = sizeof(XWDFileHeader);
	head.file_version = XWD_FILE_VERSION;
	head.pixmap_format = ximage->format;
	head.pixmap_depth = ximage->depth;
	head.pixmap_width = ximage->width;
	head.pixmap_height = ximage->height;
	head.xoffset = ximage->xoffset;
	head.byte_order = ximage->byte_order;
	head.bitmap_unit = ximage->bitmap_unit;
	head.bitmap_bit_order = ximage->bitmap_bit_order;
	head.bitmap_pad = ximage->bitmap_pad;
	head.bits_per_pixel = ximage->bits_per_pixel;
	head.bytes_per_line = ximage->bytes_per_line;
	head.visual_class = visual->class;
	head.red_mask = visual->red_mask;
	head.green_mask = visual->green_mask;
	head.blue_mask = visual->blue_mask;
	head.bits_per_rgb = visual->bits_per_rgb;
	head.colormap_entries = head.ncolors = ncolors;
	
	colors = (XColor *)malloc(sizeof(XColor) * ncolors);
	for (i = 0; visual->map_entries > i; i++) {
		colors[i].pixel = i;
	}
	XQueryColors(Xd, DefaultColormap(Xd, 0), colors, ncolors);
	
	fwrite(head, head.header_size, 1, fp);
	fwrite(colors, sizeof(XColor), ncolors, fp);
	fwrite(ximage->data, ximage->bytes_per_line * WSH[win], 1, fp);
}
***/


/******************************************************************************
		Create Pixmap of Existing Window
******************************************************************************/

void	CreatePixmap(int win, int w, int h) {
	Xp[win] = XCreatePixmap(Xd, Xw[win], w, h, DefaultDepth(Xd, 0));
}


/******************************************************************************
		Create Dummy Pixmap of Invisible Window
******************************************************************************/

void	CreateDummyPixmap(int w, int h) {
	Xwdummy = XCreateSimpleWindow(Xd, RootWindow(Xd, 0), 0, 0, w, h, 
				2, Xblk, Xwht);
	XMapWindow(Xd, Xwdummy);
	
	Xpdummy = XCreatePixmap(Xd, Xwdummy, w, h, DefaultDepth(Xd, 0));
	
	WSDummyW = w;
	WSDummyH = h;
}


/******************************************************************************
		Draw Object on Pixmap
******************************************************************************/

void	RectanglePixmap(int win, int x, int y, int w, int h) {
	XDrawRectangle(Xd, Xp[win], Xgc, x, y, UI(w), UI(h));
}


void	PutRectanglePixmap(int win, int x, int y, int w, int h) {
	XFillRectangle(Xd, Xp[win], Xgc, x, y, UI(w), UI(h));
//	XFlush(Xd);
}


void	PutRectangleDummyPixmap(int x, int y, int w, int h) {
	XFillRectangle(Xd, Xpdummy, Xgc, x, y, UI(w), UI(h));
}


void	CirclePixmap(int win, int x, int y, int r) {
	XDrawArc(Xd, Xp[win], Xgc, x - r, y - r, UI(r * 2), UI(r * 2), 
		I(0), I(360 * 64));
}


void	PutCirclePixmap(int win, int x, int y, int r) {
	XFillArc(Xd, Xp[win], Xgc, x - r, y - r, UI(r * 2), UI(r * 2), 
		I(0), I(360 * 64));
}


/******************************************************************************
		Copy Memory on Pixmap to Window
******************************************************************************/

void	CopyPixmap2Window(int win, int w, int h) {
	XCopyArea(Xd, Xp[win], Xw[win], Xgc, 0, 0, w, h, 0, 0);
}


/******************************************************************************
		Compulasory Flush
******************************************************************************/

void	CompulsoryFlush(void) {
	XFlush(Xd);
}


/******************************************************************************
		Set Pixel on Pixmap
******************************************************************************/

void	SetPixelPixmap(int win, int x, int y) {
	XDrawPoint(Xd, Xp[win], Xgc, x, y);
	Xcx[win] = x;
	Xcy[win] = y;
}



