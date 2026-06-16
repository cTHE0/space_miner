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
Vous dirigez une civilisation minière inter-galactique. Le but : extraire des minerais, développer vos planètes, bâtir un empire commercial et survivre aux attaques de pirates pour faire le meilleur score.

- **Extraire** : sur une planète, construisez des *mines* (elles remplissent les *réservoirs* du minerai correspondant).
- **Transporter** : chaque fusée fait la navette entre une *base* et une *cible*. Ouvrez sa fiche pour choisir les minerais à charger/décharger.
- **Économie** : chaque construction/amélioration coûte des ressources, prélevées sur le réservoir de la planète. L'or, le joanium et le voidor forment un véritable arbre technologique (il faut de l'or pour le joanium, etc.).
- **L'usine** fabrique de nouvelles fusées (fer + or).
- **La guerre** : des pirates apparaissent et attaquent vos transporteurs. Construisez des **tours de défense** sur vos planètes : elles tirent automatiquement sur les ennemis à portée.
- **Centre de commandement** (touche `C` ou onglet latéral droit) : vendez vos minerais contre des **crédits** et achetez des **améliorations globales** permanentes (rendement minier, puissance des tours, vitesse et blindage des fusées).
- **Crédits & primes** : chaque pirate détruit rapporte une prime. Vendez vos surplus de minerais pour financer vos améliorations.
- **Événements galactiques** : des événements aléatoires bouleversent la partie (boom minier, marché en folie, déferlante de pirates, prime triplée).
- **Objectifs** : suivez la mission affichée en haut à gauche pour gagner des crédits bonus et progresser.
- **Vaisseaux défenseurs** : recrutez-les au Centre de commandement (crédits) ; ils chassent automatiquement les pirates. Arbitrez entre améliorations et flotte militaire.
- **Minimap** : vue d'ensemble de la galaxie en bas à droite ; cliquez pour vous y déplacer.
- **But** : atteindre **25 000 points** (victoire) tout en évitant de perdre toute votre flotte (défaite).

## Difficulté
Le jeu monte en pression : ne le laissez pas tourner sans rien faire.

- **Rareté** : vous ne démarrez qu'avec 2 colonies aux stocks limités. Tout le reste est à conquérir — mais s'étendre attire les pirates.
- **Niveau de Menace** (HUD) : croît avec le temps **et** votre réussite. Plus il est haut, plus les pirates sont nombreux, rapides, résistants et fréquents.
- **Raids de colonies** : dès la menace 2, des pillards volent les minerais de vos planètes et endommagent vos bâtiments. Les **tours de défense** et les **boucliers planétaires** (recherche) sont vos parades.
- **Cuirassés pirates (boss)** : dès la menace 4, d'énormes vaisseaux très résistants apparaissent — gros danger, mais grosse prime.
- **Game over** : si vous perdez tous vos transporteurs sans pouvoir en reconstruire, votre empire s'effondre.

## Commandes  
| Touche / Action | Effet |
|---|---|
| Clic gauche | Sélectionner une planète / une fusée, valider un bouton |
| Glisser (clic maintenu) | Déplacer la caméra |
| Molette / `O` / `P` | Zoomer / dézoomer |
| Flèches | Déplacer la caméra |
| `C` | Ouvrir / fermer le Centre de commandement |
| `M` | Couper / remettre le son |
| `Échap` | Fermer une fenêtre / menu pause |
| Clic sur la minimap | Recentrer la caméra |
| Roue crantée (haut-droite) | Réglages audio |

## Contributions  
Les contributions sont les bienvenues mais je vous previens : vous ne serez pas paye et Adam gardera tout l'argent ! Forkez le repo et faites une PR.  