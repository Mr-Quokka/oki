#pragma once

#include "../make/CompilatorStrategy.h"
#include "ProjectKind.h"
#include <memory>

namespace make {
    /**
     * Fabrique de générations d'informations de construction.
     */
    class SourceFactory {
    public:
        /**
         * Crée une stratégie de compilation à partir du type de projet.
         *
         * @param kind Le type de projet.
         * @return Une stratégie de compilation, ou nullptr si le type de projet n'est pas géré.
         */
        static std::unique_ptr<make::CompilatorStrategy> fromKind(ProjectKind kind);
    };
}
