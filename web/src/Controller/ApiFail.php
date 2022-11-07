<?php

function apiFail(string $message, int $httpCode){
	echo json_encode(["error"=>"$message"]);
	http_response_code($httpCode);
}
