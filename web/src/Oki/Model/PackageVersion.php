<?php

declare(strict_types=1);

namespace Oki\Model;

use JsonSerializable;

class PackageVersion implements JsonSerializable
{
    private int $id_version;

    private Package $package;

    private int $major;

    private int $minor;

    private int $patch;

    private string $published_date;

    private int $file_size;

    /**
     * @var array<string, string>
     */
    private array $dependencies = [];

    public function getIdVersion(): int
    {
        return $this->id_version;
    }

    public function getPackage(): Package
    {
        return $this->package;
    }

    public function setPackage(Package $pck)
    {
        $this->package = $pck;
    }

    public function getIdentifier(): string
    {
        return "$this->major.$this->minor.$this->patch";
    }

    public function getPublishedDate(): string
    {
        return $this->published_date;
    }

    public function getFileSize(): int
    {
        return $this->file_size;
    }

    public function getDownloadUrl(): string
    {
        return BASE_URL . 'packages/' . $this->package->getName() . '_' . $this->getIdentifier() . '.zip';
    }

    public function getDependencies(): array
    {
        return $this->dependencies;
    }

    public function addDependency(string $dependency, string $constraint)
    {
        $this->dependencies[$dependency] = $constraint;
    }

    public function jsonSerialize(): array
    {
        $dict = [];
        $dict["id_version"] = $this->id_version;
        $dict["identifier"] = $this->getIdentifier();
        $dict["published_date"] = $this->published_date;
        $dict["download_url"] = $this->getDownloadUrl();
        $dict['dependencies'] = $this->dependencies;
        return $dict;
    }
}
