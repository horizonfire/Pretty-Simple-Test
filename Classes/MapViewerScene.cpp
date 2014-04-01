#include "MapViewerScene.h"

USING_NS_CC;

#define TILE_X 4
#define TILE_Y 5

MapViewerScene::MapViewerScene() :
    m_pLocationDialog(NULL),
    m_pMapNode(NULL),
    m_ptTouchCenter(CCPointZero),
    m_vVelocity(CCPointZero),
    m_mapSize(CCSizeZero),
    m_bIsZooming(false),
    m_fTouchDistance(0.0f),
    m_fMinScale(0.0f),
    m_fMaxScale(4.0f),
    m_fMinMapX(0.0f),
    m_fMaxMapX(0.0f),
    m_fMinMapY(0.0f),
    m_fMaxMapY(0.0f),
    m_fResistance(4.0f),
    m_fResistanceScale(30.0f),
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
    CCPoint ptCenter = ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    m_pMapNode->setPosition(ptCenter);

    //scale so that largest dimension is used
    float fWidthScale  = visibleSize.width / m_pMapNode->getContentSize().width;
    float fHeightScale = visibleSize.height / m_pMapNode->getContentSize().height;

    m_fMinScale = fWidthScale > fHeightScale ? fWidthScale : fHeightScale;
    m_pMapNode->setScale(m_fMinScale);

    m_fMinMapX = ptCenter.x - (m_pMapNode->getContentSize().width  * m_pMapNode->getScale()) / 2.0f + visibleSize.width / 2.0f;
    m_fMaxMapX = ptCenter.x + (m_pMapNode->getContentSize().width  * m_pMapNode->getScale()) / 2.0f - visibleSize.width / 2.0f;
    m_fMinMapY = ptCenter.y - (m_pMapNode->getContentSize().height * m_pMapNode->getScale()) / 2.0f + visibleSize.height / 2.0f;
    m_fMaxMapY = ptCenter.y + (m_pMapNode->getContentSize().height * m_pMapNode->getScale()) / 2.0f - visibleSize.height / 2.0f;

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
    if (m_setTouches.count() == 0 && (m_vVelocity.x != 0.0f || m_vVelocity.y != 0.0f))
    {
        //m_vVelocity.x *= 0.6f;
        //m_vVelocity.y *= 0.6f;
        
        //m_pMapNode->setPosition(ccpAdd(m_pMapNode->getPosition(), m_vVelocity * 150.0f * delta));
    }
}

void MapViewerScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++)
    {
        m_setTouches.addObject(*it);
    }
    
    m_vVelocity = CCPointZero;
    m_ptTouchCenter = GetAverageLocationInViewTouchSet(&m_setTouches);
    m_fTouchDistance = GetAverageDistanceTouchSet(&m_setTouches);
    //m_bIsZooming = true;
    
    m_pMapNode->stopAllActions();
}

void MapViewerScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    //adjust the scale of the map based touches
    if (m_setTouches.count() > 1)
    {
        float fDistance = GetAverageDistanceTouchSet(&m_setTouches);
        
        CCPoint ptCenter = CCDirector::sharedDirector()->convertToGL(m_ptTouchCenter);
        CCPoint ptDiffFromNodePos = ccpSub(ptCenter, m_pMapNode->getPosition());

        float fScale = m_pMapNode->getScale() * fDistance / m_fTouchDistance;
        
        //cause resistance to the scaling
        if ((fScale < m_fMinScale && fDistance < m_fTouchDistance) ||
            (fScale > m_fMaxScale && fDistance > m_fTouchDistance))
        {
            fDistance = (fDistance - m_fTouchDistance) / m_fResistanceScale + m_fTouchDistance;
            fScale = m_pMapNode->getScale() * fDistance / m_fTouchDistance;
        }
        
        if (fScale > m_fMinScale - 0.1f &&
            fScale < m_fMaxScale + 1.5f)
        {
            m_pMapNode->setScale(fScale);
        }
        else
        {
            fDistance = m_fTouchDistance;
        }

        float fScaleDiff = (fDistance / m_fTouchDistance) - 1.0f;
        ptDiffFromNodePos.x *= fScaleDiff;
        ptDiffFromNodePos.y *= fScaleDiff;

        m_pMapNode->setPosition(ccpSub(m_pMapNode->getPosition(), ptDiffFromNodePos));
        m_fTouchDistance = fDistance;
    }

    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint ptCenter = ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    m_fMinMapX = ptCenter.x - (m_pMapNode->getContentSize().width  * m_pMapNode->getScale()) / 2.0f + visibleSize.width / 2.0f;
    m_fMaxMapX = ptCenter.x + (m_pMapNode->getContentSize().width  * m_pMapNode->getScale()) / 2.0f - visibleSize.width / 2.0f;
    m_fMinMapY = ptCenter.y - (m_pMapNode->getContentSize().height * m_pMapNode->getScale()) / 2.0f + visibleSize.height / 2.0f;
    m_fMaxMapY = ptCenter.y + (m_pMapNode->getContentSize().height * m_pMapNode->getScale()) / 2.0f - visibleSize.height / 2.0f;

    //adjust the translation of the map based on the touches
    CCPoint ptInView = GetAverageLocationInViewTouchSet(&m_setTouches);
    CCPoint ptDiff = ccpSub(ptInView, m_ptTouchCenter);
    ptDiff.y = -ptDiff.y;
    
    m_vVelocity = ccpAdd(m_vVelocity, ptDiff);

    //cause resistance when the map is off the edge
    if (m_pMapNode->getPosition().x < m_fMinMapX - m_fStretchVariance  || m_pMapNode->getPosition().x > m_fMaxMapX + m_fStretchVariance)
    {
        ptDiff.x /= m_fResistance;
        m_vVelocity.x = 0.0f;
    }
    
    if (m_pMapNode->getPosition().y  < m_fMinMapY - m_fStretchVariance || m_pMapNode->getPosition().y > m_fMaxMapY + m_fStretchVariance)
    {
        ptDiff.y /= m_fResistance;
        m_vVelocity.y = 0.0f;
    }
    
    m_ptTouchCenter = ptInView;
    m_pMapNode->setPosition(ccpAdd(m_pMapNode->getPosition(), ptDiff));
}

void MapViewerScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++)
    {
        m_setTouches.removeObject(*it);
    }
    
    m_vVelocity = CCPointZero;
    m_ptTouchCenter = GetAverageLocationInViewTouchSet(&m_setTouches);
    m_fTouchDistance = GetAverageDistanceTouchSet(&m_setTouches);
    
    //m_bIsZooming = false;
    if (m_setTouches.count() == 0)
    {
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint ptCenter = ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
        
        CCPoint ptCenterGL = CCDirector::sharedDirector()->convertToGL(ptCenter);
        CCPoint ptDiffFromNodePos = ccpSub(ptCenterGL, m_pMapNode->getPosition());
        
        float fTargetScale = m_pMapNode->getScale();
        if (m_pMapNode->getScale() < m_fMinScale)
        {
            m_pMapNode->runAction(CCEaseInOut::create(CCScaleTo::create(0.25f, m_fMinScale), 4.0f));
            fTargetScale = m_fMinScale;
        }
        else if (m_pMapNode->getScale() > m_fMaxScale)
        {
            m_pMapNode->runAction(CCEaseInOut::create(CCScaleTo::create(0.25f, m_fMaxScale), 4.0f));
            fTargetScale = m_fMaxScale;
        }
        
        m_fMinMapX = ptCenter.x - (m_pMapNode->getContentSize().width  * fTargetScale) / 2.0f + visibleSize.width / 2.0f;
        m_fMaxMapX = ptCenter.x + (m_pMapNode->getContentSize().width  * fTargetScale) / 2.0f - visibleSize.width / 2.0f;
        m_fMinMapY = ptCenter.y - (m_pMapNode->getContentSize().height * fTargetScale) / 2.0f + visibleSize.height / 2.0f;
        m_fMaxMapY = ptCenter.y + (m_pMapNode->getContentSize().height * fTargetScale) / 2.0f - visibleSize.height / 2.0f;

        float fScaleDiff = (fTargetScale / m_pMapNode->getScale()) - 1.0f;
        ptDiffFromNodePos.x *= fScaleDiff;
        ptDiffFromNodePos.y *= fScaleDiff;
        CCPoint ptAdjust = ccpSub(m_pMapNode->getPosition(), ptDiffFromNodePos);
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
//    if (pTouches->count() == 1)
//    {
//        return 0;
//    }
//    else if (pTouches->count() == 2)
//    {
//        CCSetIterator it = pTouches->begin();
//        CCTouch* touch = dynamic_cast<CCTouch*>(*it);
//        CCPoint ptFirst = touch->getLocationInView();
//        ++it;
//        touch = dynamic_cast<CCTouch*>(*it);
//        
//        return ccpDistance(ptFirst, touch->getLocationInView());
//    }
//    
//    return 0;
//    
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
