#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/LevelLeaderboard.hpp>

using namespace geode::prelude;

class $modify(LDPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        if (!Mod::get()->getSettingValue<bool>("leaderboard-pausemenu")) return;

        std::string targetPosition = Mod::get()->getSettingValue<std::string>("leaderboard-position");
        CCMenu* targetMenu = nullptr;

        if (auto children = this->getChildren()) {
            for (int i = 0; i < children->count(); ++i) {
                // CORREGIDO: Se removio cocos2d:: para compatibilidad estricta con Android64
                if (auto child = typeinfo_cast<CCMenu*>(children->objectAtIndex(i))) {
                    float xPos = child->getPositionX();
                    
                    if (targetPosition == "Right" && xPos > 400.0f) {
                        targetMenu = child;
                        break;
                    } 
                    else if (targetPosition == "Left" && xPos < 100.0f) {
                        targetMenu = child;
                        break;
                    }
                }
            }
        }

        if (targetMenu) {
            if (auto spr = CCSprite::createWithSpriteFrameName("GJ_levelLeaderboardBtn_001.png")) {
                spr->setScale(0.65f);
                if (auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(LDPauseLayer::onLeaderboard))) {
                    targetMenu->addChild(btn);
                    targetMenu->updateLayout();
                }
            }
        }
    }
    
    void onLeaderboard(CCObject* sender) {
        if (auto playLayer = PlayLayer::get()) {
            if (playLayer->m_level) {
                auto gm = GameManager::get();
                auto type = static_cast<LevelLeaderboardType>(gm->getIntGameVariable("0098"));
                auto mode = static_cast<LevelLeaderboardMode>(gm->getIntGameVariable("0164"));
                
                LevelLeaderboard::create(playLayer->m_level, type, mode)->show();
                return;
            }
        }
        FLAlertLayer::create("Uh Oh", "No PlayLayer found, you sure you in a level?", "OK")->show();
    }
};

class $modify(LDLevelLeaderboard, LevelLeaderboard) {
    bool init(GJGameLevel* level, LevelLeaderboardType type, LevelLeaderboardMode mode) {
        if (!LevelLeaderboard::init(level, type, mode)) return false;

        if (!Mod::get()->getSettingValue<bool>("leaderboard-pausemenu")) return true;

        if (auto scene = CCDirector::sharedDirector()->getRunningScene()) {
            this->setZOrder(std::max(105, scene->getHighestChildZ()));
        }
        return true;
    }
};
