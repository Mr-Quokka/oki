<?php
require __DIR__ . '/../src/Controller/Database.php';

$short_name=$_GET['name'];

$pa = new Package();
$db = new Database();

/*
for ($i=0; $i<count($db); $i++){
	if ($db[$i]->getShortName() == $short_name){
		$pa=$db[$i];
		break;
	}
}
*/

$pa=$db->getPackageInfo($short_name);

$paVersions=$pa->getVersions();

echo '<h1><a href="index.php"><</a></h1>';

echo '<center><h2>';
print_r($pa->getShortName());
echo '</h2><h3>';
print_r($pa->getLongName());
echo '</h3><h4>';
print_r($pa->getId());
echo '</h4></center><hr/><br/>';

if(empty($paVersions)){
	echo "<h2><center>Aucune versions du packet n'a été implémentée</center></h2>";
}
else{
	for ($i=0; $i<count($paVersions); $i++){
		echo '<center><strong>';
		print_r($paVersions[$i]->getIdentifier());
		echo "</strong> : Publié le <strong>";
		print_r($paVersions[$i]->getPublishedDate());
		echo '</strong></center>';
	}
}
