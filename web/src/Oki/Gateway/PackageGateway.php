<?php

declare(strict_types=1);

namespace Oki\Gateway;

use Oki\Config\DatabaseConfig;
use Oki\Model\Package;
use Oki\Model\PackageVersion;
use Oki\Model\Dependency;
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

			$reqDependency = $this->pdo->prepare('SELECT * FROM dependency WHERE constrainer_id = :id;');
			$reqDependency->execute(['id' => $version->getIdVersion()]);
			$dependencies = $reqDependency->fetchAll(PDO::FETCH_CLASS, Dependency::class);

			$version->setDependencies($dependencies);
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

	public function insertPackage(string $short, string $long, string $language, string $version)
	{
		$req = $this->pdo->prepare('INSERT INTO package (short_name, description, language_id) values (:short, :long, :language);');

		try {
			$req->execute(['short' => $short, 'long' => $long, 'language' => $language]);
		} catch (PDOException $ex) {
			if ($this->config->isUniqueConstraintViolation($ex)) {
				return 'A package with this short name already exists';
			}
			throw $ex;
		}
		$package_id = $this->pdo->lastInsertId();

		$req = $this->pdo->prepare('INSERT INTO version (package_id, identifier, published_date) values (:package_id, :version, CURRENT_TIMESTAMP);');
		$req->execute(['package_id' => $package_id, 'version' => $version]);
		return true;
	}
}
