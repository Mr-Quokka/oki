<?php
require __DIR__ . '/../src/Controller/Database.php';

$db = new Database();

if(isset($_GET['api'])){
	header('Content-type: application/json');

	switch ($_GET['api']){

	case 'info':
		if(isset($_GET['package']))
			$packageInfo=$db->getPackageInfo($_GET['package']);
			if(empty($packageInfo))
				echo json_encode(["error"=>"this packet does not exists"]);
			else
				echo json_encode($packageInfo);
			break;

	case 'list':
		$packageList=$db->listPackages();
		if(empty($packageList))
			echo json_encode(["error"=>"there's no packages in the database"]);
		else
			echo json_encode($db->listPackages());
		break;

	case 'version':

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
		break;

	default:
		echo json_encode(["error"=>"unknown action"]);
		break;
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
