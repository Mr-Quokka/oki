<?php
require __DIR__ . '/../src/Controller/Database.php';

function errorPage(int $code){
	switch ($code) {
		case '404':
			$code="404 NOT_FOUND";
			$message="Le paquet demandé n'existe pas";
			break;
		
		default:
			break;
	}
	print_r("<html><head><meta charset=\"utf-8\"><head/><title>$code</title>
		<body><center><h1>$code</h1><hr/><h2>$message<h2/></center></body></html>");
}

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
	echo'
<center><pre>
 ██████╗ ██╗  ██╗██╗
██╔═══██╗██║ ██╔╝██║
██║   ██║█████╔╝ ██║
██║   ██║██╔═██╗ ██║
╚██████╔╝██║  ██╗██║
 ╚═════╝ ╚═╝  ╚═╝╚═╝
</pre></center>
<br/>
<center><h2><a href="publish.php">Publier un paquet</a></h2></center>
<hr/>';
	$package=$db->listPackages();
	if (empty($package)){
		echo "ERREUR : Aucun paquet n'est présent";
	}
	else{
		for ($i=0; $i<count($package); $i++){
			echo '<h3><a href="package.php?name='.$package[$i]->getShortName().'">';
			print_r($package[$i]->getLongName());
			echo '</a></h3>';
		}
	}
}
