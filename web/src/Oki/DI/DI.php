<?php

declare(strict_types=1);

namespace Oki\DI;

use Oki\Config\DatabaseConfig;
use Oki\Gateway\PackageGateway;
use Oki\Gateway\UserGateway;
use Oki\Router\Router;
use Oki\Security\Security;
use PDO;

final class DI
{
    private Router $router;

    private DatabaseConfig $dbConfig;

    private ?PDO $pdo = null;

    private ?PackageGateway $packageGateway = null;

    private ?UserGateway $userGateway = null;

    private ?Security $security = null;

    public function __construct()
    {
        $this->dbConfig = getDefaultDatabaseConfig();
    }

    public function setRouter(Router $router): void
    {
        $this->router = $router;
    }

    public function getRouter(): Router
    {
        return $this->router;
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
            $this->userGateway = new UserGateway($this->getPDO(), $this->dbConfig);
        }
        return $this->userGateway;
    }

    public function getSecurity(): Security
    {
        if ($this->security === null) {
            session_start();
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
