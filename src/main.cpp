#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
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

        // Creamos el botón misterioso
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

        // Mensaje misterioso: No le decimos el resultado al jugador
        FLAlertLayer::create(
            "Destino Sellado",
            "El azar ha decidido tu suerte en secreto. <y>Regresa al juego y averígualo...</y>",
            "¡Qué miedo!"
        )->show();
    }
};

// 2. LÓGICA DE DETECCIÓN EN EL JUEGO
class $modify(MySecretLogic, GJBaseGameLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        
        // Si el jugador presionó el botón de la suerte...
        if (g_suerteDecidida) {
            
            if (g_noclipSecreto) {
                // ¡Tuvo suerte! Mostramos una notificación rápida en la pantalla (Notification)
                // Usamos Notification porque no pausa el juego, solo aparece arriba
                Notification::create("¡BENDITO POR EL AZAR! (Noclip Activo)", NotificationIcon::Success, 1.5f)->show();
                
                // Desactivamos g_suerteDecidida para que no se llene la pantalla de mensajes a cada segundo
                g_suerteDecidida = false; 
                return; // Sobrevive
            } else {
                // Mala suerte. Mostramos la notificación de fracaso justo antes de morir
                Notification::create("¡EL DESTINO TE ABANDONÓ!", NotificationIcon::Error, 2.0f)->show();
                
                g_suerteDecidida = false;
                GJBaseGameLayer::destroyPlayer(player, object); // Muere
            }
            
        } else {
            // Si nunca presionó el botón de pausa, el juego actúa normal
            GJBaseGameLayer::destroyPlayer(player, object);
        }
    }

    void resetLevel() {
        GJBaseGameLayer::resetLevel();
        // Al reiniciar el nivel, limpiamos los estados para la siguiente prueba
        g_suerteDecidida = false;
        g_noclipSecreto = false;
    }
};
