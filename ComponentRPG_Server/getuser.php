<?php
    header("Access-Control-Allow-Origin: *");
	header("Content-Type: application/json; charset=UTF-8");

	$conn = new mysqli("localhost", "bankkuna_admin", "Superbank!123", "bankkuna_ComponentRPG");

	$result = $conn->query("SELECT email, username, password FROM useraccount");

	$response['response'] = array();

	while($rs = $result->fetch_array(MYSQLI_ASSOC)) {
	    $row['email'] = $rs['email'];
	    $row['username'] = $rs['username'];
	    $row['password'] = $rs['password'];

	    array_push( $response['response'], $row );
	    //echo json_encode($row);
	}
	
	echo json_encode($response);
	$conn->close();
php?>