<?php
if($_SERVER['REQUEST_METHOD']==='POST'){
    var_dump($_POST);
}
?>

<form action="" method="post">
<h1>Formulaire de publication d'un packet</h1>
<p>Surnom du packet : <input type="text" name="short_name" /></p>
<p>Véritable nom du packet : <input type="text" name="long_name" /></p>
<p>Langage du packet : <input type="text" name="language" /></p>
<p>Version du packet : <input type="text" name="version" /></p>
<p>Fichier du packet : <input type="file" name="packet" accept=".zip"></p>
<p><input type="submit" value="OK"></p>
</form>
