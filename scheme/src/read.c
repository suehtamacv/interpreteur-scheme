/**
 * @file read.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:31:05 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#include <stdio.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "read.h"

void flip( uint *i ) {

    if (*i == FALSE) {
        *i = TRUE;
    } else {
        *i = FALSE;
    }
}

/*
 * @fn char* first_usefull_char(char* line)
 *
 * @brief retourne un pointeur sur le premier caractere utile dans line
 * ou NULL si line ne contient que des espaces et des commentaires
 */
char* first_usefull_char(char* line) {

    int i = 0;
    if (line == NULL) {
        return NULL;
    }
    i = 0;
    /* on saute les espaces */
    while(line[i] != '\0' && isspace(line[i])) {
        i++;
    }
    /* si fin de ligne => ligne inutile */
    if(line[i] == '\0') {
        return NULL;
    }
    /* si premier caractere non espace est ';' => ligne inutile */
    if(line[i] == ';') {
        return NULL;
    }
    return line + i; /* ligne utile */
}

/**
 * @fn uint  sfs_get_sexpr( char *input, FILE *fp )
 *
 * @brief extrait la prochaine S-Expression dans le flux fp et la stocke dans input
 * (meme si elle est repartie sur plusieurs lignes)
 * @param fp (FILE *) flux d'entree (ouvert en ecriture, mode texte)
 * @param input (char *) chaine allouee de taille BIGSTRING, dans laquelle la S-Expression sera stockée
 *
 * @return S_OK si une S-Expression apparemment valide a ete trouvee
 * @return S_KO si on n'a pas trouve de S-Expression valide
 * @return S_END si fin de fichier atteinte sans avoir lu de caractere utile.
 *
 * sfs_get_sexpr commence par lire une ligne dans fp,
 * puis compte le nombre de parentheses ouvrantes et fermantes sur la ligne.
 * Les parentheses dans des chaines et les caracteres Scheme #\( et #\)
 * ne sont pas comptes.
 *
 * Si le compte devient zéro et que
 *        - la ligne est fini, la fonction retourne S_OK
 * 		  - la ligne n'est pas fini la fonction retourne S_KO
 *
 * S'il y a plus de parentheses fermantes qu'ouvrantes,
 * la fonction retourne S_KO.
 * Les commentaires et espaces qui figurent a la fin de chaque ligne
 * sont remplacés par un espace.
 * Les espaces qui figurent a la fin de la S-Expression (y compris '\n')
 * sont supprimés.
 *
 * Attention : cette fonction refuse les S-Expression multiple sur une seule ligne. Ainsi :
 *    a b c
 *    (qqchose) (autrechose)
 *    (qqchose) 78
 * seront considereees comme des erreurs
 * et la fonction retournera S_KO
 *
 * @pre fp doit etre prealablement ouvert en lecture
 * @pre input doit etre prealablement alloue en memoire, de taille BIGSTRING
 */

typedef enum {
    NOTHING,        /* rien n'a ete trouve encore.. */
    STRING_ATOME,   /* la premiere trouvee dans la ligne semble etre un atome */
    BASIC_ATOME,    /* la premiere trouvee dans la ligne semble etre d'une chaine */
    S_EXPR_PARENTHESIS, /* la premiere trouvee dans la ligne semble etre une expression parenthesee */
    FINISHED        /* on a trouve une S-Expr bien formee */
} EXPRESSION_TYPE_T;

uint  sfs_get_sexpr( char *input, FILE *fp ) {
    int       parlevel = 0;
    uint      in_string = FALSE;
    uint      s = 0;
    char      k[BIGSTRING];
    char      *ret = NULL;
    char      *chunk = NULL;
    uint      i = 0;
    string    sfs_prompt;

    EXPRESSION_TYPE_T typeOfExpressionFound = NOTHING;

    parlevel = 0;
    memset( input, '\0', BIGSTRING );

    do {
        ret = NULL;
        chunk = NULL;

        /* si en mode interactif*/
        if ( stdin == fp ) {
            uint nspaces = 2 * parlevel;

            init_string( sfs_prompt );

            /* le prompt indique le niveau de parenthese
               et decale la prochaine entrée en fonction
               de ce niveau (un peu à la python)*/
            sprintf( sfs_prompt, "SFS:%u > ", parlevel );

            for ( i = 0; i < nspaces; i++ ) {
                sfs_prompt[strlen(sfs_prompt)] = ' ';
            }

            /* si sur plusieurs lignes, le \n équivaut à un espace*/
            if (nspaces > 0) {
                input[strlen(input) + 1] = '\0';
                input[strlen(input)] = ' ';
            }

            /*saisie de la prochaine ligne à ajouter dans l'input*/
            chunk = readline( sfs_prompt );
        }
        /*si en mode fichier*/
        else {
            chunk = k;
            memset( chunk, '\0', BIGSTRING );
            ret = fgets( chunk, BIGSTRING, fp );

            if ( NULL == ret ) {
                /* fin de fichier...*/
                if ( parlevel != 0 ) {
                    WARNING_MSG( "Parse error: missing ')'" );
                    return S_KO;
                }
                return S_END;
            }

            if (strlen(chunk) == BIGSTRING - 1
                    && chunk[BIGSTRING - 1] != '\n'
                    && !feof(fp)) {
                WARNING_MSG( "Too long line for this interpreter!" );
                return S_KO;
            }
        }

        /* si la ligne est inutile
        	=> on va directement à la prochaine iteration */
        if (first_usefull_char(chunk) == NULL) {
            continue;
        }


        s = strlen( chunk );

        if ( s > 0 ) {
            if (strlen(input) + s > BIGSTRING - 1 ) {
                WARNING_MSG( "Too long a S-expression for this interpreter!" );
                return S_KO;
            }

            for ( i = 0; i < strlen(chunk); i++ ) {
                /* si la fin de la ligne chunk est inutile,
                   on ajoute un espace dans input et on sort de la boucle*/
                if ( in_string == FALSE && first_usefull_char(chunk + i) == NULL ) {
                    chunk[i] = '\0';
                    input[strlen(input)] = ' ';
                    break;
                }


                switch(chunk[i]) {
                case '(':
                    if (in_string == FALSE
                            && ! ( i > 1 && chunk[i - 1] == '\\' && chunk[i - 2] == '#' ) ) {
                        parlevel++;
                        typeOfExpressionFound = S_EXPR_PARENTHESIS;
                    }
                    break;
                case ')':
                    if ( in_string == FALSE
                            && ! ( i > 1 && chunk[i - 1] == '\\' && chunk[i - 2] == '#' ) ) {
                        parlevel--;
                        if (parlevel == 0 ) {
                            typeOfExpressionFound = FINISHED;
                        }
                        if ( parlevel < 0 ) {
                            WARNING_MSG( "Parse error : cannot start with ')'" );
                            return S_KO;
                        }
                    }
                    break;
                case '"':
                    if ( i < 2 || chunk[i - 1] != '\\' ) {
                        if ( in_string == FALSE ) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                WARNING_MSG("Parse error: invalid string after atom : '%s'", chunk + i);
                                return S_KO;
                            }
                            in_string = TRUE;
                            if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                                typeOfExpressionFound = STRING_ATOME;
                            }
                        } else {
                            in_string = FALSE;
                            if(typeOfExpressionFound == STRING_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        }
                    }
                    break;
                default:
                    if(in_string == FALSE) {
                        if(isspace(chunk[i])) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        } else if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                            typeOfExpressionFound = BASIC_ATOME;
                        }
                    }
                    break;
                }


                if(typeOfExpressionFound == FINISHED) {
                    char *first_useful = first_usefull_char(chunk + i + 1);
                    if( first_useful != NULL) {
                        if(*first_useful == ')' ) {
                            WARNING_MSG( "Parse error: too many closing parenthesis')'" );
                        } else {
                            WARNING_MSG("Parse error: invalid trailing chars after S-Expr : '%s'",
                                        chunk + i);
                        }
                        return S_KO;
                    }
                }

                /* recopie char par char*/
                input[strlen(input)] = chunk[i];
            }
            if(in_string == TRUE) {
                WARNING_MSG( "Parse error: non terminated string on line %s", chunk );
                return S_KO;
            }
        }


        if ( parlevel > 0 && fp != stdin ) {
            if ( feof( fp ) ) {
                WARNING_MSG( "Parse error: missing ')'" );
                return S_KO;
            }

            if (input[strlen(input) - 1] == '\n') {
                input[strlen(input) - 1] = ' ';
            }
        }
    } while ( parlevel > 0 );

    /* Suppression des espaces restant a la fin de l'expression, notamment le dernier '\n' */
    while (isspace(input[strlen(input) - 1])) {
        input[strlen(input) - 1] = '\0';
    }

    if(stdin == fp) {
        add_history( input );
    }
    return S_OK;
}


object sfs_read( char *input, uint *here ) {

    if ( input[*here] == '(' ) {
        if ( input[(*here) + 1] == ')' ) {
            *here += 2;
            return nil;
        } else {
            (*here)++;
            return sfs_read_pair( input, here );
        }
    } else {
        return sfs_read_atom( input, here );
    }
}

object sfs_read_atom(char *input, uint *here) {
    object atom = NULL;

    while (input[*here] == ' ' || input[*here] == '\t') {
        printf("%c", input[*here]);
        (*here)++;
    };

    if (input[*here] == '#') {
        (*here)++;
        if (input[*here] == '\\') { /* C'est un char */
            atom = sfs_read_char(input, here);
        } else { /* C'est un boolean */
            atom = sfs_read_bool(input, here);
        }
    } else if (isdigit(input[*here]) || input[*here] == '-' ||
               input[*here] == '+') { /* C'est un nombre */
        atom = sfs_read_number(input, here);
    } else if (input[*here] == '"') {
        atom = sfs_read_string(input, here);
    } else if (input[*here] == '(') {
        atom = sfs_read_pair(input, here);
    } else {
        atom = sfs_read_symbol(input, here);
    }

    return atom;
}

object sfs_read_pair(char *input, uint *here) {

    object pair = make_object(SFS_PAIR);
    pair->val.pair.car = sfs_read(input, here);
    if (input[*here] == ')') pair->val.pair.cdr = NULL;
    else
    pair->val.pair.cdr = sfs_read_pair(input, here);
    return pair;
}

object sfs_read_bool(char *input, uint *here) {
    object atom = make_object(SFS_BOOLEAN);

    switch (input[*here]) { /* Les booleens valides sont seulement #t et #f */
    case 't':
        atom->val.boolean = True;
        break;

    case 'f':
        atom->val.boolean = False;
        break;

    default:
        WARNING_MSG("#%c is not a correct boolean value", input[*here]);
        return NULL;
        break;
    }

    (*here)++;
    return atom;
}

object sfs_read_char(char *input, uint *here) {
    if (input[*here] != '\\') {
        WARNING_MSG("Invalid call to %s", __func__);
        return NULL;
    }

    object atom = make_object(SFS_CHARACTER);

    (*here)++;
    if (input[*here] == ' ' || input[*here] == '\n' || input[*here] == '\0') {
        WARNING_MSG("Invalid character found");
        return NULL;
    } else {
        string char_name;
        size_t p;
        for (p = 0;
                input[*here] != ' ' && input[*here] != '\n' &&
                input[*here] != '\t' && input[*here] != '\0' &&
                p < 8; /* Ce sont les chars que peuvent finir le char */
                p++) {
            char_name[p] = input[*here + p];
        }
        char_name[p] = '\0';

        if (strcmp(char_name, "space") == 0) {
            atom->val.character = ' ';
        } else if (strcmp(char_name, "newline") == 0) {
            atom->val.character = '\n';
        } else if (strlen(char_name) == 1) {
            atom->val.character = input[*here];
        } else {
            WARNING_MSG("%s is not a valid character", char_name);
            return NULL;
        }
    }

    (*here)++;

    return atom;
}

object sfs_read_number(char *input, uint *here) {
    short k = 1;
    if (input[*here] == '-') {
        k = -1; /* Une constante pour considerer les nombres negatifs */
    }

    object atom = make_object(SFS_NUMBER);

    /* Cherche un seul i ou j pour decider si c'est un nombre complexe */
    /* Un nombre complexe doit etre ecrit comme a{+|-}bj */
    Bool isComplex = False;
    {
        Bool foundSignal = False;
        uint i;
        for (i = *here; ; ++i) {
            if (input[i] == ' ' || input[i] == '\n' ||
                    input[i] == '\0' || input[i] == EOF) { /* C'est la fin du nombre */
                break;
            }
            if (input[i] == '+' || input[i] == '-') {
                foundSignal = True;
            }
            if (input[i] == 'i' || input[i] == 'j') {
                if (input[i + 1] != ' ' && input[i + 1] != '\0' &&
                        input[i + 1] != ')' && input[i + 1] != '\n') {
                    WARNING_MSG("Invalid complex number found: %c should be at the end of the number",
                                input[i]);
                    return NULL;
                }
                if (!foundSignal) {
                    WARNING_MSG("Invalid complex number found: it should be written as {+|-}A{+|-}Bj");
                    return NULL;
                }
                isComplex = True;
            }
        }
    }

    /* Cherche un point pour decider si c'est un reel ou un entier */
    Bool isReal = False;
    if (!isComplex) {
        uint i;
        for (i = *here; ; ++i) {
            if (input[i] == ' ' || input[i] == '\n' ||
                    input[i] == '\0' || input[i] == EOF) { /* C'est la fin du nombre */
                if (i == *here + 1) {
                    return sfs_read_symbol(input,
                                           here); /* Un seul '+' ou '-' est un symbole, pas un nombre */
                }
                break;
            }
            if (input[i] == '.') { /* On a trouvé un point */
                isReal = True;
                break;
            }
        }
    }

    if (isComplex) {
        atom->val.number.numtype = NUM_COMPLEX;
        double real = 0, imag = 0;

        if (input[*here] == '-') {
            k = -1;
            (*here)++;
        } else if (input[*here] == '+') {
            k = 1;
            (*here)++;
        }

        do { /* Ca lit la partie entiere de la partie reele chiffre par chiffre */
            if (!isdigit(input[*here])) {
                WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                            input[*here]);
                return NULL;
            }
            real  = 10 * real  + (input[*here] - '0');
            (*here)++;
        } while (input[*here] != '.' && input[*here] != '+' && input[*here] != '-');

        if (input[*here] == '.') {
            (*here)++;
            double frac_constant = 0.1;
            do { /* Ca lit la partie fractionnaire de la partie reele chiffre par chiffre */
                if (!isdigit(input[*here])) {
                    WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                                input[*here]);
                    return NULL;
                }
                real = real  + frac_constant * (input[*here] - '0');
                frac_constant /= 10.0;
                (*here)++;
            } while (input[*here] != '+' &&
                     input[*here] != '-');
        }

        real *= k;
        if (input[*here] == '+') {
            k = 1;
        } else {
            k = -1;
        }

        (*here)++; /* Apres le plus ou le moins */

        do { /* Ca lit la partie entiere de la partie reele chiffre par chiffre */
            if (!isdigit(input[*here])) {
                WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                            input[*here]);
                return NULL;
            }
            imag = 10 * imag + (input[*here] - '0');
            (*here)++;
        } while (input[*here] != '.' && input[*here] != ' ' && input[*here] != ')' &&
                 input[*here] != '\n' && input[*here] != '\0' && input[*here] != 'j' &&
                 input[*here] != 'i');

        if (input[*here] == '.') {
            (*here)++;
            double frac_constant = 0.1;
            do { /* Ca lit la partie fractionnaire de la partie reele chiffre par chiffre */
                if (!isdigit(input[*here])) {
                    WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                                input[*here]);
                    return NULL;
                }
                imag = imag + frac_constant * (input[*here] - '0');
                frac_constant /= 10.0;
                (*here)++;
            } while (input[*here] != ' ' && input[*here] != ')' && input[*here] != '\n' &&
                     input[*here] != '\0' && input[*here] != 'j' && input[*here] != 'i');
        }

        imag *= k;

        atom->val.number.val.complex.real = real;
        atom->val.number.val.complex.imag = imag;

    } else if (!isReal) {
        if (input[*here] == '+' || input[*here] == '-') {
            atom->val.number.numtype = NUM_INTEGER;
            (*here)++;
        } else {
            atom->val.number.numtype = NUM_UINTEGER;
        }

        int cur_num = 0;
        do { /* Ca lit la partie entiere chiffre par chiffre */
            if (!isdigit(input[*here])) {
                WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                            input[*here]);
                return NULL;
            }
            cur_num = 10 * cur_num + (input[*here] - '0'); /* Char - '0' => l'entier */
            (*here)++;
        } while (input[*here] != ' ' &&
                 input[*here] != '\n' &&
                 input[*here] != '\0' &&
                 input[*here] != EOF);

        /* Considere que le nombre peut etre negatif */
        atom->val.number.val.integer = cur_num * k;
    } else {
        atom->val.number.numtype = NUM_REAL;

        double cur_number = 0;
        do { /* Ca lit la partie entiere chiffre par chiffre */
            if (!isdigit(input[*here])) {
                WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                            input[*here]);
                return NULL;
            }
            cur_number  = 10 * cur_number  + (input[*here] - '0');
            (*here)++;
        } while (input[*here] != '.');

        (*here)++; /* Apres le point */

        double frac_constant = 0.1;
        do { /* Ca lit la partie fractionnaire chiffre par chiffre */
            if (!isdigit(input[*here])) {
                WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                            input[*here]);
                return NULL;
            }
            cur_number  = cur_number  + frac_constant * (input[*here] - '0');
            frac_constant /= 10.0;
            (*here)++;
        } while (input[*here] != ' ' &&
                 input[*here] != '\n' &&
                 input[*here] != '\0' &&
                 input[*here] != EOF);

        /* Considere que le nombre peut etre negatif */
        atom->val.number.val.real = cur_number * k;
    }

    return atom;
}

object sfs_read_string(char *input, uint *here) {
    if (input[*here] != '"') {
        WARNING_MSG("Invalid call to %s", __func__);
        return NULL;
    }

    object atom = make_object(SFS_STRING);
    (*here)++;

    size_t p;
    for (p = 0; input[*here] != '"' && p < STRLEN - 1; (*here)++, p++) {
        if (input[*here] == '\\' && input[(*here) + 1] == '"') { /* C'est un \" */
            atom->val.string[p] = '\\';
            atom->val.string[++p] = '"';
            (*here)++;
            continue;
        }

        atom->val.string[p] = input[*here];
    }

    if (p == STRLEN - 1) {
        WARNING_MSG("String larger than %d characters has been truncated.", STRLEN - 1);
        while (input[*here] != '"') {
            (*here)++;
        }
    }
    atom->val.string[p] = '\0';

    return atom;
}

object sfs_read_symbol(char *input, uint *here) {
    object atom = make_object(SFS_SYMBOL);

    size_t p;
    for (p = 0;
            input[*here] != ' ' && input[*here] != '\n' &&
            input[*here] != '\t' && input[*here] != '\0' &&
            p < STRLEN - 1; /* Ce sont les chars que peuvent finir une symbole */
            (*here)++, p++) {
        atom->val.symbol[p] = input[*here];
    }

    if (p == STRLEN - 1) {
        WARNING_MSG("Symbol name larger than %d characters has been truncated.",
                    STRLEN - 1);
        while (input[*here] != ' ' && input[*here] != '\n' &&
                input[*here] != '\t' && input[*here] != '\0') {
            (*here)++;
        }
    }
    atom->val.symbol[p] = '\0';

    return atom;
}
