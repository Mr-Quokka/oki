<?php
require __DIR__ . '/../src/Controller/Database.php';

$db = new Database();

if(isset($_GET['api'])){
	header('Content-type: application/json');
	if($_GET['api'] == 'info'){
		if(isset($_GET['package']))
			echo json_encode($db->getPackageInfo($_GET['package']));
	}

	if($_GET['api'] == 'list')
		echo json_encode($db->listPackages());

	if($_GET['api'] == 'version'){
		
		$packageName=$_GET['name'];
		$packageVersion=$_GET['version'];

		$version=$db->getPackageVersion($packageVersion,$packageName);
		echo json_encode($version);
	}
}

else{
	echo '<pre>';
	print_r($db->listPackages());
	echo '</pre>';
}


