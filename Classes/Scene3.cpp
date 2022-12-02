#include "Scene3.h"
#include "Player.h"
#include <cocostudio/SimpleAudioEngine.h>

USING_NS_CC;

Scene* Scene3::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Scene3::create();
    scene->addChild(layer);
    scene->addChild(Player::createNode());
    return scene;
}

bool Scene3::init()
{
    if (!Layer::init())
    {
        return false;
    }
    srand(time(NULL));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("_SPRITES.plist");
    initBackground();
    initMonster();
    initEdges();
    initMusic();

    return true;
}
void Scene3::initMonster()
{
    _monster1 = Sprite::createWithSpriteFrameName("_sprite9.png");
    _monster1->setPosition(1650, 533);

    _monster1->setScale(2.8);
    auto Body = PhysicsBody::createBox(Size(_monster1->getContentSize().width-26, _monster1->getContentSize().height), PhysicsMaterial(PHYSICSBODY_MATERIAL_DEFAULT));
    Body->setContactTestBitmask(true);
    Body->setDynamic(true);
    _monster1->addComponent(Body);

    _monster1->getPhysicsBody()->setDynamic(false);
    _monster1->setTag(10);
    this->addChild(_monster1);

    //CC_REPEAT_FOREVER
    this->schedule(CC_SCHEDULE_SELECTOR(Scene3::initEnemyBullets), 1.9, CC_REPEAT_FOREVER, 1.5);
}
void Scene3::initMusic()
{
}
void Scene3::initEnemyBullets(float dt)
{
    int n = rand() % 500;
    int t = rand() % 3 + 3;
    int p = rand() % 6 + 2;
    auto rotate = RotateBy::create(4.0f, 1000);
    auto rotate2 = RotateBy::create(0.6f, rand()%30-15);
    sequence = Sequence::create(FadeIn::create(0.85f),
                                JumpBy::create(t, Vec2(-2000, 0), n+100, p),
                                CallFuncN::create(CC_CALLBACK_1(Scene3::doRemoveFromParentAndCleanup, this, true)),
                                nullptr);
    auto sequence2 = Sequence::create(FadeIn::create(0.85f),
                                MoveBy::create(t-1, Vec2(-2000, n)),
                                CallFuncN::create(CC_CALLBACK_1(Scene3::doRemoveFromParentAndCleanup, this, true)),
                                nullptr);
    auto sequence3 = Sequence::create(FadeIn::create(0.85f),
                                MoveBy::create(5.7f, Vec2(-2000, 0)),
                                CallFuncN::create(CC_CALLBACK_1(Scene3::doRemoveFromParentAndCleanup, this, true)),
                                nullptr);

    int r = rand() % 3 + 11;
    _eBullet = Sprite::createWithSpriteFrameName("_sprite" + std::to_string(r) + ".png");
    Player::initPhysic(_eBullet);
    _eBullet->setPosition(_monster1->getPositionX() - 190, _monster1->getPositionY() - 170);
    _eBullet->getPhysicsBody()->setGroup(-1);
    //_eBullet->getPhysicsBody()->setCollisionBitmask(-2);
    this->addChild(_eBullet);

    auto eRotate = Sequence::create(RotateBy::create(0.35f, 13.0f),
        DelayTime::create(0.4f),
        RotateBy::create(0.1f, -13.0f),
        nullptr);
    _monster1->runAction(eRotate);

    _eBullet->setTag(20);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/eShoot.MP3");
    if (r == 12)
    {
        //guacamalo
        _eBullet->runAction(rotate);
        _eBullet->runAction(sequence2);
    }
    else if (r == 11)
    {
        //burrito
        _eBullet->setPosition(_monster1->getPositionX() - 80, _monster1->getPositionY() - 210);
        _eBullet->runAction(sequence3);
    }
    else
    {
        //bolas de carne
        _eBullet->runAction(rotate);
        _eBullet->runAction(sequence);
    }
}
void Scene3::doRemoveFromParentAndCleanup(Node* sender, bool cleanup)
{
    sender->removeFromParentAndCleanup(cleanup);
}
void Scene3::initEdges()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    //auto edgeBox = PhysicsBody::createEdgeBox(Size(2048, 1536), PhysicsMaterial(0.1f, 1.0f, 0.0f),3.0f, Vec2(tam.width/ 2, tam.height / 2));
    //this->addChild(edgeBox);
    auto edgeBody = PhysicsBody::createEdgeBox(Size(2600, 1200), PhysicsMaterial(1, 0, 1), 20); //cuadro
    auto edgeBody2 = PhysicsBody::createEdgeSegment(Vec2(0, 240), Vec2(1920, 240), PhysicsMaterial(1, 0, 1), 50); //piso
    //auto edgeBody3 = PhysicsBody::createEdgeSegment(Vec2(1500, 0), Vec2(1500, 1200), PhysicsMaterial(1, 0, 1), 20); //pared enemigo
    auto edgeNode = Node::create();
    auto edgeNode2 = Node::create();
    //auto edgeNode3 = Node::create();
    edgeNode->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    //edgeNode2->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    edgeNode2->setPhysicsBody(edgeBody2);
    //edgeNode3->setPhysicsBody(edgeBody3);
    edgeNode2->setTag(12);
    this->addChild(edgeNode);
    this->addChild(edgeNode2);
    //this->addChild(edgeNode3);
}
void Scene3::initBackground()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    //_background = Sprite::create("Backgrounds/pxArt1.png");
    _background = Sprite::create("Backgrounds/pxArt3.png");
    _background->setTag(50);
    _background->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    this->addChild(_background);
}
void Scene3::changeBackground()
{
    auto tinTo4 = TintTo::create(1.2, 40, 50, 50);
    _background->runAction(tinTo4);
}
void Scene3::changeScene(Ref* pSender)
{
    auto scene = HelloWorld::createScene();
    //Director::getInstance()->pushScene(TransitionFade::create(1.5f,scene));
    Director::getInstance()->pushScene(TransitionCrossFade::create(0.6, scene));

    //TransitionTurnOffTiles
}