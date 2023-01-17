#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>

#include <sstream>
#include <iomanip>

#include <list>
#include "Actor.h"

class GraphObject;
class Peach;
class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	Peach* getPeach() {
		return m_peach;
	}
	bool getCompleteLevel() const {
		return m_completeLevel;
	}
	void setCompleteLevel(bool status) {
		m_completeLevel = status;
	}
	bool finishGame() const {
		return m_finishGame;
	}
	void setFinishGame(bool status) {
		m_finishGame = status;
	}
	bool overlapPeach(int X, int Y);
	bool overlapActor(int X, int Y, Actor* ptr);
	bool damageWProjectile(int X, int Y, Actor* ptr);
	bool checkActorsOverlap(int X1, int Y1, int X2, int Y2);
	void displayGameText();
	void generalBonk(int X, int Y);
	void createPower(int X, int Y, bool hasStar, bool hasFlower, bool hasMushroom);
	void createProjectile(int X, int Y, int direction, bool peachFireball, bool piranhaFireball, bool shell);

private:
	Peach* m_peach;
	std::list<Actor*> m_actors;
	bool m_completeLevel = false;
	bool m_finishGame = false;

};

#endif // STUDENTWORLD_H_







