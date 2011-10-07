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
  //��˵�θ����̤������ͤ���¿����� 1�����ʤ���� 0

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

  if(i==0) //��
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

  if(i==1) //��
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
  
  if(i==2) //��
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

  if(i==3) //��
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

  //��˵�˥���������Ȥ������ 1�����ʤ���� 0

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

  if(i==0) //��
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

  if(i==1) //��
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
  
  if(i==2) //��
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

  if(i==3) //��
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
  
  //�Ķ������о���򻻽�
  for(i=0;i<NUM_NEIGHBOR;i++)
    { 
      // 0:�� 1:�� 2:�� 3:��
      
      sugar_neighbor_info[i] = sugar_info(agent,x,y,i,s);
      others_neighbor_info[i]=  others_info(agent,x,y,i,s);
    }  
  
  //��о���ȼ�ʬ�ξ��֤����ư�����
  //action 0:�� 1:�� 2:�� 3:�� 4:��� 
  
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
  void set_window();           //����������ȤȺ����β���ɽ��
  
  void agents_action(void);           //����������Ȥι�ư����  
  void update_status(int, int , int , struct Status *, int); //����������Ȥξ���ι���

  void regenerate_sugar(int);  //�����κ���    
  void update_agents(void);    //����������Ȥλ�˴���佼

  void init_sugar(int, int);         //�����ν���̤η���  
  void init_growback(int, int);      //�����ν�������̤η���  
  void init_full_capacity(int, int); //�����ν���������̤η���  

  int init_vision(int);          //����������Ȥλ��Ϥη���  
  int init_metabolism(int);      //����������Ȥ�����̤η���   
  int init_acquisition(int);      //����������Ȥκ�������ǽ�Ϥη���   
  int init_agent_sugar(int);     //����������Ȥκ����ν����ͭ�̤η���  
  int init_life_span(int);       //����������Ȥμ�̿�η���  
};

Environment::Environment(void)
{
}
  
void Environment::init_sugar(int center_x, int center_y)        //�����ν����  
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
 
void Environment::init_growback(int center_x, int center_y)      //�����κ�����
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
 
void Environment::init_full_capacity(int center_x, int center_y) //�����κ�������
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
  
int Environment::init_vision(int i)        //����������Ȥλ���
{ 

  return rand()%VISION_MAX + 1;

} 

int Environment::init_metabolism(int i)      //����������Ȥ������  
{

  return rand()%NUM_METABOLISM+1;

}

int Environment::init_acquisition(int i)      //����������Ȥ������  
{
  int acquisition;
  
  acquisition =  ACQUISITION_MIN + rand()%( ACQUISITION_MAX - ACQUISITION_MIN );
  
  return acquisition;

}

int Environment::init_agent_sugar(int i)     //����������Ȥκ����ν����ͭ��
{

  return  INIT_SUGAR;

}  

int Environment::init_life_span(int i)      //����������Ȥμ�̿
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
  
  init_sugar(center_x,center_y);         //�����ν����  
  init_growback(center_x,center_y);      //�����ν��������
  init_full_capacity(center_x,center_y); //�����ν����������
     

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
  for(i=0;i<AGENT;i++)//����������Ȥν������
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

  for(i=0;i<AGENT;i++)   //����������Ȥλ���
     status.agent_vision[i] = init_vision(i);
  
  for(i=0;i<AGENT;i++)   //����������Ȥ������  
     status.agent_metabolism[i] = init_metabolism(i);  
  
  for(i=0;i<AGENT;i++)   //����������Ȥκ�������ǽ��  
     status.agent_acquisition[i] = init_acquisition(i);  
  
  for(i=0;i<AGENT;i++)   //����������Ȥκ����ν����ͭ��
     status.agent_sugar[i] = init_agent_sugar(i); 

  for(i=0;i<AGENT;i++)   //����������Ȥ�ǯ��
     status.agent_age[i] = 0;   
  
  for(i=0;i<AGENT;i++)   //����������Ȥμ�̿
     status.agent_life_span[i] = init_life_span(i);  

  for(i=0;i<AGENT;i++)   //����������Ȥ�����
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
  
  //seasonal fluctuation  �����κ����̤Ⱥ������̤��ѹ�
  if(t%SUGAR_SEASON ==0 && t>0)
    {
      
      do  
	{
	  center_x = rand()%(X+1);
	  center_y = rand()%(Y+1);
	} while( (SUGAR_RADIUS+1  > center_x)||(center_x > X-SUGAR_RADIUS-1)||
		 (SUGAR_RADIUS+1  > center_y)||(center_y > X-SUGAR_RADIUS-1) );
      
      //printf("%d,%d\n",center_x,center_y);

      init_growback(center_x, center_y);      //�����κ�����
      init_full_capacity(center_x, center_y); //�����κ�������
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

	  status.E[x][y]=0;   //����������Ȥξõ�
	  status.agent_x[i]=99999;
	  status.agent_y[i]=99999;
	  status.agent_life[i]=0;
	  lastcolor[x][y]=-1;  
	  status.agent_sugar[i]=0;

	  j=1;//rand()%200; //����������Ȥκ�����Ψ

	  if(j==0)
	    {
	      do //����������Ȥκ����־��η���
		{
		  x=rand()%X;
		  y=rand()%Y;
		} while( status.E[x][y]!=0);
	      
	      status.agent_x[i]=x;
	      status.agent_y[i]=y;  
	      status.E[x][y]=1;
	      
	      status.agent_act[i]=4;
	      
	      status.agent_vision[i]     = init_vision(i);      //����������Ȥλ���
	      status.agent_metabolism[i] = init_metabolism(i);  //����������Ȥ������ 
	      status.agent_acquisition[i] = init_acquisition(i);//����������Ȥκ�������ǽ��  
	      status.agent_sugar[i]      = init_agent_sugar(i); //����������Ȥκ����ν����ͭ��
	      
	      status.agent_age[i] = 0;                         //����������Ȥ�ǯ��
	      status.agent_life_span[i] = init_life_span(i);   //����������Ȥμ�̿  
	      status.agent_life[i]=1;                          //����������Ȥ�����  
	    }
	}
    }
}

void Environment::update_status(int agent, int x,int y, struct Status *s, int act)
{

  if(status.agent_life[agent]==1 && 0<=agent && agent<AGENT)
    {
      status.E[x][y]=0;	
            
      if(act==0)//��
	{
	  if(y == 0)
	    y = Y-1;
	  else
	    y -= 1;   
	} 
      
      if(act==1)//��
	{ 
	  if(y==Y-1)
	    y = 0;
	  else
	    y += 1;  
	}
      
      if(act==2)//��
	{ 
	  if(x == 0)
	    x = X-1;
	  else
	    x -= 1; 
	}  
      
      if(act==3)//��
	{
	  if(x == X-1)
	    x = 0;
	  else
	    x += 1;
	}   
            
      if(0<=act && act<4)
	{
	  if(status.E[x][y]==0)//��ư��˥���������Ȥ����ʤ����
	    {
	      status.E[x][y]=1;	
	      status.agent_x[agent]=x;
	      status.agent_y[agent]=y;   
	      status.agent_act[agent]=act; 
	    }
	  else //��ư��˥���������Ȥ�������,��ư�Բ�
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

  // ordering  ��ư����ν���򵿻�����Ƿ���

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
      //action 0:�� 1:�� 2:�� 3:�� 4:���
      
      x=status.agent_x[agent];
      y=status.agent_y[agent];
     
      if(status.agent_life[agent]==1)
	{
	  individual.decision_making(agent, x, y, &status, &act);
      	}

      //��ư�η�� act �� status �򹹿�
      
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
      if(!QUIET) draw_window();   //����������ȤȺ����β���ɽ��
	  
//printf("2\t"); 
      //scanf("%d",&k);
          
      agents_action();        //����������Ȥι�ư����  
  
//printf("3\t"); 
      evaluation(t,fp);    //����������ȤγƼ�ʿ���̤λ���
  
//printf("4\t"); 
      regenerate_sugar(t); //�����κ���    
  
//printf("5\t");   
      update_agents();     //����������Ȥλ�˴���佼  
//printf("6\n"); 
    }

}

void Environment::evaluation(int t,FILE *fp)
{
  int i,j,k;
  
  int alive_agent;   //����������Ȥ���¸��

  double ave_age;     //����������Ȥ�ʿ��ǯ��
  int sum_age;
  int min_age,max_age;

  double ave_sugar;   //����������Ȥ�ʿ����ͭ��
  int sum_sugar; 
  int min_sugar,max_sugar;
  double sum_pow2_sugar;
  double sugar_distribution;

  double ave_metabo;  //����������Ȥ�ʿ�������
  int sum_metabo;

  double ave_vision;  //����������Ȥ�ʿ�ѻ���
  int sum_vision;

  double ave_acquisition;  //����������Ȥγ���ǽ��
  int sum_acquisition;

  double gini;        //����������ȤΥ��˷���
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

  

  //����������Ȥ���¸��--------------------------------------------------------------

  alive_agent=0;
  
//printf("1-2\t"); 

  for(i=0; i<AGENT;i++)
    {
      alive_agent += status.agent_life[i]; 
    }

  
//printf("2-2\t"); 

  //����������Ȥ�ʿ��ǯ��-------------------------------------------------------------- 
  
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

  //����������Ȥ�ʿ����ͭ��-------------------------------------------------------------- 
  
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


  //����������Ȥ�ʿ�������--------------------------------------------------------------
 
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

  //����������Ȥ�ʿ�ѻ���--------------------------------------------------------------
  
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

  //����������Ȥγ���ǽ��--------------------------------------------------------------
  
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
  
  //����������ȤΥ��˷���--------------------------------------------------------------
  
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
  
  //ɽ��--------------------------------------------------------------

 
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
 
  //�ե��������--------------------------------------------------------------

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

  printf("�ǽ�Ū�������Ĥä�����������ȿ���%d�Ǥ���\n",alive_agent);

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
    
    printf("��Ͽ�� ���ε�Ͽ %d �� %d �̤Ǥ���\n",total+1,jyuni);
    printf("��Ͽ�� �Ŀ͵�Ͽ %d �� %d �̤Ǥ���\n",personaltotal+1,personaljyuni);
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

  //���Τ�ɾ��
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











