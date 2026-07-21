#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(LikeLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        bool enabled = Mod::get()->getSettingValue<bool>("show-like");
        if (!enabled) return true;

        if (auto menu = this->getChildByID("left-side-menu")) {
            auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
            
            if (btnSprite) {
                auto myButton = CCMenuItemSpriteExtra::create(
                    btnSprite,
                    this,
                    menu_selector(LikeLayer::onShortcutLike)
                );

                myButton->setID("shortcut-like-button");
                menu->addChild(myButton);
                menu->updateLayout();
            }
        }
        return true;
    }

    void onShortcutLike(CCObject* sender) {
        if (m_level && GameLevelManager::sharedState()) {
            GameLevelManager::sharedState()->likeItem(
                static_cast<LikeItemType>(1), 
                m_level->m_levelID, 
                true, 
                0
            );
            FLAlertLayer::create("Geode", "Like submitted!", "OK")->show();
        }
    }
};
