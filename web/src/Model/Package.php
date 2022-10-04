<?php declare(strict_types = 1);

class Package implements JsonSerializable{

	private int $id_package;

	private string $short_name;

	private string $long_name;
	
	/**
	 * @var PackageVersion[]
	 */
	private array $versions = [];

	public function setVersions($versions){
		$this->versions = $versions;
	}

	public function getId(){
		return $this->id_package;
	}

	public function jsonSerialize(){
		$dict=[];
		$dict["id_package"]=$this->id_package;
		$dict["short_name"]=$this->short_name;
		$dict["long_name"]=$this->long_name;
		if(!empty($this->versions)){
			$dict["versions"]=$this->versions;
		}
		return $dict;
	}
}
