<?php ob_start(); ?>

<link rel="stylesheet" href="publish.css" >

<h1><a href="index.php"><</a></h1>
<form enctype="multipart/form-data" action="" method="post" id="form">
<h1>Form to publish a package</h1>
<p>Package nickname  <br> <input type="text" name="short_name" /></p>
<p>Real name of the package  <br> <input type="text" name="description" /></p>
<p>Package language  <br> <input type="text" name="language" /></p>
<p>Package version  <br> <input type="text" name="version" /></p>
<p>Package file  <br> <input type="file" name="packet" accept=".zip"></p>
<p><input type="submit" value="PUBLISH"></p>

</form>

<?php

$content = ob_get_clean();

require __DIR__ . '/../src/Controller/Database.php';
require __DIR__ . '/../view/layout.php';

$db = new \Controller\Database();

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
	if (empty($_POST['short_name']) || empty($_POST['description']) || empty($_POST['language']) || empty($_POST['version'])) {
        http_response_code(400);
		echo '<p style="color: red">Please enter informations in fields</p>';
	} else {
		$short = $_POST['short_name'];
		$long = $_POST['description'];
		$language = $_POST['language'];
		$version = $_POST['version'];

		$error = $db->insertPackage($short, $long, $language, $version);
		if (is_string($error)) {
			http_response_code(400);
			echo '<p style="color: red">' . $error . '</p>';
		} else {
			$name = basename($_FILES['packet']['name']);
			move_uploaded_file($_FILES['packet']['tmp_name'], "packages/$name");
			$extension = new SplFileInfo($_FILES['packet']['name']);
			$extension = $extension->getExtension();
			chdir("packages/");
			rename("$name", $short . '_' . $version . '.' . $extension);
		}
	}
}
