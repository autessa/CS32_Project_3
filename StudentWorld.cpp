#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)// nextLevel(false), won(false)
{
    actors.clear();
}

int StudentWorld::init()
{
    nextLevel = false;
    won = false;
    if (level() == GWSTATUS_LEVEL_ERROR)
        return GWSTATUS_LEVEL_ERROR;
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    for (auto it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->isAlive())
            (*it)->doSomething();
        else {
            delete *it;
            actors.erase(it);
            it = actors.begin();
        }
    }
        
    if (peach->isAlive())
        peach->doSomething();
    else {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (nextLevel) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if (won) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    
    ostringstream oss;
    
    oss.fill('0');
    oss << "Lives: ";
    oss << getLives() << "  ";
    
    oss << "Level: ";
    oss << setw(2) << getLevel() << "  ";
    
    oss << "Points: ";
    oss << setw(6) << getScore() << " ";
    
    if (peach->hasJumpPower())
        oss << "JumpPower!" << " ";
    
    if (peach->hasStarPower())
        oss << "StarPower!" << " ";
    
    if (peach->hasShootPower())
        oss << "ShootPower!" << " ";
    
    setGameStatText(oss.str());
        
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete peach;
    for (Actor* p : actors)
        delete p;
    actors.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

//get peaches coordinates to use in actor
void StudentWorld::peachCoord(double &x, double &y)
{
    x = peach->getX();
    y = peach->getY();
}

//when peach is overlapped with a flower
void StudentWorld::peachFlower()
{
    peach->shoot();
    peach->setShootTime(8);
    peach->setHit(2);
    peach->tempInvincibility();
    peach->setTempTicks(10);
}

//when peach is overlapped with a mushroom
void StudentWorld::peachMushroom()
{
    peach->jumpPower();
    peach->setHit(2);
    peach->tempInvincibility();
    peach->setTempTicks(10);
}

//when peach is overlapped with a star
void StudentWorld::peachStar()
{
    peach->starPower();
    peach->setInvincibility(150);
}

//check if shell overlaps with damagable object and call that actor's isDamaged function
bool StudentWorld::shellDamage(Actor* a)
{
    for (auto p : actors) {
        if (p != a && p->isAlive())
            if (p->damagable() && overlap(a->getX(), a->getY(), p->getX(), p->getY())) {
                p->isDamaged();
                return true;
            }
    }
    return false;
}

//if peach overlaps with actor, damage her (used for piranha)
bool StudentWorld::piranhaDamage(Actor* a)
{
    if (peachOverlap(a)) {
        peach->isDamaged();
        return true;
    }
    return false;
}

//check if object overlaps with peach
bool StudentWorld::peachOverlap(Actor* a)
{
    if (overlap(a->getX(), a->getY(), peach->getX(), peach->getY()))
        return true;
    return false;
}

void StudentWorld::addActor(Actor* a)
{
    actors.push_back(a);
}

//check if actor can move to desired location, if not call the bonk function of actor
bool StudentWorld::canMove(Actor* a, double x, double y)
{
    for (auto p : actors) {
        if (a == peach) {
            if (overlap(x, y, p->getX(), p->getY())) {
                if (!p->isEnemy())
                    p->bonk();
                if (p->canBlock())
                    return false;
            }
        }
        else if (p != a) {
            if (overlap(x, y, p->getX(), p->getY()) && p->canBlock())
                return false;
        }
    }
    return true;
}

//check if peach has an object below her to let her jump
bool StudentWorld::canJump(Actor* a)
{
    for (auto p : actors) {
        if (p != a && p->canBlock() && onTop(a->getX(), a->getY(), p->getX(), p->getY()))
            return true;
    }
    return false;
}

//check if enemy is exactly on top of an object that can block
bool StudentWorld::enemyTop(Actor* e, double x, double y)
{
    if (y+SPRITE_HEIGHT == e->getY()) {
        if (x < e->getX()) {
            if (e->getX() < x+SPRITE_WIDTH)
                return true;
        }
        else if (e->getX() <= x) {
            if (x == e->getX()+SPRITE_WIDTH)
                return true;
        }
    }
return false;
}

//check if enemy can move
bool StudentWorld::enemyMove(Actor* e)
{
    for (auto p : actors) {
        if (p != e && p->canBlock() && enemyTop(e, p->getX(), p->getY()))
            return true;
    }
    return false;
}

//check if first coordinates on top of second
bool StudentWorld::onTop(double x1, double y1, double x2, double y2)
{
        //check if the bottom y and top y are equal
        if (y2+SPRITE_HEIGHT == y1) {
            if (x2 < x1) {
                if (x1 < x2+SPRITE_WIDTH)
                    return true;
            }
            else if (x1 <= x2) {
                if (x2 < x1+SPRITE_WIDTH)
                    return true;
            }
        }
    return false;
}

//check if object coordinates overlap
bool StudentWorld::overlap(double x1, double y1, double x2, double y2)
{
    //check left and right overlap
    double leftX, rightX, leftY, rightY;
    if (x1 <= x2) {
        leftX = x1;
        leftY = y1;
        rightX = x2;
        rightY = y2;
    }
    else {
        leftX = x2;
        leftY = y2;
        rightX = x1;
        rightY = y1;
    }
    
    double upperX, upperY, lowerX, lowerY;
    if (y1 <= y2) {
        lowerX = x1;
        lowerY = y1;
        upperX = x2;
        upperY = y2;
    }
    else {
        lowerX = x2;
        lowerY = y2;
        upperX = x1;
        upperY = y1;
    }
    
    if (leftX + SPRITE_WIDTH > rightX && lowerY + SPRITE_HEIGHT > upperY)
        return true;
    
    return false;
}

//pass in level from assets
int StudentWorld::level() {
     Level lev(assetPath());
     string level;
     if (getLevel() < 10)
         level = "level0";
     else if (getLevel() >= 10)
         level = "level";
    else
        return GWSTATUS_LEVEL_ERROR;
     string level_file = level + to_string(getLevel()) + ".txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found) {
         cerr << "Could not find " << level_file << " data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
     else if (result == Level::load_fail_bad_format)
         cerr << level_file << " is improperly formatted" << endl;
     else if (result == Level::load_success) {
         cerr << "Successfully loaded level" << endl;
         for (int x = 0; x < GRID_WIDTH; x++) {
             for (int y = 0; y < GRID_HEIGHT; y++) {
                 Level::GridEntry ge;
                 ge = lev.getContentsOf(x, y);
                 switch (ge) { //TODO: actually add the elements and maybe some cerr statements
                     case Level::empty:
                         break;
                     case Level::koopa:
                         actors.push_back(new Koopa(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
                         break;
                     case Level::goomba:
                         actors.push_back(new Goomba(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
                         break;
                     case Level::peach:
                         peach = (new Peach(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
                         break;
                     case Level::flag:
                         actors.push_back(new Flag(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, IID_FLAG));
                         break;
                     case Level::block:
                         actors.push_back(new Block(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
                         break;
                     case Level::star_goodie_block:
                         actors.push_back(new Block(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, "star"));
                         break;
                     case Level::mushroom_goodie_block:
                         actors.push_back(new Block(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, "mushroom"));
                         break;
                     case Level::flower_goodie_block:
                         actors.push_back(new Block(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, "flower"));
                         break;
                     case Level::piranha:
                         actors.push_back(new Piranha(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
                         break;
                     case Level::pipe:
                         actors.push_back(new Pipe(this, x*SPRITE_WIDTH, y*SPRITE_HEIGHT));
                         break;
                     case Level::mario:
                         actors.push_back(new Mario(this, x*SPRITE_WIDTH, y*SPRITE_WIDTH, IID_MARIO));
                         break;
                 }
             }
     }
     }
    return 5;
}
