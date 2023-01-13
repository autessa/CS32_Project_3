#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
   StudentWorld(std::string assetPath);
   virtual int init();
   virtual int move();
   virtual void cleanUp();
    ~StudentWorld();
    int level();
    bool canMove(Actor* a, double x, double y);
    bool overlap(double x1, double y1, double x2, double y2);
    void peachCoord(double &x, double &y);
    void levelComplete() {nextLevel = true;};
    bool onTop(double x1, double y1, double x2, double y2);
    bool canJump(Actor* a);
    void wonGame() {won = true;};
    void addActor(Actor* a); //TODO: delete actor with pointer passed in
    void peachFlower();
    void peachMushroom();
    void peachStar();
    Peach* getPeach() {return peach;};
    bool enemyMove(Actor* e);
    bool enemyTop(Actor* e, double x, double y);
    bool shellDamage(Actor* a);
    bool peachOverlap(Actor* a);
    bool piranhaDamage(Actor* a);
    
private:
    vector<Actor*> actors;
    Peach* peach;
    bool nextLevel;
    bool won;
};


#endif // STUDENTWORLD_H_
