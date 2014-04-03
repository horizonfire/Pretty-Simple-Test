#include "MapViewerScene.h"

USING_NS_CC;

#define TILE_X 4
#define TILE_Y 5

MapViewerScene::MapViewerScene() :
    m_pLocationDialog(NULL),
    m_pMapNode(NULL),
    m_ptLandmark(1443.0f, 1738.0f),
    m_ptTouchCenter(CCPointZero),
    m_vVelocity(CCPointZero),
    m_mapSize(CCSizeZero),
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
    
    m_pSpriteLandmark = CCSprite::create("madisonsqgarden_select.png");
    m_pSpriteLandmark->setPosition(ccp(-m_mapSize.width / 2.0f + m_ptLandmark.x, m_mapSize.height / 2.0f - m_ptLandmark.y));
    m_pMapNode->addChild(m_pSpriteLandmark);
    m_pSpriteLandmark->setVisible(false);
    
    // position the sprite on the center of the screen
    CCPoint ptCenter = ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    m_pMapNode->setPosition(ptCenter);
    m_ptLastMapPosition = ptCenter;

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
    
    m_pLocationDialog = LocationDialog::create();
    m_pLocationDialog->setTintColor(ccc3(128, 255, 128));
    m_pLocationDialog->setFontColor(ccc3(32, 64, 32));
    m_pLocationDialog->setText("Madison Square Garden:\nMulti-purpose indoor arena\nin midtown Manhattan in\nNew York City.");
    addChild(m_pLocationDialog, 0);

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
    CCPoint ptLandmark = ccp(-m_mapSize.width / 2.0f + m_ptLandmark.x, m_mapSize.height / 2.0f - m_ptLandmark.y + 80.0f);
    CCPoint ptDialog = m_pMapNode->convertToWorldSpace(ptLandmark);
    m_pLocationDialog->setPosition(ptDialog);

    if (m_setTouches.count() != 0)
    {
        m_vVelocity = ccpSub(m_pMapNode->getPosition(), m_ptLastMapPosition) / delta;
        m_ptLastMapPosition = m_pMapNode->getPosition();
    }
    else if (m_vVelocity.x != 0.0f || m_vVelocity.y != 0.0f)
    {
        m_vVelocity.x -= 100.0f * delta;
        m_vVelocity.y -= 100.0f * delta;

        //cause resistance when the map is off the edge
        if (m_pMapNode->getPosition().x < m_fMinMapX - m_fStretchVariance  || m_pMapNode->getPosition().x > m_fMaxMapX + m_fStretchVariance)
        {
            m_vVelocity.x = 0.0f;
        }
        
        if (m_pMapNode->getPosition().y  < m_fMinMapY - m_fStretchVariance || m_pMapNode->getPosition().y > m_fMaxMapY + m_fStretchVariance)
        {
            m_vVelocity.y = 0.0f;
        }
        
        if (ccpLength(m_vVelocity) < 0.4f)
        {
            m_vVelocity = CCPointZero;
        }
        
        m_pMapNode->setPosition(ccpAdd(m_pMapNode->getPosition(), m_vVelocity * delta));
    }
}

void MapViewerScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++)
    {
        m_setTouches.addObject(*it);
    }
    
    if (m_pLocationDialog)
    {
        m_pSpriteLandmark->setVisible(false);
        m_pLocationDialog->HideDialog();
    }
    
    m_vVelocity = CCPointZero;
    m_ptLastMapPosition = m_pMapNode->getPosition();

    m_ptTouchCenter = GetAverageLocationInViewTouchSet(&m_setTouches);
    m_fTouchDistance = GetAverageDistanceTouchSet(&m_setTouches);
    
    m_pMapNode->stopAllActions();
    
    if (m_setTouches.count() == 1)
    {
        CCTouch* touch = dynamic_cast<CCTouch*>(*m_setTouches.begin());
        CCPoint ptTouch = m_pMapNode->convertToNodeSpace(touch->getLocation());
        
        CCPoint ptLandmark = ccp(-m_mapSize.width / 2.0f + m_ptLandmark.x, m_mapSize.height / 2.0f - m_ptLandmark.y);
        if (ccpDistance(ptLandmark, ptTouch) < m_pSpriteLandmark->getContentSize().width / 2.0f)
        {
            m_pSpriteLandmark->setVisible(true);
        }
    }
    else
    {
        m_pSpriteLandmark->setVisible(false);
    }
}

void MapViewerScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (m_pSpriteLandmark->isVisible())
    {
        CCTouch* touch = dynamic_cast<CCTouch*>(*m_setTouches.begin());
        CCPoint ptTouch = m_pMapNode->convertToNodeSpace(touch->getLocation());
        CCLOG("Testing %f, %f", ptTouch.x, ptTouch.y);
        
        CCPoint ptLandmark = ccp(-m_mapSize.width / 2.0f + m_ptLandmark.x, m_mapSize.height / 2.0f - m_ptLandmark.y);
        if (ccpDistance(ptLandmark, ptTouch) >= m_pSpriteLandmark->getContentSize().width / 2.0f)
        {
            m_pSpriteLandmark->setVisible(false);
        }
        
        m_ptTouchCenter = GetAverageLocationInViewTouchSet(&m_setTouches);
    }
    else
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
}

void MapViewerScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++)
    {
        m_setTouches.removeObject(*it);
    }
    
    m_ptTouchCenter = GetAverageLocationInViewTouchSet(&m_setTouches);
    m_fTouchDistance = GetAverageDistanceTouchSet(&m_setTouches);
    
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

        //only show the location dialog if we have touched the landmark
        if (m_pSpriteLandmark->isVisible())
        {
            CCPoint ptLandmark = ccp(-m_mapSize.width / 2.0f + m_ptLandmark.x, m_mapSize.height / 2.0f - m_ptLandmark.y + 80.0f);
            CCPoint ptDialog = m_pMapNode->convertToWorldSpace(ptLandmark);
            
            //figure out if we need to move the map view to center on the landmark
            CCSize sizeDeadZoneArea;
            sizeDeadZoneArea.width  = CCDirector::sharedDirector()->getWinSizeInPixels().width / 4.0f;
            sizeDeadZoneArea.height = CCDirector::sharedDirector()->getWinSizeInPixels().height / 4.0f;
            if (ptDialog.x < sizeDeadZoneArea.width ||
                ptDialog.x > CCDirector::sharedDirector()->getWinSizeInPixels().width - sizeDeadZoneArea.width ||
                ptDialog.y < sizeDeadZoneArea.height ||
                ptDialog.y > CCDirector::sharedDirector()->getWinSizeInPixels().height - sizeDeadZoneArea.height)
            {
                m_pMapNode->stopAllActions();
                
                ptDiffFromNodePos = ccpSub(ptDialog, ptCenterGL);
                ptAdjust = ccpSub(m_pMapNode->getPosition(), ptDiffFromNodePos);
                m_pMapNode->runAction(
                    CCSequence::create(
                        CCEaseInOut::create(CCMoveTo::create(0.7f, ptAdjust), 4.0f),
                        CCCallFunc::create(this, callfunc_selector(MapViewerScene::ShowLocationDialogCallback)),
                        NULL
                    )
                );
            }
            else
            {
                m_pLocationDialog->ShowDialog();
            }
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

void MapViewerScene::ShowLocationDialogCallback()
{
    m_pLocationDialog->ShowDialog();
}
