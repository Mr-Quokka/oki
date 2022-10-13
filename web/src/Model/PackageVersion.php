<?php declare(strict_types = 1);

class PackageVersion implements JsonSerializable{
	private int $id_version;

	private int $id_package;

	private string $identifier;

	private string $published_date;

	private string $url;

	public function getIdVersion(): int{
		return $this->id_version;
	}

	public function getIdPackage(): int{
		return $this->id_package;
	}

	public function getIdentifier(): string{
		return $this->identifier;
	}

	public function getPublishedDate(): string{
		return $this->published_date;
	}

	public function getPublishedDate(): string{
		return $this->url;
	}

	public function jsonSerialize(){
		$dict=[];
		$dict["id_version"]=$this->id_version;
		$dict["identifier"]=$this->identifier;
		$dict["published_date"]=$this->published_date;
		return $dict;
	}
}