## **Utilisation**

### **Compilation avec Make**
Le projet utilise un fichier `Makefile` pour simplifier la compilation des différents programmes. Voici les principales cibles disponibles :

| Commande           | Description                                                      |
|--------------------|------------------------------------------------------------------|
| `make default`     | Compile le programme principal sans fonctionnalités supplémentaires (`./prog`). |
| `make compile`     | Compile le compilateur complet avec toutes les fonctionnalités (`./compiler`). |
| `make test`        | Compile les tests pour les différentes fonctionnalités (`./tests`). |

### **Exécution du compilateur**
Le programme `./compiler` prend 2 ou 3 arguments :

1. **Fichier d'entrée** : Une partition LilyPond (`*.ly`).
2. **Fichier de sortie** : Le fichier audio généré (`*.wav`).
3. **Option** (facultative) : `-l`, `-p`, ou `-lp` / `-pl` :
   - `-l` : Affiche la liste des tokens générés par le lexer.
   - `-p` : Affiche l'arbre syntaxique (AST).
   - `-lp` ou `-pl` : Affiche les tokens et l'AST.

---

### **Exemple d'utilisation**
Pour compiler et exécuter le programme avec une partition d'exemple :
```bash
make compile && ./compiler Partitions/test.ly test.wav -p
```

Dans cet exemple :
- `Partitions/test.ly` : Fichier LilyPond d'entrée.
- `test.wav` : Fichier audio généré.
- `-p` : Affiche uniquement l'AST.

---

## **Fichiers de test**
Des fichiers de test sont disponibles dans le dossier `Partitions`. **Attention** : 
- Seul `test.ly` génère un fichier `.wav` correct.
- Les autres fichiers (`invalid.ly`, etc.) sont utilisés pour analyser les sorties du lexer et du parser.

---

## **Structure des Dossiers**
- **`Partitions/`** : Contient les fichiers de test au format LilyPond.
- **`src/`** : Contient le code.
- **`src/TEST/`** : Contient les fichiers de test pour le compilateur.
