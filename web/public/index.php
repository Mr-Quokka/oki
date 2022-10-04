<?php
require __DIR__ . '/../src/Controller/Database.php';

$db = new Database();

if(isset($_GET['api'])){
	header('Content-type: application/json');
	echo json_encode($db->listPackages());
}
else{
	echo '<pre>';
	print_r($db->listPackages());
	echo '</pre>';
}

