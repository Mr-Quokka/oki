<?php
require __DIR__ . '/../src/Controller/Database.php';

$db = new Database();
echo '<pre>';
print_r($db->listPackages());
echo '</pre>';
