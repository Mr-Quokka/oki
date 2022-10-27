#include <iostream>
#include <string>

#include "cli/options.h"
#include "config/Manifest.h"
#include "config/config.h"
#include "io/HttpRequest.h"
#include "repository/RemoteRepository.h"

namespace fs = std::filesystem;

using namespace oki;

int main(int argc, char *argv[]) {
    bool color = acceptColor();
    CliAction* action = parseArguments(argc, argv);
    RemoteRepository repository{"http://localhost:8000"};
    action->run(repository);
    delete action;
    return 0;
}
