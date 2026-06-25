#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

bool g_suerteDecidida = false;
bool g_noclipSecreto = false;

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        auto menu = this->getChildByID("right-button-menu");
        if (!menu) return;

        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("¿?", "goldFont.fnt", "GJ_button_02.png", 0.7f),
            this, menu_selector(MyPauseLayer::alPresionar)
        );
        menu->addChild(btn);
        menu->updateLayout();
    }

    void alPresionar(CCObject*) {
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // Semilla del azar
        g_noclipSecreto = (std::rand() % 2 == 1);
        g_suerteDecidida = true;
        FLAlertLayer::create("Destino Sellado", "El azar decidió tu suerte en secreto. <y>Averígualo jugando...</y>", "¡Ok!") -> show();
    }
};

class $modify(GameObject) {
    // Añadimos 'GJCollideInfo*' que es requerido en Geometry Dash 2.2
    bool collideWithPlayer(PlayerObject* player, GJCollideInfo* info) {
        if (g_suerteDecidida) {
            g_suerteDecidida = false; 
            if (g_noclipSecreto) {
                Notification::create("¡BENDITO POR EL AZAR! (Noclip Activo)", NotificationIcon::Success, 1.5f) -> show();
            } else {
                Notification::create("¡EL DESTINO TE ABANDONÓ!", NotificationIcon::Error, 2.0f) -> show();
            }
        }
        if (g_noclipSecreto) return false;
        
        // Pasamos ambos parámetros a la función original del juego
        return GameObject::collideWithPlayer(player, info);
    }
};
