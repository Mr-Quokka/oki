<?php declare(strict_types = 1);

class PackageVersion {
	private int $id_version;

	private string $identifier;

	private string $published_date;

	public function getIdVersion(): int{
		return $this->id_version;
	}

	public function getIdentifier(): string{
		return $this->identifier;
	}

	public function getPublishedDate(): string{
		return $this->published_date;
	}
}