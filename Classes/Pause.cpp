#include "Pause.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Player.h"
#include <cocostudio/SimpleAudioEngine.h>

Scene* Pause::createScene()
{
    return Pause::create();
}

bool Pause::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //initMusic();
    initBackground();

    continueMenu = MenuItemImage::create("MENU/_menu19.png", "MENU/_menu20.png", CC_CALLBACK_1(Pause::continueScene, this));
    homeMenu = MenuItemImage::create("MENU/_menu10.png", "MENU/_menu11.png", CC_CALLBACK_1(Pause::returnHome, this));

    Menu* menu = Menu::create(continueMenu, homeMenu, nullptr);
    menu->setPosition(Point::ZERO);
    continueMenu->setPosition(visibleSize.width / 2 + origin.x +170, visibleSize.height / 2 + origin.y-70);
    homeMenu->setPosition(visibleSize.width / 2 + origin.x -170, visibleSize.height / 2 + origin.y-70);
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
void Pause::returnHome(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
    auto scene = HelloWorld::createScene();
    //Director::getInstance()->pushScene(TransitionFade::create(1.5f,scene));
    Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.4, scene));
}
void Pause::continueScene(Ref* pSender)
{
    Director::getInstance()->popScene();
}
void Pause::initBackground()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* _background = Sprite::create("Backgrounds/pxArt1_.png");
    switch (contScene)
    {
    case 0:
    {
        _background->setTexture("Backgrounds/pxArt1_.png");
        break;
    }
    case 1:
    {
        _background->setTexture("Backgrounds/pxArt2_.png");
        break;
    }
    case 2:
    {
        _background->setTexture("Backgrounds/pxArt3_.png");
        break;
    }
    }
    //Sprite* _title = Sprite::create("MENU/_menu6.png");
    //_background->runAction(TintTo::create(0,60,60,60));
    _background->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    this->addChild(_background);
    auto label = Label::createWithTTF("PAUSED","fonts/8Bit.ttf", 120);
    label->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2 + 330);
    this->addChild(label);
}