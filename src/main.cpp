#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/GameLevelManager.hpp>

using namespace geode::prelude;

bool g_isGhostModeActive = false;

class $modify(MyLevelCell, LevelCell) {
    void loadCustomLevelCell() {
        LevelCell::loadCustomLevelCell();

        auto mainMenu = this->m_mainLayer->getChildByID("main-menu");
        if (!mainMenu) mainMenu = typeinfo_cast<CCMenu*>(this->m_mainLayer->getChildByTag(1));
        auto getItBtn = this->m_mainLayer->getChildByID("get-it-button");

        if (mainMenu) {
            auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
            if (buttonSprite) {
                buttonSprite->setScale(0.4f); 

                auto ghostButton = CCMenuItemSpriteExtra::create(
                    buttonSprite, this, menu_selector(MyLevelCell::onGhostGetIt)
                );

                if (getItBtn) {
                    ghostButton->setPosition(getItBtn->getPositionX() - 35.0f, getItBtn->getPositionY());
                } else {
                    ghostButton->setPosition(340.0f, 25.0f);
                }

                ghostButton->setID("ghost-get-button"_spr);
                mainMenu->addChild(ghostButton);
            }
        }
    }

    void onGhostGetIt(CCObject* sender) {
        g_isGhostModeActive = true;
        GameLevelManager::sharedState()->downloadLevel(this->m_level->m_levelID.value(), false);
    }
};

class $modify(GhostModeNetwork, GameLevelManager) {
    void downloadLevel(int levelID, bool isGauntlet) {
        if (g_isGhostModeActive) {
            g_isGhostModeActive = false;
            return; 
        }
        GameLevelManager::downloadLevel(levelID, isGauntlet);
    }
};
