<?php declare(strict_types = 1);

class PackageVersion implements JsonSerializable{
	private int $id_version;

	private Package $package;

	private string $identifier;

	private string $published_date;


	public function getIdVersion(): int{
		return $this->id_version;
	}

	public function getPackage(): int{
		return $this->package;
	}

	public function setPackage(Package $pck){
		$this->package = $pck;
	}


	public function getIdentifier(): string{
		return $this->identifier;
	}

	public function getPublishedDate(): string{
		return $this->published_date;
	}

	public function getDownloadUrl(): string{
		return ("/packages/" . $this->package->getShortName() . "_" . $this->identifier . ".zip");
	}


	public function jsonSerialize(){
		$dict=[];
		$dict["id_version"]=$this->id_version;
		$dict["identifier"]=$this->identifier;
		$dict["published_date"]=$this->published_date;
		$dict["download_url"]=$this->getDownloadUrl();
		return $dict;
	}
}