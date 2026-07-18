#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        if (auto menu = this->getChildByID("left-side-menu")) {
            
            // Creamos el sprite directamente usando la textura nativa del juego
            auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
            
            if (btnSprite) {
                // Creamos el botón interactivo usando el sprite limpio, sin contenedores extra
                auto myButton = CCMenuItemSpriteExtra::create(
                    btnSprite,
                    this,
                    menu_selector(MyLevelInfoLayer::onAlternativeLike)
                );

                myButton->setID("alt-like-button");

                menu->addChild(myButton);
                menu->updateLayout();
            }
        }

        return true;
    }

    void onAlternativeLike(CCObject* sender) {
        if (m_level && GameLevelManager::sharedState()) {
            GameLevelManager::sharedState()->likeItem(
                static_cast<LikeItemType>(1), 
                m_level->m_levelID, 
                true, 
                0
            );

            FLAlertLayer::create("Geode", "¡Like enviado!", "OK")->show();
        }
    }
};
