#include "DependencyBuild.h"
#include "../util/ostreamJoin.h"

namespace make {
    void DependencyBuild::addBuildStep(std::string_view staticLink, std::string_view buildStatic) {
        staticLinks.emplace_back(staticLink);
        buildsStatic.emplace_back(buildStatic);
    }

    void DependencyBuild::asMakefile(std::ostream &out) {
        out << "all: oki-built-dependencies\n\n";
        for (size_t i = 0; i < staticLinks.size(); i++) {
            out << staticLinks[i] << ":\n";
            out << "\t" << buildsStatic[i] << "\n";
        }
        out << "oki-built-dependencies:";
        util::ostreamJoin(out, staticLinks.cbegin(), staticLinks.cend(), " ", " ");
        out << "\n";
        out << ".PHONY: all oki-built-dependencies\n";
    }
}
