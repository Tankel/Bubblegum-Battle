#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Scene1.h"
#include "HelloWorldScene.h"

using namespace cocos2d;
extern int pLife;
extern int maxScore;
//extern bool isPaused;

class Player : public cocos2d::Layer
{
    //Player() {};
public:
    //int iScore=0;
    UserDefault* userdefaults;
    int musicPlaying;
    int damage;
    int eLife;
    int auxiLife;
    int wEffect;
    int gameOver;
    int power;
    int vel;
    int jumph;
    bool hasPower;
    ProgressTimer* barLife;
    ProgressTimer* rechargeBar;
    //int contScene = 0;
    static cocos2d::Node* createNode();
    virtual bool init();
    void keyPressed(EventKeyboard::KeyCode, Event*);
    void keyReleased(EventKeyboard::KeyCode, Event*);
    void mouseClick(Event*);
    void doRemoveFromParentAndCleanup(Node*, bool);
    static void initPhysic(Sprite*);
    void initPlayer();
    void initPhysicPlayer();
    void initBullet();
    void runBullet();
    bool OnContactBegin(cocos2d::PhysicsContact&);
    void initController();
    void initLife();
    void initScore();
    void darkBackground();
    void whiteBackground();
    void nextScene(float);
    void loseMenu();
    void winEffect(float dt);
    void recoverLife(float dt);
    void returnDamage(float dt);
    void returnVel(float dt);
    void returnJump(float dt);
    void youWin();
    void initRecharge();
    void timer(float);
    void plusScore(float);
    float contTime;
    cocos2d::MenuItemImage* homeMenu;
    cocos2d::MenuItemImage* againMenu;
    void returnHome(Ref* pSender);
    void playAgain(Ref* pSender);
    bool leftKeyPressed = false;
    bool rightKeyPressed = false;
    bool downKeyPressed = false;
    Vector<Sprite*>_hearts;
    Sprite* _Fbackground;
    Sprite* _Fbackground2;
    Sprite* _lifebackground;
    Sprite* _xLife;
    Label* score;
    Label* plus;
    Vec2 origin;
    Vec2 visibleSize;
    CREATE_FUNC(Player);
private:
    //static std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> keys;
    Sprite* _player{};
    Sprite* _bullet{};
    RotateBy* rotate;
    Sequence* sequence;
    static cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char*, int, int);
public:
    virtual void update(float delta) override;
};
#endif