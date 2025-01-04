#pragma once

/*
Affiche un message en rouge avec linenumbre, la ligne correspondant à l'erreur. Si linenumbre = 0, elle ne sera pas affichée*/
void error(int linenumber, char* format, ...);

/*Affiche un message en orange avec linenumbre, la ligne correspondant à l'alerte. Si linenumbre = 0, elle ne sera pas affichée*/
void warning(int linenumber, char* format, ...);