#pragma once

#include "CliAction.h"
#include "Command.h"

namespace cli {
    /**
     * Présente un arbre des dépendances à partir du fichier manifeste et verrou.
     *
     * L'arbre couché est écrit sur la sortie standard. Il présente les relations entre les dépendances à
     * l'aide de traits (des caractères UTF-8) et d'une indentation.
     */
    class TreeAction : public CliAction {
    private:
        config::UserConfig &config;

    public:
        TreeAction(config::UserConfig &config);

        int run() override;

        static Command cmd();
    };
}
