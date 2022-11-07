<?php
require __DIR__ . '/../src/Controller/Database.php';
require __DIR__ . '/../src/Controller/ErrorPage.php';

$short_name=$_GET['name'];

if (empty($short_name)){
	errorPage('400');
}
else {

	$pa = new \Model\Package();
	$db = new \Controller\Database();
	$pa=$db->getPackageInfo($short_name);

	if (empty($pa)){
		errorPage('404');
	}
	else{

		$paVersions=$pa->getVersions();

		ob_start();
		echo '<link rel="stylesheet" href="package.css">';

		echo '<div style="text-align:center"><h2>';
		print_r($pa->getShortName());
		echo '</h2><h3>';
		print_r($pa->getDescription());
		echo '</h3><hr/><br/></div>';

		if(empty($paVersions)){
			echo "<h2 style='text-align:center'>This package do not have any version</h2>";
		}
		else{
			echo "<h3 style='font-weight:bold; display:inline'>Version listing:</h3>";
			foreach ($paVersions as $p){
				echo '<div style="text-align:center"><a style="font-weight:bold" href="' . $p->getDownloadUrl() . '">';
				print_r($p->getIdentifier());
				echo '</a>: Published the <strong>';
				print_r($p->getPublishedDate());
				echo '</strong></div>';
			}
		}
		$content = ob_get_clean();
		$title = "Package (" . ($pa->getShortName() . ")");
		require __DIR__ . '/../view/layout.php';
	}
}