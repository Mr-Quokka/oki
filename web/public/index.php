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
<hr/>

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
