#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

bool g_yesclip = false;
bool g_noclip = false;

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        // 1. Leer el ajuste del archivo mod.json
        std::string side = Mod::get()->getSettingValue<std::string>("button-side");
        
        // 2. Determinar el ID del menú según la preferencia
        std::string menuID = (side == "Izquierda") ? "left-button-menu" : "right-button-menu";
        auto menu = this->getChildByID(menuID);
        
        if (!menu) return;

        // 3. Crear y añadir el botón al menú seleccionado
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
        FLAlertLayer::create("Random Noclip", "Noclip enabled? Find out by playing!", "Ok") -> show();
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    // Función auxiliar para mostrar la notificación según el resultado
    void showNoclipNotification() {
        if (g_noclip) {
            Notification::create("Noclip was activated!", NotificationIcon::Success, 2.0f) -> show();
        } else {
            Notification::create("Noclip was disabled!", NotificationIcon::Error, 2.0f) -> show();
        }
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        // Si chocamos y la ruleta sigue activa, revelamos el resultado de inmediato
        if (g_yesclip) {
            g_yesclip = false;
            showNoclipNotification();
        }

        if (g_noclip) return;

        PlayLayer::destroyPlayer(player, object);
    }

    void levelComplete() {
        // Si completas el nivel sin haber chocado nunca antes (g_yesclip sigue vivo)
        if (g_yesclip) {
            g_yesclip = false;
            showNoclipNotification();
        }

        PlayLayer::levelComplete();
    }
};
