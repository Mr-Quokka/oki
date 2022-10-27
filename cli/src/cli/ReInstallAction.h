#pragma once

#include "CliAction.h"

namespace oki {
    /**
     * Reinstalle tous les paquets present dans le manifeste.
     */
	class ReInstallAction : public CliAction {
	public:
		ReInstallAction();
		void run(Repository& repository) override;
	};
}