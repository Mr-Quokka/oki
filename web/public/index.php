<?php
require __DIR__ . '/../src/Controller/Database.php';

function errorMess(string $message, int $httpCode){
	echo json_encode(["error"=>"$message"]);
	http_response_code($httpCode);
}

$db = new Database();

if(isset($_GET['api'])){
	header('Content-type: application/json');

	switch ($_GET['api']){

		case 'info':
			if(empty($_GET['package']))
				errorMess("must have a package name", 400);
			else{
				$packageInfo=$db->getPackageInfo($_GET['package']);
				if(empty($packageInfo))
					errorMess("this packet does not exists", 404);
				else
					echo json_encode($packageInfo);
			}
				break;

		case 'list':
			$packageList=$db->listPackages();
			if(empty($packageList))
				errorMess("there is no packages in the database", 404);
			else
				echo json_encode($db->listPackages());
			break;

		case 'version':
			if(empty($_GET['name']))
				errorMess("must have a package name", 400);
			else{

				if(empty($_GET['version']))
					errorMess("must have a package version name", 400);
				else{
				$packageName=$_GET['name'];
				$packageVersion=$_GET['version'];

					$version=$db->getPackageVersion($packageVersion,$packageName);

					if(empty($version))
						errorMess("this version of the package does not exists", 404);
					else
						echo json_encode($version);
				}
			}
			break;

		default:
			errorMess("unknown action", 400);
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
		foreach ($package as $p){
			echo '<h3><a href="package.php?name='.$i->getShortName().'">';
			print_r($i->getLongName());
			echo '</a></h3>';
		}
	}
}
