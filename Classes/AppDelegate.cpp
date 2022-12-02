#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Player.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1200);

static cocos2d::Size smallResolutionSize = cocos2d::Size(640, 400);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 800);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1920, 1200);

void AppDelegate::initMultiResolution()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    glview->setDesignResolutionSize(
        designResolutionSize.width,
        designResolutionSize.height,
        ResolutionPolicy::NO_BORDER
    );


    std::vector<std::string> searchPaths;
    float scaleFactor = 1.0f;
    Size frameSize = glview->getFrameSize();

    if (frameSize.height > mediumResolutionSize.height)
    {
        searchPaths.push_back("res/HDR");
        scaleFactor = largeResolutionSize.height / designResolutionSize.height;
    }
    else if (frameSize.height > smallResolutionSize.height)
    {
        searchPaths.push_back("res/HD");
        scaleFactor = mediumResolutionSize.height / designResolutionSize.height;
    }
    else
    {
        searchPaths.push_back("res/SD");
        scaleFactor = smallResolutionSize.height / designResolutionSize.height;
    }

    director->setContentScaleFactor(scaleFactor);
    FileUtils::getInstance()->setSearchPaths(searchPaths);
}

void AppDelegate::initOpenGL()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
       //GLViewImpl::createWithFullScreen
        glview = GLViewImpl::createWithFullScreen("BUBBLEGUM BATTLE");
               
        //glview = GLViewImpl::createWithRect("BUBBLEGUM BATTLE",
            //Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Cocos2dx-SpriteSheetTutorial");
#endif
        director->setOpenGLView(glview);
    }
}

void AppDelegate::initDirector()
{
    auto director = Director::getInstance();
    director->setAnimationInterval(1.0 / 60);
    director->setDisplayStats(true);
}

void AppDelegate::createAndRunScene()
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->runWithScene(scene);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    initOpenGL();
    initDirector();
    initMultiResolution();
    createAndRunScene();
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    GLView::setGLContextAttrs(glContextAttrs);
}