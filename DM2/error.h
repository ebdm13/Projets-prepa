#pragma once

/*
Affiche un message en rouge avec linenumbre, la ligne correspondant à l'erreur, puis arrête le programme. Si linenumbre = 0, elle ne sera pas affichée*/
void error(char* message, int linenumber);

/*Affiche un message en orange avec linenumbre, la ligne correspondant à l'alerte. Si linenumbre = 0, elle ne sera pas affichée*/
void warning(char* message, int linenumber);