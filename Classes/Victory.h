#pragma once

#ifndef __VICTORY_H__
#define __VICTORY_H__

#include "cocos2d.h"

extern int contScene;
extern int iScore;
class Victory : public cocos2d::Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    //void initMusic();
    void initBackground();
    CREATE_FUNC(Victory);
private:
};
#endif