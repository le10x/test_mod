#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(GDRateStarsLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        bool enabled = Mod::get()->getSettingValue<bool>("show-rate");
        if (!enabled) return true;

        if (auto menu = this->getChildByID("left-side-menu")) {
            
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_starsIcon_001.png", 
                1.0f, 
                CircleBaseColor::Blue
            );
            
            if (btnSprite) {
                auto myButton = CCMenuItemSpriteExtra::create(
                    btnSprite,
                    this,
                    menu_selector(GDRateStarsLayer::onShortcutRateStars)
                );

                myButton->setID("shortcut-rate-stars-button");
                menu->addChild(myButton);
                menu->updateLayout();
            }
        }
        return true;
    }

    void onShortcutRateStars(CCObject* sender) {
        if (m_level && GameLevelManager::sharedState()) {

            int64_t selectedStars = Mod::get()->getSettingValue<int64_t>("rate-stars");

            GameLevelManager::sharedState()->rateStars(m_level->m_levelID, static_cast<int>(selectedStars));

            std::string message = std::to_string(selectedStars) + " Stars submitted!";
            FLAlertLayer::create("Geode", message.c_str(), "OK")->show();
        }
    }
};
