#include "Scene1.h"
#include "Scene2.h"
#include "Player.h"
#include "Victory.h"
#include <cocostudio/SimpleAudioEngine.h>

Scene* Victory::createScene()
{
    return Victory::create();
}

bool Victory::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //initMusic();
    initBackground();

    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
        auto scene = HelloWorld::createScene();
        //Director::getInstance()->pushScene(TransitionFade::create(1.5f,scene));
        Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.4, scene));
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    return true;
}
void Victory::initBackground()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* _background = Sprite::create("Backgrounds/_victory.png");
    auto userdefaults = cocos2d::UserDefault::getInstance();
    //Sprite* _title = Sprite::create("MENU/_menu6.png");
    //_background->runAction(TintTo::create(0,60,60,60));
    _background->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    this->addChild(_background);
    
    auto label = Sprite::create("MENU/_menu25.png");
    //
    label->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2 + 230);
    this->addChild(label);

    auto score = Label::createWithTTF("FINAL SCORE: "+ std::to_string(iScore), "fonts/8Bit.ttf", 90);
    score->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2 - 40);
    this->addChild(score);

    auto pressKey = Label::createWithTTF("Press any key to continue...", "fonts/NineteenNinetySeven.ttf", 60);
    pressKey->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2 - 180);
    auto fade = RepeatForever::create(Sequence::create(FadeOut::create(0.5), DelayTime::create(0.3), FadeIn::create(0.7),nullptr));
    pressKey->runAction(fade);
    this->addChild(pressKey);
}