<?php declare(strict_types=1);

namespace Config;

interface DatabaseConfig {

	public function getDsn(): string;

	public function isUniqueConstraintViolation(\PDOException $exception): bool;
}
