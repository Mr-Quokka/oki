<?php

declare(strict_types=1);

namespace Oki\Security;

use Exception;

/**
 * Levé lorsque l'utilisateur a des permissions insuffisantes.
 */
class SecurityException extends Exception
{
    public function __construct(string $message)
    {
        parent::__construct($message);
    }
}
