#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(PlayerObject) {
    void pushButton(PlayerButton button) {
        // Ejecuta primero la lógica normal del juego (saltar, activar orbes, etc.)
        PlayerObject::pushButton(button);

        // Cambia la gravedad solo con el botón principal (clic izquierdo/espacio)
        if (button == PlayerButton::Jump) {
            // Invierte la gravedad actual del jugador
            this->flipGravity();
        }
    }
};
