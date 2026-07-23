#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>

using namespace geode::prelude;

class $modify(GameManager) {

    bool isIconUnlocked(int iconID, IconType iconType) {
        bool bypassActive = Mod::get()->getSettingValue<bool>("bypass-icons");
        
        if (bypassActive) {
            return true;
        }
        return GameManager::isIconUnlocked(iconID, iconType);
    }

    bool isColorUnlocked(int colorID, UnlockType unlockType) {
        bool bypassActive = Mod::get()->getSettingValue<bool>("bypass-icons");
        
        if (bypassActive) {
            return true;
        }
        return GameManager::isColorUnlocked(colorID, unlockType);
    }

    bool hasItemUnlocked(UnlockType unlockType, int itemID) {
        bool bypassActive = Mod::get()->getSettingValue<bool>("bypass-icons");
        
        if (bypassActive) {
            return true;
        }
        return GameManager::hasItemUnlocked(unlockType, itemID);
    }
};
