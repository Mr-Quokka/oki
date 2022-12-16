<?php

declare(strict_types=1);

namespace Oki\Model;

class Package extends AbstractPackage
{
	/**
	 * @var PackageVersion[]
	 */
	private array $versions = [];

	public function setVersions(array $versions)
	{
		$this->versions = $versions;
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
