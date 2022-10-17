<?php
require __DIR__ . '/../src/Controller/Database.php';

function errorPage(int $code){
	switch ($code) {
		case '404':
			$code="404 NOT FOUND";
			$message="Le paquet demandé n'existe pas";
			http_response_code(404);
			break;

		default:
			break;
	}
	print_r("<html><head><meta charset=\"utf-8\"><head/><title>$code</title>
		<body><center><h1>$code</h1><hr/><h2>$message<h2/></center></body></html>");
}

$short_name=$_GET['name'];

$pa = new \Model\Package();
$db = new \Controller\Database();
$pa=$db->getPackageInfo($short_name);

if (! empty($pa)){

	$paVersions=$pa->getVersions();

	echo '<h1><a href="index.php"><</a></h1>';

	echo '<center><h2>';
	print_r($pa->getShortName());
	echo '</h2><h3>';
	print_r($pa->getDescription());
	echo '</h3></center><hr/><br/>';

	if(empty($paVersions)){
		echo "<h2><center>Aucune version du packet n'a été implémentée</center></h2>";
	}
	else{
		foreach ($paVersions as $p){
			echo '<center><strong><a href="' . $p->getDownloadUrl() . '">';
			print_r($p->getIdentifier());
			echo "</a></strong> : Publié le <strong>";
			print_r($p->getPublishedDate());
			echo '</strong></center>';
		}
	}
}
else {
	errorPage('404');
}
