<?php declare(strict_types = 1);

require __DIR__ . '/../Model/Package.php';

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
		$req = $this->pdo->prepare('SELECT * FROM package p INNER JOIN version v ON v.package_id = p.id WHERE short_name = :name;');
		$req->execute(['name' => $packageName]);
		$req->setFetchMode(PDO::FETCH_CLASS, Package::class);
		return $req->fetch();
	}
}
