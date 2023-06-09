#include <boost/program_options/errors.hpp>
#include <iostream>

#include "cli/ExitStatuses.h"
#include "cli/options.h"
#include "config/Manifest.h"
#include "config/UserConfig.h"
#include "config/config.h"
#include "repository/RepositoryException.h"
#include "semver/ParseException.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        std::cout << "See " << argv[0] << " help for all available actions.\n";
        return OK;
    }

    std::span<const char *> args(argv, static_cast<std::span<const char *>::size_type>(argc));
    config::UserConfig config = config::UserConfig::fromFile(config::getConfigPath());
    std::unique_ptr<cli::CliAction> action;
    try {
        action = cli::parseArguments(config, args);
    } catch (const boost::program_options::error &e) {
        std::cerr << e.what() << "\n";
        return ERR_USAGE;
    }
    if (!action) {
        return EX_OK;
    }

    int exit = ERR_BASE;
    try {
        exit = action->run();
    } catch (const semver::ParseException &e) {
        std::cerr << e << "\n";
        return ERR_DATAERR;
    } catch (const repository::RepositoryException &e) {
        std::cerr << e.what() << "\n";
    } catch (const config::ManifestException &e) {
        std::cerr << e.what() << "\n";
        return ERR_CONFIG;
    }
    return exit;
}
