<?php

declare(strict_types=1);

namespace Oki\Config;

use PDO;

class SQLiteConfig implements DatabaseConfig
{

	public function createPDO(): PDO
	{
		$pdo = new PDO('sqlite:' . __DIR__ . '/../../../oki_packages.db');
		$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		return $pdo;
	}

	public function isUniqueConstraintViolation(\PDOException $exception): bool
	{
		return $exception->getCode() === '23000';
	}

    public function textSearch(string $column, string $placeholder): string
    {
        return "$column LIKE $placeholder";
    }
}
