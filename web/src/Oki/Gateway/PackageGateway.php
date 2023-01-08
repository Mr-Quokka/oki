<?php

declare(strict_types=1);

namespace Oki\Gateway;

use Oki\Config\DatabaseConfig;
use Oki\Model\Package;
use Oki\Model\PackageManifest;
use Oki\Model\PackageResume;
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
        $req = $this->pdo->query('SELECT p.*, v1.identifier latest_version FROM package p
            LEFT JOIN version v1 ON p.id_package = v1.package_id
            LEFT JOIN version v2 ON v1.package_id = v2.package_id AND v2.published_date > v1.published_date
            WHERE v2.published_date IS NULL;');
		return $req->fetchAll(PDO::FETCH_CLASS, PackageResume::class);
	}

    public function getPackage(string $packageName): ?Package
    {
        $reqPacket = $this->pdo->prepare('SELECT * FROM package WHERE name = :name;');
        $reqPacket->execute(['name' => $packageName]);
        $reqPacket->setFetchMode(PDO::FETCH_CLASS, Package::class);
        $package = $reqPacket->fetch();
        if ($package === false) {
            return null;
        }
        return $package;
    }

	public function getPackageInfo(string $packageName): ?Package
	{
		$package = $this->getPackage($packageName);
        if ($package === null) {
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

		$reqDependency = $this->pdo->prepare('SELECT d.constrainer_id, p.name, d.constraint_value FROM version v INNER JOIN dependency d ON d.constrainer_id = v.id_version INNER JOIN package p ON p.id_package = d.package_reference_id WHERE v.package_id = :package;');
        $reqDependency->setFetchMode(PDO::FETCH_ASSOC);
        $reqDependency->execute(['package' => $package->getId()]);
		while ($dep = $reqDependency->fetch()) {
			$versions[intval($dep['constrainer_id'])]->addDependency($dep['name'], $dep['constraint_value']);
		}
		return $package;
	}

	public function getPackageId(string $packageName): ?int
	{
		$req = $this->pdo->prepare('SELECT id_package FROM package WHERE name = :name;');
		$req->execute(['name' => $packageName]);
		$res = $req->fetch();
		return $res === false ? null : intval($res['id_package']);
	}

	public function getPackageVersion(Package $package, ?string $version = null): ?PackageVersion
	{
        if ($version === null) {
            $req = $this->pdo->prepare('SELECT * FROM version WHERE package_id = :package_id ORDER BY published_date DESC LIMIT 1');
            $req->execute(['package_id' => $package->getId()]);
        } else {
            $req = $this->pdo->prepare('SELECT * FROM version WHERE package_id = :package_id AND identifier = :identifier');
            $req->execute(['package_id' => $package->getId(), 'identifier' => $version]);
        }
        $req->setFetchMode(PDO::FETCH_CLASS, PackageVersion::class);
        $version = $req->fetch();
        if (!$version) {
            return null;
        }
        $version->setPackage($package);
		return $version;
	}

    public function getPackageVersions(Package $package): Package
    {
        $req = $this->pdo->prepare('SELECT * FROM version WHERE package_id = :id ORDER BY published_date DESC;');
        $req->execute(['id' => $package->getId()]);
        $versions = $req->fetchAll(PDO::FETCH_CLASS, PackageVersion::class);
        foreach ($versions as $version) {
            $version->setPackage($package);
        }
        $package->setVersions($versions);
        return $package;
    }

    public function getPackageDependencies(PackageVersion $version): PackageVersion
    {
        $req = $this->pdo->prepare('SELECT p.name, d.constraint_value FROM dependency d INNER JOIN package p ON p.id_package = d.package_reference_id WHERE d.constrainer_id = :id_version;');
        $req->execute(['id_version' => $version->getIdVersion()]);
        while ($dep = $req->fetch()) {
            $version->addDependency($dep['name'], $dep['constraint_value']);
        }
        return $version;
    }

	public function insertVersion(PackageManifest $manifest, int $fileSize): int
	{
		$req = $this->pdo->prepare('INSERT INTO version (package_id, identifier, file_size) values (:package_id, :version, :file_size);');
		try {
			if ($req->execute(['package_id' => $manifest->getPackageId(), 'version' => $manifest->getVersion(), 'file_size' => $fileSize])) {
				return 200;
			}
			return 500;
		} catch (PDOException $e) {
			if ($this->config->isUniqueConstraintViolation($e)) {
				return 409;
			}
			throw $e;
		}
	}
}
