<?php

namespace Oki\Model;

class PackageManifest
{
    private int $packageId;

    private string $name;

    private string $description;

    private string $version;

    private string $kind;

    private string $checksum;

    /**
     * @var array<string, string>
     */
    private array $dependencies;

    public function __construct(string $name, string $description, string $version, string $kind, string $checksum, array $dependencies)
    {
        $this->name = $name;
        $this->description = $description;
        $this->version = $version;
        $this->kind = $kind;
        $this->checksum = $checksum;
        $this->dependencies = $dependencies;
    }

    public function getPackageId(): int
    {
        return $this->packageId;
    }

    public function getName(): string
    {
        return $this->name;
    }

    public function getDescription(): string
    {
        return $this->description;
    }

    public function getVersion(): string
    {
        return $this->version;
    }

    public function getKind(): string
    {
        return $this->kind;
    }

    public function getChecksum(): string
    {
        return $this->checksum;
    }

    /**
     * @return array<string, string>
     */
    public function getDependencies(): array
    {
        return $this->dependencies;
    }

    public function setPackageId(int $packageId): void
    {
        $this->packageId = $packageId;
    }
}
