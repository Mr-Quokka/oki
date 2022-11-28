<?php

ob_start();
$content = ob_get_clean();
$title = "Register";

require __DIR__ . '/../src/Oki/Config/SplClassLoader.php';
require __DIR__ . '/../src/Oki/Config/Config.php';

$loader = new SplClassLoader('Oki', __DIR__ . '/../src');
$loader->register();

?>

<link rel="stylesheet" href="style/form-layout.css" >
<form enctype="multipart/form-data" action="" method="post" id="form">
<h1>Form to register</h1>
<p>Username  <br> <input type="text" name="login" /></p>
<p>Password  <br> <input type="password" name="password" /></p>
<p>Confirm password  <br> <input type="password" name="password_confirmation" /></p>
<p><input type="submit" value="REGISTER"></p>

</form>


<?php

$userController = new \Oki\Controller\UserController();

print_r($_POST);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {

	if (empty($_POST['login']) || empty($_POST['password']) || empty($_POST['password_confirmation'])) {
        http_response_code(400);
		echo '<p style="color: red">Please enter informations in fields</p>';
	} else {
		$login = settype($_POST['login'],"string");
		$password = settype($_POST['password'],"string");
		$password_confirmtion = settype($_POST['password_confirmtion'],"string");

		$userController->register($login, $password, $password_confirmtion);
	}
}

?>