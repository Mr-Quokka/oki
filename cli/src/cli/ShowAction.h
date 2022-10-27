#pragma once

#include "CliAction.h"

namespace oki {
 	/**
     * Affiche les informations d'un paquet d'après son nom.
     */
	class ShowAction : public CliAction {
	private:
		std::string_view packageName;
	public:
		void run(Repository& repository) override;
        explicit ShowAction(const char *packageName);
	};
}