<?php
require __DIR__ . '/../src/Controller/Database.php';
require __DIR__ . '/../src/Controller/ErrorPage.php';

$short_name=$_GET['name'];

if (empty($short_name)){
	errorPage(400);
}
else {

	$pa = new \Model\Package();
	$db = new \Controller\Database();
	$pa=$db->getPackageInfo($short_name);

	if (empty($pa)){
		errorPage(404);
	}
	else{

		$paVersions=$pa->getVersions();

		ob_start();
		echo '<link rel="stylesheet" href="package.css">';

		echo '<div class="packageName"><h2>' . $pa->getShortName() . '</h2><h3>' . $pa->getDescription() . '</h3><hr/><br/></div>';

		if(empty($paVersions)){
			echo '<h2 style="text-align:center">This package do not have any version</h2>';
		}
		else{
			echo '<h3 style="font-weight:bold">Version listing:</h3>';
			foreach ($paVersions as $p){
				echo '<div style="text-align:center"><a class="downloadLink" href="' . $p->getDownloadUrl() . '">';
				echo $p->getIdentifier() . '</a>: Published the <div style="font-weight:bold; display:inline">' . $p->getPublishedDate() . '</div></div>';
			}
		}
		$content = ob_get_clean();
		$title = "Package (" . ($pa->getShortName() . ")");
		require __DIR__ . '/../view/layout.php';
	}
}