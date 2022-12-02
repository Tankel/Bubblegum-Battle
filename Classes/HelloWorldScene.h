
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

extern int contScene;
extern int iScore;
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    cocos2d::MenuItemImage* startMenu;
    cocos2d::MenuItemImage* exitMenu;
    void initMusic();
    void changeScene(Ref *pSender);
    void exitGame(Ref* pSender);
    void initBackground();
    CREATE_FUNC(HelloWorld);
private:
};
#endif