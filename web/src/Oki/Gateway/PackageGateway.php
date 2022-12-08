<?php

declare(strict_types=1);

namespace Oki\Gateway;

use Oki\Config\DatabaseConfig;
use Oki\Model\Package;
use Oki\Model\PackageVersion;
use PDO;
use PDOException;

class PackageGateway
{

	private DatabaseConfig $config;
	private PDO $pdo;

	public function __construct(PDO $pdo, DatabaseConfig $config)
	{
		$this->config = $config;
		$this->pdo = $pdo;
	}

	public function listPackages(): array
	{
		$req = $this->pdo->query('SELECT * FROM package;');
		return $req->fetchAll(PDO::FETCH_CLASS, Package::class);
	}

	public function getPackageInfo(string $packageName): ?Package
	{
		$reqPacket = $this->pdo->prepare('SELECT * FROM package WHERE short_name = :name;');
		$reqPacket->execute(['name' => $packageName]);
		$reqPacket->setFetchMode(PDO::FETCH_CLASS, Package::class);
		$packet = $reqPacket->fetch();
		if ($packet === false) {
			return null;
		}

		$reqVersion = $this->pdo->prepare('SELECT * FROM version WHERE package_id = :id ORDER BY published_date DESC;');
		$reqVersion->execute(['id' => $packet->getId()]);
		$versions = $reqVersion->fetchAll(PDO::FETCH_CLASS, PackageVersion::class);

		foreach ($versions as $version) {
			$version->setPackage($packet);
		}

		$packet->setVersions($versions);
		return $packet;
	}

	public function getPackageVersion(string $name, string $version): ?string
	{
		$req = $this->pdo->prepare('SELECT v.identifier, p.short_name FROM version v, package p WHERE p.id_package=v.package_id AND :version=v.identifier AND :name=p.short_name');
		$req->execute(['name' => $name, 'version' => $version]);
		$res = $req->fetch();
		if (!$res) {
			return null;
		}

		return "/packages/" . $res["short_name"] . "_" . $res["identifier"] . ".zip";
	}

	public function insertVersion(int $package_id, string $description, string $version): int
	{
		$req = $this->pdo->prepare('INSERT INTO version (package_id, identifier) values (:package_id, :version);');
		try{
			if($req->execute(['package_id' => $package_id, 'version' => $version])){
				return 200;
			}
			return 500;
		} catch(PDOException $e){
			if($this->config->isUniqueConstraintViolation($e)){
				return 409;
			}
			throw $e;
		}
	}
}
