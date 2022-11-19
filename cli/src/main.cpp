#include <iostream>
#include <string>

#include "cli/options.h"
#include "io/HttpRequest.h"
#include "repository/RemoteRepository.h"

namespace fs = std::filesystem;

using namespace oki;

int main(int argc, char *argv[]) {
    CliAction *action = parseArguments(argc, argv);
    RemoteRepository repository{"http://localhost:8000"};
    try {
        action->run(repository);
    } catch (const oki::APIException &e) {
        std::cerr << e.what() << "\n";
    }
    delete action;
    return 0;
}
