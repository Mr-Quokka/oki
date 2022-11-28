#include <iostream>
#include <string>

#include "cli/options.h"
#include "config/Manifest.h"
#include "io/HttpRequest.h"
#include "repository/RemoteRepository.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    cli::CliAction *action = cli::parseArguments(argc, argv);
    repository::RemoteRepository repository{"http://localhost:8000"};
    try {
        action->run(repository);
    } catch (const io::APIException &e) {
        std::cerr << e.what() << "\n";
    } catch (const config::ManifestException &e) {
        std::cerr << e.what() << "\n";
    }
    delete action;
    return 0;
}
