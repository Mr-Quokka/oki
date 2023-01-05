<?php

declare(strict_types=1);

namespace Oki\Gateway;

use Oki\Model\Ownership;
use PDO;

class OwnershipGateway
{
    private PDO $pdo;

    public function __construct(PDO $pdo)
    {
        $this->pdo = $pdo;
    }

    public function granted(Ownership $ownership): bool
    {
        return $this->grantedAccess($ownership->getUserId(), $ownership->getPackageId());
    }

    public function grantedAccess(int $userId, int $packageId): bool
    {
        $req = $this->pdo->prepare('SELECT is_pending FROM ownership WHERE user_id = :user_id AND package_id = :package_id;');
        $req->bindValue(':user_id', $userId, PDO::PARAM_INT);
        $req->bindValue(':package_id', $packageId, PDO::PARAM_INT);
        if (!$req->execute()) {
            return false;
        }
        $req->setFetchMode(PDO::FETCH_CLASS, Ownership::class);
        $d = $req->fetch();
        if ($d) {
            return $d->isActive();
        }
        return false;
    }

    public function insertOwnership(Ownership $ownership): void
    {
        $req = $this->pdo->prepare('INSERT INTO ownership (user_id, package_id, is_pending) VALUES (:user_id, :package_id, :is_pending);');
        $req->bindValue(':user_id', $ownership->getUserId(), PDO::PARAM_INT);
        $req->bindValue(':package_id', $ownership->getPackageId(), PDO::PARAM_INT);
        $req->bindValue(':is_pending', $ownership->isPending(), PDO::PARAM_BOOL);
        $req->execute();
    }
}
