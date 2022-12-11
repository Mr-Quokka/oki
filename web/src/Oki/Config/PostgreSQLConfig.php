<?php

declare(strict_types=1);

namespace Oki\Config;

use PDO;

class PostgreSQLConfig implements DatabaseConfig
{
    public function createPDO(): PDO
    {
        $pdo = new PDO('pgsql:host=localhost;dbname=oki', 'oki', 'oki');
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        return $pdo;
    }

    public function isUniqueConstraintViolation(\PDOException $exception): bool
    {
        return $exception->getCode() === '23505';
    }
}
