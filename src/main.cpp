#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

bool g_isRouletteActive = false;       
bool g_isNoclipEnabled = false;        
bool g_hasPendingNotification = false; 

// Puntero global seguro para rastrear nuestra etiqueta de texto en pantalla
class MyPlayLayer;
CCLabelBMFont* g_statusLabel = nullptr;

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        std::string side = Mod::get()->getSettingValue<std::string>("button-side");
        std::string menuID = (side == "Left") ? "left-button-menu" : "right-button-menu";
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
        g_isNoclipEnabled = (std::rand() % 2 == 1);
        g_isRouletteActive = true;
        g_hasPendingNotification = true;
        
        // Si el nivel ya está corriendo y pulsamos el botón, creamos o actualizamos el texto inmediatamente
        if (g_statusLabel) {
            g_statusLabel->stopAllActions();
            g_statusLabel->setString("Random Noclip");
            g_statusLabel->setFntFile("goldFont.fnt");
            g_statusLabel->setColor({255, 255, 255}); // Color nativo de goldFont
            g_statusLabel->setVisible(true);
        }
        
        FLAlertLayer::create("Random Noclip", "Noclip enabled? Find out by playing!", "Ok") -> show();
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    // Inicializamos el nivel y creamos la etiqueta debajo del porcentaje
    bool init(GJGameLevel* level, bool useReplay, bool dontRunLevel) {
        if (!PlayLayer::init(level, useReplay, dontRunLevel)) return false;
        
        g_statusLabel = nullptr; // Resetear puntero al cargar un nivel nuevo

        // Buscamos la interfaz de usuario (UILayer)
        if (auto uiLayer = this->getChildByID("UILayer")) {
            // Buscamos el texto del porcentaje nativo del juego
            if (auto percentageLabel = uiLayer->getChildByID("percentage-label")) {
                
                // Creamos nuestro texto inicial oculto en fuente dorada
                g_statusLabel = CCLabelBMFont::create("", "goldFont.fnt");
                g_statusLabel->setScale(0.45f); // Tamaño ideal para que quepa debajo
                
                // Lo posicionamos exactamente debajo del porcentaje actual
                g_statusLabel->setPosition({
                    percentageLabel->getPositionX(), 
                    percentageLabel->getPositionY() - 15.0f
                });
                
                // Si el mod ya venía activo de la pausa anterior, le damos formato
                if (g_isRouletteActive) {
                    g_statusLabel->setString("Random Noclip");
                    g_statusLabel->setVisible(true);
                } else {
                    g_statusLabel->setVisible(false);
                }

                uiLayer->addChild(g_statusLabel);
            }
        }
        return true;
    }

    void triggerNoclipNotification() {
        if (g_hasPendingNotification) {
            g_hasPendingNotification = false; 
            if (g_isNoclipEnabled) {
                Notification::create("Noclip was activated!", NotificationIcon::Success, 2.0f) -> show();
            } else {
                Notification::create("Noclip was disabled!", NotificationIcon::Error, 2.0f) -> show();
            }
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        g_isNoclipEnabled = false;
        g_isRouletteActive = false;
        g_hasPendingNotification = false;
        
        // Ocultamos la etiqueta al reiniciar de forma limpia
        if (g_statusLabel) {
            g_statusLabel->stopAllActions();
            g_statusLabel->setVisible(false);
        }
    }

    void onQuit() {
        PlayLayer::onQuit();
        g_isNoclipEnabled = false;
        g_isRouletteActive = false;
        g_hasPendingNotification = false;
        g_statusLabel = nullptr;
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        // Ocurre un choque letal (con o sin Noclip)
        if (g_isRouletteActive) {
            triggerNoclipNotification();
            
            // Transformamos el texto a "Game Over" parpadeante en rojo (Letra Pusab estándar del juego)
            if (g_statusLabel) {
                g_statusLabel->stopAllActions();
                g_statusLabel->setString("Game Over");
                g_statusLabel->setFntFile("bigFont.fnt"); // "bigFont" es la fuente Pusab clásica
                g_statusLabel->setColor({255, 50, 50});    // Tinte Rojo brillante
                g_statusLabel->setVisible(true);

                // Creamos una animación cíclica que cambia la opacidad para simular el pulso
                auto fadeOut = CCFadeTo::create(0.3f, 75);   // Baja opacidad a 75
                auto fadeIn = CCFadeTo::create(0.3f, 255);   // Sube opacidad al máximo
                auto sequence = CCSequence::create(fadeOut, fadeIn, nullptr);
                auto repeat = CCRepeatForever::create(sequence);
                
                g_statusLabel->runAction(repeat);
            }
        }

        if (g_isNoclipEnabled) {
            return;
        }

        PlayLayer::destroyPlayer(player, object);
    }

    void levelComplete() {
        if (g_isRouletteActive && g_hasPendingNotification) {
            triggerNoclipNotification();
        }

        g_isNoclipEnabled = false;
        g_isRouletteActive = false;
        g_hasPendingNotification = false;

        if (g_statusLabel) {
            g_statusLabel->stopAllActions();
            g_statusLabel->setVisible(false);
        }

        PlayLayer::levelComplete();
    }
};
