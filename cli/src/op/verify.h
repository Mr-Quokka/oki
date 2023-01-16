#pragma once

#include "../config/Manifest.h"
#include <span>

namespace op {
    /**
     * Vérifie la normalité d'un manifeste.
     *
     * @param manifest Le manifeste à tester.
     * @param include Les fichiers à inclure.
     * @return true si valide, false sinon.
     */
    bool verify(const config::Manifest &manifest, std::span<const std::filesystem::path> include);
}
