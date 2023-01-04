#include <iostream>

#include "cli/options.h"
#include "config/Manifest.h"
#include "config/config.h"
#include "io/HttpRequest.h"
#include "repository/RemoteRepository.h"
#include "semver/ParseException.h"

int main(int argc, char *argv[]) {
    std::unique_ptr<cli::CliAction> action = cli::parseArguments(argc, argv);
    repository::RemoteRepository repository{config::getDefaultRemoteRepository()};
    try {
        action->run(repository);
    } catch (const semver::ParseException &e) {
        std::cerr << e << "\n";
    } catch (const io::APIException &e) {
        std::cerr << e.what() << "\n";
    } catch (const config::ManifestException &e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
}
