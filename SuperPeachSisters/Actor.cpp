
#include "Actor.h"

using namespace std;

Actor::Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) :
    GraphObject(imageID, startX, startY, dir, depth, size) {
    m_studentWorld = world;
}


StudentWorld* Actor::returnWorld() {
    return m_studentWorld;
}

void Actor::getBonked() {
    ;
}

void Actor::getDamaged() {
    ;
}


Flag::Flag(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, 0, 1, 1.0, world) {
    ;
}


bool Flag::commonFlag() {
    if (isAlive()) {
        if (returnWorld()->overlapPeach(getX(), getY())) {
            setAliveStatus(false);
            returnWorld()->increaseScore(1000);
            return true;
        }

    }
    return false;
}

void Flag::doSomething() {
    if (commonFlag()) {
        returnWorld()->setCompleteLevel(true);

    }
}



Mario::Mario(int imageID, int startX, int startY, StudentWorld* world)
    : Flag(IID_MARIO, startX, startY, world) {
    ;
}


void Mario::doSomething() {
    if (commonFlag()) {
        returnWorld()->setFinishGame(true);
    }
}



Pipe::Pipe(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, 0, 2, 1.0, world) {
    ;
}


void Pipe::doSomething() {
    ;
}

void Pipe::getBonked() {
    returnWorld()->playSound(SOUND_PLAYER_BONK);

}



Block::Block(int ImageID, int startX, int startY, bool hasStar, bool hasFlower, bool hasMushroom, StudentWorld* world)
    : Pipe(IID_BLOCK, startX, startY, world) {
    if (hasStar || hasFlower || hasMushroom) {
        hasGoodies = true;
    }
    m_hasStar = hasStar;
    m_hasFlower = hasFlower;
    m_hasMushroom = hasMushroom;
}


void Block::getBonked() {
    if (hasGoodies && !beenBonked) {
        returnWorld()->playSound(SOUND_POWERUP_APPEARS);
        beenBonked = true;
        returnWorld()->createPower(getX(), (getY() + 8), m_hasStar, m_hasFlower, m_hasMushroom);

    }
    else {
        returnWorld()->playSound(SOUND_PLAYER_BONK);
    }

}



Enemy::Enemy(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, 0, 2, 1.0, world) {
    int startDirection = randInt(0, 1);
    if (startDirection == 0) {
        setDirection(0);
    }
    else {
        setDirection(180);
    }
}


void Enemy::moveEnemy() {
    if (isAlive()) {
        if (returnWorld()->overlapPeach(getX(), getY())) {
            returnWorld()->getPeach()->getBonked();
            getBonked();

            return;
        }

        if (bonkPeach()) {
            return;
        }

        if (getDirection() == 180) {
            if (returnWorld()->overlapActor(getX() - 1, getY(), this)) {
                setDirection(0);

            }

        }
        else if (getDirection() == 0) {
            if (returnWorld()->overlapActor(getX() + 1, getY(), this)) {
                setDirection(180);
            }

        }

        if (getDirection() == 180) {
            if (!returnWorld()->overlapActor(getX() - 1, getY(), this)
                && returnWorld()->overlapActor(getX() - SPRITE_WIDTH, getY() - 1, this)) {
                ;
            }
            else {
                setDirection(0);
            }

        }
        else if (getDirection() == 0) {
            if (!returnWorld()->overlapActor(getX() + 1, getY(), this)
                && returnWorld()->overlapActor(getX() + SPRITE_WIDTH, getY() - 1, this)) {
                ;
            }
            else {
                setDirection(180);
            }

        }

        if (getDirection() == 180) {
            if (returnWorld()->overlapActor(getX() - 1, getY(), this)) {
                return;
            }
            else {
                moveTo(getX() - 1, getY());
            }
        }
        else  if (getDirection() == 0) {
            if (returnWorld()->overlapActor(getX() + 1, getY(), this)) {
                return;
            }
            else {
                moveTo(getX() + 1, getY());
            }
        }

    }
}

bool Enemy::bonkPeach() {
    if (isAlive()) {
        if (returnWorld()->overlapPeach(getX(), getY())) {
            returnWorld()->getPeach()->getBonked();
            return true;
        }
    }
    return false;
}

bool Enemy::getBonkedCommon() {
    if (isAlive()) {
        if (returnWorld()->overlapPeach(getX(), getY())) {
            if (returnWorld()->getPeach()->getStar()) {
                returnWorld()->playSound(SOUND_PLAYER_KICK);
                returnWorld()->increaseScore(100);
                return true;
            }
        }
    }
    return false;
}

bool Enemy::getDamagedCommon() {
    if (isAlive()) {
        returnWorld()->increaseScore(100);
        return true;
    }
    return false;
}

void Enemy::getBonked() {
    ;
}

void Enemy::getDamaged() {
    ;
}

void Enemy::doSomething() {
    ;
}


Goomba::Goomba(int ImageID, int startX, int startY, StudentWorld* world)
    : Enemy(IID_GOOMBA, startX, startY, world) {
    ;
}


void Goomba::getBonked() {
    if (getBonkedCommon()) {
        setAliveStatus(false);
    }
}

void Goomba::getDamaged() {
    if (getDamagedCommon()) {
        setAliveStatus(false);
    }
}

void Goomba::doSomething() {
    moveEnemy();
}


Koopa::Koopa(int ImageID, int startX, int startY, StudentWorld* world)
    : Enemy(IID_KOOPA, startX, startY, world) {
    ;
}


void Koopa::getBonked() {
    if (getBonkedCommon()) {
        if (getDirection() == 0) {
            returnWorld()->createProjectile(getX(), getY(), 0, false, false, true);
        }
        else {
            returnWorld()->createProjectile(getX(), getY(), 180, false, false, true);
        }
        setAliveStatus(false);
    }
}

void Koopa::getDamaged() {
    if (getDamagedCommon()) {
        if (getDirection() == 0) {
            returnWorld()->createProjectile(getX(), getY(), 0, false, false, true);
        }
        else {
            returnWorld()->createProjectile(getX(), getY(), 180, false, false, true);
        }
        setAliveStatus(false);
    }
}

void Koopa::doSomething() {
    moveEnemy();
}



Piranha::Piranha(int ImageID, int startX, int startY, StudentWorld* world)
    : Enemy(IID_PIRANHA, startX, startY, world) {

}


void Piranha::getBonked() {
    if (getBonkedCommon()) {
        setAliveStatus(false);
    }
}

void Piranha::getDamaged() {
    if (getDamagedCommon()) {
        setAliveStatus(false);
    }
}

void Piranha::doSomething() {
    if (isAlive()) {
        increaseAnimationNumber();
        if (bonkPeach()) {
            return;
        }

        if (returnWorld()->getPeach()->getY() <= getY() + (1.5 * SPRITE_HEIGHT) &&
            (returnWorld()->getPeach()->getY() >= getY() - (1.5 * SPRITE_HEIGHT))) {
            if (returnWorld()->getPeach()->getX() > getX()) {
                setDirection(0);
            }
            else {
                setDirection(180);
            }

            if (firing_delay > 0) {
                firing_delay--;
            }
            else {
                if (returnWorld()->getPeach()->getX() < getX() + (8 * SPRITE_WIDTH) && returnWorld()->getPeach()->getX() > getX() - (8 * SPRITE_WIDTH)) {

                    returnWorld()->createProjectile(getX(), getY(), getDirection(), false, true, false);

                    returnWorld()->playSound(SOUND_PIRANHA_FIRE);
                    firing_delay = 40;
                }
            }


        }

    }
}



Goodies::Goodies(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, 0, 1, 1.0, world) {
    ;
}


void Goodies::doSomething() {
    ;
}


void Goodies::commonGoodieDoSomething(bool hasStar, bool hasShoot, bool hasJump, int points) {
    moveGoodies();
    if (isAlive() && returnWorld()->overlapPeach(getX(), getY())) {
        returnWorld()->increaseScore(points);
        if (hasStar) {
            returnWorld()->getPeach()->setStarTime(150);
            returnWorld()->getPeach()->gainedStar();
        }

        if (hasShoot) {
            returnWorld()->getPeach()->setHitPoints(2);
            returnWorld()->getPeach()->gainedShoot();
        }

        if (hasJump) {
            returnWorld()->getPeach()->setHitPoints(2);
            returnWorld()->getPeach()->gainedJump();
        }
    }
}

void Goodies::moveGoodies() {
    if (isAlive()) {
        if (returnWorld()->overlapPeach(getX(), getY())) {
            setAliveStatus(false);
            returnWorld()->playSound(SOUND_PLAYER_POWERUP);
        }
        else {
            if (!returnWorld()->overlapActor(getX(), getY() - 2, this)) {
                moveTo(getX(), getY() - 2);
            }

            if (getDirection() == 180) {
                if (!returnWorld()->overlapActor(getX() - 2, getY(), this)) {

                    moveTo(getX() - 2, getY());

                }
                else {
                    setDirection(0);
                }

            }
            else if (getDirection() == 0) {
                if (!returnWorld()->overlapActor(getX() + 2, getY(), this)) {
                    moveTo(getX() + 2, getY());

                }
                else {
                    setDirection(180);
                }

            }
        }

    }
}

Star::Star(int ImageID, int startX, int startY, StudentWorld* world)
    : Goodies(IID_STAR, startX, startY, world) {
}


void Star::doSomething() {
    commonGoodieDoSomething(true, false, false, 100);
}

Flower::Flower(int ImageID, int startX, int startY, StudentWorld* world)
    : Goodies(IID_FLOWER, startX, startY, world) {
}


void Flower::doSomething() {
    commonGoodieDoSomething(false, true, false, 50);
}

Mushroom::Mushroom(int ImageID, int startX, int startY, StudentWorld* world)
    : Goodies(IID_MUSHROOM, startX, startY, world) {
}


void Mushroom::doSomething() {
    commonGoodieDoSomething(false, false, true, 75);
}




Projectiles::Projectiles(int imageID, int startX, int startY, int direction, StudentWorld* world)
    : Actor(imageID, startX, startY, direction, 1, 1.0, world) {
    ;
}


void Projectiles::doSomething() {

}

void Projectiles::moveProjectile() {
    if (!returnWorld()->overlapActor(getX(), getY() - 2, this)) {
        moveTo(getX(), getY() - 2);
    }

    if (getDirection() == 180) {
        if (returnWorld()->overlapActor(getX() - 2, getY(), this)) {
            setAliveStatus(false);
            return;


        }
        else {
            moveTo(getX() - 2, getY());
        }

    }
    else if (getDirection() == 0) {
        if (returnWorld()->overlapActor(getX() + 2, getY(), this)) {
            setAliveStatus(false);
            return;


        }
        else {
            moveTo(getX() + 2, getY());
        }

    }
}

void Projectiles::commonPeachFireShell() {
    if (isAlive()) {

        if (returnWorld()->damageWProjectile(getX(), getY(), this)) {
            setAliveStatus(false);
            return;
        }

        moveProjectile();
    }
}


PeachFireball::PeachFireball(int imageID, int startX, int startY, int direction, StudentWorld* world)
    : Projectiles(IID_PEACH_FIRE, startX, startY, direction, world) {
    ;
}

void PeachFireball::doSomething() {
    commonPeachFireShell();
}

PiranhaFireball::PiranhaFireball(int imageID, int startX, int startY, int direction, StudentWorld* world)
    : Projectiles(IID_PIRANHA_FIRE, startX, startY, direction, world) {
    ;
}


void PiranhaFireball::doSomething() {
    if (isAlive()) {
        if (returnWorld()->overlapPeach(getX(), getY())) {
            returnWorld()->getPeach()->getBonked();
            setAliveStatus(false);
            return;
        }

        moveProjectile();

    }
}

Shell::Shell(int imageID, int startX, int startY, int direction, StudentWorld* world)
    : Projectiles(IID_SHELL, startX, startY, direction, world) {
    ;
}


void Shell::doSomething() {
    commonPeachFireShell();
}



Peach::Peach(int startX, int startY, StudentWorld* world) :
    Actor(IID_PEACH, startX, startY, 0, 0, 1.0, world) {
    ;
}


void Peach::getBonked() {
    if (isAlive()) {

        if (m_hasStar || tempInvincibility) {
            return;
        }
        else {
            hitPoint--;
            tempInvincibility = true;
            temp_invncibility_time = 10;
            if (m_hasShoot) {
                m_hasShoot = false;
            }
            if (m_hasJump) {
                m_hasJump = false;
            }

            if (hitPoint >= 1) {
                returnWorld()->playSound(SOUND_PLAYER_HURT);
            }

            if (hitPoint <= 0) {
                setAliveStatus(false);
                returnWorld()->decLives();
            }
        }
    }
}

void Peach::doSomething() {
    int key;
    if (isAlive()) {

        if (m_hasStar) {
            star_time--;
            if (star_time == 0) {
                m_hasStar = false;
            }
        }

        if (tempInvincibility) {
            temp_invncibility_time--;
            if (temp_invncibility_time == 0) {
                tempInvincibility = false;
            }
        }

        if (recharge_fire_time > 0) {
            recharge_fire_time--;
        }

        if (initiateJump && remaining_jump_distance > 0) {
            if (!returnWorld()->overlapActor(getX(), getY() + 4, this)) {
                moveTo(getX(), getY() + 4);
                remaining_jump_distance--;
            }
            else {
                returnWorld()->generalBonk(getX(), getY() + 4);
                initiateJump = false;
                remaining_jump_distance = 0;
                return;
            }
        }

        else {
            if (!returnWorld()->overlapActor(getX(), getY() - 0, this) &&
                !returnWorld()->overlapActor(getX(), getY() - 1, this) &&
                !returnWorld()->overlapActor(getX(), getY() - 2, this) &&
                !returnWorld()->overlapActor(getX(), getY() - 3, this)) {
                moveTo(getX(), getY() - 4);
            }
        }

        returnWorld()->generalBonk(getX(), getY());

        if (returnWorld()->getKey(key)) {
            switch (key) {
            case KEY_PRESS_LEFT:
                setDirection(180);
                if (!returnWorld()->overlapActor(getX() - 4, getY(), this)) {
                    moveTo(getX() - 4, getY());
                }
                else {
                    returnWorld()->generalBonk(getX() - 4, getY());
                    return;
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                if (!returnWorld()->overlapActor(getX() + 4, getY(), this)) {
                    moveTo(getX() + 4, getY());
                }
                else {
                    returnWorld()->generalBonk(getX() + 4, getY());
                    return;

                }
                break;
            case KEY_PRESS_UP:
                initiateJump = true;
                if (returnWorld()->overlapActor(getX(), getY() - 1, this)) {
                    if (m_hasJump) {
                        remaining_jump_distance = 12;
                    }
                    else {
                        remaining_jump_distance = 8;
                    }
                    returnWorld()->playSound(SOUND_PLAYER_JUMP);
                }

                break;
            case KEY_PRESS_SPACE:
                if (m_hasShoot) {
                    if (!(recharge_fire_time > 0)) {
                        returnWorld()->playSound(SOUND_PLAYER_FIRE);
                        recharge_fire_time = 8;
                        if (getDirection() == 0) {
                            returnWorld()->createProjectile(getX() + 4, getY(), 0, true, false, false);
                        }
                        else {
                            returnWorld()->createProjectile(getX() - 4, getY(), 180, true, false, false);
                        }
                    }
                }
                break;
            }
        }
    }
}











