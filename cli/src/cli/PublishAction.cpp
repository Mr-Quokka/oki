#include "PublishAction.h"

#include "../io/Archive.h"
#include "../io/TmpFile.h"
#include "../io/oki.h"

namespace fs = std::filesystem;

namespace cli {
    void PublishAction::run(repository::Repository &repository) {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        io::TmpFile tmp;
        io::Compressor compressor{tmp.getFilename()};
        compressor.compress(fs::current_path() / "src");
        repository.publish(manifest, tmp.getFilename());
    }
}
