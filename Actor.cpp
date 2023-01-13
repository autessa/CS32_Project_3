#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Block::bonk()
{
    int x1 = getX();
    int y1 = getY()+8;

        if (has_goodie && released_goodie == false) {
            getWorld()->playSound(SOUND_POWERUP_APPEARS);
            if (m_type == "flower")
                getWorld()->addActor(new Flower(getWorld(), x1, y1));
            if (m_type == "star")
                getWorld()->addActor(new Star(getWorld(), x1, y1));
            if (m_type == "mushroom")
                getWorld()->addActor(new Mushroom(getWorld(), x1, y1));
            released_goodie = true;
        }
        else
            getWorld()->playSound(SOUND_PLAYER_BONK);
}

void Pipe::bonk()
{
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

//to call in doSomething, for flag it notifies level is complete
void Flag::levelOrGame()
{
    getWorld()->levelComplete();
}

//for mario is notifies game is won
void Mario::levelOrGame()
{
    getWorld()->wonGame();
}

void Flag::doSomething()
{
    if (!isAlive())
        return;
    
    double x, y;
    getWorld()->peachCoord(x, y);
    
    if (getWorld()->overlap(getX(), getY(), x, y)) {
        getWorld()->increaseScore(1000);
        setDead();
        levelOrGame();
    }
}

void Enemy::moveEnemy()
{
    if (getDirection() == 0) {
        if (!getWorld()->enemyMove(this) || !getWorld()->canMove(this, getX()+1, getY()))
            setDirection(180);
    }
    else if (getDirection() == 180) {
        if (!getWorld()->enemyMove(this) || !getWorld()->canMove(this, getX()-1, getY()))
            setDirection(0);
    }

    if (getDirection() == 0) {
        if (getWorld()->canMove(this, getX()+1, getY()))
            moveTo(getX()+1, getY());
        else
            return;
    }
    else if (getDirection() == 180) {
        if (getWorld()->canMove(this, getX()-1, getY()))
            moveTo(getX()-1, getY());
        else
            return;
    }

}

//call when enemy is bonked
void Enemy::isDamaged()
{
    getWorld()->increaseScore(100);
    setDead();
}

void Enemy::bonk()
{
    if (getWorld()->getPeach()->hasStarPower()) {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        isDamaged();
    }
}

void Enemy::doSomething()
{
    if (!isAlive())
        return;
    
    if (getWorld()->peachOverlap(this)) {
        bonk();
        getWorld()->getPeach()->bonk();
        return;
    }
    
    moveEnemy();
}

//check if peach is on same y coordinate
bool Piranha::sameLevel()
{
    double x, y;
    getWorld()->peachCoord(x, y);
    if ((getY()-y) <= 1.5*SPRITE_HEIGHT && (getY()-y) >= -1.5*SPRITE_HEIGHT)
        return true;
    return false;
}

//check if peach is within fireball range
bool Piranha::canfire()
{
    double x, y;
    getWorld()->peachCoord(x, y);
    if ((getX()-x) < 8*SPRITE_WIDTH && (getX()-x) > -8*SPRITE_WIDTH)
        return true;
    return false;
}

void Piranha::doSomething()
{
    if (!isAlive())
        return;
    
    increaseAnimationNumber();
    
    if (getWorld()->peachOverlap(this)) {
        bonk();
        getWorld()->getPeach()->bonk();
        return;
    }
    
    if (!sameLevel())
        return;
    
    else {
        double x, y;
        getWorld()->peachCoord(x, y);
        if (x < getX())
            setDirection(180);
        else if (x > getX())
            setDirection(0);
        
        if (firing_delay > 0) {
            firing_delay--;
            return;
        }
        else if (firing_delay == 0) {
            if (canfire()) {
                getWorld()->addActor(new piranhaFireball(getWorld(), getX(), getY(), getDirection()));
                getWorld()->playSound(SOUND_PIRANHA_FIRE);
                firing_delay = 40;
            }
            else
                return;
        }
    }
}

//different isDamaged for koopa because introduce shell
void Koopa::isDamaged()
{
    int x = getX();
    int y = getY();
    int dir = getDirection();
    
    getWorld()->increaseScore(100);
    getWorld()->addActor(new Shell(getWorld(), x, y, dir, IID_SHELL));
    setDead();
}

//damage any damagable object except peach
bool Shell::exceptPeach()
{
    if (!getWorld()->peachOverlap(this) && getWorld()->shellDamage(this))
        return true;
    return false;
}

//for piranha want it to overlap with peach
bool piranhaFireball::exceptPeach()
{
    if (getWorld()->piranhaDamage(this))
        return true;
    return false;
}

void Shell::doSomething()
{
    if (exceptPeach()) {
            setDead();
            return;
    }
    
    else {
    if (!getWorld()->canJump(this)) {
        moveTo(getX(), getY()-2);
    }
    
        int dir = getDirection();
        switch (dir) {
            case 0: {
                if (getWorld()->canMove(this, getX()+2, getY())) {
                    moveTo(getX()+2, getY());
                    return;
                }
                else {
                    setDead();
                    return;
                }
            }
            case 180: {
                if (getWorld()->canMove(this, getX()-2, getY())) {
                    moveTo(getX()-2, getY());
                    return;
                }
                else {
                    setDead();
                    return;
                }
            }
        }
    }
}

void Goodie::moveGoodie()
{
    if (!getWorld()->canJump(this)) {
        moveTo(getX(), getY()-2);
    }
    
        int dir = getDirection();
        switch (dir) {
            case 0: {
                if (getWorld()->canMove(this, getX()+2, getY())) {
                    moveTo(getX()+2, getY());
                    return;
                }
                else {
                    setDirection(180);
                    return;
                }
            }
            case 180: {
                if (getWorld()->canMove(this, getX()-2, getY())) {
                    moveTo(getX()-2, getY());
                    return;
                }
                else {
                    setDirection(0);
                    return;
                }
            }
        }
}

void Flower::doSomething()
{
    if(getWorld()->peachOverlap(this)) {
        getWorld()->increaseScore(50);
        getWorld()->peachFlower();
        setDead();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else {
        moveGoodie();
    }
}

void Mushroom::doSomething()
{
    if(getWorld()->peachOverlap(this)) {
        getWorld()->increaseScore(75);
        getWorld()->peachMushroom();
        setDead();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else
        moveGoodie();
}

void Star::doSomething()
{
    if(getWorld()->peachOverlap(this)) {
        getWorld()->increaseScore(100);
        getWorld()->peachStar();
        setDead();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else
        moveGoodie();
}

bool Peach::jump()
{
    if (getWorld()->canJump(this)){
        if (getWorld()->canMove(this, getX(), getY()+4))
            return true;
    }
    return false;
}

void Peach::addFireball()
{
    int x;
    int y = getY();
    int dir = getDirection();
    
    if (dir == 0)
        x = getX()+4;
    else
        x = getX()-4;
    
    getWorld()->addActor(new peachFireball(getWorld(), x, y, dir));
}

void Peach::bonk()
{
    if (star_power || temp_invin)
        return;
    
    else {
        hit--;
        temp_invin = true;
        setTempTicks(10);
        shoot_power = false;
        jump_power = false;
        if (hit >= 1)
            getWorld()->playSound(SOUND_PLAYER_HURT);
        else if (hit == 0)
            setDead();
    }
}

void Peach::isDamaged()
{
    bonk();
}

void Peach::doSomething()
{
    if (!isAlive())
        setDead();
    
    if (star_power) {
        star_ticks--;
        if (star_ticks == 0)
            star_power = false;
    }
    
    if (temp_invin) {
        temp_ticks--;
        if (temp_ticks == 0)
            temp_invin = false;
    }
    
    if (shoot_time > 0)
        shoot_time--;
    
    if (jump_distance > 0) {
        if (getWorld()->canMove(this, getX(), getY()+4)) {
            moveTo(getX(), getY()+4);
            jump_distance--;
        }
        else {
            //TODO: call bonk() of other object
            jump_distance = 0;
        }
    }
    else {
        if (!getWorld()->canJump(this)) {
            moveTo(getX(), getY()-4);
        }
    }
        
    int key;
    getWorld()->getKey(key);
    if (key == KEY_PRESS_LEFT) {
        setDirection(180);
        if (getWorld()->canMove(this, getX()-4, getY())) //make this a function
            moveTo(getX()-4, getY());
        return;
    }
    if (key == KEY_PRESS_RIGHT) {
        setDirection(0);
        if (getWorld()->canMove(this, getX()+4, getY())) //to avoid repetition?
            moveTo(getX()+4, getY());
        return;
    }
    if (key == KEY_PRESS_UP) {
        if (jump()) {
            if (!jump_power)
                jump_distance = 8;
            else
                jump_distance = 12;
            getWorld()->playSound(SOUND_PLAYER_JUMP);
        }
        return;
    }
    if (key == KEY_PRESS_SPACE) {
        if (shoot_power == false)
            return;
        else {
            if (shoot_time > 0)
                return;
            else if (shoot_time == 0) {
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                setShootTime(8);
                addFireball();
            }
        }
    }
}

