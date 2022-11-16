#include <string>

#include "cli/options.h"
#include "repository/RemoteRepository.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    cli::CliAction *action = cli::parseArguments(argc, argv);
    repository::RemoteRepository repository{"http://localhost:8000"};
    action->run(repository);
    delete action;
    return 0;
}
