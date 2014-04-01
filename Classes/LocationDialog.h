#ifndef __LOCATION_DIALOG_H__
#define __LOCATION_DIALOG_H__

#include "cocos2d.h"
#include "..\extensions\GUI\CCControlExtension\CCScale9Sprite.h"

class LocationDialog : public cocos2d::CCNode
{
public:
    LocationDialog(void);
    virtual ~LocationDialog(void);

    static LocationDialog* create();

    virtual bool init();

    void ShowDialog();
    void HideDialog();

    void setTintColor(const cocos2d::ccColor3B& color);
    void setFontColor(const cocos2d::ccColor3B& color);

protected:
    cocos2d::CCLabelTTF*                m_pTextLabel;
    cocos2d::CCSprite*                  m_pArrowSprite;
    cocos2d::extension::CCScale9Sprite* m_pBackgroundSprite;
};

#endif // __LOCATION_DIALOG_H__
