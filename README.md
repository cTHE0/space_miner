# Void Reign: The Minerals War 🚀  
Un jeu en C avec SDL2 dans lequel vous pourrez creer, developper et defendre une civilisation inter-galactique ! (visuel disponible sur Sigma : https://www.figma.com/design/dyd2ouuvGYLKhz5VeyWADk/Untitled?node-id=0-1&p=f)

## Installation  
1. Installez SDL2 et SDL2_gfx :  
   ```sh  
   sudo apt install libsdl2-dev libsdl2-gfx-dev  
   ```  
2. Clonez le projet :  
   ```sh  
   git clone https://github.com/cTHE0/space_miner.git  
   cd space_miner  
   ```  
3. Compilez et lancez :  
   ```sh  
   make  
   ./void_reign  
   ```  

## Comment jouer  
Vous dirigez une civilisation minière inter-galactique. Le but : extraire des minerais, développer vos planètes et survivre aux attaques de pirates.

- **Extraire** : sur une planète, construisez des *mines* (elles remplissent les *réservoirs* du minerai correspondant).
- **Transporter** : chaque fusée fait la navette entre une *base* et une *cible*. Ouvrez sa fiche pour choisir les minerais à charger/décharger.
- **Économie** : chaque construction/amélioration coûte des ressources, prélevées sur le réservoir de la planète. L'or, le joanium et le voidor forment un véritable arbre technologique (il faut de l'or pour le joanium, etc.).
- **L'usine** fabrique de nouvelles fusées (fer + or).
- **La guerre** : des pirates apparaissent et attaquent vos transporteurs. Construisez des **tours de défense** sur vos planètes : elles tirent automatiquement sur les ennemis à portée.

## Commandes  
| Touche / Action | Effet |
|---|---|
| Clic gauche | Sélectionner une planète / une fusée, valider un bouton |
| Glisser (clic maintenu) | Déplacer la caméra |
| Molette / `O` / `P` | Zoomer / dézoomer |
| Flèches | Déplacer la caméra |
| `M` | Couper / remettre le son |
| `Échap` | Fermer une fenêtre / menu pause |
| Roue crantée (haut-droite) | Réglages audio |

## Contributions  
Les contributions sont les bienvenues mais je vous previens : vous ne serez pas paye et Adam gardera tout l'argent ! Forkez le repo et faites une PR.  