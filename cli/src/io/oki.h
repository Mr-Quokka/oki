#pragma once

#include <filesystem>

static std::filesystem::path OKI_MANIFEST_FILE = "oki.toml";
static std::filesystem::path OKI_LOCK_FILE = "oki-lock.toml";
static std::filesystem::path OKI_PACKAGES_DIRECTORY = "oki-packages";
static std::filesystem::path OKI_INTERNAL_MAKEFILE = OKI_PACKAGES_DIRECTORY / "Makefile";
static std::filesystem::path OKI_INTERNAL_REGISTRY_FILE = OKI_PACKAGES_DIRECTORY / ".oki-lock.toml";
