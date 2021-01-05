# README - LIDAR project - ROB5 Temps réel

Auteurs : [F. Beroujon](https://github.com/Flosilver) & [F. Cormée](https://github.com/Florian-Cormee)

---

Source d'inspiration [dépôt grove](https://github.com/Seeed-Studio/grove.py) plus particulièrement [adc.py](https://github.com/Seeed-Studio/grove.py/blob/master/grove/adc.py) pour la lecture de registre du sensor infra red

---

## Figure imposée

### Concept

Utiliser une carte raspberry pi3 pour créer un mini-système qui détecte les objects proches grâce à un capteur infra-rouge monté sur un moteur à courant continu. Une caméra est là pour visionner ce que le capteur infra-rouge détecte.

### Lancer le programme

Se placer dans un premier temps dans le dossier **device** (`cd device/`).

> make
>
> make install

Revenir dans le dossier du projet et compiler le projet (`cd ..`)!

> make
>
> ./itr-project --proximity-tracker or -pt --lidar [step|continuous] or -l [step|continuous]

### Désinstaller proprement

Dans le dossier du projet faire:

> make mrproper

Dans le dossier **device**, faire:

> make uninstall
>
> make clean
## Figure libre

### Concept

Nous devions reprendre le système de la figure imposée copomsée d'un LiDAR et d'une campéra pan-tilt. Nous avons décidé de créer une sorte de 1,2,3 soleil de son nom anglais grandmother's footstep (d'où le nom de la class C++). En effet, 2 jouers s'affrontent et doivent se rapprocher le plus possible du lidar à l'avant du système. Mais le LiDAR est là pour les surveillez et indique à la caméra de regarder celui qui bouge trop. une fois sur sa cible, la caméra a pour objectif de repérer de trop grand mouvmeent en face d'elle avec du traitment opencv basic. Si le joueur surveillé par la caméra est pris en flagrant déli, il lui sera indiqué dans le terminal de revenir au point de départ. La partie s'arrête lorsque l'un des joueurs est arrivé assez proche du LiDAR.

### Lancer le programme

Ici aussi il fait utiliser le compteur incrémental de **device**.

> make
>
> make install

Revenir dans le dossier du projet et compiler le projet (`cd ..`)!

> make
>
> ./itr-project --grandmother-footstep or -gf

Nous déconseillons l'utilisation de l'option `--lidar` ou `-l` pour lancer le `GMFTGame`. En effet, sans cette option, le programme utilisera la version continue du LiDAR qui est plus propice au jeu.

### Désinstaller proprement

Idem que pour la figure imposée.