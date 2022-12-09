<?php

namespace Oki\Model;

class PackageManifest
{
    private int $packageId;

    private string $name;

    private string $description;

    private string $version;

    /**
     * @var array<string, string>
     */
    private array $dependencies;

    public function __construct(string $name, string $description, string $version, array $dependencies)
    {
        $this->name = $name;
        $this->description = $description;
        $this->version = $version;
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
