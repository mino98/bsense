<?php

//=== Includes the configuration parameters:
global $_config;
require("includes/config.inc.php");

//=== Parse the request:
$post = (!empty($_POST)) ? true : false;

if($post) {
	$name 	 = stripslashes($_POST['name']);
	$email 	 = trim($_POST['email']);
	$message = stripslashes($_POST['message']);

	// additional fields for the support form:
	$UUID = htmlentities($_REQUEST['UUID']);
	$version = htmlentities($_REQUEST['version']);
	$OS = htmlentities($_REQUEST['OS']);

	$error = '';

	// Check name
	if(!$name || $name == "Your name")
		$error .= 'Please enter your name.<br />';

	// Check email
	if(!$email || $email == "Your email" || !ValidateEmail($email))
		$error .= 'Please enter a valid e-mail address.<br />';

	// Check message
	if(!$message || $message == "Your comments")
		$error .= "Please enter your message. <br />";

	if(!$error) {
		$msg = "Name: $name\n";
		$msg.= "Email: $email\n";
		$msg.= "Date: ".date("r")."\n";
		$msg.= "IP: ".$_SERVER['REMOTE_ADDR']."\n";
		
		if($UUID) $msg.= "UUID: ".$UUID."\n";
		if($version) $msg.= "BSense version: ".$version."\n";
		if($OS) $msg.= "OS: ".$OS."\n";
		
		$msg.= "\n";
		$msg.= "Message: $message";
		
		$headers = "From: \"".addslashes($name)."\" <".$email.">\r\n";
		
		$mail = mail($_config['contactEmail'], "Message from broadbandforall.net", $msg, $headers);
		
		if($mail)
			echo '<div class="formstatusok">Your feedback has been sent. Thank you!</div>';
	} else {
		echo '<div class="formstatuserror">'.$error.'</div>';
	}
}

function ValidateEmail($email) {
	$regex = '/([a-z0-9_.-]+)'. # name
	'@'. # at
	'([a-z0-9.-]+){2,255}'. # domain & possibly subdomains
	'.'. # period
	'([a-z]+){2,10}/i'; # domain extension 

	if($email == '') 
		return false;
	else
		$eregi = preg_replace($regex, '', $email);
	return empty($eregi) ? true : false;
}


?>
