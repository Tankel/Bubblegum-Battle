#pragma once

#ifndef __SCORE_H__
#define __SCORE_H__

#include "cocos2d.h"

extern int contScene;
extern int iScore;
class Score : public cocos2d::Scene
{
public:
    static Scene* createScene();
    virtual bool init();
        
    cocos2d::MenuItemImage* homeMenu;
    void returnHome(Ref* pSender);
    //void initMusic();
    void initBackground();
    CREATE_FUNC(Score);
private:
};
#endif