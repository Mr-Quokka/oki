<?php
require __DIR__ . '/../src/Controller/Database.php';

$db = new Database();

if(isset($_GET['api'])){
	if($_GET['api'] == 'info'){
		if(isset($_GET['package'])){
			header('Content-type: application/json');
			// var_dump($db->getPackageInfo($_GET['package']));
			echo json_encode($db->getPackageInfo($_GET['package']));
		}
	}
	if($_GET['api'] == 'list'){
		header('Content-type: application/json');
		echo json_encode($db->listPackages());
	}	
}

else{
	echo '<pre>';
	print_r($db->listPackages());
	echo '</pre>';
}

