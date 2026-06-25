#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GameObject.hpp>

using namespace geode::prelude;

bool g_suerteDecidida = false;
bool g_noclipSecreto = false;

// 1. AÑADIR EL BOTÓN DE PAUSA
class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        auto rightMenu = this->getChildByID("right-button-menu");
        if (!rightMenu) return;

        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("¿?", "goldFont.fnt", "GJ_button_02.png", 0.7f),
            this, menu_selector(MyPauseLayer::alPresionar)
        );
        rightMenu->addChild(btn);
        rightMenu->updateLayout();
    }

    void alPresionar(CCObject*) {
        g_noclipSecreto = (std::rand() % 2 == 1);
        g_suerteDecidida = true;
        FLAlertLayer::create("Destino Sellado", "El azar ha decidido tu suerte en secreto. <y>Averígualo jugando...</y>", "¡Ok!") -> show();
    }
};

// 2. LÓGICA DEL NOCLIP SECRETO (Evita errores de funciones inline)
class $modify(GameObject) {
    bool collideWithPlayer(PlayerObject* player) {
        if (g_suerteDecidida) {
            g_suerteDecidida = false; // Solo avisa en el primer choque
            if (g_noclipSecreto) {
                Notification::create("¡BENDITO POR EL AZAR! (Noclip Activo)", NotificationIcon::Success, 1.5f) -> show();
            } else {
                Notification::create("¡EL DESTINO TE ABANDONÓ!", NotificationIcon::Error, 2.0f) -> show();
            }
        }
        // Si ganaste el 50% secreto, cancelamos la colisión regresando falso
        if (g_noclipSecreto) return false;
        
        return GameObject::collideWithPlayer(player);
    }
};
