#include "Command.h"

#include "ArgMatches.h"
#include <iostream>
#include <utility>

namespace cli {
    Command::Command(std::string_view name, std::string_view desc, Command::Action action)
        : options{name.data()}, desc{desc}, action{std::move(action)} {
        flag("help", "Display this help message");
    }

    Command &Command::flag(std::string_view name, std::string_view description) {
        options.add_options()(name.data(), description.data());
        return *this;
    }

    Command &Command::arg(std::string_view name, std::string_view description) {
        options.add_options()(name.data(), description.data());
        return *this;
    }

    Command &Command::positional(std::string_view name, int maxCount) {
        positionals.add(name.data(), maxCount);
        return *this;
    }

    std::unique_ptr<CliAction> Command::create(config::UserConfig &config, std::span<const char *> args) const {
        ArgMatches matches{args, *this};
        if (matches.contains("help")) {
            std::cout << options << "\n";
            return nullptr;
        }
        return action(config, std::move(matches));
    }

    std::string_view Command::getDescription() const {
        return desc;
    }

    const boost::program_options::options_description &Command::getBackingOptions() const {
        return options;
    }

    const boost::program_options::positional_options_description &Command::getBackingPositionalOptions() const {
        return positionals;
    }
}