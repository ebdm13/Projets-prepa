# Projets-prepa

# Compilateur lilypond
## Lexer
**Comment ajouter une nouvelle commande ?**
1. Ajouter son nom dans l'énumération dans le fichier token.h. Ex: COMMAND_NEWCOMMAND
2. Ajouter ce même nom dans la liste: ```char* type_name_map[]``` (token.c) **au même endroit que dans l'énumération**
3. Ajouter la condition correspondante dans le fichier lexer.c: ```TokenType get_command_type(char* command) ```.
