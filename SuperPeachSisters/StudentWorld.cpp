#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

#include "GraphObject.h"


GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    ostringstream oss;
    oss << getLevel();
    string s = oss.str();
    string level_file = "level0" + s + ".txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;

    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;

    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        Level::GridEntry ge;
        for (int x = 0; x < 32; x++)
            for (int y = 0; y < 32; y++)
            {
                ge = lev.getContentsOf(x, y);
                switch (ge)
                {
                case Level::empty:
                    break;

                case Level::peach:
                    m_peach = new Peach(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this);
                    break;

                case Level::block:
                    m_actors.push_back(new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, false, false, false, this));
                    break;

                case Level::pipe:
                    m_actors.push_back(new Pipe(IID_PIPE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;

                case Level::flag:
                    m_actors.push_back(new Flag(IID_FLAG, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;

                case Level::mario:
                    m_actors.push_back(new Mario(IID_MARIO, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;

                case Level::goomba:
                    m_actors.push_back(new Goomba(IID_GOOMBA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;

                case Level::koopa:
                    m_actors.push_back(new Koopa(IID_KOOPA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;

                case Level::piranha:
                    m_actors.push_back(new Piranha(IID_PIRANHA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;

                case Level::star_goodie_block:
                    m_actors.push_back(new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true, false, false, this)); 
                    break;

                case Level::flower_goodie_block:
                    m_actors.push_back(new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT,false, true, false, this));
                    break;

                case Level::mushroom_goodie_block:
                    m_actors.push_back(new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, false, false, true, this));
                    break;
                }
            }
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::createPower(int X, int Y, bool hasStar, bool hasFlower, bool hasMushroom) {
    if (hasStar) {
        m_actors.push_back(new Star(IID_STAR, X, Y, this));
    }
    if (hasFlower) {
        m_actors.push_back(new Flower(IID_FLOWER, X, Y, this));
    }
    if (hasMushroom) {
        m_actors.push_back(new Mushroom(IID_MUSHROOM, X, Y, this));
    }
}

void StudentWorld::createProjectile(int X, int Y, int direction, bool peachFireball, bool piranhaFireball, bool shell) {
    if (peachFireball) {
        m_actors.push_back(new PeachFireball(IID_PEACH_FIRE, X, Y, direction, this));
    }
    if (piranhaFireball) {
        m_actors.push_back(new PiranhaFireball(IID_PIRANHA_FIRE, X, Y, direction, this));
    }
    if (shell) {
        m_actors.push_back(new Shell(IID_SHELL, X, Y, direction, this));
    }
}

int StudentWorld::move()
{

    if (m_peach->isAlive()) {
        m_peach->doSomething();
    }

    list<Actor*>::iterator it;
    it = m_actors.begin();
    while ((it) != m_actors.end()) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();

            if (!m_peach->isAlive()) {
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }

            if (finishGame()) {
                playSound(SOUND_GAME_OVER);
                return GWSTATUS_PLAYER_WON;
            }

            if (getCompleteLevel()) {
                setCompleteLevel(false);
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }

        }
        it++;
    }

    //delete dead actors
    list<Actor*>::iterator it2;
    it2 = m_actors.begin();

    while ((it2) != m_actors.end()) {
        if (!(*it2)->isAlive()) {
            delete* it2;
            it2 = m_actors.erase(it2);
        }
        else
        {
            it2++;
        }
    }

    displayGameText();

    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::displayGameText() {
    ostringstream oss;
    oss << "Lives: " << getLives();
    oss << "  Level: ";
    oss.fill('0');
    oss << setw(2) << getLevel();
    oss << "  Points: ";
    oss.fill('0');
    oss << setw(6) << getScore(); 
    if (m_peach->getStar()) {
        oss << " StarPower!";
    }
    if (m_peach->getFlower()) {
        oss << " ShootPower!";
    }
    if (m_peach->getMushroom()) {
        oss << " JumpPower!";
    }
    string s = oss.str();
    setGameStatText(s);
}

void StudentWorld::generalBonk(int X, int Y) {
    list<Actor*>::iterator it;
    it = m_actors.begin(); 

    while (it != m_actors.end()) {

        if (checkActorsOverlap(X, Y, (*it)->getX(), (*it)->getY())) {
            (*it)->getBonked();
        }
       
        it++; 
    }
}

bool StudentWorld::checkActorsOverlap(int X1, int Y1, int X2, int Y2) {
    
    if (X1 >= X2 && X1 <= (X2 + SPRITE_WIDTH - 1) &&
            Y1 >= Y2 && Y1 <= (Y2 + SPRITE_HEIGHT - 1) //bottom left point 
        ||
        (X1 + SPRITE_WIDTH - 1 >= X2 && X1 + SPRITE_WIDTH - 1 <= (X2 + SPRITE_WIDTH - 1) &&
            Y1 >= Y2 && Y1 <= (Y2 + SPRITE_HEIGHT - 1)) //bottom right point  
        ||
        (X1 >= X2 && X1 <= (X2 + SPRITE_WIDTH - 1) &&
            Y1 + SPRITE_HEIGHT - 1 >= Y2 && Y1 + SPRITE_HEIGHT - 1 <= (Y2 + SPRITE_HEIGHT - 1)) //top right point 
        ||
        (X1 + SPRITE_WIDTH - 1 >= X2 && X1 + SPRITE_WIDTH - 1 <= (X2 + SPRITE_WIDTH - 1) &&
            Y1 + SPRITE_HEIGHT - 1 >= Y2 && Y1 + SPRITE_HEIGHT - 1 <= (Y2 + SPRITE_HEIGHT - 1))) //top left point 
        {
        return true; 
        }

    return false; 
}

bool StudentWorld::damageWProjectile(int X, int Y, Actor* ptr) {
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if ((*it) == ptr) { //if they are the same item, then increment it and continue
            it++;
            continue;
        }
        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y)) {
            if ((*it)->damagable() && (*it)->isAlive()) {
                (*it)->getDamaged(); 
                return true;
            }
        }
        it++;
    }
    return false; 
}

bool StudentWorld::overlapPeach(int X, int Y) {

    return checkActorsOverlap( X, Y, m_peach->getX(), m_peach->getY());

}

bool StudentWorld::overlapActor(int X, int Y, Actor* ptr) {

    list<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {

        if ((*it) == ptr) { 

            it++;
            continue;
        }

        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y)) {
            if (!(*it)->canOverlap()) {
                return true;
            }
            
        }

        it++;
    }
    return false;
}



void StudentWorld::cleanUp()
{
    //iterate through actor list and delete each one
    list<Actor*>::iterator it;
    it = m_actors.begin();

    while (it != m_actors.end()) {
        delete* it;
        it = m_actors.erase(it);
    }

    delete m_peach;
}







