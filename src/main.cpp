#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <string>
#include <sstream>
#include <iomanip>

using namespace geode::prelude;

std::string formatDownloads(int count) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << (count / 1000.0) << "k";
    return ss.str();
}

class $modify(MyLevelCell, LevelCell) {
    void loadCustomLevelCell() {
        LevelCell::loadCustomLevelCell();
        
        if (!m_level) return;

        // Recorremos todos los elementos (hijos) de la celda de forma segura
        auto children = this->getChildren();
        if (!children) return;

        int downloads = m_level->m_downloads;
        std::string formattedText = formatDownloads(downloads);

        for (int i = 0; i < children->count(); ++i) {
            auto child = typeinfo_cast<CCLabelBMFont*>(children->objectAtIndex(i));
            
            if (child) {
                // Obtenemos el texto actual que tiene la etiqueta
                std::string currentStr = child->getString();
                
                // Convertimos el número de descargas original a texto para buscar la coincidencia exacta
                std::string originalDownloadsStr = std::to_string(downloads);
                
                // Si la etiqueta tiene exactamente el número original de descargas, la modificamos
                if (currentStr == originalDownloadsStr) {
                    child->setString(formattedText.c_str());
                    break; // Ya lo encontramos y modificamos, salimos del ciclo
                }
            }
        }
    }
};
