<?php

declare(strict_types=1);

namespace Oki\Model;

class PackageResume extends AbstractPackage
{
    private ?string $latest_version;

    public function setLatestVersion(string $latest_version)
    {
        $this->latest_version = $latest_version;
    }

    public function getLatestVersion(): ?string
    {
        return $this->latest_version;
    }

    public function jsonSerialize(): array
    {
        $dict = [
            'id_package' => $this->id_package,
            'name' => $this->name,
            'description' => $this->description
        ];
        if (!empty($this->latest_version)) {
            $dict['latest_version'] = $this->latest_version;
        }
        return $dict;
    }
}
