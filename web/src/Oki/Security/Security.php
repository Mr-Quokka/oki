<?php

declare(strict_types=1);

namespace Oki\Security;

use Oki\Gateway\UserGateway;
use Oki\Model\User;

const USER = 'USER';

final class Security
{
    private UserGateway $userGateway;
    private array $session;
    private ?User $user = null;

    public function __construct(UserGateway $userGateway, array &$session)
    {
        $this->userGateway = $userGateway;
        $this->session = &$session;
    }

    public function register(User $user): ?User
    {
        if (!$this->userGateway->insert($user)) {
            return null;
        }
        $this->session[USER] = $user->getId();
        $this->user = $user;
        return $user;
    }

    public function initLogin(string $login, string $rawPassword): bool
    {
        $user = $this->userGateway->getByLogin($login);
        if ($user === null || !password_verify($rawPassword, $user->getPassword())) {
            return false;
        }
        $this->session[USER] = $user->getId();
        $this->user = $user;
        return true;
    }

    public function changeCurrentPassword(?User $user, string $currentPassword, string $newPassword): bool
    {
        if ($user === null || !password_verify($currentPassword, $user->getPassword())) {
            return false;
        }
        $user->setPassword($newPassword);
        $this->userGateway->update($user);
        return true;
    }

    public function assertPerms(int $level): void
    {
        $user = $this->getCurrentUser();
        if ($user === null || $user->getPermissions() < $level) {
            throw new SecurityException('Insufficient permissions');
        }
    }

    public function assertLogged(): void
    {
        if ($this->user === null && empty($this->session[USER])) {
            throw new SecurityException('Not logged');
        }
    }

    public function logout()
    {
        $this->user = null;
        unset($this->session[USER]);
    }

    public function getCurrentUser(): ?User
    {
        if (!empty($this->session[USER]) && $this->user === null) {
            $this->user = $this->userGateway->getById($this->session[USER]);
        }
        return $this->user;
    }
}
