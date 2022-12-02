#include "HelloWorldScene.h"
#include "Player.h"
#include "Scene1.h"
#include <cocostudio/SimpleAudioEngine.h>

USING_NS_CC;

int contScene = 0;
int iScore = 0;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
    /*
    auto scene = Scene1::createScene();
    //scene->getPhysicsWorld()->setGravity(Vec2(0,0));
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;*/
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    initMusic();
    initBackground();

    startMenu = MenuItemImage::create("MENU/_menu0.png","MENU/_menu1.png", CC_CALLBACK_1(HelloWorld::changeScene,this));
    exitMenu = MenuItemImage::create("MENU/_menu2.png", "MENU/_menu3.png", CC_CALLBACK_1(HelloWorld::exitGame, this));

    Menu* menu = Menu::create(startMenu, exitMenu, nullptr);
    menu->setPosition(Point::ZERO);
    startMenu->setPosition(visibleSize.width / 2 + origin.x + 520, visibleSize.height / 2 + origin.y + 300);
    exitMenu->setPosition(visibleSize.width / 2 + origin.x + 520, visibleSize.height / 2 + origin.y + 100);
    this->addChild(menu);
    auto author2 = Label::createWithTTF("by", "fonts/NineteenNinetySeven.ttf", 35);
    auto author = Label::createWithTTF("Luis Reyes", "fonts/NineteenNinetySeven.ttf", 55);
    author2->setPosition(80,80);
    author->setPosition(334,70);
    this->addChild(author2);
    this->addChild(author);

    return true;
}

void HelloWorld::initMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/Menu.MP3",true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2);
}
void HelloWorld::changeScene(Ref *pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/blipSelect.mp3");
    pLife = 5;
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    auto scene = Scene1::createScene();
    //Director::getInstance()->pushScene(TransitionFade::create(1.5f,scene));
    Director::getInstance()->replaceScene(TransitionProgressHorizontal::create(0.4,scene));
    
        //TransitionTurnOffTiles
}
void HelloWorld::exitGame(Ref* pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/blipSelect.mp3");
    //Director::getInstance()->end();
    exit(0);
}
void HelloWorld::initBackground()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* _background = Sprite::create("Backgrounds/_menu.png");
    Sprite* _title = Sprite::create("MENU/_menu6.png");
    //_background->runAction(TintTo::create(0,60,60,60));
    _background->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    _title->setPosition(origin.x + visibleSize.x / 2 - 350, origin.y + visibleSize.y / 2);
    _title->setScale(2.4);
    this->addChild(_background);
    this->addChild(_title);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("_SPRITES.plist");
    auto _character = Sprite::createWithSpriteFrameName("_sprite14.png");
    _character->setFlippedX(true);
    _character->setScale(8);
    _character->setPosition(origin.x + visibleSize.x / 2 + 540, origin.y + visibleSize.y / 2 - 440);
    _character->setRotation(15);
    auto r2 = RotateBy::create(2.7f, -30);
    auto r3 = RotateBy::create(2.7f, 30);
    auto sequqnce = Sequence::create(r2,r3,nullptr);
    auto repeaty = RepeatForever::create(sequqnce);
    this->addChild(_character);
    _character->runAction(repeaty);
}