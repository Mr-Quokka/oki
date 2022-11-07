<?php
require __DIR__ . '/../src/Controller/Database.php';
require __DIR__ . '/../src/Controller/ApiFail.php';

$db = new \Controller\Database();

if(isset($_GET['api'])){
	header('Content-type: application/json');

	switch ($_GET['api']){

		case 'info':
			if(empty($_GET['package']))
				apiFail("must have a package name", 400);
			else{
				$packageInfo=$db->getPackageInfo($_GET['package']);
				if(empty($packageInfo))
					apiFail("this packet does not exists", 404);
				else
					echo json_encode($packageInfo);
			}
				break;

		case 'list':
			$packageList=$db->listPackages();
			if(empty($packageList))
				apiFail("no such packages found in the database", 404);
			else
				echo json_encode($db->listPackages());
			break;

		case 'version':
			if(empty($_GET['name']))
				apiFail("must have a package name", 400);
			else{

				if(empty($_GET['version']))
					apiFail("must have a package version name", 400);
				else{
				$packageName=$_GET['name'];
				$packageVersion=$_GET['version'];

					$version=$db->getPackageVersion($packageVersion,$packageName);

					if(empty($version))
						apiFail("this version of the package does not exists", 404);
					else
						echo json_encode($version);
				}
			}
			break;

		default:
			apiFail("unknown action", 400);
			break;
	}
}

else{
	ob_start();
	echo'<link rel="stylesheet" href="index.css">
	<h2 style="text-align:center"><a href="publish.php">Publish a package</a></h2></center><hr/>';
	$package=$db->listPackages();
	if (empty($package)){
		echo "<h2 style='text-align:center'>ERROR : no such packages found in the database</h2>";
		http_response_code(404);
	}
	else{
		foreach ($package as $p){
			echo '<h3><a class="packageName" href="package.php?name='.$p->getShortName().'">' . $p->getShortName() . '</a></h3>';
			echo '<p class="packageDesc">' . $p->getDescription() . '</p>';
		}
	}
	$content = ob_get_clean();
	$title = "Home";
	require __DIR__ . '/../view/layout.php';
}
