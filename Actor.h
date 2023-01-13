#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size) : GraphObject(imageID, startX, startY, dir, depth, size), alive(true), m_world(world) {};
    virtual void doSomething() = 0;
    virtual StudentWorld* getWorld() {return m_world;};
    virtual bool canBlock() {return false;};
    virtual bool damagable() {return true;};
    void setDead() {alive = false;};
    bool isAlive() {return alive;};
    virtual void bonk() {return;};
    virtual bool isEnemy() {return false;};
    virtual void isDamaged() {return;};

private:
    bool alive;
    StudentWorld* m_world;
};

//base class for enemy
class Enemy : public Actor 
{
public:
    Enemy(StudentWorld* world, int ID, int x, int y) : Actor(world, ID, x, y, 180*randInt(0,1), 0, 1) {};
    virtual bool isEnemy() {return true;};
    virtual void doSomething();
    virtual void bonk();
    virtual void moveEnemy();
    virtual void isDamaged();
};

class Goomba : public Enemy
{
public:
    Goomba(StudentWorld* world, int x, int y) : Enemy(world, IID_GOOMBA, x, y) {};
};

class Koopa : public Enemy
{
public:
    Koopa(StudentWorld* world, int x, int y) : Enemy(world, IID_KOOPA, x, y) {};
    virtual void isDamaged();
};

class Piranha : public Enemy
{
public:
    Piranha(StudentWorld* world, int x, int y) : Enemy(world, IID_PIRANHA, x, y), firing_delay(0) {};
    virtual void doSomething();
    bool sameLevel();
    bool canfire();
private:
    int firing_delay;
};

//base class for star, flower, mushroom
class Goodie : public Actor
{
public:
    Goodie(StudentWorld* world, int ID, int x, int y) : Actor(world, ID, x, y, 0, 1, 1) {};
    virtual bool isGoodie() {return true;};
    virtual bool damagable() {return false;};
    virtual void doSomething() {return;};
    virtual void moveGoodie();
};

class Star : public Goodie
{
public:
    Star(StudentWorld* world, int x, int y) : Goodie(world, IID_STAR, x, y) {};
    virtual void doSomething();
};

class Flower : public Goodie
{
public:
    Flower(StudentWorld* world, int x, int y) : Goodie(world, IID_FLOWER, x, y) {};
    virtual void doSomething();
};

class Mushroom : public Goodie
{
public:
    Mushroom(StudentWorld* world, int x, int y) : Goodie(world, IID_MUSHROOM, x, y) {};
    virtual void doSomething();
};

//base class for projectiles
class Shell : public Actor
{
public:
    Shell(StudentWorld* world, int x, int y, int dir, int ID) : Actor(world, ID, x, y, dir, 1, 1) {};
    virtual bool damagable() {return false;};
    virtual void doSomething();
    virtual bool exceptPeach();
};

class peachFireball : public Shell
{
public:
    peachFireball(StudentWorld* world, int x, int y, int dir) : Shell(world, x, y, dir, IID_PEACH_FIRE) {};
};

class piranhaFireball : public Shell
{
public:
    piranhaFireball(StudentWorld* world, int x, int y, int dir) : Shell(world, x, y, dir, IID_PIRANHA_FIRE) {};
    virtual bool exceptPeach();
};

class Peach : public Actor
{
public:
    Peach(StudentWorld* world, int x, int y) : Actor(world, IID_PEACH, x, y, 0, 0, 1), jump_power(false), star_power(false), shoot_power(false), temp_invin(false), hit(1), jump_distance(0), star_ticks(0), temp_ticks(0) {};
    virtual void doSomething();
    bool jump();
    void shoot() {shoot_power = true;};
    void jumpPower() {jump_power = true;};
    void starPower() {star_power = true;};
    bool hasStarPower() {return star_power;};
    bool hasJumpPower() {return jump_power;};
    bool hasShootPower() {return shoot_power;};
    void tempInvincibility() {temp_invin = true;};
    void setHit(int h) {hit = h;};
    void setInvincibility(int i) {star_ticks = i;};
    void setShootTime(int i) {shoot_time = i;};
    void setTempTicks(int t) {temp_ticks = t;};
    void addFireball();
    virtual void bonk();
    virtual void isDamaged();
    
private:
    int hit;
    bool jump_power; //mushroom
    bool star_power;
    int star_ticks;
    bool shoot_power; //flower
    int shoot_time;
    bool temp_invin;
    int temp_ticks;
    int jump_distance;
};

//base class for block and pipe classes
class blockOrPipe : public Actor
{
public:
    blockOrPipe(StudentWorld* world, int ID, int x, int y) : Actor(world, ID, x, y, 0, 2, 1) {};
    virtual bool canBlock() {return true;};
    virtual bool damagable() {return false;};
    virtual void doSomething() {return;};
};

class Block : public blockOrPipe
{
public:
    Block(StudentWorld* world, int x, int y) : blockOrPipe(world, IID_BLOCK, x, y), has_goodie(false) {};
    Block(StudentWorld* world, int x, int y, std::string type) : blockOrPipe(world, IID_BLOCK, x, y), has_goodie(true), released_goodie(false), m_type(type) {};
    virtual void bonk();

private:
    bool released_goodie;
    bool has_goodie;
    std::string m_type;
};

class Pipe : public blockOrPipe
{
public:
    Pipe(StudentWorld* world, int x, int y) : blockOrPipe(world, IID_PIPE, x, y) {};
    virtual void bonk();
};

//base class for flag and mario
class Flag : public Actor
{
public:
    Flag(StudentWorld* world, int x, int y, int ID) : Actor(world, ID, x, y, 0, 1, 1) {};
    virtual void doSomething();
    virtual bool damagable() {return false;};
    virtual void levelOrGame();
};

class Mario : public Flag
{
public:
    Mario(StudentWorld* world, int x, int y, int ID) : Flag(world, x, y, ID) {};
    virtual void levelOrGame();
};



#endif // ACTOR_H_
