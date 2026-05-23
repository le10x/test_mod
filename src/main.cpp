#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variable global interna para almacenar de forma temporal si forzamos el modo 2P
static bool g_forceTwoPlayerOverride = false;
static bool g_hasModifiedThisSession = false;

// 1. MANEJO DE LA INTERFAZ (MENÚ DE PAUSA)
class $modify(MyPauseLayer, PauseLayer) {
    void onToggleTwoPlayer(CCObject* sender) {
        if (auto pl = PlayLayer::get()) {
            // Marcamos que el usuario alteró el ajuste manualmente
            g_hasModifiedThisSession = true;
            g_forceTwoPlayerOverride = !g_forceTwoPlayerOverride;
            
            // Forzamos también el dato en el objeto del nivel
            if (pl->m_level) {
                pl->m_level->m_twoPlayerMode = g_forceTwoPlayerOverride;
            }

            // REFRESCAR EN TIEMPO REAL: Re-ejecutamos la configuración de controles del juego
            pl->setupTwoPlayerMode();

            // Mensaje emergente con el estado actual
            auto msg = g_forceTwoPlayerOverride ? "Modo 2P Activo" : "Modo 1P Activo";
            Notification::create(msg, NotificationIcon::Info)->show();
        }
    }

    bool init(bool unfocused) {
        if (!PauseLayer::init(unfocused)) return false;

        if (auto menu = this->getChildByID("right-button-menu")) {
            auto pl = PlayLayer::get();
            
            // Si el jugador no ha tocado el botón, leemos el estado base del nivel.
            // Si ya lo tocó, respetamos el estado actual de la sesión.
            if (!g_hasModifiedThisSession && pl && pl->m_level) {
                g_forceTwoPlayerOverride = pl->m_level->m_twoPlayerMode;
            }

            auto toggleBtn = CCMenuItemToggler::create(
                CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
                CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
                this, menu_selector(MyPauseLayer::onToggleTwoPlayer)
            );

            toggleBtn->toggle(g_forceTwoPlayerOverride); 
            toggleBtn->setID("two-player-toggle"_spr);
            
            menu->addChild(toggleBtn);
            menu->updateLayout(); 
        }
        return true;
    }
};

// 2. MANEJO DE LA LÓGICA INTERNA Y AUTO-LIMPIEZA
class $modify(MyPlayLayer, PlayLayer) {
    
    // Forzamos el estado real al iniciar el nivel por si se configuró antes
    bool init(GJGameLevel* level, bool useReplay) {
        if (g_hasModifiedThisSession && level) {
            level->m_twoPlayerMode = g_forceTwoPlayerOverride;
        }
        return PlayLayer::init(level, useReplay);
    }

    // Se ejecuta estrictamente cuando el jugador abandona el nivel al menú
    void onQuit() {
        // Apagamos los modificadores manuales y limpiamos la memoria
        g_forceTwoPlayerOverride = false;
        g_hasModifiedThisSession = false;
        
        if (m_level) {
            m_level->m_twoPlayerMode = false;
        }
        
        PlayLayer::onQuit();
    }
};
