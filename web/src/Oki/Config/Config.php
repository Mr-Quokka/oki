<?php

use Oki\Config\DatabaseConfig;
use Oki\Config\SQLiteConfig;

function getDefaultDatabaseConfig(): DatabaseConfig
{
    return new SQLiteConfig();
}
