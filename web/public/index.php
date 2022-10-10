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
			$packageInfo=$db->getPackageInfo($_GET['package']);
			if(empty($packageInfo))
				echo json_encode(["error"=>"this packet does not exists"]);
			else
				echo json_encode($packageInfo);
	}

	if($_GET['api'] == 'list'){
		$packageList=$db->listPackages();
		if(empty($packageList))
			echo json_encode(["error"=>"there's no packages in the database"]);
		else
			echo json_encode($db->listPackages());
	}

	if($_GET['api'] == 'version'){

		if(empty($_GET['name']))
			echo json_encode(["error"=>"must have a package name"]);
		else{
			$packageName=$_GET['name'];

			if(empty($_GET['version']))
				echo json_encode(["error"=>"must have a package version name"]);
			else{
			$packageVersion=$_GET['version'];

				$version=$db->getPackageVersion($packageVersion,$packageName);

				if(empty($version))
					echo json_encode(["error"=>"this version of the package does not exists"]);
				else
					echo json_encode($version);
			}
		}
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
