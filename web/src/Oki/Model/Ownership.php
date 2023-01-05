<?php

declare(strict_types=1);

namespace Oki\Model;

class Ownership
{
    private int $user_id;
    private int $package_id;
    private bool $is_pending;

    public static function from(int $userId, int $packageId, bool $isPending = false): Ownership
    {
        $ownership = new Ownership();
        $ownership->user_id = $userId;
        $ownership->package_id = $packageId;
        $ownership->is_pending = $isPending;
        return $ownership;
    }

    public function getUserId(): int
    {
        return $this->user_id;
    }

    public function getPackageId(): int
    {
        return $this->package_id;
    }


    public function isPending(): bool
    {
        return $this->is_pending;
    }

    public function isActive(): bool
    {
        return !$this->is_pending;
    }
}
