CREATE TABLE adherent (
     id int(3) PRIMARY KEY,
     nom STRING NOT NULL,
     prenom STRING NOT NULL,
     categorie STRING,
     datenaissance DATE NOT NULL,
     adresse STRING,
     telephone char(10),
     mail STRING
);

CREATE TABLE licences(
  id int(3) PRIMARY KEY,
  federation STRING,
  numlicence int,
  paiement STRING,
  FOREIGN KEY id references adherent(id)
);

CREATE TABLE article(
    id int(3) PRIMARY KEY, commentaire : clé quandidate : nom et taille
    nom STRING,
    description STRING,
    prixVente float,
    quantite int,
    taille STRING
);

CREATE TABLE achatAdherent(
  idArticle int(3),
  idAdherent int(3),
  dateAchat DATE,
  paiement BOOLEAN,
  PRIMARY KEY(idArticle, idAdherent),
  FOREIGN KEY idArticle references article(id),
  FOREIGN KEY idAdherent references adherent(id)
);


CREATE TABLE bonDeCommande(
  id int(3) PRIMARY KEY,
  dateAchat DATE,
  prixTotal float,
  validation BOOLEAN,

);

CREATE TABLE articleCommande(
  idBonDeCommande int(3),
  nom STRING,
  taille STRING,
  quantite int,
  prixDEVente float

);
