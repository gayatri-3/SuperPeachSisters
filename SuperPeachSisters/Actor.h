#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

class Actor : public GraphObject { //Abstract Base Class every other actor is derived from 
public:
	Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	StudentWorld* returnWorld();
	bool isAlive() const{
		return m_isAlive;
	}
	void setAliveStatus(bool status) {
		m_isAlive = status;
	}
	virtual bool canOverlap() const {
		return true;
	}
	bool returnBonked() const{
		return m_bonked;
	}
	void setBonked(bool status) {
		m_bonked = status;
	}
	virtual bool damagable() const {
		return false;
	}
	virtual void getBonked();
	virtual void getDamaged();
	virtual void doSomething() = 0;
private:
	StudentWorld* m_studentWorld;
	bool m_isAlive = true;
	bool m_bonked = false;

};




class Flag : public Actor {
public:
	Flag(int imageID, int startX, int startY, StudentWorld* world);
	bool commonFlag();

	virtual void doSomething();
private:
};

class Mario : public Flag {
public:
	Mario(int imageID, int startX, int startY, StudentWorld* world);

	virtual void doSomething();
private:
};





class Pipe : public Actor {
public:
	Pipe(int imageID, int startX, int startY, StudentWorld* world);
	virtual bool canOverlap() const {
		return false;
	}

	virtual void doSomething();
	virtual void getBonked();
private:
};

class Block : public Pipe {
public:
	Block(int imageID, int startX, int startY, bool hasStar, bool hasFlower, bool hasMushroom, StudentWorld* world);

	virtual void getBonked();
private:
	bool hasGoodies = false;
	bool beenBonked = false;
	bool m_hasStar = false;
	bool m_hasFlower = false;
	bool m_hasMushroom = false;
};





class Enemy : public Actor { //Abstract Base Class that Goomba, Koopa, and Piranha are derived from 
public:
	Enemy(int imageID, int startX, int startY, StudentWorld* world);
	void moveEnemy();
	bool bonkPeach();

	bool getBonkedCommon();
	bool getDamagedCommon();
	virtual void getBonked() = 0;
	virtual void getDamaged() = 0;
	virtual void doSomething();
	virtual bool damagable() const {
		return true;
	}
private:
};

class Goomba : public Enemy {
public:
	Goomba(int imageID, int startX, int startY, StudentWorld* world);

	virtual void doSomething();
	virtual void getBonked();
	virtual void getDamaged();
private:
};

class Koopa : public Enemy {
public:
	Koopa(int imageID, int startX, int startY, StudentWorld* world);

	virtual void doSomething();
	virtual void getBonked();
	virtual void getDamaged();
private:
};

class Piranha : public Enemy {
public:
	Piranha(int imageID, int startX, int startY, StudentWorld* world);

	virtual void doSomething();
	virtual void getBonked();
	virtual void getDamaged();
private:
	int firing_delay = 0;
};



class Goodies : public Actor { //Abstract Base Class that Star, Flower, and Mushroom are derived from 
public:
	Goodies(int imageID, int startX, int startY, StudentWorld* world);

	void commonGoodieDoSomething(bool hasStar, bool hasShoot, bool hasJump, int points);
	virtual void doSomething() = 0;
	void moveGoodies();
private:
};

class Star : public Goodies {
public:
	Star(int imageID, int startX, int startY, StudentWorld* world);

	virtual void doSomething();
private:
};

class Flower : public Goodies {
public:
	Flower(int imageID, int startX, int startY, StudentWorld* world);

	virtual void doSomething();
private:
};

class Mushroom : public Goodies {
public:
	Mushroom(int imageID, int startX, int startY, StudentWorld* world);

	virtual void doSomething();
private:
};



class Projectiles : public Actor { //Abstract Base Class that PeachFireball, PiranhaFireball, and Shell are derived from 
public:
	Projectiles(int imageID, int startX, int startY, int direction, StudentWorld* world);

	void moveProjectile();
	void commonPeachFireShell();
	virtual void doSomething() = 0;
private:
};

class PeachFireball : public Projectiles {
public:
	PeachFireball(int imageID, int startX, int startY, int direction, StudentWorld* world);

	virtual void doSomething();
private:
};

class PiranhaFireball : public Projectiles {
public:
	PiranhaFireball(int imageID, int startX, int startY, int direction, StudentWorld* world);

	virtual void doSomething();
private:
};

class Shell : public Projectiles {
public:
	Shell(int imageID, int startX, int startY, int direction, StudentWorld* world);

	virtual void doSomething();
private:
};



class Peach : public Actor {
public:
	Peach(int startX, int startY, StudentWorld* world);

	void getBonked();
	void setHitPoints(int n) {
		hitPoint = n;
	}
	
	void gainedStar() {
		m_hasStar = true;
	}
	void gainedShoot() {
		m_hasShoot = true;
	}
	void gainedJump() {
		m_hasJump = true;
	}
	void setStarTime(int n) {
		star_time = n;
	}
	virtual void doSomething();
	bool getStar() const {
		return m_hasStar;
	}
	bool getMushroom() const{
		return m_hasJump;
	}
	bool getFlower() const{
		return m_hasShoot;
	}
private:
	int hitPoint = 1;
	bool tempInvincibility = false;
	bool m_hasStar = false;
	bool m_hasShoot = false;
	bool m_hasJump = false;
	bool initiateJump = false;
	int remaining_jump_distance = 0;
	int recharge_fire_time = 0;
	int star_time = 0;
	int temp_invncibility_time = 0;
};


#endif // ACTOR_H_





