#include "Player.h"
#include "HelloWorldScene.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Pause.h"
#include <cocostudio/SimpleAudioEngine.h>

USING_NS_CC;

int pLife = 5;
int nb = 18;
Node* Player::createNode()
{
    auto layer = Player::create();
    return layer;
}
bool Player::init() {
    if (!Layer::init())
    {
        return false;
    }
    damage = 1;
    vel = 600;
    jumph = 200;
    nb = 18;
    //contTime = 0;
    hasPower = false;
    gameOver = false;
    srand(time(NULL));
    origin = Director::getInstance()->getVisibleOrigin();
    visibleSize = Director::getInstance()->getVisibleSize();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("_SPRITES.plist");
    initScore();
    initLife();
    initPlayer();
    initController();   
    initRecharge();
    //this->schedule(CC_SCHEDULE_SELECTOR(Player::recoverLife), 1, CC_REPEAT_FOREVER, 1);
    this->schedule(CC_SCHEDULE_SELECTOR(Player::recoverLife), 15, CC_REPEAT_FOREVER, 15);
    this->scheduleUpdate();

    return true;
}
void Player::initRecharge()
{
    auto rechargeBarB = Sprite::create("MENU/_menu17.png");
    rechargeBar = ProgressTimer::create(Sprite::create("MENU/_menu18.png"));
    rechargeBar->setType(ProgressTimer::Type::BAR);
    rechargeBar->setReverseDirection(false);
    //barLife->setAnchorPoint(Vec2(0.0, 0.0));
    //barLife->setAnchorPoint(Vec2(0.0, 0.0));
    rechargeBar->setBarChangeRate(Vec2(1, 0));
    rechargeBar->setMidpoint(Vec2(0.0, 0.0));
    rechargeBar->setScale(1.3);
    rechargeBar->setPercentage(100);
    rechargeBar->setPosition(300, 140);
    rechargeBarB->setPosition(300, 140);
    rechargeBarB->setScale(1.3);
    this->addChild(rechargeBarB);
    this->addChild(rechargeBar);
}
void Player::initController()
{
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = CC_CALLBACK_2(Player::keyPressed, this);
    eventListener->onKeyReleased = CC_CALLBACK_2(Player::keyReleased, this);

    auto eventListener3 = EventListenerMouse::create();
    eventListener3->onMouseDown = CC_CALLBACK_1(Player::mouseClick, this);

    auto eventListener2 = EventListenerKeyboard::create();
    eventListener2->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
    {
        auto director = Director::getInstance();
        auto tam = director->getWinSize();
        auto jump = JumpBy::create(1, Vec2(0, 0), jumph, 1);
        //auto jump = MoveBy::create(1, Vec3(0,200,0));
        auto frames = getAnimation("_sprite%d.png", 3, 15);
        auto animation = Animation::createWithSpriteFrames(frames, 1.0f / 5);
        auto fire = RepeatForever::create(Animate::create(animation));
        fire->setTag(3);
        initBullet();
        //DelayTime* pause = DelayTime::create(4);
        Vec2 loc = _player->getPosition();
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W:
            if (!gameOver)
            {
                if (_player->getPositionY() < 900)
                {
                    if (_player->getPositionY() < 400)
                    {
                        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.15f);
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/Jump.mp3");
                    }
                    else
                    {
                        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.23f);
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/Fire.mp3");
                    }

                    _player->runAction(jump);
                }
                //_player->setPositionY(loc.y+100);
                _player->runAction(fire);
            }
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:
        {
            auto notBullets = Sequence::create(TintTo::create(0.13f, 180, 180, 180),
                TintTo::create(0.02f, 255, 255, 255),
                nullptr);
            if (!gameOver)
            {
                if (rechargeBar->getPercentage() > 0)
                {
                    runBullet();
                    //contTime++;
                    rechargeBar->setPercentage(rechargeBar->getPercentage() - 12.5);
                }
                else
                {
                    _player->runAction(notBullets);
                }
            }
            break;
        }
        case EventKeyboard::KeyCode::KEY_R:
        {
            if (!gameOver)
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.28);
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/Reload.mp3");
                this->schedule(CC_SCHEDULE_SELECTOR(Player::timer), 0.06, 8, 0.14);
            }
            break;
        }
        case EventKeyboard::KeyCode::KEY_E:
        {
            Vector<Node*> childs = this->getChildren();
            for (auto child : childs)
            {
                Sprite* sprite = (Sprite*)child;
                child->pause();
                //child->pause();
                //child->pauseSchedulerAndActions();
            }
            break;
        }
        case EventKeyboard::KeyCode::KEY_ESCAPE:
        {
            /*
            if (!gameOver)
                darkBackground();
            else
                whiteBackground();

            isPaused = !isPaused;
            gameOver = ~gameOver;
            */
            auto scene = Pause::createScene();
            Director::getInstance()->pushScene(TransitionCrossFade::create(0.1, scene));
                /*
                Vector<Node*> childs = this->getChildren();
                for (auto child : childs)
                {
                    Sprite* sprite = (Sprite*)child;
                    child->pause();
                    //child->pause();
                    //child->pauseSchedulerAndActions();
                }*/
                /*
                Director::getInstance()->pause();
                Director::getInstance()->resume();

                    for (auto nod : this->getChildren()) {

                        nod->getPhysicsBody()->setResting(true);
                    }*/
            break;
        }
        }

    };
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Player::OnContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener2, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener3, this);
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener2, this);
}

void Player::mouseClick(Event* event)
{
    initBullet();
    auto notBullets = Sequence::create(TintTo::create(0.13f, 180, 180, 180),
        TintTo::create(0.02f, 255, 255, 255),
        nullptr);
    EventMouse* e = (EventMouse*)event;
    switch (e->getMouseButton())
    {
    case EventMouse::MouseButton::BUTTON_LEFT:
        if (!gameOver)
        {
            if (rechargeBar->getPercentage() > 0)
            {
                runBullet();
                //contTime++;
                rechargeBar->setPercentage(rechargeBar->getPercentage() - 12.5);
            }
            else
            {
                _player->runAction(notBullets);
            }
        }
        break;
    }
}
void Player::timer(float delta)
{
    rechargeBar->setPercentage(rechargeBar->getPercentage() + 12.5);
}
void Player::runBullet()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.8);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/Shoot.mp3");
    _bullet->setPosition(_player->getPositionX() + (!_player->isFlippedX() ? 10 : -10), _player->getPositionY() + 30);
    this->addChild(_bullet,1,5);
    _bullet->runAction(rotate);
    _bullet->runAction(sequence);
}
void Player::initBullet()
{ 

    int n2 = rand() % 6 + 18;
    if (nb > 23)nb = 18;
    _bullet = Sprite::createWithSpriteFrameName("_sprite" + std::to_string(nb) + ".png");
    nb++;
    rotate = RotateBy::create(2.0f, 600);
    /*
    sequence = Sequence::create(MoveBy::create(2, Vec2(!_player->isFlippedX() ? 2048 : -2048, 400 + rand() % (200) - 100)),
        CallFuncN::create(CC_CALLBACK_1(Player::doRemoveFromParentAndCleanup, this, true)),
        nullptr);
        */
    sequence = Sequence::create(MoveBy::create(2, Vec2(!_player->isFlippedX() ? 2048 : -2048, 380)),
        CallFuncN::create(CC_CALLBACK_1(Player::doRemoveFromParentAndCleanup, this, true)),
        nullptr);
    initPhysic(_bullet);
    if(damage>1)_bullet->setScale(4.7);
    _bullet->getPhysicsBody()->setVelocity(Vect(100, -100));
    _bullet->getPhysicsBody()->setGroup(-1);
    //_bullet->removeFromParentAndCleanup(true);
    //_bullet->getPhysicsBody()->setCollisionBitmask(1);
}
void Player::doRemoveFromParentAndCleanup(Node* sender, bool cleanup)
{
    sender->removeFromParentAndCleanup(cleanup);
}
void Player::initPhysic(Sprite* _sprite)
{
    _sprite->setScale(2.8);
    auto Body = PhysicsBody::createCircle(_sprite->getContentSize().height / 2, PhysicsMaterial(PHYSICSBODY_MATERIAL_DEFAULT));
    Body->setContactTestBitmask(true);
    Body->setDynamic(true);
    _sprite->addComponent(Body);
}
void Player::initPlayer()
{
    //pLife = 5;
    auto director = Director::getInstance();
    auto tam = director->getWinSize();
    auto frames = getAnimation("_sprite%d.png", 1, 14);
    _player = Sprite::createWithSpriteFrame(frames.front());
    _player->setTag(30);
    //_player->setPosition(200, tam.height / 3.3);
    _player->setPosition(200, 344);
    _player->setScale(2.8);
    initPhysicPlayer();
}
void Player::initPhysicPlayer()
{
    auto Body = PhysicsBody::createBox(Size(_player->getContentSize().width, _player->getContentSize().width), PhysicsMaterial(PHYSICSBODY_MATERIAL_DEFAULT), Vec2(0, 12));
    Body->setContactTestBitmask(true);
    Body->setDynamic(false);
    _player->addComponent(Body);
    _player->getPhysicsBody()->setRotationEnable(false);
    //_player->getPhysicsBody()->setGravityEnable(true);
    //_player->getPhysicsBody()->setVelocity(Vec2(0, -400));
    _player->getPhysicsBody()->setGroup(5);
    //_player->getPhysicsBody()->setCollisionBitmask(-2);
    //_player->getPhysicsBody()->setCollisionBitmask(1);
    this->addChild(_player);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Player::OnContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}
void Player::update(float delta)
{
    /////////////////////////////////////
    Vec2 loc = _player->getPosition();
    if (loc.y < 400)
    {
        _player->stopActionByTag(3);
        _player->setSpriteFrame("_sprite14.png");
    }
    if (pLife > 0)
    {
        if (rightKeyPressed)
        {
            if (_player->getPositionX() < 1300)
                _player->setPositionX(loc.x + vel * delta);
        }
        if (leftKeyPressed)
        {
            if (_player->getPositionX() > 25)
                _player->setPositionX(loc.x - vel * delta);
        }
        
        if (downKeyPressed && loc.y < 400)
        {
            _player->setTexture("MENU/sprite_0.png");
            _player->getPhysicsBody()->setPositionOffset(Vec2(0,-150));
            //initPhysic(_player);
        }
        else if (downKeyPressed && loc.y >= 400)
        {
            _player->setSpriteFrame("_sprite14.png");
            _player->getPhysicsBody()->setPositionOffset(Vec2(0,0));
            //initPhysicPlayer();
            //_player->getPhysicsBody()->setPositionOffset(_player->getPhysicsBody()->getPositionOffset());
        }
        else
        {
            _player->getPhysicsBody()->setPositionOffset(Vec2(0, 0));
        }
        
    }
    //if(pLife==0)_player->runAction(tinTo3);
    //if(eLife==0)eLife->_monster1->runAction(tinTo3);
}
void Player::keyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_D:
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        if (!gameOver)
        {
            if (_player->getPositionY() < 400)
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.4f);
                wEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/Walk.mp3");
            }
            else
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(wEffect);
            }
            rightKeyPressed = true;
            _player->setFlippedX(false);
        }
        break;
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        if (!gameOver)
        {
            if (_player->getPositionY() < 400)
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.4f);
                wEffect = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/Walk.mp3");
            }
            else
            {
            CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(wEffect);
            }
            _player->setFlippedX(true);
            leftKeyPressed = true;
        }
        break;
    case EventKeyboard::KeyCode::KEY_S:
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_SHIFT:
        downKeyPressed = true;
        break;
    }
};
void Player::keyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(wEffect);
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            rightKeyPressed = false;
            break;
        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            leftKeyPressed = false;
            break;
        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_SHIFT:
            downKeyPressed = false;
            break;
    }
};
Vector<SpriteFrame*> Player::getAnimation(const char* format, int cont, int desde) //
{
    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for (int i = desde; i < cont + desde; i++)
    {
        sprintf(str, format, i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }
    return animFrames;
}
void Player::initLife()
{
    _lifebackground = Sprite::create("MENU/_menu15.png");
    barLife = ProgressTimer::create(Sprite::create("MENU/_menu16.png"));
    barLife->setType(ProgressTimer::Type::BAR);
    barLife->setReverseDirection(false);
    //barLife->setAnchorPoint(Vec2(0.0, 0.0));
    //barLife->setAnchorPoint(Vec2(0.0, 0.0));
    barLife->setBarChangeRate(Vec2(1, 0));
    barLife->setMidpoint(Vec2(0.0, 0.0));
    barLife->setScale(1.3);
    barLife->setPercentage(100);
    barLife->setPosition(1650, 800);
    _lifebackground->setPosition(1650, 800);
    _lifebackground->setScale(1.3);
    this->addChild(_lifebackground);
    this->addChild(barLife);
    //Sprite* sp = Sprite::createWithTexture(Texture2D::drawAtPoint(Vec2(120,1100),2.0f));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("_SPRITES.plist");
    eLife = 50 + contScene*20;
    _hearts.pushBack(Sprite::create("MENU/_menu14.png"));
    _hearts.at(0)->setPosition(Vec2(120, 1070));
    _hearts.at(0)->setScale(2);
    this->addChild(_hearts.at(0));
    for (int i = 1; i < 5; i++)
    {
        _hearts.pushBack(Sprite::create("MENU/_menu14.png"));
        _hearts.at(i)->setPosition(_hearts.at(i - 1)->getPositionX() + 180, _hearts.at(i - 1)->getPositionY());
        _hearts.at(i)->setScale(2);
        this->addChild(_hearts.at(i));
    }
    for (int i = 4; i > pLife - 1; i--)
    {
        _hearts.at(i)->setColor(Color3B::BLACK);
    }
    /*
    auto auxi = Sprite::createWithSpriteFrameName("_SPRITES11.png");
    auxi->setScale(5);
    ProgressTimer* bar = ProgressTimer::create(auxi);
    bar->setPosition(_monster1->getPositionX(), _monster1->getPositionY()+200);
    this->addChild(bar);*/
}
void Player::initScore()
{
    score = Label::createWithTTF("SCORE:" + std::to_string(iScore), "fonts/8Bit.ttf", 60);
    score->setPosition(1500, 1100);
    this->addChild(score);
}
bool Player::OnContactBegin(cocos2d::PhysicsContact& contact)
{
    try
    {
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();
        //auto _monster2 = Sprite::createWithSpriteFrameName("sprite_2.png");
        //auto jumpy = JumpBy::create(0.2, Vec2(0, 0), 20, 1);
        auto tinTo = TintTo::create(0.5, 200, 0, 0);
        //auto tinTo2 = TintTo::create(1, 255, 255, 255);
        auto returnTint = TintTo::create(1, 255, 255, 255);
        returnTint->setTag(200);
        auto tinTo3 = TintTo::create(1.5, 80, 50, 50);
        auto sequence = Sequence::create(JumpBy::create(0.2, Vec2(0, 0), 20, 1),
            returnTint,
            nullptr);
        tinTo->setTag(201);
        auto PlayerLosing = JumpBy::create(0.4f, Vec2(-140, 0), 34.0f, 1.0f);
        auto EnemyLosing = JumpBy::create(1.5, Vec2(0, 0), 424, 1);
        auto rotate2 = RotateBy::create(1.5, -150);
        //auto rotate3 = RotateBy::create(1.5, -150);
        auto eliminate = CallFuncN::create(CC_CALLBACK_1(Player::doRemoveFromParentAndCleanup, this, true));
        auto jumpyy = JumpBy::create(0.8f, Vec2(0, 0), 300.0f, 1.0f);
        auto repeaty = RepeatForever::create(jumpyy);

        if (nodeA != nullptr && (nodeB != nullptr))
        {
            if (((nodeA->getTag() == 9) && (nodeB->getTag() == 30)) 
                || ((nodeB->getTag() == 9) && (nodeA->getTag() == 30)))
            {
                switch (power)
                {
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/PowerUp.mp3");
                case 0:
                {
                    if (pLife < 5)
                    {
                        pLife++;
                        _hearts.at(pLife - 1)->setColor(Color3B::WHITE);
                    }
                    else
                    {
                        for (int i = 0; i < 3; i++)
                        {
                            iScore += 50;
                            score->setString("SCORE:" + std::to_string(iScore));
                            Sleep(1.2);
                        }
                    }
                    break;
                }
                case 1:
                {
                    hasPower = true;
                    _player->stopActionByTag(200);
                    _player->stopActionByTag(201);
                    _player->runAction(TintTo::create(0.5,245,136,73));
                    _player->stopActionByTag(200);
                    _player->stopActionByTag(201);
                    damage = 2;
                    this->schedule(CC_SCHEDULE_SELECTOR(Player::returnDamage),5);
                    break;
                }
                case 2:
                {
                    hasPower = true;
                    _player->stopActionByTag(200);
                    _player->stopActionByTag(201);
                    _player->runAction(TintTo::create(0.5, 128, 232, 109));
                    _player->stopActionByTag(200);
                    _player->stopActionByTag(201);
                    jumph = 450;
                    this->schedule(CC_SCHEDULE_SELECTOR(Player::returnJump), 7);
                    break;
                }
                case 3:
                {
                    hasPower = true;
                    _player->stopActionByTag(200);
                    _player->stopActionByTag(201);
                    _player->runAction(TintTo::create(0.5, 94, 139, 230));
                    _player->stopActionByTag(200);
                    _player->stopActionByTag(201);
                    vel = 1050;
                    this->schedule(CC_SCHEDULE_SELECTOR(Player::returnVel), 7);
                    break;
                }
                default:
                    break;
                }
                if((nodeA->getTag() == 9))
                    nodeA->removeFromParent();
                else 
                    nodeB->removeFromParent();
            }
            else if (((nodeA->getTag() == 5) && (nodeB->getTag() == 10))  //nodeA = balas
                || ((nodeA->getTag() == 20) && (nodeB->getTag() == 30))) //nodeB = personajes
            {
                if (nodeB->getTag() == 10)
                {
                    nodeB->runAction(tinTo);
                    nodeB->runAction(sequence);
                }
                else if (eLife > 0 && pLife > 0 && !hasPower)
                {
                    if (nodeB->getNumberOfRunningActions() == 0)
                    {
                        nodeB->runAction(tinTo);
                        nodeB->runAction(sequence);
                    }
                }
                if (nodeB->getTag() == 10)
                {
                    eLife-=damage;

                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.3);
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/eGrunt.mp3");
                    iScore += damage*5;
                    score->setString("SCORE:" + std::to_string(iScore));
                    //50 + contScene*50 es a 100, 1 es 10/(5+(2*contScene
                    //barLife->setPercentage(eLife * 2);
                    barLife->setPercentage(eLife*10/(5+(2*contScene)));


                    if (eLife < 1)
                    {
                        _player->stopActionByTag(3);
                        _player->setSpriteFrame("_sprite14.png");
                        _player->setFlippedX(false);
                        gameOver = true;
                        //pLife = 0 
                        barLife->setPercentage(0);
                        _lifebackground->setOpacity(0);
                        auto delay = DelayTime::create(4);
                        nodeB->stopAction(sequence);
                        nodeB->runAction(tinTo3);
                        nodeB->runAction(EnemyLosing);
                        if (contScene == 1)nodeB->runAction(MoveBy::create(1.5, Vec3(0,-36,0)));
                        nodeB->runAction(rotate2);
                        nodeB->getParent()->unscheduleAllCallbacks();
                        this->unscheduleAllCallbacks();
                        _player->runAction(repeaty);
                        darkBackground();
                        nodeB->getPhysicsBody()->setEnabled(false);
                        youWin();
                        auto delayAction = DelayTime::create(auxiLife * 1.59f);  // For 2 Seconds of Delay
                        auto funcCallback = CallFunc::create([]() {
                            CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
                            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
                            });
                        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1);
                        this->runAction(Sequence::create(delayAction, funcCallback, NULL));
                        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.9);
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/Win.mp3");
                        this->schedule(CC_SCHEDULE_SELECTOR(Player::plusScore), 0.8f, auxiLife - 1, 1.0f);
                        
                        //auto in = this->getScene();
                        this->schedule(CC_SCHEDULE_SELECTOR(Player::nextScene), 0, 0, auxiLife * 1.6f);
                    }
                }
                else
                {
                    pLife--;
                    iScore -= 50;
                    if (iScore < 100)iScore = 0;
                    score->setString("SCORE:" + std::to_string(iScore));
                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/pGrunt.mp3");
                    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/hitHurt.mp3");
                    _hearts.at(pLife)->setColor(Color3B::BLACK);
                    if (pLife == 0)
                    {
                        nodeA->getPhysicsBody()->setEnabled(false);
                        gameOver = true;
                        barLife->setPercentage(0);
                        barLife->setPosition(4000, 4000);
                        _lifebackground->setOpacity(0);
                        _lifebackground->setPosition(4000, 4000);
                        nodeB->stopAction(sequence);
                        nodeB->runAction(tinTo3);
                        nodeB->runAction(PlayerLosing);
                        nodeB->runAction(rotate2);
                        nodeA->getParent()->unscheduleAllCallbacks();
                        this->unscheduleAllCallbacks();
                        nodeA->getParent()->getChildByTag(10)->runAction(repeaty);
                        //unscheduleAllCallbacks();
                        //hacer que salte el monstruo
                        darkBackground();
                        //nodeB->runAction(tinTo3);
                        nodeB->getPhysicsBody()->setEnabled(false);
                        contScene = 0;
                        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
                        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
                        //nodeB->removeFromParent();
                        loseMenu();
                        //youLose();
                    }
                }
                nodeA->removeFromParent();
            }
            else if (((nodeB->getTag() == 5) && (nodeA->getTag() == 10))  //nodeb = balas
                || ((nodeB->getTag() == 20) && (nodeA->getTag() == 30))) //nodeB = personajes
            {
                if (nodeA->getTag() == 10)
                {
                    nodeA->runAction(tinTo);
                    nodeA->runAction(sequence);
                }
                else if (eLife > 0 && pLife > 0 && !hasPower)
                {
                    if (nodeA->getNumberOfRunningActions() == 0)
                    {
                        nodeA->runAction(tinTo);
                        nodeA->runAction(sequence);
                    }
                }
                if (nodeA->getTag() == 10)
                {
                    eLife -= damage;

                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.3);
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/eGrunt.mp3");
                    iScore += damage * 5;
                    score->setString("SCORE:" + std::to_string(iScore));
                    //50 + contScene*50 es a 100, 1 es 10/(5+(2*contScene
                    //barLife->setPercentage(eLife * 2);
                    barLife->setPercentage(eLife * 10 / (5 + (2 * contScene)));
                    if (eLife < 1)
                    {
                        //pLife = 0;
                        _player->stopActionByTag(3);
                        _player->setSpriteFrame("_sprite14.png");
                        _player->setFlippedX(false);
                        youWin();
                        auto delay = DelayTime::create(4);
                        nodeA->stopAction(sequence);
                        nodeA->runAction(tinTo3);
                        nodeA->runAction(EnemyLosing);
                        if (contScene == 1)nodeA->runAction(MoveBy::create(1.5, Vec3(0, -36, 0)));
                        nodeA->runAction(rotate2);
                        nodeA->getParent()->unscheduleAllCallbacks();
                        this->unscheduleAllCallbacks();
                        _player->runAction(repeaty);
                        darkBackground();
                        nodeA->getPhysicsBody()->setEnabled(false);
                        //contScene++;
                        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
                        //CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

                        this->schedule(CC_SCHEDULE_SELECTOR(Player::plusScore), 0.8, auxiLife - 1, 1);
                        this->schedule(CC_SCHEDULE_SELECTOR(Player::nextScene), auxiLife * 1.6);
                    }
                }
                else
                {
                    pLife--;
                    iScore -= 50;
                    if (iScore < 100)iScore = 0;
                    score->setString("SCORE:" + std::to_string(iScore));
                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/pGrunt.mp3");
                    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/hitHurt.mp3");
                    _hearts.at(pLife)->setColor(Color3B::BLACK);
                    if (pLife == 0)
                    {
                        nodeB->getPhysicsBody()->setEnabled(false);
                        nodeA->stopAction(sequence);
                        nodeA->runAction(tinTo3);
                        nodeA->runAction(PlayerLosing);
                        nodeA->runAction(rotate2);
                        nodeB->getParent()->unscheduleAllCallbacks();
                        this->unscheduleAllCallbacks();
                        nodeB->getParent()->getChildByTag(10)->runAction(repeaty);
                        //unscheduleAllCallbacks();
                        //hacer que salte el monstruo
                        darkBackground();
                        //nodeB->runAction(tinTo3);
                        nodeA->getPhysicsBody()->setEnabled(false);
                        contScene = 0;
                        //nodeB->removeFromParent();
                        loseMenu();
                        //youLose();
                    }
                }
                nodeB->removeFromParent();
            }
        }
    }
    catch (std::exception e) {};
    //bodies can collide
    return true;
}
void Player::returnDamage(float dt)
{
    hasPower = false;
    _player->runAction(TintTo::create(0.2, 255, 255, 255));
    damage = 1;
}
void Player::returnJump(float dt)
{
    hasPower = false;
    _player->runAction(TintTo::create(0.2, 255, 255, 255));
    jumph = 200;
}
void Player::returnVel(float dt)
{
    hasPower = false;
    _player->runAction(TintTo::create(0.2, 255, 255, 255));
    vel = 600;
}
void Player::youWin()
{
    auxiLife = pLife;
    pLife = 0;
    damage = 1;
    _player->getPhysicsBody()->setEnabled(false);
    //auto in = this->getScene();
    auto completed = Sprite::create("MENU/_menu12.png");
    completed->setScale(1.5);
    completed->setPosition(visibleSize.x / 2 + origin.x, visibleSize.y / 2 + origin.y + 250);
    this->addChild(completed,103);
}
void Player::loseMenu()
{
    homeMenu = MenuItemImage::create("MENU/_menu10.png", "MENU/_menu11.png", CC_CALLBACK_1(Player::returnHome, this));
    againMenu = MenuItemImage::create("MENU/_menu8.png", "MENU/_menu9.png", CC_CALLBACK_1(Player::playAgain, this));
    auto menu = Menu::create(homeMenu, againMenu, nullptr);
    menu->setPosition(Point::ZERO);
    againMenu->setPosition(visibleSize.x / 2 + origin.x + 370, visibleSize.y / 2 + origin.y + 123);
    homeMenu->setPosition(visibleSize.x / 2 + origin.x + 370, visibleSize.y / 2 + origin.y - 123);
    Sprite* _title = Sprite::create("MENU/_menu7.png");
    _title->setPosition(origin.x + visibleSize.x / 2 - 150, origin.y + visibleSize.y / 2);
    score->setTag(104);
    this->addChild(_title,103);
    this->addChild(menu,103);
}
void Player::plusScore(float dt)
{
    pLife = auxiLife;
    plus = Label::createWithTTF("+100", "fonts/8Bit.ttf", 60);
    plus->setPosition(1670.0f, 1060.0f);
    this->addChild(plus);
    auto fade = FadeOut::create(0.7f);
    plus->runAction(fade);
    iScore += 100;
    score->setString("SCORE:" + std::to_string(iScore));
}
void Player::returnHome(Ref* pSender)
{
    auto scene = HelloWorld::createScene();
    //Director::getInstance()->pushScene(TransitionFade::create(1.5f,scene));
    Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.4, scene));
}
void Player::nextScene(float dt)
{
    contScene++;
    damage = 1;
    Scene* scene1 = Scene2::createScene();
    Scene* scene2 = Scene3::createScene();
    Scene* scene3 = HelloWorld::createScene();
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
    switch (contScene)
    {
    case 1:
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/Boss2.MP3", true);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1f);
        //checar diferencia entre TransitionSlideInR y TransitionMoveInR
        Director::getInstance()->replaceScene(TransitionMoveInR::create(1.1, scene1));
        break;
    case 2:
        //pLife = auxiLife;
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/Boss3.MP3", true);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2);
        Director::getInstance()->replaceScene(TransitionMoveInR::create(1.1, scene2));
        break;
    case 3:
        //pLife = auxiLife;
        iScore = 0;
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/Menu.MP3", true);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2);
        contScene = 0;
        Director::getInstance()->replaceScene(TransitionProgressInOut::create(0.4, scene3));
        break;
    default:
        break;
    }
}
void Player::recoverLife(float delta)
{
    _xLife = Sprite::create("MENU/_menu21.png");
    power = rand()%4; 
    switch (power)
    {
    case 0:
    {
        //vida
        _xLife->setTexture("MENU/_menu21.png");
        break;
    }
    case 1:
    {
        //ataque
        _xLife->setTexture("MENU/_menu22.png");
        break;
    }
    case 2:
    {
        //salto
        _xLife->setTexture("MENU/_menu23.png");
        break;
    }
    case 3:
    {
        //velocidad
        _xLife->setTexture("MENU/_menu24.png");
        break;
    }
    default:
        break;
    }
    int xCoor = rand() % 1300 + 50;
    _xLife ->setPosition(xCoor,1200);
    initPhysic(_xLife);
    _xLife->setScale(1);
    //_xLife->getPhysicsBody()->setDynamic(false);
    _xLife->getPhysicsBody()->setGroup(-1);
    _xLife->setTag(9);
    //_xLife->getPhysicsBody()->set
    _xLife->getPhysicsBody()->setVelocity(Vec2(0,-150));
    this->addChild(_xLife);
    auto fadeout = FadeOut::create(0.15f);
    auto fadeint = FadeIn::create(0.15f);
    auto fadeseq = Sequence::create(fadeout, fadeint,
                                    fadeout, fadeint,
                                    fadeout, fadeint,nullptr);
    auto removeAfterTime = Sequence::create(DelayTime::create(6), fadeseq,
        CallFuncN::create(CC_CALLBACK_1(Player::doRemoveFromParentAndCleanup, this, true)),
        nullptr);
    _xLife->runAction(removeAfterTime);
}
void Player::playAgain(Ref* pSender)
{
    pLife = 5;
    iScore = 0;
    auto scene = Scene1::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4, scene));
}
void Player::whiteBackground()
{
    _Fbackground->setOpacity(0);
    _Fbackground2->setOpacity(0);
}
void Player::darkBackground()
{
    _Fbackground = Sprite::createWithSpriteFrameName("_sprite18.png");
    _Fbackground->setScale(100.0f);
    _Fbackground->setColor(Color3B::BLACK);
    _Fbackground->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    _Fbackground->setOpacity(98.0f);
    _Fbackground2 = Sprite::createWithSpriteFrameName("_sprite18.png");
    _Fbackground2->setScale(100.0f);
    _Fbackground2->setColor(Color3B::BLACK);
    _Fbackground2->setPosition(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);
    _Fbackground2->setOpacity(98.0f);
    this->addChild(_Fbackground,100);
    this->addChild(_Fbackground2,101);
}
