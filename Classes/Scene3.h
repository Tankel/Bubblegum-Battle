#pragma once

#ifndef __SCENE_3_H__
#define __SCENE_3_H__

#include "cocos2d.h"

using namespace cocos2d;

extern Sprite* _background;
class Scene3 : public cocos2d::Layer
{
public:
    int eLife;
    static Scene* createScene();
    virtual bool init();
    void initEdges();
    void initMonster();
    void initBackground();
    void initEnemyBullets(float);
    void changeBackground();
    void initMusic();
    void doRemoveFromParentAndCleanup(Node*, bool);
    void changeScene(Ref* pSender);
    Vector<Sprite*>_hearts;
    Sequence* sequence;
    Sprite* _monster1{};
    Sprite* _eBullet{};
    Sprite* _background;
    CREATE_FUNC(Scene3);
private:
};
#endif