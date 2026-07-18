#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        if (auto menu = this->getChildByID("left-side-menu")) {
            
            // Usamos la textura de la mano sola ("GJ_thumbsUpIcon_001.png") 
            // sobre el fondo verde para evitar que se duplique el círculo.
            auto myButton = CCMenuItemSpriteExtra::create(
                CircleButtonSprite::createWithSpriteFrameName("GJ_thumbsUpIcon_001.png", 1.0f, CircleBaseColor::Green),
                this,
                menu_selector(MyLevelInfoLayer::onAlternativeLike)
            );

            myButton->setID("alt-like-button");

            menu->addChild(myButton);
            menu->updateLayout();
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
