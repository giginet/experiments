#define NAME "giginet"       //please change to your name
#define GROUP "complex"             //please change to your group

// don't change follows!!
//-------------------------------------------------------------------------------------
#define AGENT 500             //エージェントの数
#define NUM_METABOLISM  12    //エージェントの代謝量の最大値
#define INIT_SUGAR  5000      //エージェントの初期砂糖保有量
#define LIFE_MAX    10000      //エージェントの寿命の最大値
#define VISION_MAX 30         //エージェントの視力の最大値
#define ACQUISITION_MAX 200   //エージェントの砂糖獲得能力の最大値
#define ACQUISITION_MIN 120   //エージェントの砂糖獲得能力の最小値
#define NEIGHBOR 1            //エージェントの近傍
#define NUM_NEIGHBOR 4        //エージェントの近傍数
#define NUM_ACTION   5        //エージェントの行動数
#define HUNGER_LEVEL 0.1      //エージェントの飢餓状態

#define SUGAR_MAX 5           //砂糖の初期の最大値 
#define NUM_SUGAR_LEVEL 5     //砂糖の段階数
#define SUGAR_RADIUS 8        //砂糖の分布半径
#define GROWBACK_MAX 20        //砂糖の再生量の最大値
#define CAPACITY_MAX 300       //砂糖の最大容量の最大値
#define SUGAR_SEASON  500      //砂糖の移動期間
 
#define X 50                  //X座標の最大値
#define Y 50                  //Y座標の最大値
#define TMAX 4000             //繰り返し回数

#define BACKCOLOR "white"

//-------------------------------------------------------------------------------------
struct Status {
  int agent_x[AGENT],agent_y[AGENT]; //エージェントの位置 x座標とy座標
  int agent_vision[AGENT];           //エージェントの視力
  int agent_metabolism[AGENT];       //エージェントの代謝量
  int agent_acquisition[AGENT];      //エージェントの獲得量
  int agent_sugar[AGENT];            //エージェントの砂糖の保有量
  int agent_age[AGENT];              //エージェントの年齢
  int agent_life_span[AGENT];        //エージェントの寿命 
  int agent_act[AGENT];              //エージェントの行動
  int agent_life[AGENT];        //エージェントの生死 生:1 死:0

  int E[X+2][Y+2];                   //座標(x,y)の状態 エージェントの有無 有:1 無:0 

  int sugar_current[X+2][Y+2];       //砂糖の現在量
  int sugar_growback[X+2][Y+2];      //砂糖の再生量
  int sugar_full_capacity[X+2][Y+2]; //砂糖の最大容量
};
