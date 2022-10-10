<?php
if($_SERVER['REQUEST_METHOD']==='POST'){
    var_dump($_POST);
}
?>

<form enctype="multipart/form-data" action="" method="post">
<h1>Formulaire de publication d'un packet</h1>
<p>Surnom du packet : <input type="text" name="short_name" /></p>
<p>Véritable nom du packet : <input type="text" name="long_name" /></p>
<p>Langage du packet : <input type="text" name="language" /></p>
<p>Version du packet : <input type="text" name="version" /></p>
<p>Fichier du packet : <input type="file" name="packet" accept=".zip"></p>
<p><input type="submit" value="OK"></p>
</form>

<?php

require __DIR__ . '/../src/Controller/Database.php';

$db = new Database();

if( !empty($_POST['short_name']) or !empty($_POST['long_name']) or !empty($_POST['language']) or !empty($_POST['version']) or !empty($_POST['packet'])){
    
    $short=$_POST['short_name'];
    $long=$_POST['long_name'];
    $language=$_POST['language'];
    $version=$_POST['version'];
    $packet=$_POST['packet'];

    if($db->getPackageVersion($short,$version) == NULL){

        $db->insererPacket($short,$long,$language,$version);

        move_uploaded_file($_FILES, "packages/");
    }
}

?>