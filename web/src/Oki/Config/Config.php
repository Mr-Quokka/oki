<?php

use Oki\Config\DatabaseConfig;
use Oki\Config\SQLiteConfig;

const BASE_URL = 'http://localhost:8000/';
function getDefaultDatabaseConfig(): DatabaseConfig
{
    return new SQLiteConfig();
}
