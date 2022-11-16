<?php

function errorPage(int $code){
	switch ($code) {
		case 400:
			$code="400 BAD REQUEST";
			$message="Must Missing Something";
			http_response_code(400);
			break;

		case 404:
			$code="404 NOT FOUND";
			$message="Unknown package";
			http_response_code(404);
			break;

		default:
			break;
	}
	echo "<html><head><meta charset=\"utf-8\"><head/><title>$code</title>
		<body style='text-align:center'><h1>$code</h1><hr/><h2>$message<h2/></body></html>";
}
