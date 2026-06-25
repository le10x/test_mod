#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp> // Usamos únicamente PlayLayer para el juego
#include <cstdlib> 
#include <ctime>   

using namespace geode::prelude;

// Variables globales para controlar el estado del juego
bool g_suerteDecidida = false; // Indica si ya presionaste el botón en esta pausa
bool g_noclipSecreto = false;  // Guarda el resultado secreto (true = vives, false = mueres)

// 1. INYECTAMOS EL BOTÓN EN EL MENÚ DE PAUSA
class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto rightMenu = this->getChildByID("right-button-menu");
        if (!rightMenu) return;

        // Creamos el botón misterioso con el signo "¿?"
        auto botonMisterio = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("¿?", "goldFont.fnt", "GJ_button_02.png", 0.7f),
            this,
            menu_selector(MyPauseLayer::alPresionarBotonMisterio)
        );

        rightMenu->addChild(botonMisterio);
        rightMenu->updateLayout();
    }

    void alPresionarBotonMisterio(CCObject* sender) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // Calculamos el destino en secreto (50% de probabilidad)
        g_noclipSecreto = (std::rand() % 2 == 1);
        g_suerteDecidida = true;

        // Mensaje misterioso al jugador
        FLAlertLayer::create(
            "Destino Sellado",
            "El azar ha decidido tu suerte en secreto. <y>Regresa al juego y averígualo...</y>",
            "¡Qué miedo!"
        )->show();
    }
};

// 2. LÓGICA DE DETECCIÓN Y REINICIO (Todo consolidado en PlayLayer)
class $modify(MyPlayLayer, PlayLayer) {
    
    // Interceptamos la muerte del jugador de forma segura desde PlayLayer
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        
        // Si el jugador decidió tentar a la suerte en el menú de pausa...
        if (g_suerteDecidida) {
            
            if (g_noclipSecreto) {
                // ¡Ganó el 50%! Mostramos la notificación flotante de éxito
                Notification::create("¡BENDITO POR EL AZAR! (Noclip Activo)", NotificationIcon::Success, 1.5f)->show();
                
                g_suerteDecidida = false; 
                return; // Evitamos la muerte (Noclip activo)
            } else {
                // Perdió el 50%. Mostramos notificación de fracaso justo antes de explotar
                Notification::create("¡EL DESTINO TE ABANDONÓ!", NotificationIcon::Error, 2.0f)->show();
                
                g_suerteDecidida = false;
                PlayLayer::destroyPlayer(player, object); // Ejecuta la muerte normal
            }
            
        } else {
            // Comportamiento normal si no usó el botón de la suerte
            PlayLayer::destroyPlayer(player, object);
        }
    }

    // Interceptamos el reinicio del nivel de forma segura desde PlayLayer
    void resetLevel() {
        PlayLayer::resetLevel();
        
        // Limpiamos los estados para la próxima ronda o intento
        g_suerteDecidida = false;
        g_noclipSecreto = false;
    }
};
