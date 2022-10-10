<?php declare(strict_types = 1);

class PackageVersion implements JsonSerializable{
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

	public function jsonSerialize(){
		$dict=[];
		$dict["id_version"]=$this->id_version;
		$dict["identifier"]=$this->identifier;
		$dict["published_date"]=$this->published_date;
		return $dict;
	}
}