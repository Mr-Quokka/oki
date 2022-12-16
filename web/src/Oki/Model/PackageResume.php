<?php

declare(strict_types=1);

namespace Oki\Model;

use JsonSerializable;

class PackageResume implements JsonSerializable
{
	private int $id_package;

	private string $short_name;

	private string $description;

	private ?string $latest_version;

	public function setLatestVersion(string $latest_version)
	{
		$this->latest_version = $latest_version;
	}

	public function getId()
	{
		return $this->id_package;
	}

	public function getShortName(): string
	{
		return $this->short_name;
	}

	public function getDescription(): string
	{
		return $this->description;
	}

	public function getLatestVersion(): ?string
	{
		return $this->latest_version;
	}

	public function jsonSerialize()
	{
		$dict = [
			'id_package' => $this->id_package,
			'short_name' => $this->short_name,
			'description' => $this->description
		];
		if (!empty($this->latest_version)) {
			$dict['latest_version'] = $this->latest_version;
		}
		return $dict;
	}
}
