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
        // Agregamos un 0 al final como tercer argumento (dailyID)
        GameLevelManager::sharedState()->downloadLevel(this->m_level->m_levelID.value(), false, 0);
    }
};

class $modify(GhostModeNetwork, GameLevelManager) {
    // Añadimos 'int dailyID' a la firma del Hook
    void downloadLevel(int levelID, bool isGauntlet, int dailyID) {
        if (g_isGhostModeActive) {
            g_isGhostModeActive = false;
            return; 
        }
        // Pasamos los 3 argumentos a la función original
        GameLevelManager::downloadLevel(levelID, isGauntlet, dailyID);
    }
};
