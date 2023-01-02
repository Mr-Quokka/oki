#include "PublishAction.h"

#include "../io/TmpFile.h"
#include "../io/oki.h"
#include "../op/package.h"

namespace fs = std::filesystem;

namespace cli {
    void PublishAction::run(repository::Repository &repository) {
        io::TmpFile tmp;
        op::package(tmp.path(), fs::current_path());
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        repository.publish(manifest, tmp.path());
    }
}
