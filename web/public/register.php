<?php 

ob_start();
$content = ob_get_clean();
$title = "Register";

require __DIR__ . '/../src/Controller/Database.php';
require __DIR__ . '/../view/layout.php';


?>
<link rel="stylesheet" href="register.css" >
<script src="https://kit.fontawesome.com/489ac465c6.js" crossorigin="anonymous"></script>
<form enctype="multipart/form-data" action="" method="post" id="form">
<h1>Form to publish a package</h1>
<p>Package nickname  <br> <input type="text" name="short_name" /></p>
<p>Real name of the package  <br> <input type="text" name="description" /></p>
<p>Package language  <br> <input type="text" name="language" /></p>
<p>Package version  <br> <input type="text" name="version" /></p>
<p>Package file  <br> <input type="file" name="packet" accept=".zip"></p>
<p><input type="submit" value="PUBLISH"></p>

</form>