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
		$package = $reqPacket->fetch();
		if ($package === false) {
			return null;
		}

		$reqVersion = $this->pdo->prepare('SELECT * FROM version WHERE package_id = :id ORDER BY published_date DESC;');
		$reqVersion->execute(['id' => $package->getId()]);
		$reqVersion->setFetchMode(PDO::FETCH_CLASS, PackageVersion::class);
		$versions = [];
		while ($version = $reqVersion->fetch()) {
            $version->setPackage($package);
			$versions[$version->getIdVersion()] = $version;
		}
		$package->setVersions(array_values($versions));

		$reqDependency = $this->pdo->prepare('SELECT d.constrainer_id, p.short_name, d.constraint_value FROM version v INNER JOIN dependency d ON d.constrainer_id = v.id_version INNER JOIN package p ON p.id_package = d.package_reference_id WHERE v.package_id = :package;');
        $reqDependency->setFetchMode(PDO::FETCH_ASSOC);
        $reqDependency->execute(['package' => $package->getId()]);
		while ($dep = $reqDependency->fetch()) {
			$versions[intval($dep['constrainer_id'])]->addDependency($dep['short_name'], $dep['constraint_value']);
		}
		return $package;
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
