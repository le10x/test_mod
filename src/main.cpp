#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        // Buscar el menú lateral izquierdo de la pantalla
        if (auto menu = this->getChildByID("left-side-menu")) {
            
            // Crear un botón circular verde con el icono de LIKE nativo de GD
            auto myButton = CCMenuItemSpriteExtra::create(
                CircleButtonSprite::createWithSpriteFrameName("GJ_likeBtn_001.png", 1.0f, CircleBaseColor::Green),
                this,
                menu_selector(MyLevelInfoLayer::onAlternativeLike)
            );

            // Añadir el botón al menú y actualizar la posición automáticamente
            menu->addChild(myButton);
            menu->updateLayout();
        }

        return true;
    }

    void onAlternativeLike(CCObject* sender) {
        // Enviar el LIKE real directamente saltándose el bloqueo del juego
        if (m_level && GameLevelManager::sharedState()) {
            GameLevelManager::sharedState()->likeItem(GJLevelType::Saved, m_level->m_levelID, true);
            FLAlertLayer::create("Geode", "¡Like enviado!", "OK")->show();
        }
    }
};
