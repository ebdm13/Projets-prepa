#pragma once

/*
Affiche un message en rouge avec linenumber, la ligne correspondant à l'erreur. Si linenumbre = 0, elle ne sera pas affichée*/
void error(int linenumber, char* format, ...);

/*Affiche un message en orange avec linenumber, la ligne correspondant à l'alerte. Si linenumbre = 0, elle ne sera pas affichée*/
void warning(int linenumber, char* format, ...);


/*Affiche un message en vert avec linenumber, la ligne correspondant à l'alerte. Si linenumbre = 0, elle ne sera pas affichée*/
void info(int linenumber, char* format,...);