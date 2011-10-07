#define NAME "giginet"       //please change to your name
#define GROUP "complex"             //please change to your group

// don't change follows!!
//-------------------------------------------------------------------------------------
#define AGENT 500             //$B%(!<%8%'%s%H$N?t(B
#define NUM_METABOLISM  12    //$B%(!<%8%'%s%H$NBe<UNL$N:GBgCM(B
#define INIT_SUGAR  5000      //$B%(!<%8%'%s%H$N=i4|:=E|J]M-NL(B
#define LIFE_MAX    10000      //$B%(!<%8%'%s%H$N<wL?$N:GBgCM(B
#define VISION_MAX 30         //$B%(!<%8%'%s%H$N;kNO$N:GBgCM(B
#define ACQUISITION_MAX 200   //$B%(!<%8%'%s%H$N:=E|3MF@G=NO$N:GBgCM(B
#define ACQUISITION_MIN 120   //$B%(!<%8%'%s%H$N:=E|3MF@G=NO$N:G>.CM(B
#define NEIGHBOR 1            //$B%(!<%8%'%s%H$N6aK5(B
#define NUM_NEIGHBOR 4        //$B%(!<%8%'%s%H$N6aK5?t(B
#define NUM_ACTION   5        //$B%(!<%8%'%s%H$N9TF0?t(B
#define HUNGER_LEVEL 0.1      //$B%(!<%8%'%s%H$N522n>uBV(B

#define SUGAR_MAX 5           //$B:=E|$N=i4|$N:GBgCM(B 
#define NUM_SUGAR_LEVEL 5     //$B:=E|$NCJ3,?t(B
#define SUGAR_RADIUS 8        //$B:=E|$NJ,I[H>7B(B
#define GROWBACK_MAX 20        //$B:=E|$N:F@8NL$N:GBgCM(B
#define CAPACITY_MAX 300       //$B:=E|$N:GBgMFNL$N:GBgCM(B
#define SUGAR_SEASON  500      //$B:=E|$N0\F04|4V(B
 
#define X 50                  //X$B:BI8$N:GBgCM(B
#define Y 50                  //Y$B:BI8$N:GBgCM(B
#define TMAX 4000             //$B7+$jJV$72s?t(B

#define BACKCOLOR "white"

//-------------------------------------------------------------------------------------
struct Status {
  int agent_x[AGENT],agent_y[AGENT]; //$B%(!<%8%'%s%H$N0LCV(B x$B:BI8$H(By$B:BI8(B
  int agent_vision[AGENT];           //$B%(!<%8%'%s%H$N;kNO(B
  int agent_metabolism[AGENT];       //$B%(!<%8%'%s%H$NBe<UNL(B
  int agent_acquisition[AGENT];      //$B%(!<%8%'%s%H$N3MF@NL(B
  int agent_sugar[AGENT];            //$B%(!<%8%'%s%H$N:=E|$NJ]M-NL(B
  int agent_age[AGENT];              //$B%(!<%8%'%s%H$NG/Np(B
  int agent_life_span[AGENT];        //$B%(!<%8%'%s%H$N<wL?(B 
  int agent_act[AGENT];              //$B%(!<%8%'%s%H$N9TF0(B
  int agent_life[AGENT];        //$B%(!<%8%'%s%H$N@8;`(B $B@8(B:1 $B;`(B:0

  int E[X+2][Y+2];                   //$B:BI8(B(x,y)$B$N>uBV(B $B%(!<%8%'%s%H$NM-L5(B $BM-(B:1 $BL5(B:0 

  int sugar_current[X+2][Y+2];       //$B:=E|$N8=:_NL(B
  int sugar_growback[X+2][Y+2];      //$B:=E|$N:F@8NL(B
  int sugar_full_capacity[X+2][Y+2]; //$B:=E|$N:GBgMFNL(B
};
