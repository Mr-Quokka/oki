<?php

declare(strict_types=1);

namespace Oki\Util;

final class CrossOriginResourceSharing
{
    public static function enable(): void
    {
        header('Access-Control-Allow-Origin: *');
        if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
            header('Access-Control-Allow-Methods: GET, POST, OPTIONS');
        }
    }
}
