<?php

require_once 'class.php';

$type = 3;
// $pdf_path = dirname(dirname(__DIR__)) . "/input/example_protected.pdf";
$pdf_path = dirname(dirname(__DIR__)) . "/input/example_with_char.pdf";
$isProtected = 0;
$image_path = dirname(dirname(__DIR__)) . "/input/image.png";
$output_path = dirname(dirname(__DIR__)) . "/output/output.pdf";
$p12Path = dirname(dirname(__DIR__)) . "/input/usertesting.p12";
$char = "#";
$nik = "1234567890123456";
$passphraseBSrE = "Hantek1234.!";
$passphraseCert = "123456";
$page = 1;
$visibility = 0;
$x = 100;
$y = 100;
$width = 128;
$height = 45.374;
$id = "L3IqxuKvTRHKUvHim2YPXBEj7U0a";
$secret = "oAH8f9DGsWd4XJLZZiKGhlaRk2ca";
$isLTV = 0;
$isSeal = 0;

// Create an instance of the Car class
$besign = new BeSign($type, $pdf_path, $isProtected, $char, $image_path, $output_path, $p12Path, $nik, $passphraseBSrE, $passphraseCert, $page, $x, $y, $width, $height, $id, $secret, $isLTV, $isSeal);

// Call the display method to show car details
$besign->sign();

?>
