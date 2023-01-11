#pragma once

namespace cli {
    /**
     * Une action paramétrée à effectuer.
     */
    class CliAction {
    public:
        /**
         * Exécute l'action.
         */
        virtual int run() = 0;

        virtual ~CliAction() = default;
    };
}