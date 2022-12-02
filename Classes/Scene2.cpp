#include "Scene2.h"
#include "Scene3.h"
#include "Player.h"
#include <cocostudio/SimpleAudioEngine.h>

USING_NS_CC;

Scene* Scene2::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Scene2::create();
    scene->addChild(layer);
    scene->addChild(Player::createNode());
    return scene;
}

bool Scene2::init()
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

    return true;
}
void Scene2::initMonster()
{
    _monster1 = Sprite::createWithSpriteFrameName("_sprite4.png");
    _monster1->setPosition(1650, 572);

    _monster1->setScale(2.8);
    auto Body = PhysicsBody::createCircle(_monster1->getContentSize().height / 2 - 40, PhysicsMaterial(PHYSICSBODY_MATERIAL_DEFAULT));
    Body->setContactTestBitmask(true);
    Body->setDynamic(true);
    _monster1->addComponent(Body);

    _monster1->getPhysicsBody()->setDynamic(false);
    _monster1->setTag(10);
    this->addChild(_monster1);

    //CC_REPEAT_FOREVER
    this->schedule(CC_SCHEDULE_SELECTOR(Scene2::initEnemyBullets), 1.8, CC_REPEAT_FOREVER, 1.5);
}
void Scene2::initEnemyBullets(float dt)
{
    int n = rand() % 700;
    int t = rand() % 3 + 1;
    int p = rand() % 6 + 2;
    auto rotate = RotateBy::create(4.0f, 1000.0f);
    sequence = Sequence::create(FadeIn::create(0.85f),
                            MoveBy::create(t, Vec2(-2000, n-200)),
                            CallFuncN::create(CC_CALLBACK_1(Scene2::doRemoveFromParentAndCleanup, this, true)),
                            nullptr);
    int r = rand() % 3 + 6;

    _eBullet = Sprite::createWithSpriteFrameName("_sprite" + std::to_string(r) + ".png");
    Player::initPhysic(_eBullet);
    _eBullet->setPosition(_monster1->getPositionX() - 170.0f, _monster1->getPositionY() - 100.0f);
    _eBullet->getPhysicsBody()->setGroup(-1);
    //_eBullet->getPhysicsBody()->setCollisionBitmask(-2);
    this->addChild(_eBullet);
    auto eRotate = Sequence::create(RotateBy::create(0.35f, 13.0f),
                                    DelayTime::create(0.4f),
                                    RotateBy::create(0.1f, -13.0f),
                                    nullptr);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/eShoot.MP3");
    _monster1->runAction(eRotate);
    _eBullet->setTag(20);
    _eBullet->runAction(rotate);
    _eBullet->runAction(sequence);
}
void Scene2::doRemoveFromParentAndCleanup(Node* sender, bool cleanup)
{
    sender->removeFromParentAndCleanup(cleanup);
}
void Scene2::initEdges()
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
void Scene2::initBackground()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    //_background = Sprite::create("Backgrounds/pxArt1.png");
    _background = Sprite::create("Backgrounds/pxArt2.png");
    _background->setTag(50);
    _background->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    this->addChild(_background);
}
void Scene2::changeBackground()
{
    auto tinTo4 = TintTo::create(1.2, 40, 50, 50);
    _background->runAction(tinTo4);
}
void Scene2::changeScene(Ref* pSender)
{
    auto scene = Scene3::createScene();
    //Director::getInstance()->pushScene(TransitionFade::create(1.5f,scene));
    Director::getInstance()->pushScene(TransitionCrossFade::create(0.7, scene));

    //TransitionTurnOffTiles
}