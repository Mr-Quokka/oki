<?php

declare(strict_types=1);

namespace Oki\DI;

use Oki\Config\DatabaseConfig;
use Oki\Gateway\PackageGateway;
use Oki\Gateway\UserGateway;
use Oki\Security\Security;
use PDO;

final class DI
{
    private DatabaseConfig $dbConfig;

    private ?PDO $pdo = null;

    private ?PackageGateway $packageGateway = null;

    private ?UserGateway $userGateway = null;

    private ?Security $security = null;

    public function __construct()
    {
        $this->dbConfig = getDefaultDatabaseConfig();
    }

    public function getPackageGateway(): PackageGateway
    {
        if ($this->packageGateway === null) {
            $this->packageGateway = new PackageGateway($this->getPDO(), $this->dbConfig);
        }
        return $this->packageGateway;
    }

    public function getUserGateway(): UserGateway
    {
        if ($this->userGateway === null) {
            $this->userGateway = new UserGateway($this->getPDO());
        }
        return $this->userGateway;
    }

    public function getSecurity(): Security
    {
        if ($this->security === null) {
            $this->security = new Security($this->getUserGateway(), $_SESSION);
        }
        return $this->security;
    }

    private function getPDO(): PDO
    {
        if ($this->pdo === null) {
            $this->pdo = $this->dbConfig->createPDO();
        }
        return $this->pdo;
    }
}
