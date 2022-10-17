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
		<body style='text-align:center'><h1>$code</h1><hr/><h2>$message<h2/></body></html>");
}

$short_name=$_GET['name'];

$pa = new \Model\Package();
$db = new \Controller\Database();
$pa=$db->getPackageInfo($short_name);

if (! empty($pa)){

	$paVersions=$pa->getVersions();

	ob_start();
	echo '<h1><a href="index.php"><</a></h1>';

	echo '<div style="text-align:center"><h2>';
	print_r($pa->getShortName());
	echo '</h2><h3>';
	print_r($pa->getDescription());
	echo '</h3><hr/><br/></div>';

	if(empty($paVersions)){
		echo "<h2 style='text-align:center'>Aucune version du packet n'a été implémentée</h2>";
	}
	else{
		echo "<h3 style='font-weight:bold; display:inline'>Liste des versions :</h3>";
		foreach ($paVersions as $p){
			echo '<div style="text-align:center"><a style="font-weight:bold" href="' . $p->getDownloadUrl() . '">';
			print_r($p->getIdentifier());
			echo '</a>: Publié le <strong>';
			print_r($p->getPublishedDate());
			echo '</strong></div>';
		}
	}
	$content = ob_get_clean();
	require __DIR__ . '/../view/layout.php';
}
else {
	errorPage('404');
}
