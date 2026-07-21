#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(RateLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) return false;

        // Comprobar si el usuario activó el botón alternativo en la configuración
        bool habilitado = Mod::get()->getSettingValue<bool>("mostrar-rate");
        if (!habilitado) return true;

        if (auto menu = this->getChildByID("left-side-menu")) {
            
            // Creamos la base circular de Geode usando el fondo azul (o puedes cambiar a Green si prefieres)
            // e insertamos la textura de la estrella dorada encima de forma correcta.
            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_starsIcon_001.png", 
                1.0f, 
                CircleBaseColor::Blue
            );
            
            if (btnSprite) {
                auto myButton = CCMenuItemSpriteExtra::create(
                    btnSprite,
                    this,
                    menu_selector(RateLayer::onAlternativeRateStars)
                );

                myButton->setID("alt-rate-stars-button");
                menu->addChild(myButton);
                menu->updateLayout();
            }
        }
        return true;
    }

    void onAlternativeRateStars(CCObject* sender) {
        if (m_level && GameLevelManager::sharedState()) {
            // Lee el valor elegido en el menú de Geode (1-10)
            int64_t estrellasElegidas = Mod::get()->getSettingValue<int64_t>("cantidad-estrellas");

            // Envía el voto de Rate Stars al servidor del GDPS
            GameLevelManager::sharedState()->rateStars(m_level->m_levelID, static_cast<int>(estrellasElegidas));

            std::string mensaje = "Voto de " + std::to_string(estrellasElegidas) + " estrellas enviado.";
            FLAlertLayer::create("Geode", mensaje.c_str(), "OK")->show();
        }
    }
};
