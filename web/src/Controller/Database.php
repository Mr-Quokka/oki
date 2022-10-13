<?php declare(strict_types = 1);

require __DIR__ . '/../Model/Package.php';
require __DIR__ . '/../Model/PackageVersion.php';


class Database {
	private PDO $pdo;

	function __construct() {
		$this->pdo = new PDO('sqlite:' . __DIR__ . '/../../oki_packages.db');
		$this->pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	}

	public function listPackages(): array {
		$req = $this->pdo->query('SELECT * FROM package;');
		return $req->fetchAll(PDO::FETCH_CLASS, Package::class);
	}

	public function getPackageInfo(string $packageName): ?Package {
		$reqPacket = $this->pdo->prepare('SELECT * FROM package WHERE short_name = :name;');
		$reqPacket->execute(['name' => $packageName]);
		$reqPacket->setFetchMode(PDO::FETCH_CLASS, Package::class);
		$packet = $reqPacket->fetch();
		if(!$packet)
			return NULL;

		$reqVersion = $this->pdo->prepare('SELECT * FROM version v WHERE v.package_id = :id ORDER BY published_date DESC');
		$reqVersion->execute(['id' => $packet->getId()]);
		$versions = $reqVersion->fetchAll(PDO::FETCH_CLASS, PackageVersion::class);

		foreach($versions as $version)
			$version->setPackage($packet);

		$packet->setVersions($versions);
		return $packet;
	}

	public function getPackageVersion(string $version, string $name): ?string{
		$req = $this->pdo->prepare('SELECT v.identifier, p.short_name FROM version v, package p WHERE p.id_package=v.package_id AND :version=v.identifier AND :name=p.short_name');
		$req->execute(['name' => $name, 'version' => $version]);
		$res = $req->fetch();
		if($res == false)
			return NULL;

        return "packages/" . $res["short_name"] . "_" . $res["identifier"] . ".zip";
	}

	public function insererPacket(string $short, string $long, string $language, string $version){

		$req = $this->pdo->prepare('INSERT INTO package (short_name,long_name,langage_id) values (:short,:long,:language);');
		
		try {
			$res = $req->execute(['short'=>$short,'long'=>$long,'language'=>$language]);
			if(!$res)
				throw new Exception('CPT');
		}catch(Exception $e){
			echo '<p style="color: red">A package with this short name already exists</p>';
			http_response_code(500);
			return ;
		}
			$package_id=$this->pdo->lastInsertId();

        $req = $this->pdo->prepare('INSERT INTO version (package_id,identifier,published_date) values (:package_id,:version,CURRENT_DATE);');

		$res = $req->execute(['package_id'=>$package_id,'version'=>$version]);
	}
}
?>