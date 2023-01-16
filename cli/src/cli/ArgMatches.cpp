#include "ArgMatches.h"
#include "Command.h"
#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;

namespace cli {
    ArgMatches::ArgMatches(std::span<const char *> args, const Command &command) {
        po::store(
            po::command_line_parser(static_cast<int>(args.size()), args.data())
                .options(command.getBackingOptions())
                .positional(command.getBackingPositionalOptions())
                .run(),
            vm);
        po::notify(vm);
    }

    ArgMatches::ArgMatches(po::variables_map &&vm)
        : vm(std::move(vm)) {}

    bool ArgMatches::contains(const std::string &name) const {
        return vm.count(name) > 0;
    }

    repository::Repository &ArgMatches::getRegistry(config::UserConfig &config) {
        if (!vm.contains("registry")) {
            return config.getGlobalRepository();
        }
        return config.getRepository(vm["registry"].as<std::string>());
    }
}
