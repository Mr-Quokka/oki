<?php

declare(strict_types=1);

namespace Oki\Config;

use PDO;

interface DatabaseConfig
{
	public function createPDO(): PDO;

	public function isUniqueConstraintViolation(\PDOException $exception): bool;

    public function textSearch(string $column, string $placeholder): string;
}
