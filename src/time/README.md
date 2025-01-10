# quelques test :
##    Affichage par défaut (bleu, blanc, rouge) :

./program

## Modifier l'ordre en TIME, DATE, WEEKDAY :

./program -o 2,3,1

## Personnaliser les couleurs (WEEKDAY en cyan, TIME en magenta, DATE en vert) :

./program -c "\033[36m,\033[35m,\033[32m"

## Désactiver l'affichage de la date :

./program -d

## Tester toutes les options combinées :

./program -w -t -o 3,2,1 -c "\033[31m,\033[33m,\033[34m"
