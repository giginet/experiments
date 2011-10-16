#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "define.h"

using namespace std;

/*-*-c++-*-*/

class Agent{
  private:
    vector<int> list_others;
    vector<int> list_sugar;
    int not_collide(vector<int> list);
  public:
    Agent(int* si, int* oi, int s, int m, int v, int a);
    int sugar;
    int met;
    int vis;
    int acq;
    int escape(bool stream);
    int random_walk(bool stream);
    int approach(bool stream);
    double scouter();
    double turn();
    int action();
};

bool contains(vector<int> v, int i);

Agent::Agent(int* si, int* oi, int s, int m, int v, int a){
  for(int i=0;i<4;++i){
    if(si[i] == 1){ 
      list_sugar.push_back(i);
    }
  }
  for(int i=0;i<4;++i){
    if(oi[i] == 1){
      list_others.push_back(i);
    }
  }
  sugar = s;
  met = m;
  vis = v;
  acq = a;
}

bool contains(vector<int> v, int i){
  vector<int>::iterator p = find(v.begin(), v.end(), i);
  return !(p == v.end());
}

int Agent::action(){
  int t = (int)turn();
  if(t <= 0 || met > 10){
    return escape(true);
  }else if(list_others.size() == 4){
    return 4; 
  }else if(!list_sugar.empty()){
    if(sugar < min(met * 175 * 15/vis, 3000)){
      return approach(false);
    }else if(sugar < 5000){
      return random_walk(true);
    }else{
      return escape(true);
    }
  }else if(list_sugar.empty()){
    return random_walk(true);
  }
}

int Agent::escape(bool stream){
  vector<int> nonsugar;
  for(int i=0;i<4;++i){
    if(!contains(list_sugar, i)){
      if(!stream || (stream && i%2==0)){
        nonsugar.push_back(i);
      }
    }
  }
  return not_collide(nonsugar);
  /*  int rnd;
  if(stream){
    int up = contains(list_sugar, 0);
    int left = contains(list_sugar, 2);
    if(up && left){
      return random_walk(true);
    }else if(up){
      return 2;
    }else if(left){
      return 0;
    }
    return random_walk(true);
  }else{
    if(list_sugar.size() == 4){
      return rand()%4;
    }else{
      do{
        rnd = rand()%4;
      }while(contains(list_sugar, rnd));
      return rnd;
    }
  }*/
}

int Agent::not_collide(vector<int> list){
  //渡した行きたい方向リストから、他人にぶつからない道をいい感じで選択してきます
  //誰もいない方向リストを作る
  vector<int> nobody;
  for(int i=0;i<4;++i){
    if(!contains(list_others, i)) nobody.push_back(i);
  }
  //行きたい方向リストと誰もいないリストの積集合をとる
  vector<int> set(list.size() + 4);
  vector<int>::iterator end = set_intersection(list.begin(), list.end(), nobody.begin(), nobody.end(), set.begin());
  int size = (int)(end - set.begin());
  if(size == 0){
    // 存在しなければ、誰もいない方向をランダムで返す
    if(nobody.empty()) return 4;
    return nobody[rand()%nobody.size()];
  }else{
    // 存在すればランダムで返却
    return set[rand()%size];
  }
  return rand()%4;
}

int Agent::random_walk(bool stream){
  vector<int> list;
  list.push_back(0);
  list.push_back(2);
  if(!stream){
    list.push_back(1);
    list.push_back(3);
  }
  return not_collide(list);
  /*int rnd;
  if(stream){
    int left = contains(list_others, 2);
    if(!up || !left){
      do{
        rnd = (rand()%2)*2;
      }while(contains(list_others, rnd));
      return rnd;
    }
    return (rand()%2)*2;
  }else{
    if(list_others.size() == 4) return rand()%4;
    do{
      rnd = rand()%5;
      if (rnd==4) rnd = 0;
    }while(contains(list_others, rnd));
    return rnd;
  }*/
}

double Agent::scouter(){
  return vis * 1000 + (12 - met) * 1000 + acq;
}

double Agent::turn(){ 
  return (4000 * met - 5000)/acq;
}

int Agent::approach(bool stream){
  vector<int> sugar;
  for(vector<int>::iterator it=list_sugar.begin();it<list_sugar.end();++it){
    if( !stream || (stream && *it%2==0) ){
      sugar.push_back(*it);
    }
  }
  return not_collide(sugar);
  /*if(!stream) return list_sugar[rand()%list_sugar.size()];
  int up = contains(list_sugar, 0);
  int left = contains(list_sugar, 2);
  if(up && left){
    return random_walk(true);
  }else if(up){
    return 0;
  }else if(left){
    return 2;
  }
  return random_walk(true);*/
}

int action(int *sugar_neighbor_info, int *others_neighbor_info, int sugar, int met, int vis, int acq){
  Agent* agent = new Agent(sugar_neighbor_info, others_neighbor_info, sugar, met, vis, acq); 
  int direction = agent->action();
  delete agent;
  return direction;
}
