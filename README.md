# README - LIDAR project - ROB5 Temps réel

Auteurs : [F. Beroujon](https://github.com/Flosilver) &
[F. Cormée](https://github.com/Florian-Cormee)

## 1. Installation

Se placer dans un premier temps dans le dossier **device**.

```bash
cd device
make
make install
```

Revenir dans le dossier du projet et compiler le projet !

```bash
cd..
make
```

## 2. Lancer le programme

```bash
./itr-project <--proximity-tracker|-pt> [--lidar [step|continuous]]
```

La commande ci-dessus permet de démarrer le programme en mode *Figure imposée*.
L'argument `--proximity-tracker` ou `-pt` indique la sélection du mode. Cet
argument est obligatoire. En l'absence de cet arguement, une aide sera affichée.
L'argument `--lidar [step|continuous]` indique le mode de fonctionnement désiré
du LiDAR. En mode *step*, le LiDAR alterne des phases de déplacement et de
mesure. En mode *continuous*, le LiDAR ne s'arrête pas pendant une mesure.

```bash
./itr-project <--grandmother-footstep|-gf>
```

La commande ci-dessus permet de démarrer le programme en mode *Figure libre*.
L'argument `--grandmother-footstep` ou `-gf` indique la sélection du mode. Cet
argument est obligatoire. En l'absence de cet arguement, une aide sera affichée.

Nous déconseillons l'utilisation de l'option `--lidar` ou `-l` pour lancer la
*figure libre*. En effet, sans cette option, le programme utilisera la version
continue du LiDAR qui est plus propice au jeu.

## 3. Figure imposée

Utiliser une carte Raspberry Pi 3 pour créer un système qui détecte les objets
proches grâce à un capteur infra-rouge monté sur un moteur à courant continu.
Une caméra est là pour visionner ce que le capteur infra-rouge détecte.

## 4. Figure libre

Nous devions reprendre le système de la *figure imposée* composée d'un LiDAR et
d'une campéra pan-tilt. Nous avons décidé de créer une sorte de 1,2,3 soleil de
son nom anglais grandmother's footstep (d'où le nom de la class C++). En effet,
2 joueurs s'affrontent et doivent se rapprocher le plus possible du lidar à
l'avant du système. Mais le LiDAR est là pour les surveiller et indique à la
caméra de regarder celui qui bouge trop. Une fois sur sa cible, la caméra a pour
objectif de repérer de trop grand mouvements en face d'elle avec du traitment
opencv basique. Si le joueur surveillé par la caméra est pris en flagrant déli,
il lui sera indiqué dans le terminal de revenir au point de départ. La partie
s'arrête lorsque l'un des joueurs est arrivé assez proche du LiDAR.

## 5. Désinstallation

Dans le dossier du projet faire :

```bash
make mrproper
```

Dans le dossier **device**, faire :

```bash
make uninstall
make clean
```
