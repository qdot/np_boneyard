<?php
$slfile = fopen("sllog.txt", "r+");
ftruncate($slfile,0);
fwrite($slfile, $_POST['chatlog']);
fclose($slfile);
$logfile = fopen("mucklog.txt", "r+");
$str = fread($logfile, 1800);
rewind($logfile);
ftruncate($logfile, 1800);
fclose($logfile);
echo "12345\n";

//Pages and Whispers - Removed
$pattern[0] = "/^##*\n/";
$replacement[0] = "";
//Spoofs - Removed
$pattern[1] = "/^##*\n/";
$replacement[1] = "";


$str = preg_replace("/^##/", 
while(!feof($logfile))
{
	$str = fgets($logfile);
	if(!(preg_match("/^\(/", $str)) && !(preg_match("/##/", $str))) echo $str;
}
?>