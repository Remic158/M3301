<?php

//formulaire pour la demande d'autorisation conseil generale

//bibliotheque a utiliser
require('fpdm.php');

//Tableau contenant les paramètres à passer au PDF
$champs = array(

  //coordonné du destinataire
  'nom du responsable' => date($nresp),
  'adresse ' => $adresse,
  'Mail destinaire' => $maildestinataire,
  'distance' => date($distance),
  'nom epreuve' => $nepreuve,
//Relatif a l'organisation
'date courrier ' => date($datecourrier),
'Lieux de la course  ' => $lieux,
'date de la course ' => date($datecourse),
'distance' => date($distance),
'nom epreuve' => $nepreuve,
//fourchette du nombre de courreur
'nbconcurrent borne inferieur ' => $nbcinf ,
'nbconcurrent borne superieur ' => $nbcsup ,
'distance' => date($distance),
'heure borne inferieur ' => $hinf ,
'heure borne superieur ' => $hsup ,
'mail' => $mail , //peut etre pas necessaire à voir
);


$pdf = new FPDM('demande_dautorisation_conseil_generale.pdf');
$pdf->Load($champs, false);
$pdf->Merge();
$pdf->Output("D", "example_formulaire.pdf")
