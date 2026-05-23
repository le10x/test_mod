#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// 1. MANEJO DE LA INTERFAZ (MENÚ DE PAUSA)
class $modify(MyPauseLayer, PauseLayer) {
    void onToggleTwoPlayer(CCObject* sender) {
        if (auto pl = PlayLayer::get()) {
            // Modifica directamente el estado del nivel en tiempo real
            pl->m_level->m_twoPlayerMode = !pl->m_level->m_twoPlayerMode;
            
            // Muestra una notificación emergente con el estado actual
            auto msg = pl->m_level->m_twoPlayerMode ? "Modo 2P Activo" : "Modo 1P Activo";
            Notification::create(msg, NotificationIcon::Info)->show();
        }
    }

    // Corregido: Ahora recibe el argumento unfocused exigido por las nuevas versiones de Geode
    bool init(bool unfocused) {
        // Le pasamos el argumento al init original del juego
        if (!PauseLayer::init(unfocused)) return false;

        // Buscamos el menú lateral derecho nativo del juego
        if (auto menu = this->getChildByID("right-button-menu")) {
            auto pl = PlayLayer::get();
            // Detecta si el nivel ya es de 2 jugadores en este momento
            bool is2P = pl ? pl->m_level->m_twoPlayerMode : false;

            // Creamos el botón "Check" con texturas originales de Geometry Dash
            auto toggleBtn = CCMenuItemToggler::create(
                CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
                CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
                this, menu_selector(MyPauseLayer::onToggleTwoPlayer)
            );

            toggleBtn->toggle(is2P); // Sincroniza el aspecto visual del check
            toggleBtn->setID("two-player-toggle"_spr); // ID único para evitar conflictos con otros mods
            
            menu->addChild(toggleBtn);
            menu->updateLayout(); // Reordena el menú automáticamente
        }
        return true;
    }
};

// 2. MANEJO DE LA LÓGICA (REINICIO AL SALIR)
class $modify(MyPlayLayer, PlayLayer) {
    void onQuit() {
        // Al abandonar la partida, aseguramos que el nivel regrese a su estado original (1 Jugador)
        if (m_level) {
            m_level->m_twoPlayerMode = false;
        }
        
        // Ejecuta la salida normal del juego
        PlayLayer::onQuit();
    }
};
