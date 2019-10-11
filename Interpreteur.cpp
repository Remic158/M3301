#include "Interpreteur.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
int i = 0;
int j =0;

Interpreteur::Interpreteur(ifstream & fichier) :
m_lecteur(fichier), m_table(), m_arbre(nullptr) {
}

void Interpreteur::analyse() {
  m_arbre = programme(); // on lance l'analyse de la première règle
}

void Interpreteur::tester(const string & symboleAttendu) const {
  // Teste si le symbole courant est égal au symboleAttendu... Si non, lève une exception
  static char messageWhat[256];
  if (m_lecteur.getSymbole() != symboleAttendu) {
    sprintf(messageWhat,
            "Ligne %d, Colonne %d - Erreur de syntaxe - Symbole attendu : %s - Symbole trouvé : %s",
            m_lecteur.getLigne(), m_lecteur.getColonne(),
            symboleAttendu.c_str(), m_lecteur.getSymbole().getChaine().c_str());
    throw SyntaxeException(messageWhat);
  }
}

void Interpreteur::testerEtAvancer(const string & symboleAttendu) {
  // Teste si le symbole courant est égal au symboleAttendu... Si oui, avance, Sinon, lève une exception
  tester(symboleAttendu);
  m_lecteur.avancer();
}

void Interpreteur::erreur(const string & message) const {
  // Lève une exception contenant le message et le symbole courant trouvé
  // Utilisé lorsqu'il y a plusieurs symboles attendus possibles...
  static char messageWhat[256];
  sprintf(messageWhat,
          "Ligne %d, Colonne %d - Erreur de syntaxe - %s - Symbole trouvé : %s",
          m_lecteur.getLigne(), m_lecteur.getColonne(), message.c_str(), m_lecteur.getSymbole().getChaine().c_str());
  throw SyntaxeException(messageWhat);
}

Noeud* Interpreteur::programme() {
  // <programme> ::= procedure principale() <seqInst> finproc FIN_FICHIER
  testerEtAvancer("procedure");
  testerEtAvancer("principale");
  testerEtAvancer("(");
  testerEtAvancer(")");
  Noeud* sequence = seqInst();
  testerEtAvancer("finproc");
  tester("<FINDEFICHIER>");
  return sequence;
}

Noeud* Interpreteur::seqInst() {
  // <seqInst> ::= <inst> { <inst> }
  NoeudSeqInst* sequence = new NoeudSeqInst();
  do {
      inst(sequence);
  } while (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "si"
        || m_lecteur.getSymbole() == "tantque" || m_lecteur.getSymbole() == "pour");
  // Tant que le symbole courant est un début possible d'instruction...
  // Il faut compléter cette condition chaque fois qu'on rajoute une nouvelle instruction
  return sequence;
}



void Interpreteur::inst(NoeudSeqInst* noeud) {

// <inst> ::= <affectation> ;| <instSiRiche> | <instTantQue> | <instRepeter> ;|
    // <instPour> |,<instEcrire> ;| <instLire> ;

  if (m_lecteur.getSymbole() == "<VARIABLE>") {
    Noeud *affect = affectation();
    testerEtAvancer(";");
    noeud->ajoute(affect);

  }
  else if (m_lecteur.getSymbole() == "si"
          ||m_lecteur.getSymbole() == "sinonsi"
          ||m_lecteur.getSymbole() == "sinon"){
      vector<Noeud*> noeuds = this->instSi();
      for(int i=0; i < noeuds.size(); i++){
         noeud->ajoute(noeuds.at(i));
      }
  } else if (m_lecteur.getSymbole() == "tantque"){
      noeud->ajoute(instTantque());
  } else if (m_lecteur.getSymbole() == "finpour"
          || m_lecteur.getSymbole() == "finsi"){
      noeud->ajoute(repeter());
  } else if (m_lecteur.getSymbole() == "pour") {
      noeud->ajoute(pour());
  } else if (m_lecteur.getSymbole() == "ecrire"){
      noeud->ajoute(ecrire());
  } else if (m_lecteur.getSymbole() == "lire"){
      noeud->ajoute(lire());
  } else {
      erreur("Instruction incorrecte");
  }
}

Noeud* Interpreteur::affectation() {
  // <affectation> ::= <variable> = <expression> 
    
  tester("<VARIABLE>");
  Noeud* var = m_table.chercheAjoute(m_lecteur.getSymbole()); // La variable est ajoutée à la table eton la mémorise
  m_lecteur.avancer();
  testerEtAvancer("=");
  Noeud* exp = expression();             // On mémorise l'expression trouvée
  return new NoeudAffectation(var, exp); // On renvoie un noeud affectation
}

Noeud* Interpreteur::expression() {
    
 
    
  // <expression> ::= <facteur> { <opBinaire> <facteur> }
  //  <opBinaire> ::= + | - | *  | / | < | > | <= | >= | == | != | et | ou
  Noeud* fact = facteur();
  while ( m_lecteur.getSymbole() == "+"  || m_lecteur.getSymbole() == "-"  ||
          m_lecteur.getSymbole() == "*"  || m_lecteur.getSymbole() == "/"  ||
          m_lecteur.getSymbole() == "<"  || m_lecteur.getSymbole() == "<=" ||
          m_lecteur.getSymbole() == ">"  || m_lecteur.getSymbole() == ">=" ||
          m_lecteur.getSymbole() == "==" || m_lecteur.getSymbole() == "!=" ||
          m_lecteur.getSymbole() == "et" || m_lecteur.getSymbole() == "ou" ||
          m_lecteur.getSymbole() == "++" || m_lecteur.getSymbole() == "--") {
    Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
    m_lecteur.avancer();
    Noeud* factDroit = facteur(); // On mémorise l'opérande droit
    fact = new NoeudOperateurBinaire(operateur, fact, factDroit); // Et on construuit un noeud opérateur binaire
  }
  return fact; // On renvoie fact qui pointe sur la racine de l'expression
}

Noeud* Interpreteur::facteur() {
    
  // <facteur> ::= <entier> | <variable> | - <facteur> | non <facteur> | ( <expression> )
  Noeud* fact = nullptr;
  if (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "<ENTIER>") {
    fact = m_table.chercheAjoute(m_lecteur.getSymbole()); // on ajoute la variable ou l'entier à la table
    m_lecteur.avancer();
  } else if (m_lecteur.getSymbole() == "-") { // - <facteur>
    m_lecteur.avancer();
    // on représente le moins unaire (- facteur) par une soustraction binaire (0 - facteur)
    fact = new NoeudOperateurBinaire(Symbole("-"), m_table.chercheAjoute(Symbole("0")), facteur());
  } else if (m_lecteur.getSymbole() == "non") { // non <facteur>
    m_lecteur.avancer();
    // on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
    fact = new NoeudOperateurBinaire(Symbole("non"), facteur(), nullptr);
  } else if (m_lecteur.getSymbole() == "(") { // expression parenthésée
    m_lecteur.avancer();
    fact = expression();
    testerEtAvancer(")");
  } else if (m_lecteur.getSymbole() == "++") { 
    fact = new NoeudIteration(Symbole("++"), facteur());
  } 
  else
    erreur("Facteur incorrect");
  return fact;
}

vector<Noeud*> Interpreteur::instSi() {
  // <instSi> ::= si ( <expression> ) <seqInst> finsi
    vector<Noeud*> vector;
    do{
        this->m_lecteur.avancer();

        testerEtAvancer("(");
        Noeud* condition = expression(); // On mémorise la condition
        testerEtAvancer(")");
        Noeud* sequence = seqInst();     // On mémorise la séquence d'instruction  
        vector.push_back(new NoeudInstSi(condition, sequence));
    } while(m_lecteur.getSymbole() == "si"
          ||m_lecteur.getSymbole() == "sinonsi");

  if(m_lecteur.getSymbole() == "sinon"){
      this->m_lecteur.avancer();
      Noeud* sequence = seqInst();
      vector.push_back(new NoeudInstSi(nullptr, sequence));
  }
  testerEtAvancer("finsi");

  return vector;
}



Noeud* Interpreteur::instTantque(){
    Noeud* tq = nullptr;
    testerEtAvancer("tantque");
    testerEtAvancer("(");
    Noeud* condition = expression();
    testerEtAvancer(")");
    Noeud* sequence = seqInst();     // On mémorise la séquence d'instruction
    testerEtAvancer("fintantque");
    tq = new NoeudInstTantQue(condition, sequence);
    return tq;
}

Noeud* Interpreteur::pour(){
    
  Noeud* pour = nullptr;
  testerEtAvancer("pour");
  testerEtAvancer("(");
  Noeud* condition = expression(); // On mémorise la condition
  
  
  
  testerEtAvancer(")");
  Noeud* sequence = seqInst();     // On mémorise la séquence d'instruction
  testerEtAvancer("finpour");
  pour = new NoeudInstTantQue(condition, sequence);
  return pour;
}











Noeud* Interpreteur::repeter(){
    

    
      
    return nullptr;
}


Noeud* Interpreteur::ecrire(){
    

    
    return nullptr;
}



Noeud* Interpreteur::lire(){
    

    
    return nullptr;
}

