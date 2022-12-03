<?php

use Oki\DI\DI;
use Oki\Model\User;

require __DIR__ . '/../autoload.php';

$user = User::fromRawPassword('test', 'test');
$security = (new DI())->getSecurity();
$security->register($user);
