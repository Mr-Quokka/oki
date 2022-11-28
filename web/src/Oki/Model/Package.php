<?php

declare(strict_types=1);

namespace Oki\Model;

use JsonSerializable;

class Package implements JsonSerializable
{
	private int $id_package;

	private string $short_name;

	private string $description;

	/**
	 * @var PackageVersion[]
	 */
	private array $versions = [];

	public function setVersions(array $versions)
	{
		$this->versions = $versions;
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

	public function getVersions(): array
	{
		return $this->versions;
	}

	public function jsonSerialize()
	{
		$dict = [
			'id_package' => $this->id_package,
			'short_name' => $this->short_name,
			'description' => $this->description
		];
		if (!empty($this->versions)) {
			$dict['versions'] = $this->versions;
		}
		return $dict;
	}
}
