#pragma once

#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"

extern int contScene;
extern int iScore;
class Pause : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    cocos2d::MenuItemImage* continueMenu;
    cocos2d::MenuItemImage* homeMenu;
    //void initMusic();
    void continueScene(Ref* pSender);
    void returnHome(Ref* pSender);
    void initBackground();
    CREATE_FUNC(Pause);
private:
};
#endif