#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

bool g_yesclip = false;
bool g_noclip = false;

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        auto menu = this->getChildByID("right-button-menu");
        if (!menu) return;

        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("?", "goldFont.fnt", "GJ_button_02.png", 0.7f),
            this, menu_selector(MyPauseLayer::Press)
        );
        menu->addChild(btn);
        menu->updateLayout();
    }

    void Press(CCObject*) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        g_noclip = (std::rand() % 2 == 1);
        g_yesclip = true;
        // CORREGIDO: Se añadió la comilla que faltaba antes de Ok
        FLAlertLayer::create("Random Noclip", "Noclip enabled? Find out by playing!", "Ok") -> show();
    }
};

class $modify(MyPlayerObject, PlayerObject) {
    void playerDestroyed(bool p0) {
        if (g_yesclip) {
            g_yesclip = false;
            if (g_noclip) {
                Notification::create("Noclip was activated!", NotificationIcon::Success, 2.0f) -> show();
            } else {
                Notification::create("Noclip was disabled!", NotificationIcon::Error, 2.0f) -> show();
            }
        }

        if (g_noclip) return;

        PlayerObject::collidedWithObject(p0);
    }
};
