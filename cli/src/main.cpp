#include <iostream>

#include "cli/options.h"
#include "io/HttpRequest.h"
#include "repository/RemoteRepository.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    std::unique_ptr<cli::CliAction> action = cli::parseArguments(argc, argv);
    repository::RemoteRepository repository{"http://localhost:8000"};
    try {
        action->run(repository);
    } catch (const io::APIException &e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
}
