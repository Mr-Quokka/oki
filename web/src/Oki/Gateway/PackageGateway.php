<?php

declare(strict_types=1);

namespace Oki\Gateway;

use Oki\Config\DatabaseConfig;
use Oki\Model\Ownership;
use Oki\Model\Package;
use Oki\Model\PackageManifest;
use Oki\Model\PackageResume;
use Oki\Model\PackageVersion;
use Oki\Model\TransactionResult;
use Oki\Model\User;
use PDO;
use PDOException;

class PackageGateway
{

	private DatabaseConfig $config;
	private PDO $pdo;
	private OwnershipGateway $ownershipGateway;

	public function __construct(PDO $pdo, DatabaseConfig $config, OwnershipGateway $ownershipGateway)
	{
		$this->config = $config;
		$this->pdo = $pdo;
		$this->ownershipGateway = $ownershipGateway;
	}

	public function listPackages(): array
	{
        $req = $this->pdo->query('SELECT p.*, v1.identifier latest_version FROM package p
            LEFT JOIN version v1 ON p.id_package = v1.package_id
            LEFT JOIN version v2 ON v1.package_id = v2.package_id AND v2.published_date > v1.published_date
            WHERE v2.published_date IS NULL;');
		return $req->fetchAll(PDO::FETCH_CLASS, PackageResume::class);
	}

	public function listPackagesPagination(int $limit, int $page): array
	{
        $req = $this->pdo->prepare('SELECT p.*, v1.identifier latest_version FROM package p
            LEFT JOIN version v1 ON p.id_package = v1.package_id
            LEFT JOIN version v2 ON v1.package_id = v2.package_id AND v2.published_date > v1.published_date
            WHERE v2.published_date IS NULL LIMIT :limit OFFSET :offset;');
		$req->bindValue('limit', $limit, PDO::PARAM_INT);
        $req->bindValue('offset', ($page - 1) * $limit, PDO::PARAM_INT);
		if (!$req->execute()) {
            return [];
        }
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

	private function getLanguageId(string $kind): ?int
	{
		$stmt = $this->pdo->prepare('SELECT id_language FROM language WHERE designation = :kind');
		$stmt->execute(['kind' => $kind]);
		$row = $stmt->fetch();
		return $row ? intval($row['id_language']) : null;
	}

	public function getOrCreatePackage(User $user, PackageManifest $manifest): TransactionResult
	{
		$id = $this->getPackageId($manifest->getName());
		if ($id !== null) {
			return new TransactionResult($id, 200, 'Valid existing package name');
		}
		$languageId = $this->getLanguageId($manifest->getKind());
		if ($languageId === null) {
			return new TransactionResult(-1, 400, 'Unknown package kind');
		}
		try {
			$this->pdo->beginTransaction();
			$req = $this->pdo->prepare('INSERT INTO package (name, description, language_id) VALUES (:name, :description, :language_id);');
			$req->execute([
				'name' => $manifest->getName(),
				'description' => $manifest->getDescription(),
				'language_id' => $languageId
			]);
			$ownership = Ownership::from($user->getId(), intval($this->pdo->lastInsertId()));
			$this->ownershipGateway->insertOwnership($ownership);
			$this->pdo->commit();
			return new TransactionResult($ownership->getPackageId(), 201, 'Created package');
		} catch (PDOException $e) {
			$this->pdo->rollBack();
			throw $e;
		}
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

	public function insertVersion(PackageManifest $manifest, int $fileSize): TransactionResult
	{
		try {
			$this->pdo->beginTransaction();
			$res = $this->insertVersionUnsafe($manifest, $fileSize);
			if ($res->isSuccess()) {
				$res->setTerminate([$this->pdo, 'commit']);
			} else {
				$this->pdo->rollBack();
			}
			return $res;
		} catch (PDOException $e) {
			$this->pdo->rollBack();
			throw $e;
		}
	}

	public function insertVersionUnsafe(PackageManifest $manifest, int $fileSize): TransactionResult
	{
		$req = $this->pdo->prepare('INSERT INTO version (package_id, identifier, file_size, publisher_id) values (:package_id, :version, :file_size, :publisher_id);');
		try {
			if (!$req->execute([
                'package_id' => $manifest->getPackageId(),
                'version' => $manifest->getVersion(),
                'file_size' => $fileSize,
                'publisher_id' => $manifest->getPublisherId()
            ])) {
				return new TransactionResult($manifest->getPackageId(), 500, 'Cannot add new version');
			}
			$req = $this->pdo->prepare('UPDATE package SET description = :description WHERE id_package = :package_id;');
			$req->execute(['description' => $manifest->getDescription(), 'package_id' => $manifest->getPackageId()]);
		} catch (PDOException $e) {
			if ($this->config->isUniqueConstraintViolation($e)) {
				return new TransactionResult($manifest->getPackageId(), 409, 'This version already exists');
			}
			throw $e;
		}
		return $this->insertDependencies($manifest);
	}

	private function insertDependencies(PackageManifest $manifest): TransactionResult
	{
		$constrainer_id = $this->pdo->lastInsertId();
		$req = $this->pdo->prepare('INSERT INTO dependency values (:package_reference_id, :constrainer_id, :constraint_value);');

		foreach ($manifest->getDependencies() as $package => $range) {
			if (($package_reference_id = $this->getPackageId($package)) !== null) {
				$req->execute(['package_reference_id' => $package_reference_id, 'constrainer_id' => $constrainer_id, 'constraint_value' => $range]);
			} else {
				return new TransactionResult($manifest->getPackageId(), 400, "The `$package` dependency is not present in the registry");
			}
		}
		return new TransactionResult($manifest->getPackageId(), 201, 'The version has been successfully published');
	}

	public function getCount(): int
	{
		$req = $this->pdo->prepare('SELECT COUNT(*) c FROM package;');
		$req->execute();
		return intval($req->fetch() ['c']);
	}
}
