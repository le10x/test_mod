#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

// Variables de control de estado global
bool g_yesclip = false;
bool g_noclip = false;
bool g_notificationPending = false; // Rastrea si la notificación aún no se ha mostrado

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        // Leer la posición desde el mod.json
        std::string side = Mod::get()->getSettingValue<std::string>("button-side");
        
        std::string menuID = (side == "Izquierda") ? "left-button-menu" : "right-button-menu";
        auto menu = this->getChildByID(menuID);
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
        g_notificationPending = true; // La ruleta se jugó, hay un anuncio pendiente
        FLAlertLayer::create("Random Noclip", "Noclip enabled? Find out by playing!", "Ok") -> show();
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    // Función centralizada para disparar la notificación de forma segura una sola vez
    void triggerNoclipNotification() {
        if (g_notificationPending) {
            g_notificationPending = false; // Consumimos la notificación de inmediato
            if (g_noclip) {
                Notification::create("Noclip was activated!", NotificationIcon::Success, 2.0f) -> show();
            } else {
                Notification::create("Noclip was disabled!", NotificationIcon::Error, 2.0f) -> show();
            }
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        // Limpieza total al reiniciar
        g_noclip = false;
        g_yesclip = false;
        g_notificationPending = false;
    }

    void onQuit() {
        PlayLayer::onQuit();
        // Limpieza total al salir al menú principal
        g_noclip = false;
        g_yesclip = false;
        g_notificationPending = false;
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        // Si el mod está activo y hay colisión, revelamos el destino del jugador
        if (g_yesclip) {
            triggerNoclipNotification();
        }

        // Si la ruleta dio Noclip, evitamos que el jugador sea destruido
        if (g_noclip) {
            return;
        }

        // De lo contrario, muere normalmente
        PlayLayer::destroyPlayer(player, object);
    }

    void levelComplete() {
        // Si el jugador llega a la meta y nunca chocó, revelamos su suerte al ganar
        if (g_yesclip && g_notificationPending) {
            triggerNoclipNotification();
        }

        // Reseteamos estados tras completar exitosamente el nivel
        g_noclip = false;
        g_yesclip = false;
        g_notificationPending = false;

        PlayLayer::levelComplete();
    }
};
