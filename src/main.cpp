#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables globales para el estado temporal de la sesión
static bool g_forceTwoPlayerOverride = false;
static bool g_hasModifiedThisSession = false;

// 1. MANEJO DE LA INTERFAZ (MENÚ DE PAUSA)
class $modify(MyPauseLayer, PauseLayer) {
    void onToggleTwoPlayer(CCObject* sender) {
        if (auto pl = PlayLayer::get()) {
            g_hasModifiedThisSession = true;
            g_forceTwoPlayerOverride = !g_forceTwoPlayerOverride;
            
            if (pl->m_level) {
                pl->m_level->m_twoPlayerMode = g_forceTwoPlayerOverride;
            }

            // CORREGIDO: Usamos el nombre real del método en los bindings de Geode
            pl->toggleTwoPlayerMode();

            auto msg = g_forceTwoPlayerOverride ? "Modo 2P Activo" : "Modo 1P Activo";
            Notification::create(msg, NotificationIcon::Info)->show();
        }
    }

    bool init(bool unfocused) {
        if (!PauseLayer::init(unfocused)) return false;

        if (auto menu = this->getChildByID("right-button-menu")) {
            auto pl = PlayLayer::get();
            
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
    
    // CORREGIDO: Ahora recibe y pasa los 3 parámetros requeridos por la versión 2.2081
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (g_hasModifiedThisSession && level) {
            level->m_twoPlayerMode = g_forceTwoPlayerOverride;
        }
        return PlayLayer::init(level, useReplay, dontCreateObjects);
    }

    void onQuit() {
        g_forceTwoPlayerOverride = false;
        g_hasModifiedThisSession = false;
        
        if (m_level) {
            m_level->m_twoPlayerMode = false;
        }
        
        PlayLayer::onQuit();
    }
};
