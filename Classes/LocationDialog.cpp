#include "LocationDialog.h"
#include "AppMacros.h"

USING_NS_CC;
USING_NS_CC_EXT;

LocationDialog::LocationDialog() :
    m_pTextLabel(NULL),
    m_pArrowSprite(NULL),
    m_pBackgroundSprite(NULL)
{
}


LocationDialog::~LocationDialog()
{
}

LocationDialog* LocationDialog::create()
{
    LocationDialog* pRet = new LocationDialog();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool LocationDialog::init()
{
    //add the background sprite
    m_pBackgroundSprite = CCScale9Sprite::create("dialogbackground.png");
    addChild(m_pBackgroundSprite, 0);

    //add the arrow
    m_pArrowSprite = CCSprite::create("dialogarrow.png");
    m_pArrowSprite->setPosition(CCPointZero);
    addChild(m_pArrowSprite, 0);

    //add the text label
    m_pTextLabel = CCLabelTTF::create("Hello World\nHello World\nHello World", "Arial", LABEL_FONT_SIZE);
    m_pTextLabel->setColor(ccc3(0,0,0));
    addChild(m_pTextLabel, 0);

    m_pBackgroundSprite->setContentSize(m_pTextLabel->getContentSize() + CCSizeMake(20, 20));

    //layout the various elements

    CCPoint ptTextPos = m_pArrowSprite->getPosition();
    ptTextPos.y += (m_pBackgroundSprite->getContentSize().height + m_pArrowSprite->getContentSize().height) / 2.0f - 1.0f;
    m_pTextLabel->setPosition(ptTextPos);
    m_pBackgroundSprite->setPosition(ptTextPos);

    setVisible(false);

    return true;
}

void LocationDialog::ShowDialog()
{
    stopAllActions();
    runAction(
        CCSequence::create(
            CCScaleTo::create(0.0f, 0.0f, 0.0f),
            CCShow::create(),
            //CCOrbitCamera::create(0.2f, 1.0f, 0.0f, 90.0f, -90.0f, 0.0f, 0.0f),
            CCScaleTo::create(0.1f, 0.1f, 1.1f),
            CCScaleTo::create(0.1f, 1.1f, 1.0f),
            CCScaleTo::create(0.1f, 1.0f, 1.0f),
            NULL
        )
    );
}

void LocationDialog::HideDialog()
{
    stopAllActions();
    runAction(
        CCSequence::create(
            CCScaleTo::create(0.1f, 1.1f, 1.0f),
            CCScaleTo::create(0.1f, 0.1f, 1.0f),
            CCScaleTo::create(0.1f, 0.0f, 0.0f),
            //CCOrbitCamera::create(0.2f, 1.0f, 0.0f, 0.0f, -90.0f, 0.0f, 0.0f),
            CCHide::create(),
            CCRemoveSelf::create(),
            NULL
        )
    );
}

void LocationDialog::setTintColor(const ccColor3B& color)
{
    m_pArrowSprite->setColor(color);
    m_pBackgroundSprite->setColor(color);
}

void LocationDialog::setFontColor(const cocos2d::ccColor3B& color)
{
    m_pTextLabel->setColor(color);
}


