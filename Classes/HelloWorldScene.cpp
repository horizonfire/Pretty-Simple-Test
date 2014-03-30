#include "HelloWorldScene.h"

USING_NS_CC;

HelloWorld::HelloWorld() :
    m_pMapSprite(NULL),
    m_ptTouchCenter(0.0f, 0.0f),
    m_bIsZooming(false),
    m_fMinMapX(0.0f),
    m_fMaxMapX(0.0f),
    m_fMinMapY(0.0f),
    m_fMaxMapY(0.0f),
    m_fResistance(3.0f),
    m_fStretchVariance(2.0f)
{
    scheduleUpdate();
}

HelloWorld::~HelloWorld()
{
    unscheduleUpdate();
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

 //   /////////////////////////////
 //   // 2. add a menu item with "X" image, which is clicked to quit the program
 //   //    you may modify it.

 //   // add a "close" icon to exit the progress. it's an autorelease object
 //   CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
 //                                       "CloseNormal.png",
 //                                       "CloseSelected.png",
 //                                       this,
 //                                       menu_selector(HelloWorld::menuCloseCallback));
 //   
    //pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
 //                               origin.y + pCloseItem->getContentSize().height/2));

 //   // create menu, it's an autorelease object
 //   CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
 //   pMenu->setPosition(CCPointZero);
 //   this->addChild(pMenu, 1);

 //   /////////////////////////////
 //   // 3. add your codes below...

 //   // add a label shows "Hello World"
 //   // create and initialize a label
 //   
 //   CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
 //   
 //   // position the label on the center of the screen
 //   pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
 //                           origin.y + visibleSize.height - pLabel->getContentSize().height));

 //   // add the label as a child to this layer
 //   this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    m_pMapSprite = CCSprite::create("new_york_map.jpg");

    // position the sprite on the center of the screen
    m_pMapSprite->setPosition(ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    //scale so that largest dimension is used
    float fWidthScale  = visibleSize.width / m_pMapSprite->getContentSize().width;
    float fHeightScale = visibleSize.height / m_pMapSprite->getContentSize().height;

    m_fDefaultScale = fWidthScale > fHeightScale ? fWidthScale : fHeightScale;
    m_pMapSprite->setScale(m_fDefaultScale);

    m_fMinMapX = m_pMapSprite->boundingBox().getMinX() + visibleSize.width / 2;
    m_fMaxMapX = m_pMapSprite->boundingBox().getMaxX() - visibleSize.width / 2;
    m_fMinMapY = m_pMapSprite->boundingBox().getMinY() + visibleSize.height / 2;
    m_fMaxMapY = m_pMapSprite->boundingBox().getMaxY() - visibleSize.height / 2;

    // add the sprite as a child to this layer
    this->addChild(m_pMapSprite, 0);

    setTouchEnabled(true);
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}


void HelloWorld::update(float delta)
{
    if (m_bIsZooming)
    {
        CCPoint ptCenter = CCDirector::sharedDirector()->convertToGL(m_ptTouchCenter);
        CCPoint ptDiff = ccpSub(ptCenter, m_pMapSprite->getPosition());

        float fScaleDiff = 0.5f * delta;
        m_pMapSprite->setScale(m_pMapSprite->getScale() * (1.0f + fScaleDiff));

        ptDiff.x *= fScaleDiff;
        ptDiff.y *= fScaleDiff;

        m_pMapSprite->setPosition(ccpSub(m_pMapSprite->getPosition(), ptDiff));
    }
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    m_ptTouchCenter = GetAverageLocationInViewTouchSet(pTouches);

    m_bIsZooming = true;
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    ////adjust the translation of the map based on the touches
    CCPoint ptInView = GetAverageLocationInViewTouchSet(pTouches);
    //CCPoint ptDiff = ccpSub(ptInView, m_ptTouchCenter);
    //ptDiff.y = -ptDiff.y;

    ////cause resistance when the map is off the edge
    //if (IsMapPositionStretched(m_pMapSprite->getPosition()))
    //{
    //    ptDiff.x /= m_fResistance;
    //    ptDiff.y /= m_fResistance;
    //}

    m_ptTouchCenter = ptInView;
    //m_pMapSprite->setPosition(ccpAdd(m_pMapSprite->getPosition(), ptDiff));
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    m_bIsZooming = false;
    //if (pTouches->count() == 1)
    //{
    //    CCPoint ptAdjust = m_pMapSprite->getPosition();
    //    if (ptAdjust.x < m_fMinMapX)
    //    {
    //        ptAdjust.x = m_fMinMapX;
    //    }
    //    else if (ptAdjust.x > m_fMaxMapX)
    //    {
    //        ptAdjust.x = m_fMaxMapX;
    //    }

    //    if (ptAdjust.y < m_fMinMapY)
    //    {
    //        ptAdjust.y = m_fMinMapY;
    //    }
    //    else if (ptAdjust.y > m_fMaxMapY)
    //    {
    //        ptAdjust.y = m_fMaxMapY;
    //    }

    //    m_pMapSprite->runAction(CCEaseInOut::create(CCMoveTo::create(0.25f, ptAdjust), 4.0f));
    //}
}

void HelloWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    ccTouchesEnded(pTouches, pEvent);
}


CCPoint HelloWorld::GetAverageLocationInViewTouchSet(cocos2d::CCSet *pTouches)
{
    CCPoint ptAverage = CCPointZero;
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++)
    {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        ptAverage = ccpAdd(ptAverage, touch->getLocationInView());
    }

    if (pTouches->count() > 0)
    {
        ptAverage.x /= pTouches->count();
        ptAverage.y /= pTouches->count();
    }
    return ptAverage;
}

bool HelloWorld::IsMapPositionStretched(const CCPoint& ptPosition)
{
    return (ptPosition.x < m_fMinMapX - m_fStretchVariance  || ptPosition.x > m_fMaxMapX + m_fStretchVariance ||
            ptPosition.y  < m_fMinMapY - m_fStretchVariance || ptPosition.y > m_fMaxMapY + m_fStretchVariance);
}
