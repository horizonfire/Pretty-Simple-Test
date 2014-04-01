#include "MapViewerScene.h"

USING_NS_CC;

#define TILE_X 4
#define TILE_Y 5

MapViewerScene::MapViewerScene() :
    m_pLocationDialog(NULL),
    m_pMapNode(NULL),
    m_ptTouchCenter(CCPointZero),
    m_mapSize(CCSizeZero),
    m_bIsZooming(false),
    m_fMinMapX(0.0f),
    m_fMaxMapX(0.0f),
    m_fMinMapY(0.0f),
    m_fMaxMapY(0.0f),
    m_fResistance(4.0f),
    m_fStretchVariance(2.0f)
{
    scheduleUpdate();
}

MapViewerScene::~MapViewerScene()
{
    unscheduleUpdate();
}

CCScene* MapViewerScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MapViewerScene *layer = MapViewerScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MapViewerScene::init()
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
    m_pMapNode = CCNode::create();
    
    m_mapSize.setSize(3398, 4842);
    char buf[256];
    for (int iY = 0; iY < TILE_Y; ++iY)
    {
        for (int iX = 0; iX < TILE_X; ++iX)
        {
            sprintf(buf, "new_york_map_%02i.png", iX + TILE_X * iY + 1);
            CCSprite* pSpriteTile = CCSprite::create(buf);
            
            CCPoint ptPos = CCPointZero;
            ptPos.x = -m_mapSize.width / 2.0f + pSpriteTile->getContentSize().width  * (iX + 0.5f);
            ptPos.y = m_mapSize.height / 2.0f - pSpriteTile->getContentSize().height * (iY + 0.5f);
            
            m_pMapNode->addChild(pSpriteTile);
            
            pSpriteTile->setPosition(ptPos);
        }
    }
    
    m_pMapNode->setContentSize(m_mapSize);
    
    // position the sprite on the center of the screen
    m_pMapNode->setPosition(ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    //scale so that largest dimension is used
    float fWidthScale  = visibleSize.width / m_pMapNode->getContentSize().width;
    float fHeightScale = visibleSize.height / m_pMapNode->getContentSize().height;

    m_fDefaultScale = fWidthScale > fHeightScale ? fWidthScale : fHeightScale;
    m_pMapNode->setScale(m_fDefaultScale);

    m_fMinMapX = m_pMapNode->getPositionX() - (m_pMapNode->getContentSize().width  * m_fDefaultScale) / 2.0f + visibleSize.width / 2.0f;
    m_fMaxMapX = m_pMapNode->getPositionX() + (m_pMapNode->getContentSize().width  * m_fDefaultScale) / 2.0f - visibleSize.width / 2.0f;
    m_fMinMapY = m_pMapNode->getPositionY() - (m_pMapNode->getContentSize().height * m_fDefaultScale) / 2.0f + visibleSize.height / 2.0f;
    m_fMaxMapY = m_pMapNode->getPositionY() + (m_pMapNode->getContentSize().height * m_fDefaultScale) / 2.0f - visibleSize.height / 2.0f;

    // add the sprite as a child to this layer
    this->addChild(m_pMapNode, 0);

    setTouchEnabled(true);
    
    return true;
}


void MapViewerScene::menuCloseCallback(CCObject* pSender)
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


void MapViewerScene::update(float delta)
{
//    if (m_bIsZooming)
//    {
//        CCPoint ptCenter = CCDirector::sharedDirector()->convertToGL(m_ptTouchCenter);
//        CCPoint ptDiff = ccpSub(ptCenter, m_pMapNode->getPosition());
//
//        float fScaleDiff = 0.5f * delta;
//        m_pMapNode->setScale(m_pMapNode->getScale() * (1.0f + fScaleDiff));
//
//        ptDiff.x *= fScaleDiff;
//        ptDiff.y *= fScaleDiff;
//
//        m_pMapNode->setPosition(ccpSub(m_pMapNode->getPosition(), ptDiff));
//    }
}

void MapViewerScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++)
    {
        m_setTouches.addObject(*it);
    }
    
    m_ptTouchCenter = GetAverageLocationInViewTouchSet(pTouches);

    m_fTouchDistance = GetAverageDistanceTouchSet(pTouches);
    //m_bIsZooming = true;
}

void MapViewerScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    //adjust the translation of the map based on the touches
    CCPoint ptInView = GetAverageLocationInViewTouchSet(pTouches);
    CCPoint ptDiff = ccpSub(ptInView, m_ptTouchCenter);
    ptDiff.y = -ptDiff.y;

    //cause resistance when the map is off the edge
    if (m_pMapNode->getPosition().x < m_fMinMapX - m_fStretchVariance  || m_pMapNode->getPosition().x > m_fMaxMapX + m_fStretchVariance)
    {
        ptDiff.x /= m_fResistance;
    }
    
    if (m_pMapNode->getPosition().y  < m_fMinMapY - m_fStretchVariance || m_pMapNode->getPosition().y > m_fMaxMapY + m_fStretchVariance)
    {
        ptDiff.y /= m_fResistance;
    }
    
    m_ptTouchCenter = ptInView;
    m_pMapNode->setPosition(ccpAdd(m_pMapNode->getPosition(), ptDiff));
    
    
    //adjust the scale of the map based touches
    if (pTouches->count() > 1)
    {
        float fDistance = GetAverageDistanceTouchSet(pTouches);
        
        CCPoint ptCenter = CCDirector::sharedDirector()->convertToGL(m_ptTouchCenter);
        CCPoint ptDiffFromNodePos = ccpSub(ptCenter, m_pMapNode->getPosition());

        m_pMapNode->setScale(m_pMapNode->getScale() * fDistance / m_fTouchDistance);

        float fScaleDiff = (fDistance / m_fTouchDistance) - 1.0f;
        ptDiffFromNodePos.x *= fScaleDiff;
        ptDiffFromNodePos.y *= fScaleDiff;

        m_pMapNode->setPosition(ccpSub(m_pMapNode->getPosition(), ptDiffFromNodePos));
        m_fTouchDistance = fDistance;
    }
}

void MapViewerScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++)
    {
        m_setTouches.removeObject(*it);

        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        CCPoint ptTouch = m_pMapNode->convertTouchToNodeSpace(touch);
        CCLOG("Touch at: %f, %f", ptTouch.x, ptTouch.y);
    }
    
    //m_bIsZooming = false;
    if (m_setTouches.count() == 0)
    {
        CCPoint ptAdjust = m_pMapNode->getPosition();
        if (ptAdjust.x < m_fMinMapX)
        {
            ptAdjust.x = m_fMinMapX;
        }
        else if (ptAdjust.x > m_fMaxMapX)
        {
            ptAdjust.x = m_fMaxMapX;
        }

        if (ptAdjust.y < m_fMinMapY)
        {
            ptAdjust.y = m_fMinMapY;
        }
        else if (ptAdjust.y > m_fMaxMapY)
        {
            ptAdjust.y = m_fMaxMapY;
        }

        m_pMapNode->runAction(CCEaseInOut::create(CCMoveTo::create(0.25f, ptAdjust), 4.0f));


        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

        if (!m_pLocationDialog)
        {
            m_pLocationDialog = LocationDialog::create();

            m_pLocationDialog->setPosition(ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
            addChild(m_pLocationDialog, 0);

            m_pLocationDialog->setTintColor(ccc3(255, 0, 0));
            m_pLocationDialog->ShowDialog();
        }
        else
        {
            m_pLocationDialog->HideDialog();
            m_pLocationDialog = NULL;
        }
    }
}

void MapViewerScene::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    ccTouchesEnded(pTouches, pEvent);
}

float MapViewerScene::GetAverageDistanceTouchSet(cocos2d::CCSet *pTouches)
{
    float fDistance = 0.0f;
    CCPoint ptCenter = GetAverageLocationInViewTouchSet(pTouches);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++)
    {
        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
        fDistance += ccpDistance(touch->getLocationInView(), ptCenter);
    }
    
    return fDistance;
}

CCPoint MapViewerScene::GetAverageLocationInViewTouchSet(cocos2d::CCSet *pTouches)
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
