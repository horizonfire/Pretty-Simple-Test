#ifndef __MAPVIEWER_SCENE_H__
#define __MAPVIEWER_SCENE_H__

#include "cocos2d.h"
#include "LocationDialog.h"

class MapViewerScene : public cocos2d::CCLayer
{
public:
    MapViewerScene();
    virtual ~MapViewerScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(MapViewerScene);


    virtual void update(float delta);

    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
private:
    float GetAverageDistanceTouchSet(cocos2d::CCSet *pTouches);
    cocos2d::CCPoint GetAverageLocationInViewTouchSet(cocos2d::CCSet *pTouches);
    
    void ShowLocationDialogCallback();

    LocationDialog*    m_pLocationDialog;

    cocos2d::CCNode*   m_pMapNode;
    cocos2d::CCSprite* m_pSpriteLandmark;
    cocos2d::CCPoint   m_ptLandmark;
    cocos2d::CCPoint   m_ptTouchCenter;

    cocos2d::CCPoint   m_ptLastMapPosition;
    cocos2d::CCPoint   m_vVelocity;
    cocos2d::CCSize    m_mapSize;

    cocos2d::CCSet     m_setTouches;
    float              m_fTouchDistance;
    float			   m_fMinScale;
    float              m_fMaxScale;
    float              m_fMinMapX;
    float              m_fMaxMapX;
    float              m_fMinMapY;
    float              m_fMaxMapY;

    float              m_fResistance;
    float              m_fResistanceScale;
    float              m_fStretchVariance;
};

#endif // __MAPVIEWER_SCENE_H__
