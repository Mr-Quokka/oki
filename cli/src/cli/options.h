#pragma once

#include <string_view>
#include <variant>

namespace oki{
    struct ListAction {
    };
    struct InstallAction {
        std::string_view packageName;
        explicit InstallAction(const char *packageName) : packageName{packageName} {}
    };
    struct ShowAction {
        std::string_view packageName;
        explicit ShowAction(const char *packageName) : packageName{packageName} {}
    };
    using CliAction = std::variant<ListAction, InstallAction, ShowAction>;

    CliAction parseArguments(int argc, char *argv[]);
}


