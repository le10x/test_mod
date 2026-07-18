#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(DislikeLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        // Comprobar si el usuario activó este botón en la configuración
        bool habilitado = Mod::get()->getSettingValue<bool>("mostrar-dislike");
        if (!habilitado) return true;

        if (auto menu = this->getChildByID("left-side-menu")) {
            auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_dislikeBtn_001.png");
            
            if (btnSprite) {
                auto myButton = CCMenuItemSpriteExtra::create(
                    btnSprite,
                    this,
                    menu_selector(DislikeLayer::onAlternativeDislike)
                );

                myButton->setID("alt-dislike-button");
                menu->addChild(myButton);
                menu->updateLayout();
            }
        }
        return true;
    }

    void onAlternativeDislike(CCObject* sender) {
        if (m_level && GameLevelManager::sharedState()) {
            GameLevelManager::sharedState()->likeItem(
                static_cast<LikeItemType>(1), 
                m_level->m_levelID, 
                false, 
                0
            );
            FLAlertLayer::create("Geode", "¡Dislike enviado!", "OK")->show();
        }
    }
};
