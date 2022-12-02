#include "Scene1.h"
#include "Scene2.h"
#include "Player.h"
#include "Score.h"
#include <cocostudio/SimpleAudioEngine.h>

Scene* Score::createScene()
{
    return Score::create();
}

bool Score::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //initMusic();
    initBackground();

    auto homeMenu = MenuItemImage::create("MENU/_menu10.png", "MENU/_menu11.png", CC_CALLBACK_1(Score::returnHome, this));

    Menu* menu = Menu::create(homeMenu, nullptr);
    menu->setPosition(Point::ZERO);
    homeMenu->setPosition(100,100);
    this->addChild(menu);

    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch (keyCode)
        {
            case EventKeyboard::KeyCode::KEY_ESCAPE:
            {
                Director::getInstance()->popScene();
                break;
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    return true;
}
void Score::returnHome(Ref* pSender)
{
    auto scene = HelloWorld::createScene();
    //Director::getInstance()->pushScene(TransitionFade::create(1.5f,scene));
    Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.4, scene));
}
void Score::initBackground()
{
    auto userdefaults = cocos2d::UserDefault::getInstance();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* _background = Sprite::create("Backgrounds/_score.png");

    //Sprite* _title = Sprite::create("MENU/_menu6.png");
    //_background->runAction(TintTo::create(0,60,60,60));
    _background->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    this->addChild(_background);

    auto score = Label::createWithTTF("HIGH SCORE:", "fonts/8Bit.ttf", 120);
    score->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2 + 200);
    this->addChild(score);

    auto pressKey = Label::createWithTTF(std::to_string(userdefaults->getIntegerForKey("FScore")), "fonts/NineteenNinetySeven.ttf", 400);
    pressKey->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2 - 100);
    this->addChild(pressKey);
}