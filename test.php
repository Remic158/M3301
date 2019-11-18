<?php
require('fpdm/fpdm.php');

//Tableau contenant les paramètres à passer au PDF
$fields = array(
'nom' => 'Doe',
'prenom' => 'John',
);

//Le paramètre correspond au chemin vers le formulaire PDF
$pdf = new FPDM('teste.pdf');
$pdf->Load($fields, false); // le second paramètre vaut false si les valeurs sont en ISO-8859-1, vrai si UTF-8
$pdf->Merge();

/*
Le premier paramètre peut prendre 4 valeurs :
D pour que l’utilisateur soit obligé de télécharger le fichier
I pour afficher le fichier dans le navigateur
F pour sauvegarder le document en local
S pour retourner le document en tant que chaine de caractère.
Le deuxième paramètre est le nom du fichier
*/
$pdf->Output("D", "example_formulaire.pdf");
