/*-*-c++-*-*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "define.h"
#include "xgra5-3.h"
#define SEED 1
#define QUIET 0
//#include "action.c"

int lastcolor[X+3][Y+3];

int action(int *sugar_neighbor_info,int *others_neighbor_info,int sugar,int met, int vis, int acq);

int sgn(int i){
  return ((i>0)-(i<0));
}

double sigmoid(double x){
  return (1.0/(1.0+exp(-x)));
}

void swap(double *a, double *b){
  double w;
  w=*a;
  *a=*b;
  *b=w;
}

void init_seed(void){
  int seed;
  if(SEED){
    printf("randomize seed:#");
    scanf("%d",&seed);
  }else{
    seed = SEED;
  }
  srand(seed);
}

void q_sort(int *a, int n)
{
  int i,j,k;
  int hold, pivot;
  
  i=0;
  j=n-1;
  pivot = a[(i+j)/2];
  
  while(1)
    {
      while(a[i] < pivot)
	{
	  i++;
	}
      while(a[j] > pivot)
	{
	  j--;
	}
      if(i>=j)
	{ 
	  break;
	}

      hold= a[i];
      a[i]= a[j];
      a[j]= hold;
      i++;
      j--; 
    }
  
  if(i-1>0)
    { 
      q_sort(a,i);      
    }

  if(j+2<n)
    {
      q_sort(a+j+1,n-j-1);      
    }
}




//-------------------------------------------------------------------------------------
class Agent 
{
public:
  Agent(void);
  int sugar_info(int,int, int, int, struct Status *);
  int others_info(int,int, int, int, struct Status *);  
  void decision_making(int, int, int, struct Status *, int *);
private:
  
};

Agent::Agent(void)
{
 
}

int Agent::sugar_info(int agent,int x,int y,int i, struct Status *s)
{ 
  //近傍の現在量が現在値よりも多ければ 1，少なければ 0

  int vision_front[VISION_MAX],vision_back[VISION_MAX];
  int vision_left[VISION_MAX],vision_right[VISION_MAX];
  int j,k;
  int flg;

  for(j=0;j<VISION_MAX;j++)
    {
      vision_front[j]=0;
      vision_back[j] =0;
      vision_left[j] =0;
      vision_right[j]=0;
    }
      
  flg=0;

  if(i==0) //前
    {       
      for(j=0;j< s->agent_vision[agent];j++)
	{

	  //	  vision_front[j]=( y-(j+1) >= 0 ? y-(j+1):Y-(j+1) );
	  vision_front[j]= ( y-(j+1) + Y ) % Y;
	}

      for(j=0;j< s->agent_vision[agent];j++)
	{
	  if(s->sugar_current[x][ vision_front[j] ]  > s->sugar_current[x][y] )
	    {
	      flg=1;
	    }  
	}
      return flg; 
    }

  if(i==1) //後
    {       
      for(j=0;j< s->agent_vision[agent];j++)
	{
	  //	  vision_back[j]=( y+(j+1) < Y ? y+(j+1): y+j+1-Y ); 
	  vision_back[j] = ( y+(j+1) + Y ) % Y;
	}
      
      for(j=0;j< s->agent_vision[agent];j++)
	{
	  if(s->sugar_current[x][ vision_back[j] ]  > s->sugar_current[x][y] )
	    {
	      flg=1;
	    }  
	}
      return flg; 
    }
  
  if(i==2) //左
    {       
      for(j=0;j< s->agent_vision[agent];j++)
	{
	  //	  vision_left[j]=( x-(j+1) >= 0 ? x-(j+1): Y-(j+1) );     
	  vision_left[j] = ( x-(j+1) + X ) % X;
	}
  
      for(j=0;j< s->agent_vision[agent];j++)
	{
	  if(s->sugar_current[ vision_left[j] ][y]  > s->sugar_current[x][y] )
	    {
	      flg=1;
	    }  
	}
      return flg; 
    }

  if(i==3) //右
    {       
      for(j=0;j< s->agent_vision[agent];j++)
	{
	  //	  vision_right[j]=( x+(j+1) < X ? x+(j+1): x+j+1-X );     
	  vision_right[j] = (x+(j+1) + X) % X;
	}
      
      for(j=0;j< s->agent_vision[agent];j++)
	{
	  if(s->sugar_current[ vision_right[j] ][y]  > s->sugar_current[x][y] )
	    {
	      flg=1;
	    }  
	}
      return flg; 
    }
  
  /*
  printf("\n%d,%d\n",x,y); 
  for(j=0;j<VISION_MAX;j++)
    {
      printf("%d\t", vision_front[j]); 
      printf("%d\t", vision_back[j]); 
      printf("%d\t", vision_left[j]); 
      printf("%d\t", vision_right[j]);
      printf("\n");
    }
  scanf("%d",&j); printf("\n"); 
  
  return flg; 
  */

}

int Agent::others_info(int agent,int x,int y,int i, struct Status *s)
{ 

  //近傍にエージェントがいれば 1，いなければ 0

  int vision_front[NEIGHBOR],vision_back[NEIGHBOR];
  int vision_left[NEIGHBOR],vision_right[NEIGHBOR];
  int j,k;
  int flg;

  for(j=0;j<NEIGHBOR;j++)
    {
      vision_front[j]=0;
      vision_back[j] =0;
      vision_left[j] =0;
      vision_right[j]=0;
    }
      
  flg=0;

  if(i==0) //前
    {       
      for(j=0;j< NEIGHBOR;j++)
	{
	  //	  vision_front[j]=( y-(j+1) >= 0 ? y-(j+1):Y-(j+1) );
	  vision_front[j]=( y-(j+1) + Y ) % Y;
	}

      for(j=0;j<NEIGHBOR;j++)
	{
	  if(s->E[x][vision_front[j]]!=0 )
	    {
	      flg=1;
	    }  
	}
      return flg; 
    }

  if(i==1) //後
    {       
      for(j=0;j< NEIGHBOR;j++)
	{
	  //	  vision_back[j]=( y+(j+1) < Y ? y+(j+1): y+j+1-Y ); 
	  vision_back[j]=( y+(j+1) + Y ) % Y;
	}
      
      for(j=0;j<NEIGHBOR;j++)
	{
	  if(s->E[x][ vision_back[j] ]!=0 )
	    {
	      flg=1;
	    }  
	}
      return flg; 
    }
  
  if(i==2) //左
    {       
      for(j=0;j< NEIGHBOR;j++)
	{
	  //	  vision_left[j]=( x-(j+1) >= 0 ? x-(j+1): Y-(j+1) );     
	  vision_left[j] = ( x-(j+1) + X ) % X;
	}
  
      for(j=0;j<NEIGHBOR;j++)
	{
	  if(s->E[ vision_left[j] ][y]!=0 )
	    {
	      flg=1;
	    }  
	}
      return flg; 
    }

  if(i==3) //右
    {       
      for(j=0;j< NEIGHBOR;j++)
	{
	  //	  vision_right[j]=( x+(j+1) < X ? x+(j+1): x+j+1-X );     
	  vision_right[j]=( x+(j+1) + X ) % X;
	}
      
      for(j=0;j< NEIGHBOR;j++)
	{
	  if(s->E[ vision_right[j] ][y]!=0 )
	    {
	      flg=1;
	    }  
	}
      return flg; 
    }
  
}

void Agent::decision_making(int agent, int x,int y,struct Status *s,int *a)
{ 
 
  int i,j,k;

  int sugar_neighbor_info[NUM_NEIGHBOR];
  int others_neighbor_info[NUM_NEIGHBOR];
  
  //環境から視覚情報を算出
  for(i=0;i<NUM_NEIGHBOR;i++)
    { 
      // 0:上 1:下 2:左 3:右
      
      sugar_neighbor_info[i] = sugar_info(agent,x,y,i,s);
      others_neighbor_info[i]=  others_info(agent,x,y,i,s);
    }  
  
  //視覚情報と自分の状態から行動を決定
  //action 0:上 1:下 2:左 3:右 4:停止 
  
  *a = action(sugar_neighbor_info,others_neighbor_info,s->agent_sugar[agent],s->agent_metabolism[agent], s->agent_vision[agent],s->agent_acquisition[agent]);
  
}


//-------------------------------------------------------------------------------------
class Environment {
public:
  struct Status status;
  Environment(void);
  void fight(void);
  void evaluation(int,FILE *);
  void total_evaluation(void);
private: 
  void initializer(void);

  int pre_E[X+3][Y+3];
  void draw_window();
  void set_window();           //エージェントと砂糖の画面表示
  
  void agents_action(void);           //エージェントの行動決定  
  void update_status(int, int , int , struct Status *, int); //エージェントの情報の更新

  void regenerate_sugar(int);  //砂糖の再生    
  void update_agents(void);    //エージェントの死亡と補充

  void init_sugar(int, int);         //砂糖の初期量の決定  
  void init_growback(int, int);      //砂糖の初期再生量の決定  
  void init_full_capacity(int, int); //砂糖の初期最大容量の決定  

  int init_vision(int);          //エージェントの視力の決定  
  int init_metabolism(int);      //エージェントの代謝量の決定   
  int init_acquisition(int);      //エージェントの砂糖獲得能力の決定   
  int init_agent_sugar(int);     //エージェントの砂糖の初期保有量の決定  
  int init_life_span(int);       //エージェントの寿命の決定  
};

Environment::Environment(void)
{
}
  
void Environment::init_sugar(int center_x, int center_y)        //砂糖の初期量  
{

  double distance;
  int sugar;
  int i;
  int x,y;
  
  for(x=0;x<X;x++)
    {
      for(y=0;y<Y;y++)
	{
	  distance = sqrt( pow(x-center_x,2.0) + pow(y-center_y,2.0) );
	  
	  for(i=0; i<NUM_SUGAR_LEVEL-1; i++)
	    {
	      if( ( ( (double)SUGAR_RADIUS/(NUM_SUGAR_LEVEL-1) )*i < distance ) && 
		  ( distance <= ( (double)SUGAR_RADIUS/(NUM_SUGAR_LEVEL-1) )*(i+1) ) )
		{
		  status.sugar_current[x][y] = 
		    (int)( (double)SUGAR_MAX/(NUM_SUGAR_LEVEL-1) * (NUM_SUGAR_LEVEL-(i+1) ) ) ;	  
		} 
	    }
	  
	  if( distance == 0 )
	    {
	      status.sugar_current[x][y]= SUGAR_MAX;	  
	    }
	  
	  if( distance > SUGAR_RADIUS )
	    {
	      status.sugar_current[x][y] = 0;	  
	    }
	}
    }

}  
 
void Environment::init_growback(int center_x, int center_y)      //砂糖の再生量
{

  double distance;
  int growback;
  int i;
  int x,y;
   
  for(x=0;x<X;x++)
    {
      for(y=0;y<Y;y++)
	{
	  distance = sqrt( pow(x-center_x,2.0) + pow(y-center_y,2.0) );
	 
	  for(i=0; i<NUM_SUGAR_LEVEL-1; i++)
	    {
	      if( ( ( (double)SUGAR_RADIUS/(NUM_SUGAR_LEVEL-1) )*i < distance ) && 
		  ( distance <= ( (double)SUGAR_RADIUS/(NUM_SUGAR_LEVEL-1) )*(i+1) ) )
		{
		  status.sugar_growback[x][y] = 
		    (int)( (double)GROWBACK_MAX/(NUM_SUGAR_LEVEL-1)*(NUM_SUGAR_LEVEL-(i+1)) ) ;
		} 
	    }
	  
	  if( distance == 0 )
	    {
	      status.sugar_growback[x][y]= GROWBACK_MAX;	  
	    }
	  
	  if( distance > SUGAR_RADIUS )
	    {
	      status.sugar_growback[x][y]= 0;	  
	    }
	  if (status.sugar_growback[x][y]<0) status.sugar_growback[x][y]=0;
	}
    }

}  
 
void Environment::init_full_capacity(int center_x, int center_y) //砂糖の最大容量
{
  double distance;
  int full_capacity;
  int i;
  int x,y;
   
  for(x=0;x<X;x++)
    {
      for(y=0;y<Y;y++)
	{
	  distance = sqrt( pow(x-center_x,2.0) + pow(y-center_y,2.0) );
	 	
	  for(i=0; i<NUM_SUGAR_LEVEL-1; i++)
	    {
	      if( ( ( (double)SUGAR_RADIUS/(NUM_SUGAR_LEVEL-1) )*i < distance ) && 
		  ( distance <= ( (double)SUGAR_RADIUS/(NUM_SUGAR_LEVEL-1) )*(i+1) ) )
		{
		  status.sugar_full_capacity[x][y] = 
		    (int)( (double)CAPACITY_MAX/(NUM_SUGAR_LEVEL-1) * (NUM_SUGAR_LEVEL-(i+1)) ) ;	  
		} 
	    }
	  
	  if( distance == 0 )
	    {
	       status.sugar_full_capacity[x][y] = CAPACITY_MAX;	  
	    }
	  
	  if( distance > SUGAR_RADIUS )
	    {
	       status.sugar_full_capacity[x][y] = 0;	  
	    }
	}
    }

}
  
int Environment::init_vision(int i)        //エージェントの視力
{ 

  return rand()%VISION_MAX + 1;

} 

int Environment::init_metabolism(int i)      //エージェントの代謝量  
{

  return rand()%NUM_METABOLISM+1;

}

int Environment::init_acquisition(int i)      //エージェントの代謝量  
{
  int acquisition;
  
  acquisition =  ACQUISITION_MIN + rand()%( ACQUISITION_MAX - ACQUISITION_MIN );
  
  return acquisition;

}

int Environment::init_agent_sugar(int i)     //エージェントの砂糖の初期保有量
{

  return  INIT_SUGAR;

}  

int Environment::init_life_span(int i)      //エージェントの寿命
{

  return LIFE_MAX;

}

void Environment::initializer(void)
{
  int i,j,k,x,y,xx,yy;
  int center_x,center_y;
  
  // clear E
  for(x=0;x<X+2;x++)
    for(y=0;y<Y+2;y++)
      {
	status.E[x][y]=0;
	pre_E[x][y]=0;
      }
  
  //set sugar  
  center_x= (int)(X/2);
  center_y= (int)(Y/2);
  
  init_sugar(center_x,center_y);         //砂糖の初期量  
  init_growback(center_x,center_y);      //砂糖の初期再生量
  init_full_capacity(center_x,center_y); //砂糖の初期最大容量
     

  /*
  for(y=0;y<Y;y++)
    { 
      printf("%d\t",y);
      for(x=0;x<X;x++)
	{
	  printf("%d",status.sugar_growback[x][y]);     
	}
      printf("\n");
    }
  */

  //set agents
  for(i=0;i<AGENT;i++)//エージェントの初期位置
    {
      do 
	{
	  x=rand()%X;
	  y=rand()%Y;
	} while( status.E[x][y]!=0);
      
      status.E[x][y]=1;
      status.agent_x[i]=x;
      status.agent_y[i]=y;
      status.agent_act[i]=4;
    }

  for(i=0;i<AGENT;i++)   //エージェントの視力
     status.agent_vision[i] = init_vision(i);
  
  for(i=0;i<AGENT;i++)   //エージェントの代謝量  
     status.agent_metabolism[i] = init_metabolism(i);  
  
  for(i=0;i<AGENT;i++)   //エージェントの砂糖獲得能力  
     status.agent_acquisition[i] = init_acquisition(i);  
  
  for(i=0;i<AGENT;i++)   //エージェントの砂糖の初期保有量
     status.agent_sugar[i] = init_agent_sugar(i); 

  for(i=0;i<AGENT;i++)   //エージェントの年齢
     status.agent_age[i] = 0;   
  
  for(i=0;i<AGENT;i++)   //エージェントの寿命
     status.agent_life_span[i] = init_life_span(i);  

  for(i=0;i<AGENT;i++)   //エージェントの生死
     status.agent_life[i] = 1; 
}

void Environment::set_window(void)
{
  int x,y;
  // window1
  MngrOn( 0, 1);
  BackingOn( 0 );
  SetPosition( 0, 0 , 0 );
  NamingWindow( 0, "Sugarscape - Harmonious Systems Engineering -");
  SetSize( 0, X*10+40 , Y*10+40  );
  OpenWindow( 0 );
  SetColor ( BACKCOLOR ); 
  PutRectangle(0, 0, 0, X*10+40 , Y*10+40);

  SetLineWidth(1);
  for(x=1;x<X+1;x++)
    for(y=1;y<Y+1;y++){
      SetColor ( "gray" ); 
      Rectangle(0, 10+x*10, 10+y*10, 10 , 10);
    }

}

void Environment::draw_window(void)
{

  int x,y;
  int i;
  int sugar;
  int sugar_level;
  int cn,hl;
  char s[20];

  //sugar
  for(x=0;x<X;x++)
    {
      for(y=0;y<Y;y++)
	{
	  sugar=status.sugar_current[x][y];

	  cn=15-int(sqrt(double(sugar)/double(CAPACITY_MAX))*15.0);
	  
	  //	  if (cn==0) SetColor("white");
	  //	  if (cn==1) SetColor("#ffffd0");
	  //	  if (cn==2) SetColor("#ffff90");
	  //	  if (cn==3) SetColor("#ffff00");
	  //	  if (cn>=4) {
	  //	    SetColor("#f0f000");
	  //	    cn=4;
	  //	  }
	  
	  sprintf(s,"#ffff%xf",cn);
	  SetColor( s ); 
		    

	  if (status.E[x][y]==1){
	    for(i=0;i<AGENT;i++){
	      if (status.agent_x[i]==x && status.agent_y[i]==y){
		hl=status.agent_sugar[i]/status.agent_metabolism[i]/10;
		if (hl>15) hl=15;
		cn=100+hl;

		sprintf(s,"#%x000ff",15-hl);
		SetColor( s ); 

		//		if (status.agent_sugar[i]>INIT_SUGAR*HUNGER_LEVEL){
		//SetColor("#3050ff");
		//cn=100;
		//}
		//else{
		//SetColor("#00ffff");
		// cn=101;
		//}
		break;
	      }
	    }
	  }

	  if (lastcolor[x][y]!=cn){
	    PutRectangle(0, 10+10+x*10+1, 10+10+y*10+1, 9,9);
	    lastcolor[x][y]=cn;
	  }
	
	}
    }
}

void Environment::regenerate_sugar(int t)
{

  int x,y;
  int center_x,center_y;
  
  //seasonal fluctuation  砂糖の再生量と最大容量の変更
  if(t%SUGAR_SEASON ==0 && t>0)
    {
      
      do  
	{
	  center_x = rand()%(X+1);
	  center_y = rand()%(Y+1);
	} while( (SUGAR_RADIUS+1  > center_x)||(center_x > X-SUGAR_RADIUS-1)||
		 (SUGAR_RADIUS+1  > center_y)||(center_y > X-SUGAR_RADIUS-1) );
      
      //printf("%d,%d\n",center_x,center_y);

      init_growback(center_x, center_y);      //砂糖の再生量
      init_full_capacity(center_x, center_y); //砂糖の最大容量
    }

  //regenerate sugar
  for(x=0;x<X;x++)
    {
      for(y=0;y<Y;y++)
	{
	  if( status.sugar_current[x][y] <  status.sugar_full_capacity[x][y] )
	    {
	      status.sugar_current[x][y] += status.sugar_growback[x][y]; 
	      
	      if( status.sugar_current[x][y] > status.sugar_full_capacity[x][y])
		{
		  status.sugar_current[x][y] = status.sugar_full_capacity[x][y];
		}
	    }
	}
    } 
}

void Environment::update_agents(void)
{ 
  int i,j,k,x,y; 

  for(i=0;i<AGENT;i++)
    {
      //reset agent
      if( status.agent_life[i]==1 && ((status.agent_sugar[i]<=0) ||( status.agent_age[i] > status.agent_life_span[i])) )
	{
	
	  x=status.agent_x[i];
	  y=status.agent_y[i];

	  status.E[x][y]=0;   //エージェントの消去
	  status.agent_x[i]=99999;
	  status.agent_y[i]=99999;
	  status.agent_life[i]=0;
	  lastcolor[x][y]=-1;  
	  status.agent_sugar[i]=0;

	  j=1;//rand()%200; //エージェントの再配置率

	  if(j==0)
	    {
	      do //エージェントの再配置場所の決定
		{
		  x=rand()%X;
		  y=rand()%Y;
		} while( status.E[x][y]!=0);
	      
	      status.agent_x[i]=x;
	      status.agent_y[i]=y;  
	      status.E[x][y]=1;
	      
	      status.agent_act[i]=4;
	      
	      status.agent_vision[i]     = init_vision(i);      //エージェントの視力
	      status.agent_metabolism[i] = init_metabolism(i);  //エージェントの代謝量 
	      status.agent_acquisition[i] = init_acquisition(i);//エージェントの砂糖獲得能力  
	      status.agent_sugar[i]      = init_agent_sugar(i); //エージェントの砂糖の初期保有量
	      
	      status.agent_age[i] = 0;                         //エージェントの年齢
	      status.agent_life_span[i] = init_life_span(i);   //エージェントの寿命  
	      status.agent_life[i]=1;                          //エージェントの生死  
	    }
	}
    }
}

void Environment::update_status(int agent, int x,int y, struct Status *s, int act)
{

  if(status.agent_life[agent]==1 && 0<=agent && agent<AGENT)
    {
      status.E[x][y]=0;	
            
      if(act==0)//前
	{
	  if(y == 0)
	    y = Y-1;
	  else
	    y -= 1;   
	} 
      
      if(act==1)//後
	{ 
	  if(y==Y-1)
	    y = 0;
	  else
	    y += 1;  
	}
      
      if(act==2)//左
	{ 
	  if(x == 0)
	    x = X-1;
	  else
	    x -= 1; 
	}  
      
      if(act==3)//右
	{
	  if(x == X-1)
	    x = 0;
	  else
	    x += 1;
	}   
            
      if(0<=act && act<4)
	{
	  if(status.E[x][y]==0)//移動先にエージェントがいない場合
	    {
	      status.E[x][y]=1;	
	      status.agent_x[agent]=x;
	      status.agent_y[agent]=y;   
	      status.agent_act[agent]=act; 
	    }
	  else //移動先にエージェントがいる場合,移動不可
	    {
	      x=status.agent_x[agent];
	      y=status.agent_y[agent]; 
	      status.E[x][y]=1;	
	      status.agent_act[agent]=4; 
	    }
	}  
      else
	{
	  status.E[x][y]=1;
	  x=status.agent_x[agent];
	  y=status.agent_y[agent]; 
	  status.agent_act[agent]=4;       
	}
      
      if( status.sugar_current[x][y] > status.agent_acquisition[agent])
	{
	     status.agent_sugar[agent] += status.agent_acquisition[agent];
	     status.sugar_current[x][y] -= status.agent_acquisition[agent];       
	} 
      else
	{ 
	  status.agent_sugar[agent] += status.sugar_current[x][y];
	  status.sugar_current[x][y] = 0;
	}
      
      if( status.agent_sugar[agent] > status.agent_metabolism[agent])
	{
	  status.agent_sugar[agent] -= status.agent_metabolism[agent];
	  status.agent_age[agent]++;
	}
      else
	{
	  status.agent_sugar[agent]=0;
	  status.agent_age[agent]++;
	}
    }
  

  /*
    for(y=0;y<Y;y++)
    {	
    for(x=0;x<X;x++)
	{
	  printf("%d\t",status.E[x][y]);
	}
      printf("\n");
    } 
    */

  //printf("\n"); 
  //printf("act %d\t",status.agent_act[agent]);
  //printf("\n");

}


void Environment::agents_action(void)
{
  
  int o[AGENT];

  int i,j,r;
  int agent;
  int act;
  int vx,vy,x,y;
  int temp;
 

  //init

  // ordering  行動決定の順序を疑似乱数で決定

  for(i=0;i<AGENT;i++)
    o[i]=i;

  for(i=0;i<AGENT;i++)
    {
      j=rand()%AGENT;
      
      temp = o[i];
      o[i] = o[j]; 
      o[j] = temp;  
    }

  Agent individual;

  //action agent
  for(i=0;i<AGENT;i++)
    {
      agent=o[i];
      
      //decide action
      //action 0:上 1:下 2:右 3:左 4:停止
      
      x=status.agent_x[agent];
      y=status.agent_y[agent];
     
      if(status.agent_life[agent]==1)
	{
	  individual.decision_making(agent, x, y, &status, &act);
      	}

      //行動の結果 act で status を更新
      
      update_status(agent, x, y, &status, act);
	
    }
}

void Environment::fight(void)
{
  int t; 
  int i,j,k;
  
  FILE *fp;
  
  fp=fopen("log","w");
  fclose(fp);


  if(!QUIET) set_window();
  initializer();

  //loop    
  for(t=0; t<TMAX ;t++)
    {  
//printf("1\t"); 
      if(!QUIET) draw_window();   //エージェントと砂糖の画面表示
	  
//printf("2\t"); 
      //scanf("%d",&k);
          
      agents_action();        //エージェントの行動決定  
  
//printf("3\t"); 
      evaluation(t,fp);    //エージェントの各種平均量の算出
  
//printf("4\t"); 
      regenerate_sugar(t); //砂糖の再生    
  
//printf("5\t");   
      update_agents();     //エージェントの死亡と補充  
//printf("6\n"); 
    }

}

void Environment::evaluation(int t,FILE *fp)
{
  int i,j,k;
  
  int alive_agent;   //エージェントの生存数

  double ave_age;     //エージェントの平均年齢
  int sum_age;
  int min_age,max_age;

  double ave_sugar;   //エージェントの平均保有量
  int sum_sugar; 
  int min_sugar,max_sugar;
  double sum_pow2_sugar;
  double sugar_distribution;

  double ave_metabo;  //エージェントの平均代謝量
  int sum_metabo;

  double ave_vision;  //エージェントの平均視力
  int sum_vision;

  double ave_acquisition;  //エージェントの獲得能力
  int sum_acquisition;

  double gini;        //エージェントのジニ係数
  double area_Lorenz, area_triangle;
  int amount_sugar[AGENT];
  int temp[AGENT];


//printf("\n");

/*  for(i=0; i<AGENT;i++)
    {
     
      if( i%20==0 && i!=0)
	printf("\n");

      printf("%d\t",status.agent_sugar[i]);
    
    }*/

  

  //エージェントの生存数--------------------------------------------------------------

  alive_agent=0;
  
//printf("1-2\t"); 

  for(i=0; i<AGENT;i++)
    {
      alive_agent += status.agent_life[i]; 
    }

  
//printf("2-2\t"); 

  //エージェントの平均年齢-------------------------------------------------------------- 
  
  sum_age=0;
  
  for(i=0; i<AGENT;i++)
    {
      if(status.agent_life[i]==1)
	{
	  sum_age += status.agent_age[i];
	}
    }

  ave_age = (double)sum_age/alive_agent;
  
  min_age = (int)ave_age;
  max_age =0;

  for(i=0; i<AGENT;i++)
    { 
      if(status.agent_life[i]==1)
	{
	  if( ( min_age > status.agent_age[i])&&( status.agent_age[i] > 0) )
	    min_age = status.agent_age[i]; 
	  
	  if( max_age < status.agent_age[i])
	    max_age = status.agent_age[i];
	}
    }

  //エージェントの平均保有量-------------------------------------------------------------- 
  
  sum_sugar=0;
  
  for(i=0; i<AGENT;i++)
    {  
      if(status.agent_life[i]==1 && status.agent_sugar[i]>0 )
	{
	  sum_sugar += status.agent_sugar[i];
	}
    }
  
  ave_sugar=(double)sum_sugar/alive_agent;
  
  min_sugar = (int)ave_sugar;
  max_sugar =0;
  
  for(i=0; i<AGENT;i++)
    { 
      if( ( min_sugar > status.agent_sugar[i])&&( status.agent_sugar[i] > 0) )
	min_sugar = status.agent_sugar[i]; 
      
      if( max_sugar < status.agent_sugar[i])
	max_sugar = status.agent_sugar[i];
    }

  sum_pow2_sugar=0;
  
//printf("3-2\t"); 

  for(i=0; i<AGENT;i++)
    { 
      sum_pow2_sugar += pow(status.agent_sugar[i],2);
    }

//printf("4-2\t"); 

  sugar_distribution = sqrt( pow(sum_sugar,2) - sum_pow2_sugar ); 


  //エージェントの平均代謝量--------------------------------------------------------------
 
  sum_metabo=0;

  for(i=0; i<AGENT;i++)
    {  
      if(status.agent_life[i]==1)
	{
	  sum_metabo += status.agent_metabolism[i];
	}
    }
  
  ave_metabo=(double)sum_metabo/alive_agent;

//printf("5-2\t"); 

  //エージェントの平均視力--------------------------------------------------------------
  
  sum_vision=0;
  
  for(i=0; i<AGENT;i++)
    {  
      if(status.agent_life[i]==1)
	{
	  sum_vision += status.agent_vision[i];
	}
    }
  
  ave_vision=(double)sum_vision/alive_agent;
  
//printf("6-2\t"); 

  //エージェントの獲得能力--------------------------------------------------------------
  
  sum_acquisition=0;
  
  for(i=0; i<AGENT;i++)
    {  
      if(status.agent_life[i]==1)
	{
	  sum_acquisition += status.agent_acquisition[i];
	}
    }
  
  ave_acquisition=(double)sum_acquisition/alive_agent;
  
//printf("7-2\t"); 
  
  //エージェントのジニ係数--------------------------------------------------------------
  
  for(i=0; i<AGENT;i++)
    {
      amount_sugar[i] = 0;
    }

//printf("8-2\t"); 

  for(i=0; i<AGENT;i++)
    {
      temp[i] = status.agent_sugar[i];
    }

//printf("9-2\t"); 

  q_sort(temp,AGENT);

  for(i=0; i<AGENT;i++)
    {
      for(j=0; j<=i;j++)
	{
	  amount_sugar[i] += temp[j];
	}
    }

//printf("10-2\t");
 
  area_Lorenz = 0;
  
  for(i=0; i<AGENT;i++)
    {
      area_Lorenz += (double)amount_sugar[i];
    }
  
  area_triangle = 0.5 * amount_sugar[AGENT-1] * AGENT;

  gini = ( area_triangle - area_Lorenz  )/area_triangle;
  
//printf("11-2\t"); 
 
  //--------------------------------------------------------------

  if( alive_agent==0)
    {
      ave_age =0;
      min_age =0;
      max_age =0;
      ave_sugar =0;
      min_sugar =0;
      max_sugar =0;
      ave_metabo =0;
      ave_vision =0;
      ave_acquisition =0;
      gini =0;
    }
  
  //表示--------------------------------------------------------------

 
//printf("12-2\n"); 
 
  if(!QUIET){
    printf("t=%4d ",t);
    printf("agent=%3d ",alive_agent);
    //  printf("age ave=%.1f ",ave_age); 
    //  printf("min=%4d ",min_age);   
    //  printf("max=%4d ",max_age);
    printf("sg_ave=%.1f ",ave_sugar); 
    printf("sg_min=%5d ",min_sugar);   
    printf("sg_max=%7d ",max_sugar); 
    printf("met_ave=%.1f ",ave_metabo);  
    printf("vis_ave=%.1f ",ave_vision); 
    printf("acq_ave=%.1f ",ave_acquisition);
    printf("gini=%f",gini);  
    //printf("12-2-3\t");
    printf("\n");
  }
  
//printf("12-2-2\t");
 
  //ファイル出力--------------------------------------------------------------

  if(t==0)
    {
      fp=fopen("log","a");
      fprintf(fp,"t\t");
      fprintf(fp,"agent\t");
      //      fprintf(fp,"age\t");  
      //      fprintf(fp,"age_ave\t"); 
      //      fprintf(fp,"age_min\t"); 
      //      fprintf(fp,"age_max\t");
      //      fprintf(fp,"sugar\t");  
      fprintf(fp,"sg_ave\t"); 
      fprintf(fp,"sg_min\t");   
      fprintf(fp,"sg_max\t"); 
      fprintf(fp,"met_ave\t");  
      fprintf(fp,"vis_ave\t"); 
      fprintf(fp,"acq_ave\t");
      fprintf(fp,"gini\t");  
      fprintf(fp,"\n");
      fclose(fp);
    }
   
//printf("13-2\t"); 
   
  fp=fopen("log","a");
  fprintf(fp,"%4d\t",t);
  fprintf(fp,"%3d\t",alive_agent);
  //  fprintf(fp,"\t");  
  //  fprintf(fp,"%.1f\t",ave_age); 
  //  fprintf(fp,"%5d\t",min_age);   
  //  fprintf(fp,"%5d\t",max_age);  
  //  fprintf(fp,"\t");
  fprintf(fp,"%.1f\t",ave_sugar); 
  fprintf(fp,"%d\t",min_sugar);   
  fprintf(fp,"%d\t",max_sugar); 
  fprintf(fp,"%.1f\t",ave_metabo);  
  fprintf(fp,"%.1f\t",ave_vision); 
  fprintf(fp,"%.1f\t",ave_acquisition);
  fprintf(fp,"%f\t",gini);  
  fprintf(fp,"\n");
  fclose(fp);

//printf("14-2\t"); 

}
void Environment::total_evaluation(void)
{
  int alive_agent=0,i,jyuni=1,tmpalive,total=0,personaljyuni=1,personaltotal=0;
  FILE *fp;
  char *name=NAME,tmpname[20],filename[256];

  sprintf(filename,"/home/work/harmony_score/score2011.scr",GROUP);
  printf("%s\n",filename);

  for(i=0; i<AGENT;i++)
    {
      alive_agent += status.agent_life[i]; 
    }

  printf("最終的に生き残ったエージェント数は%dです。\n",alive_agent);

  //
  
  if((fp=fopen(filename,"r"))!=NULL){
    
    while(fscanf(fp,"%s %d",tmpname,&tmpalive)!=EOF){
      total++;
      //    printf("%s %d\n",tmpname,tmpalive);
      if (tmpalive>alive_agent) jyuni++;
      if (!strcmp(name,tmpname)){
	personaltotal++;
	if (tmpalive>alive_agent) personaljyuni++;
      }
    }
    fclose(fp);
    
    printf("記録は 全体記録 %d 中 %d 位です。\n",total+1,jyuni);
    printf("記録は 個人記録 %d 中 %d 位です。\n",personaltotal+1,personaljyuni);
  }
  
  //save score
  fp=fopen(filename,"a");
  fprintf(fp,"%s %d\n",name,alive_agent);
  fclose(fp);

}

//-------------------------------------------------------------------------------------
class Analysis{
  
public : 
  Analysis(void);
  void analyzer(void); 
  void draw_window(void);
    
private :

};

Analysis::Analysis(void)
{

}

void Analysis::draw_window(void)
{

}

void Analysis::analyzer(void)
{
  char s[20],cmd[20];
  int i,j,k,g1,p1,g2,p2,id1,id2,ck;

  FILE *fp;
  
  //do 
  //  {
  
  if(!QUIET) draw_window();
  
  //fight
  Environment env;
  env.fight();

  //全体の評価
  env.total_evaluation();
      
  // } while (strcmp(cmd,"exit")!=0);


}


int main(){
  int x,y;

  if (!strcmp(NAME,"kawamura")){
    printf("* WARNING!! Please change NAME field in define.h file to your name.\n");
    }

  char cmd[20];

  for(x=0;x<X;x++)
    for(y=0;y<Y;y++)
      lastcolor[x][y]=-1;

  init_seed();
  InitWindow(1);

  class Analysis analysis;
  analysis.analyzer();

  return(1);

}











