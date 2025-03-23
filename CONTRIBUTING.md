# Contribuer à Space Miner 🚀  

Merci de votre intérêt pour l'amélioration de Space Miner !  

## 🛠️ Comment contribuer ?  

### 1️⃣ **Forker le projet**  
Allez sur [le dépôt GitHub](https://github.com/TON_NOM_UTILISATEUR/space_miner) et cliquez sur **Fork** en haut à droite.  

### 2️⃣ **Cloner le dépôt forké**  
Clonez votre copie du projet sur votre machine locale :  
```sh  
git clone https://github.com/VOTRE_UTILISATEUR/space_miner.git  
cd space_miner  
```  

### 3️⃣ **Créer une branche pour votre modification**  
Toujours créer une nouvelle branche pour chaque amélioration ou correction de bug :  
```sh  
git checkout -b feature-nom-de-la-fonctionnalite  
```  

### 4️⃣ **Coder et tester**  
Ajoutez/modifiez le code et testez bien vos changements.  

### 5️⃣ **Commiter vos modifications**  
Ajoutez les fichiers modifiés et faites un commit avec un message clair :  
```sh  
git add .  
git commit -m "Ajout de [fonctionnalité] : description courte"  
```  

### 6️⃣ **Pousser la branche sur GitHub**  
Envoyez votre branche sur GitHub :  
```sh  
git push origin feature-nom-de-la-fonctionnalite  
```  

### 7️⃣ **Créer une Pull Request (PR)**  
1. Allez sur le dépôt original **Space Miner** sur GitHub.  
2. Cliquez sur **Pull Requests** → **New Pull Request**.  
3. Sélectionnez **Compare across forks**.  
4. Choisissez votre branche et soumettez la PR.  

### 8️⃣ **Attendre la revue et les modifications**  
L’équipe examinera votre code et pourra demander des ajustements avant la fusion.  

---

## 📌 Notes à moi-même (Admin du projet)  

### ✅ Accepter une Pull Request  
1. Aller sur **Pull Requests**.  
2. Ouvrir la PR soumise et lire les changements.  
3. Vérifier que tout fonctionne et laisser des commentaires si nécessaire.  
4. Si tout est bon, accepter avec :  
   ```sh  
   git checkout main  
   git pull origin main  
   git merge feature-nom-de-la-fonctionnalite  
   git push origin main  
   ```  

### 🔄 Mettre à jour mon dépôt local  
Si quelqu'un d'autre a fait des modifications, je dois mettre mon dépôt local à jour :  
```sh  
git checkout main  
git pull origin main  
```  

### 🚀 Publier une nouvelle version  
1. Aller dans **Releases** sur GitHub.  
2. Cliquer sur **Draft a new release**.  
3. Donner un tag (ex: v1.0.1) et une description des changements.  
4. Publier la release.  

### 🛑 Annuler un commit local  
Si j’ai fait un mauvais commit et que je veux l’annuler :  
```sh  
git reset --soft HEAD~1  
```  
Si je veux annuler définitivement un commit :  
```sh  
git reset --hard HEAD~1  
```  

Merci pour votre contribution à Space Miner ! 🚀
