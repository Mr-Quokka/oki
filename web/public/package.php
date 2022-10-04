<?php
require __DIR__ . '/../src/Controller/Database.php';

$short_name=$_GET['name'];

$pa = new Package();
$db = new Database();
$db=$db->listPackages();

for ($i=0; $i<count($db); $i++){
	if ($db[$i]->getShortName() == $short_name){
		$pa=$db[$i];
		break;
	}
}

echo '<h1><a href="index.php"><</a></h1>';

echo '<center><h2>';
print_r($pa->getShortName());
echo '</h2><h3>';
print_r($pa->getLongName());
echo '</h3><h4>';
print_r($pa->getId());
echo '</h4></center>';
