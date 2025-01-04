# Compilateur LilyPond vers WAV

## Introduction
Ce projet implémente un compilateur qui prend en entrée des fichiers de notation musicale LilyPond et génére un fichier audio `.wav`. Le programme est structuré en plusieurs étapes:

1. **Analyse lexicale** : Extraction de tokens depuis le fichier d'entrée.
2. **Analyse syntaxique** : Construction d'un AST (Abstract Syntax Tree) à partir des tokens.
3. **Rendu audio**: Génère le fichier .wav à partir de l'AST.

---

## Format des fichiers supportés
Seules certaines fonctionnalités de LilyPond ont été implémentées. Ce compilateur ne gère pas les commandes avancées, variables ou macros. Il peut compiler uniquement des fichiers dans ce format simplifié :

```
{
    note note rest ... note rest ...
}
```

- **note** : `<nom><altération><octave><durée>`
- **rest** : `r<durée>`

### Détails du format :
- **Nom** : `a`, `b`, `c`, `d`, `e`, `f`, `g` correspondant à la notation musicale (la, si, do, etc.).
- **Altération** : `is`, `es`, `isis`, `eses` (dièse, bémol, double dièse, double bémol).
- **Octave** : `'` ou `,` pour indiquer un octave supérieur ou inférieur.
- **Durée** : `1`, `2`, `4`, `8`, `16`, etc., correspondant à des rondes, blanches, noires, croches, etc.

### Exemple :
```ly
{
  c' c' c' d' e'2 d'2 c' e' d' d' c'
}
```

---

## Fonctionnalités Implémentées

## Structures de Données

### **Token**
```c
typedef struct Token {
    TokenType type; // Type du token (e.g., NOTE, COMMAND, etc.)
    char* lexem;    // Lexème associé au token
} Token;
```

### **TokenNode**
```c
typedef struct TokenNode {
    Token* token;          // Pointeur vers un token
    int linenumber;        // Numéro de ligne où le token a été trouvé
    struct TokenNode* next; // Pointeur vers le prochain noeud
} TokenNode;
```

### **AST_node**
```c
typedef struct AST_node {
    Token* token;              // Token associé au noeud
    int linenumber;            // Numéro de ligne du noeud
    struct AST_node* parent;   // Pointeur vers le parent
    struct AST_node* first_child; // Premier enfant
    struct AST_node* next_sibling; // Frère suivant
} AST_node;
```

---

### Gestion des Tokens
Le module **token** fournit les outils pour manipuler les tokens générés par le lexer.

#### Structures :
- **`Token`** : Représente un élément lexical avec les attributs :
  - `type` : Type du token (ex. NOTE, COMMAND, etc.).
  - `lexem` : Chaîne associée au token.
- **`TokenNode`** : Élément d'une liste chaînée contenant un token, un numéro de ligne et un pointeur vers le prochain noeud.

#### Fonctions principales :
- **`create_token(TokenType type, char* lexem)`** : Crée un nouveau token.
- **`add_token(TokenNode* head, Token* token, int linenumber)`** : Ajoute un token à la fin d'une liste de tokens.
- **`insert_token(TokenNode* head, Token* token, int linenumber)`** : Insère un token après un noeud spécifique.
- **`free_list(TokenNode* queue)`** : Libère la mémoire associée à une liste de tokens.
- **`print_tokens(TokenNode* queue)`** : Affiche tous les tokens de la liste.
- **`bool is_type_command(Token* token)`** : Vérifie si un token est une command.
- **`char* get_type(Token* token)`** : Renvoie le type du token en chaine de charactères.

---

### Gestion de l'AST (Abstract Syntax Tree)
Le module **AST** gère la structure hiérarchique des noeuds représentant les éléments musicaux.

#### Structures :
- **`AST_node`** : Représente un noeud de l'AST avec les attributs :
  - `Token* token` : Le token associé au noeud.
  - `int linenumber` : Le numéro de ligne associé au noeud.
  - `AST_node* parent` : Le parent du noeud dans l'AST.
  - `AST_node* first_child` : Le premier enfant du noeud.
  - `AST_node* next_sibling` : Le prochain frère du noeud.

#### Fonctions principales :
- **`create_AST_node(Token* token, int linenumber)`** : Crée un nouveau noeud de l'AST.
- **`create_node(TokenNode* node)`** : Simplifie la création de noeuds en acceptant un `TokenNode` comme argument.
- **`add_child(AST_node* parent, AST_node* child)`** : Ajoute un noeud enfant à un noeud parent.
- **`free_AST(AST_node* root)`** : Libère la mémoire allouée à un AST complet.
- **`print_AST_node(AST_node* node, AST_node* root, int indent, bool sticks[100])`** : Affiche un noeud de l'AST avec indentation et symboles hiérarchiques.
- **`print_AST(AST_node* root)`** : Affiche tout l'AST de manière structurée.

---

### Analyse Lexicale
Le module **lexer** analyse le fichier d'entrée pour en extraire des tokens décrivant les éléments de la notation musicale.

#### Fonctions principales :
- **`isCommand(char* s)`** : Vérifie si une chaîne est une commande valide (ex. `\relative`).
- **`isRest(char* s)`** : Vérifie si une chaîne représente une pause (`r`).
- **`isNote(char* s)`** : Vérifie si une chaîne est une note musicale valide (ex. `c'4`).
- **`isNumber(char* s)`** : Vérifie si une chaîne est un nombre entier.
- **`skip_comment(FILE* f, int* linenumber, TokenNode* queue)`** : Ignore les commentaires dans l'entrée, incrémente le numéro de ligne.
- **`get_string(FILE* f, int linenumber, TokenNode* queue)`** : Extrait une chaîne délimitée par des guillemets.
- **`Token* get_token(char* buffer)`** : Retourne le token correspondant au buffer (=lexem).
- **`lexer(FILE* f)`** : Analyse l'entrée complète et retourne une liste de tokens sous forme de `TokenNode`.

---

### Analyse Syntaxique
Le module **parser** prend la liste de tokens produite par le lexer et construit un arbre syntaxique abstrait (AST).

#### Fonctions principales :
- **`concat(char* s1, char* s2)`** : Concatène deux chaînes de caractères.
- **`is_var(TokenNode* head, TokenNode* TokenQueue, AST_node* root)`** : Vérifie si un token représente une définition de variable valide.
- **`add_var(TokenNode* head, AST_node* node, AST_node* root, TokenNode* TokenQueue)`** : Ajoute une variable à l'AST.
- **`TokenNode* add_unknown(TokenNode* head, AST_node* node, TokenNode *TokenQueue, AST_node* root)`** : Ignore la commande ou l'ajoute si c'est un appelle de variable
- **`add_note(TokenNode* head, AST_node* node)`** : Ajoute une note musicale à l'AST en décomposant ses éléments (nom, octave, altération, durée).
- **`add_relative(TokenNode* head, AST_node* node, TokenNode* TokenQueue, AST_node* root)`** : Gère la commande \relative dans l'AST.
- **`parse(TokenNode* TokenQueue)`** : Construit l'AST à partir de la liste de tokens.

---

### Gestion des Erreurs
Le module **error** gère l'affichage des erreurs et des avertissements rencontrés durant les différentes étapes.

#### Fonctions principales :
- **`error(int linenumber, char* format, ...)`** : Affiche un message d'erreur en rouge avec le numéro de ligne correspondant.
- **`warning(int linenumber, char* format, ...)`** : Affiche un message d'avertissement en orange avec le numéro de ligne correspondant.

---

### Rendu Audio (Module `renderer.c` et `renderer.h`)
Le module **renderer** est responsable de convertir l'AST en données audio.

#### Structures :
- **`note_t`** : Représente une note musicale.
  ```c
  typedef struct {
      int pitch;  // Hauteur de la note en demi-tons
      int duree;  // Durée de la note en fractions de la durée de référence
  } note_t;
  ```

#### Fonctions principales :
- **`note_t* create_note(int pitch, int duree)`** : Crée une structure représentant une note avec sa hauteur (`pitch`) et sa durée (`duree`).
- **`note_t* render_note(AST_node* note)`** : Convertit un noeud de l'AST représentant une note en une structure `note_t`.
- **`float duree_to_second(int duree, int duree_ref, int bpm)`** : Convertit la durée d'une note en secondes en fonction des paramètres `duree_ref` (durée de référence) et `bpm` (battements par minute).
- **`track_t* render(AST_node* root, int f_ech, int duree_ref, int bpm)`** :
  - Génère une structure `track_t` contenant les sons associés à chaque note et silence.
  - Prend en compte les paramètres : fréquence d'échantillonnage (`f_ech`), durée de référence (`duree_ref`), et tempo (`bpm`).

---

### Exécution Principale (Module `compiler.c`)
Le fichier **compiler.c** implémente le pipeline complet du compilateur, de l'entrée texte au rendu audio.

#### Fonctionnement :
1. **Analyse lexicale** :
   - Appelle `lexer()` pour extraire les tokens à partir du fichier d'entrée.
   - Affiche le temps d'exécution de l'analyse lexicale.

2. **Analyse syntaxique** :
   - Appelle `parse()` pour générer l'AST à partir des tokens.
   - Affiche le temps d'exécution de l'analyse syntaxique.

3. **Rendu audio** :
   - Appelle `render()` pour convertir l'AST en une piste audio (`track_t`).
   - Réduit la piste en un fichier sonore avec `reduce_track()` et `save_sound()`.

4. **Options** :
   - Permet d'afficher les tokens (`-l`), l'AST (`-p`), ou les deux (`-lp` ou `-pl`).

#### Exemple d'exécution :
```bash
./compiler input.ly output.wav -lp
```

- **Entrée** : `input.ly` (fichier LilyPond)
- **Sortie** : `output.wav` (fichier audio généré)
- **Options** :
  - `-l` : Affiche les tokens extraits.
  - `-p` : Affiche l'AST.
  - `-lp` ou `-pl` : Affiche les deux.

---
