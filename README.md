# minishell

*Projet réalisé dans le cadre du cursus 42 par msingh et akaddo.*

Un shell minimal écrit en C, inspiré du comportement de `bash` en mode interactif : lecture de ligne avec historique, guillemets, variables d'environnement, pipes, redirections, here-documents, builtins et signaux.

## Compilation et lancement

Prérequis : `cc`, `make` et la bibliothèque `readline` (`libreadline-dev`).

```bash
make
./minishell
```

```bash
minishell$ ls -l | grep ".c" | wc -l
minishell$ echo "Bonjour $USER" > fichier.txt
minishell$ cat < fichier.txt >> copie.txt
minishell$ cat << FIN
> ligne avec $HOME
> FIN
minishell$ export A=42 && echo $A
minishell$ echo $?
```

## Fonctionnalités

- **Guillemets** : `'...'` bloque toute interprétation, `"..."` n'autorise que l'expansion de `$`.
- **Expansion** : `$VAR`, `$?` (code de sortie de la dernière commande), concaténation avec du texte entre guillemets (`$USER"x"`, `a$HOME'b'`).
- **Redirections** : `<`, `>`, `>>`, `<<` (here-doc, avec ou sans expansion selon que le délimiteur est entre guillemets).
- **Pipes** : `cmd1 | cmd2 | … | cmdN`.
- **Recherche des exécutables** via `PATH`, ou par chemin relatif ou absolu.
- **Builtins** : `echo` (avec `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- **Signaux** : `Ctrl-C` affiche un nouveau prompt, `Ctrl-D` quitte, `Ctrl-\` est ignoré au prompt. Les processus fils retrouvent le comportement par défaut.
- **Erreurs de syntaxe** détectées avant exécution : pipe en début ou en fin, `||`, redirection sans cible, guillemet non fermé. Le code de sortie vaut alors 2.

## Architecture

La ligne lue suit une chaîne de traitement en plusieurs étapes :

```
readline ─► tokenisation ─► vérification syntaxe ─► expansion ─► construction du pipeline ─► heredocs ─► exécution
```

| Dossier / fichier | Rôle |
|---|---|
| `src/main.c` | boucle principale : prompt, historique, enchaînement des étapes, libération |
| `src/token/` | **tokenisation** : découpe la ligne en `WORD`, `PIPE`, `REDIR_IN`, `REDIR_OUT`, `REDIR_APPEND`, `HEREDOC` |
| `src/parsing/ft_check_syntax.c` | validation de la suite de tokens |
| `src/parsing/expand*.c` | **expansion** des variables et de `$?` |
| `src/ft_contruct/` | transforme les tokens en `t_pipeline` : un `t_command` (argv + liste de redirections) par commande |
| `src/execution/` | heredocs, redirections, recherche dans `PATH`, `fork`/`pipe`/`dup2`/`execve`, attente des fils |
| `src/builtins/` | les 7 builtins |
| `src/signals.c` | gestionnaires de signaux ; une seule variable globale contient le numéro du signal reçu |
| `src/utils/` | libération de la mémoire et outils |
| `src/libft/` | libft du projet |

### Le suivi des guillemets

Les guillemets sont retirés dès la tokenisation, mais chaque token garde une chaîne `qstate` de même longueur que sa valeur. Pour chaque caractère, elle indique s'il venait d'un passage sans guillemets (`0`), entre `'...'` (`S`) ou entre `"..."` (`D`). Ensuite :

- l'expansion ignore les `$` marqués `S` ;
- un nom de variable s'arrête à la frontière d'un changement de `qstate`. Ainsi `$USER"x"` donne bien la valeur de `USER` suivie de `x`, et non la variable `USERx` ;
- un délimiteur de here-doc qui contenait des guillemets désactive l'expansion à l'intérieur du here-doc.

### Exécution

- Une commande seule qui est un builtin s'exécute **dans le processus du shell**, sans fork. C'est indispensable pour que `cd`, `export`, `unset` et `exit` modifient vraiment l'état du shell.
- Pour un pipeline, chaque commande est lancée dans un fils. Ses redirections s'appliquent après le branchement des pipes, donc une redirection explicite l'emporte sur le pipe. Le shell attend tous les fils et garde le code du dernier. Une commande tuée par un signal donne `128 + n`.
- Codes d'erreur alignés sur bash : `127` pour une commande introuvable, `126` pour une permission refusée, `1` pour une redirection impossible.

## Tests

Une trentaine de scénarios ont été comparés à `bash` (sortie et code de retour) : expansions mêlées de guillemets, pipes, redirections multiples, here-docs avec et sans expansion, erreurs de syntaxe, builtins et `exit` avec arguments invalides. Valgrind (avec une suppression pour `readline`) ne signale aucune fuite dans le shell ni dans ses fils.

Ce projet ne vise pas la parité complète avec bash : `;`, `&&`, `||`, les wildcards, `cd -` et le découpage en mots des variables non quotées ne sont pas gérés.

## Ressources

- [Bash Manual (GNU)](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/)
- [Minishell: Building a Mini Bash (Medium)](https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218)
- [Mes conseils si je devais recommencer minishell (Medium)](https://medium.com/@mostafa.omrane/mes-conseils-si-je-devais-recommencer-minishell-a9783c51ba1b)
- `man bash`, `man 3 readline`

### Usage de l'IA

Des outils d'IA ont servi d'aide pendant le projet : comprendre certaines fonctions C, se faire guider sur la structure d'un minishell, relecture, corrections grammaticales, traduction et structure du README. Ils n'ont pas servi à copier-coller directement les modules de parsing ou d'exécution sans relecture.
