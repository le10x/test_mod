#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/GameLevelManager.hpp>

using namespace geode::prelude;

bool g_isGhostModeActive = false;

class $modify(MyLevelCell, LevelCell) {
    void loadCustomLevelCell() {
        LevelCell::loadCustomLevelCell();

        // Buscamos el botón original usando tanto su ID como su tipo en el árbol de nodos
        auto getItBtn = this->m_mainLayer->getChildByID("get-it-button");
        
        // Creamos un menú limpio y dedicado exclusivamente para nuestro botón para que no falle el renderizado
        auto customMenu = CCMenu::create();
        if (!customMenu) return;
        
        customMenu->setPosition(0, 0);
        customMenu->setID("ghost-menu"_spr);

        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        if (buttonSprite) {
            buttonSprite->setScale(0.4f); 

            auto ghostButton = CCMenuItemSpriteExtra::create(
                buttonSprite, this, menu_selector(MyLevelCell::onGhostGetIt)
            );

            // Si detecta el botón original, se posiciona exactamente a su izquierda
            if (getItBtn) {
                ghostButton->setPosition(getItBtn->getPositionX() - 35.0f, getItBtn->getPositionY());
            } else {
                // Posición manual de respaldo ajustada para pantallas de Android de 16:9 y 20:9
                ghostButton->setPosition(this->m_mainLayer->getContentSize().width - 95.0f, 28.0f);
            }

            ghostButton->setID("ghost-get-button"_spr);
            customMenu->addChild(ghostButton);
            
            // Añadimos el menú directamente sobre la capa principal de la celda con máxima prioridad visual (Z-Order)
            this->m_mainLayer->addChild(customMenu, 100);
        }
    }

    void onGhostGetIt(CCObject* sender) {
        g_isGhostModeActive = true;
        GameLevelManager::sharedState()->downloadLevel(this->m_level->m_levelID.value(), false, 0);
    }
};

class $modify(GhostModeNetwork, GameLevelManager) {
    void downloadLevel(int levelID, bool isGauntlet, int dailyID) {
        if (g_isGhostModeActive) {
            g_isGhostModeActive = false;
            return; 
        }
        GameLevelManager::downloadLevel(levelID, isGauntlet, dailyID);
    }
};
