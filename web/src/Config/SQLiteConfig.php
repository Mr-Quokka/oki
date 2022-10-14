<?php

namespace Config;

class SQLiteConfig implements DatabaseConfig {

	public function getDsn(): string {
		return 'sqlite:' . __DIR__ . '/../../oki_packages.db';
	}

	public function isUniqueConstraintViolation(\PDOException $exception): bool {
		return $exception->getCode() === '23000';
	}
}
