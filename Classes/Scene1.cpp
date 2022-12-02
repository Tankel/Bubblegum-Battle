#include "Scene1.h"
#include "Scene2.h"
#include "Player.h"
#include <cocostudio/SimpleAudioEngine.h>

USING_NS_CC;

Scene* Scene1::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Scene1::create();
    scene->addChild(layer);
    scene->addChild(Player::createNode());
    return scene;
}

bool Scene1::init()
{
    if (!Layer::init())
    {
        return false;
    }
    srand(time(NULL));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("_SPRITES.plist");
    initBackground();
    initEdges();
    initMonster();
    initMusic();
    this->scheduleUpdate();
    return true;
}
void Scene1::initMonster()
{
    _monster1 = Sprite::createWithSpriteFrameName("_sprite0.png");
    _monster1->setPosition(1650, 508);

    Player::initPhysic(_monster1);
    _monster1->getPhysicsBody();
    //_monster1->getPhysicsBody()->setGroup(-2);
    //_monster1->getPhysicsBody()->setCollisionBitmask(2);
    _monster1->getPhysicsBody()->setDynamic(false);
    _monster1->setTag(10);
    this->addChild(_monster1);

    //CC_REPEAT_FOREVER
    //this->schedule(CC_SCHEDULE_SELECTOR(Scene1::initEnemyBullets), 2.6f, CC_REPEAT_FOREVER, 1.5f);
    this->schedule(CC_SCHEDULE_SELECTOR(Scene1::initEnemyBullets), 2.6f, CC_REPEAT_FOREVER, 1.5f);
}
void Scene1::initEnemyBullets(float dt)
{
    if (!isPaused)
    {
        int n = rand() % 500 + 100;
        int t = rand() % 5 + 4;
        int p = rand() % 6 + 2;
        //auto b = BezierBy::create(4, );;
        auto rotate = RotateBy::create(6.0f, 1500);
        //auto jump = JumpBy::create(t, Vec2(-2000, 0), n, p);
        //auto move = MoveBy::create(t, Vec3(-2000,0,0));
        //auto move_ease_in = EaseBounceIn::create(move->clone());
        //auto move_ease_in_back = move_ease_in->reverse();
        int r = rand() % 2 + 2;
        ccBezierConfig bezier;
        bezier.controlPoint_1 = Point(-500 + rand() % 700, 200 + (10 * p));
        bezier.controlPoint_2 = Point(-600 + rand() % 900, 1000 + n);
        bezier.endPosition = Point(_monster1->getPositionX() - 100, _monster1->getPositionY() - 100);
        _eBullet = Sprite::createWithSpriteFrameName("_sprite" + std::to_string(r) + ".png");
        if (r == 2)
        {
            initPhysicPickle();
        }
        else
        {
            initPhysicFrie();
        }
        _eBullet->setOpacity(0.0f);
        sequence = Sequence::create(FadeIn::create(0.85f),
            JumpBy::create(t, Vec2(-2000, 0), n, p),
            CallFuncN::create(CC_CALLBACK_1(Scene1::doRemoveFromParentAndCleanup, this, true)),
            nullptr);
        auto sequence2 = Sequence::create(FadeIn::create(0.85f),
            BezierTo::create(t - 2.8f, bezier),
            CallFuncN::create(CC_CALLBACK_1(Scene1::doRemoveFromParentAndCleanup, this, true)),
            nullptr);
        //Player::initPhysic(_eBullet);
        //_eBullet->setAnchorPoint(Vec2(0, 0));
        _eBullet->setPosition(_monster1->getPositionX() - 200, _monster1->getPositionY() - 180);
        _eBullet->getPhysicsBody()->setGroup(-1);
        //_eBullet->getPhysicsBody()->setCollisionBitmask(-2);
        this->addChild(_eBullet);

        _eBullet->setTag(20);
        _eBullet->runAction(rotate);
        //_monster1->runAction(RotateBy::create(0.3f, -13));
        auto eRotate = Sequence::create(RotateBy::create(0.3f, 13.0f),
            DelayTime::create(0.4),
            RotateBy::create(0.15f, -13.0f),
            nullptr);
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/eShoot.MP3");
        _monster1->runAction(eRotate);
        if (r == 3)
        {
            _eBullet->runAction(sequence2);
        }
        else
        {
            _eBullet->runAction(sequence);
        }
    }
}
void Scene1::initMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/Boss1.MP3", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.08);
}
void Scene1::doRemoveFromParentAndCleanup(Node* sender, bool cleanup)
{
    sender->removeFromParentAndCleanup(cleanup);
}
void Scene1::initEdges()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    //auto edgeBox = PhysicsBody::createEdgeBox(Size(2048, 1536), PhysicsMaterial(0.1f, 1.0f, 0.0f),3.0f, Vec2(tam.width/ 2, tam.height / 2));
    //this->addChild(edgeBox);
    auto edgeBody = PhysicsBody::createEdgeBox(Size(2600, 1200), PhysicsMaterial(1, 0, 1), 20); //cuadro
    auto edgeBody2 = PhysicsBody::createEdgeSegment(Vec2(0, 250), Vec2(1920, 250), PhysicsMaterial(1, 0, 1), 50); //piso
    auto edgeBody3 = PhysicsBody::createEdgeSegment(Vec2(1500, 0), Vec2(1500, 1200), PhysicsMaterial(1, 0, 1), 20); //pared enemigo
    auto edgeNode = Node::create();
    auto edgeNode2 = Node::create();
    auto edgeNode3 = Node::create();
    edgeNode->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    //edgeNode2->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    edgeNode2->setPhysicsBody(edgeBody2);
    edgeNode3->setPhysicsBody(edgeBody3);
    edgeNode2->setTag(12);
    this->addChild(edgeNode);
    this->addChild(edgeNode2);
    this->addChild(edgeNode3);
}
void Scene1::initBackground()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    //_background = Sprite::create("Backgrounds/pxArt1.png");
    _background = Sprite::create("Backgrounds/pxArt1.png");
    _background->setTag(50);
    _background->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    this->addChild(_background);
}
void Scene1::initPhysicFrie()
{
    _eBullet->setScale(2.3);

    auto Body = PhysicsBody::createBox(Size(_eBullet->getContentSize().width, _eBullet->getContentSize().height), PhysicsMaterial(PHYSICSBODY_MATERIAL_DEFAULT));
    Body->setContactTestBitmask(true);
    Body->setDynamic(true);
    _eBullet->addComponent(Body);
}
void Scene1::initPhysicPickle()
{
    _eBullet->setScale(1.6);
    auto Body = PhysicsBody::createCircle(_eBullet->getContentSize().height / 2, PhysicsMaterial(PHYSICSBODY_MATERIAL_DEFAULT));
    Body->setContactTestBitmask(true);
    Body->setDynamic(true);
    _eBullet->addComponent(Body);
}
void Scene1::changeBackground()
{
    auto tinTo4 = TintTo::create(1.2,40,50,50);
    _background->runAction(tinTo4);
}