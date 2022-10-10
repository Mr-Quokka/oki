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
    using CliAction = std::variant<ListAction, InstallAction>;

    CliAction parseArguments(int argc, char *argv[]);
}


