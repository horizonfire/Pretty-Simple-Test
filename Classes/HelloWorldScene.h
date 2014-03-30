#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    HelloWorld();
    virtual ~HelloWorld();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);


    virtual void update(float delta);

    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
private:
    cocos2d::CCPoint GetAverageLocationInViewTouchSet(cocos2d::CCSet *pTouches);
    bool IsMapPositionStretched(const cocos2d::CCPoint& ptPosition);

    cocos2d::CCSprite* m_pMapSprite;
    cocos2d::CCPoint   m_ptTouchCenter;

    bool               m_bIsZooming;
    float			   m_fDefaultScale;
    float              m_fMinMapX;
    float              m_fMaxMapX;
    float              m_fMinMapY;
    float              m_fMaxMapY;

    float              m_fResistance;
    float              m_fStretchVariance;
};

#endif // __HELLOWORLD_SCENE_H__
