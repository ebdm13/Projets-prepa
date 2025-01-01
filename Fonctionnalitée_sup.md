# Compilateur LilyPond vers WAV

## Introduction
Ce projet implémente un compilateur qui prend en entrée des fichiers de notation musicale LilyPond et génére un fichier audio `.wav`. Le programme est structuré en plusieurs étapes:

1. **Analyse lexicale** : Extraction de tokens depuis le fichier d'entrée.
2. **Analyse syntaxique** : Construction d'un AST (Abstract Syntax Tree) à partir des tokens.

---

## Fonctionnalités Implémentées

### Analyse Lexicale
Le module **lexer** analyse le fichier d'entrée pour en extraire des tokens décrivant les éléments de la notation musicale.

#### Fonctions principales :
- **`isCommand(char* s)`** : Vérifie si une chaîne est une commande valide (ex. `\relative`).
- **`isRest(char* s)`** : Vérifie si une chaîne représente une pause (`r`).
- **`isNote(char* s)`** : Vérifie si une chaîne est une note musicale valide (ex. `c'4`).
- **`isNumber(char* s)`** : Vérifie si une chaîne est un nombre entier.
- **`skip_comment(FILE* f, int* linenumber, TokenNode* queue)`** : Ignore les commentaires dans l'entrée, incrémente le numéro de ligne.
- **`get_string(FILE* f, int linenumber, TokenNode* queue)`** : Extrait une chaîne délimitée par des guillemets.
- **`lexer(FILE* f)`** : Analyse l'entrée complète et retourne une liste de tokens sous forme de `TokenNode`.

---

### Analyse Syntaxique
Le module **parser** prend la liste de tokens produite par le lexer et construit un arbre syntaxique abstrait (AST).

#### Fonctions principales :
- **`concat(char* s1, char* s2)`** : Concatène deux chaînes de caractères.
- **`slice(char* s, int begin, int end)`** : Retourne une sous-chaîne d'une chaîne donnée.
- **`is_var(TokenNode* head, TokenNode* TokenQueue, AST_node* root)`** : Vérifie si un token représente une définition de variable valide.
- **`add_var(TokenNode* head, AST_node* node, AST_node* root, TokenNode* TokenQueue)`** : Ajoute une variable à l'AST.
- **`add_note(TokenNode* head, AST_node* node)`** : Ajoute une note musicale à l'AST en décomposant ses éléments (nom, octave, altération, durée).
- **`add_relative(TokenNode* head, AST_node* node, TokenNode* TokenQueue, AST_node* root)`** : Gère la commande \relative dans l'AST.
- **`parse(TokenNode* TokenQueue)`** : Construit l'AST à partir de la liste de tokens.

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

---

### Gestion des Erreurs
Le module **error** gère l'affichage des erreurs et des avertissements rencontrés durant les différentes étapes.

#### Fonctions principales :
- **`error(int linenumber, char* format, ...)`** : Affiche un message d'erreur en rouge avec le numéro de ligne correspondant.
- **`warning(int linenumber, char* format, ...)`** : Affiche un message d'avertissement en orange avec le numéro de ligne correspondant.

---

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

## Pipeline Global
1. **Lecture du fichier** : Le fichier LilyPond est ouvert et lu ligne par ligne.
2. **Analyse lexicale** : Les lignes sont converties en tokens.
3. **Analyse syntaxique** : Les tokens sont organisés en un AST représentant la hiérarchie musicale.
4. **Conversion audio** : Une fois l'AST construit, il est utilisé pour générer un fichier `.wav`.

---
