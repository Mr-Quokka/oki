<h1><a href="index.php"><</a></h1>
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

$db = new \Controller\Database();

if( !empty($_POST['short_name']) or !empty($_POST['long_name']) or !empty($_POST['language']) or !empty($_POST['version']) or !empty($_POST['packet'])){

    $short=$_POST['short_name'];
    $long=$_POST['long_name'];
    $language=$_POST['language'];
    $version=$_POST['version'];

    $db->insererPacket($short,$long,$language,$version);

    $name = basename($_FILES['packet']['name']);

    move_uploaded_file($_FILES['packet']['tmp_name'], "packages/$name");

    $extension = new SplFileInfo($_FILES['packet']['name']);

    $extension = $extension->getExtension();

    chdir("packages/");

    rename("$name","$short"."_"."$version"."."."$extension");

}

else{
    echo '<p style="color: red">Please enter informations in fields</p>';
}
