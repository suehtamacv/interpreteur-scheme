/**
 * @file repl.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:13:02 2012
 * @brief Main REPL for SFS.
 *
 * Main REPL for SFS.
 */



#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "object.h"
#include "read.h"
#include "eval.h"
#include "forms.h"
#include "print.h"
#include "primitives.h"

/* mode d'interaction avec l'interpreteur (exemple)*/
typedef enum {INTERACTIF, SCRIPT} inter_mode;

void usage_error( char *command ) {
    fprintf(stderr,
            "Usage: %s [file.scm]\n   If no file is given, executes in Shell mode.\n",
            command);
}

/* Singletons */
object nil;
object _void;
object _true;
object _false;
object master_environment;
object plus_inf;
object minus_inf;
object NaN;

int main (int argc, char *argv[]);

void init_interpreter (void) {
    /* Crée les singletons */
    INFO_MSG("Creating the singletons");
    nil = make_nil();
    _void = make_nil();
    _true = make_true();
    _false = make_false();
    plus_inf = make_number(NUM_PINFTY);
    minus_inf = make_number(NUM_MINFTY);
    NaN = make_number(NUM_UNDEF);

    /* Crée l'environment top-level */
    INFO_MSG("Creating top-level environment");
    master_environment = form_interaction_environment(nil, nil);
    master_environment = create_env_layer(master_environment);
}

int main (int argc, char *argv[]) {
    char       input[BIGSTRING];
    uint       here = 0;
    object     output = NULL;
    object     sexpr = NULL;
    inter_mode mode;
    /* le flux dans lequel les commande seront lues : stdin (mode shell) ou un fichier */
    FILE *     fp = NULL;

    /* exemples d'utilisation des macros du fichier notify.h */
    /* WARNING_MSG : sera toujours affiche */
    /* WARNING_MSG("Un message WARNING_MSG !"); */

    /* macro INFO_MSG : uniquement si compil avec -DVERBOSE. Cf Makefile*/
    /* INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]); */

    /* macro DEBUG_MSG : uniquement si compil avec -DDEBUG (ie : compil avec make debug). Cf Makefile */
    /* DEBUG_MSG("Un message DEBUG_MSG !"); */

    /* La ligne suivante provoquerait l'affichage du message
       puis la sortie du programme avec un code erreur non nul (EXIT_FAILURE) */
    /* ERROR_MSG("Erreur. Arret du programme"); */

    if (argc > 2) {
        usage_error(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        usage_error(argv[0]);
        exit(EXIT_SUCCESS);
    }

    init_interpreter();

    /*par defaut : mode shell interactif */
    fp = stdin;
    mode = INTERACTIF;
    if(argc == 2) {
        /* mode fichier de commandes */
        fp = fopen( argv[1], "r" );
        if ( fp == NULL ) {
            perror( "fopen" );
            exit( EXIT_FAILURE );
        }
        mode = SCRIPT;
    }

    while (1) {
        input[0] = '\0';
        here = 0;
        output = NULL;
        sexpr = NULL;
        uint Sexpr_err;

        Sexpr_err = sfs_get_sexpr( input, fp );

        if ( S_OK != Sexpr_err) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                if (Sexpr_err == S_END) {
                    /* Cas fin de fichier script */
                    return EXIT_SUCCESS;
                }
                /* Cas S-Expression mal formee dans le fichier script */
                ERROR_MSG("Malformed S-expression --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
            continue;
        }

        if ( 0 == strlen( input ) ) {
            continue;
        }

        here  = 0;
        sexpr = sfs_read( input, &here );
        if ( NULL == sexpr ) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("Invalid S-expression --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
            continue ;
        }

        output = sfs_eval(sexpr, master_environment);
        if(NULL == output) {
            /* si fichier alors on sort*/
            if (mode == SCRIPT) {
                fclose( fp );
                /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
                ERROR_MSG("Error while evaluating input --- Aborts");
            }
            /*sinon on rend la main à l'utilisateur*/
            continue ;
        }

        if (output == _void) {
            continue;
        }

        printf( "==> " );
        sfs_print( output );
        printf( "\n" );
    }

    if (mode == SCRIPT) {
        fclose( fp );
    }
    return EXIT_SUCCESS;
}
