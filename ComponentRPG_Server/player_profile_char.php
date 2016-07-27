<?php
    header("Access-Control-Allow-Origin: *");
	header("Content-Type: application/json; charset=UTF-8");

	$conn = new mysqli("localhost", "bankkuna_admin", "Superbank!123", "bankkuna_ComponentRPG");
	$result = $conn->query("SELECT * FROM tbl_character where username = '" . $_GET['username'] . "' order by char_index");

	$response['response'] = array();

	while($rs = $result->fetch_array(MYSQLI_ASSOC)) {
		$row['char_id'] 					= $rs['char_id'];
		$row['char_index'] 					= $rs['char_index'];
		$row['char_name'] 					= $rs['char_name'];
		$row['char_level'] 					= $rs['char_level'];

	    $row['char_stat_damage'] 			= $rs['char_stat_damage'];
		$row['char_stat_effectiveness'] 	= $rs['char_stat_effectiveness'];
		$row['char_stat_defense'] 			= $rs['char_stat_defense'];
		$row['char_stat_resistance'] 		= $rs['char_stat_resistance'];
		$row['char_stat_speed'] 			= $rs['char_stat_speed'];
		$row['char_stat_recovery'] 			= $rs['char_stat_recovery'];
		$row['char_stat_maxbar'] 			= $rs['char_stat_maxbar'];

		$tmpColumnName = 'char_skill_';
		for ($i = 1; $i <= 7; ++$i)
		{
			$tmpSubObject = array();
			$tmpArray = explode(",", $rs['char_skill_' . $i]);
			$tmpSubObject['skillcomponent_id'] = str_replace("x", "", $tmpArray[0]);
			
			for ($j = 1; $j <= 4; ++$j)
			{
				$tmpVal = $tmpArray[$j];
				if ($tmpVal == null)
				{
					$tmpVal = 0;
				}
				$tmpSubObject['int' . $j] = $tmpVal;
			}

			for ($j = 1; $j <= 4; ++$j)
			{
				$tmpVal = 0;
				if (j+4 < count($tmpArray)-1)
				{
					$tmpVal = $tmpArray[$j+4];
				}
				$tmpSubObject['float' . $j] = $tmpVal;
			}

			$row['char_skill_' . $i] = $tmpSubObject;
		}

	    array_push( $response['response'], $row );
	}
	
	echo json_encode($response);
	$conn->close();



	// // first char
	// $row['STATPOINT_DAMAGE'] 			= 5;
	// $row['STATPOINT_EFFECTIVENESS'] 	= 6;
	// $row['STATPOINT_DEFENSE'] 			= 3;
	// $row['STATPOINT_RESISTANCE'] 		= 8;
	// $row['STATPOINT_SPEED'] 			= 4;
	// $row['STATPOINT_MAXBAR'] 			= 4;
	// array_push( $response['response'], $row );

	// // second char
	// $row['STATPOINT_DAMAGE'] 			= 3;
	// $row['STATPOINT_EFFECTIVENESS'] 	= 3;
	// $row['STATPOINT_DEFENSE'] 			= 7;
	// $row['STATPOINT_RESISTANCE'] 		= 4;
	// $row['STATPOINT_SPEED'] 			= 8;
	// $row['STATPOINT_MAXBAR'] 			= 5;
	// array_push( $response['response'], $row );

	// // third char
	// $row['STATPOINT_DAMAGE'] 			= 0;
	// $row['STATPOINT_EFFECTIVENESS'] 	= 3;
	// $row['STATPOINT_DEFENSE'] 			= 6;
	// $row['STATPOINT_RESISTANCE'] 		= 6;
	// $row['STATPOINT_SPEED'] 			= 7;
	// $row['STATPOINT_MAXBAR'] 			= 8;
	// array_push( $response['response'], $row );
php?>