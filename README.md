# SafeGuard JNI - Laboratoire de Sécurité Native

Ce projet illustre l'implémentation de mécanismes défensifs en utilisant le NDK (Native Development Kit) d'Android et JNI (Java Native Interface). L'objectif est de déplacer des contrôles sensibles dans une couche native C++ pour accroître la résistance à l'analyse et au débogage.

## Fonctionnalités de Sécurité

L'application effectue un audit d'intégrité au démarrage via la bibliothèque native `liblab23.so` :

1.  **Détection de Debugger (ptrace)** : Utilise l'appel système `ptrace(PTRACE_TRACEME)` pour détecter si un débogueur (comme GDB ou LLDB) est déjà attaché au processus.
2.  **Scan de l'Espace Mémoire (`/proc/self/maps`)** : Inspecte les cartes mémoire du processus à la recherche de signatures liées à des frameworks d'instrumentation dynamique ou de hooking tels que :
    *   Frida
    *   Xposed
    *   Magisk / Zygisk
    *   Substrate

## Structure du Projet

*   **`app/src/main/cpp/native-lib.cpp`** : Contient l'implémentation C++ des contrôles de sécurité et des fonctions mathématiques protégées.
*   **`app/src/main/java/com/example/lab23/MainActivity.java`** : Point d'entrée de l'application qui coordonne l'audit de sécurité et adapte l'UI en conséquence.
*   **`app/src/main/res/values/strings.xml`** : Centralisation des messages utilisateur pour une meilleure maintenabilité.

## Fonctionnement Logic

L'application suit un flux strict :
1.  Chargement de la bibliothèque native au démarrage.
2.  Appel à `verifySystemIntegrity()`.
3.  Si un environnement suspect est détecté :
    *   L'UI affiche un avertissement critique.
    *   Les appels aux fonctions métier natives (ex: `computeFactorialNative`) sont bloqués logiciellement.
4.  Si l'environnement est sain :
    *   L'accès aux fonctionnalités natives est autorisé.
    *   Affichage d'un message sécurisé provenant du C++.

## Prérequis

*   Android Studio Jellyfish ou plus récent.
*   Android NDK & CMake installés.
*   Un appareil Android (physique de préférence pour tester l'absence de debug).

---

