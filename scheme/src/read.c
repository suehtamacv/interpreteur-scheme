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
#include <limits.h>
#include <strings.h>
#include "read.h"
#include "primitives.h"
#include "lists.h"

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


object sfs_read( char *input, uint *h ) {
    /* Ces caracteres doivent etre ignores */
    while (input[*h] == ' ' || input[*h] == '\t') {
        (*h)++;
    };
    if (input[*h] == '\0' || input[*h] == EOF) {
        ERROR_MSG("Trying to read besides the end of the line");
        return NULL;
    }

    if ( input[*h] == '(' ) {
        (*h)++;
        /* Ces caracteres doivent etre ignores */
        while (input[*h] == ' ' || input[*h] == '\t') {
            (*h)++;
        };

        if ( input[*h] == ')' ) {
            (*h)++;
            return nil;
        } else {
            return sfs_read_pair( input, h );
        }
    } else if (input[*h] == '\'') {
        (*h)++;
        return make_pair(make_symbol("quote"), make_pair(sfs_read(input, h), nil));
    } else {
        return sfs_read_atom( input, h );
    }
}

object sfs_read_atom(char *input, uint *h) {
    object atom = NULL;
    /* Ces caracteres doivent etre ignores */
    while (input[*h] == ' ' || input[*h] == '\t') {
        (*h)++;
    };

    if (input[*h] == '#') {
        (*h)++;
        if (input[*h] == '\\') { /* C'est un char */
            atom = sfs_read_char(input, h);
        } else { /* C'est un boolean */
            atom = sfs_read_bool(input, h);
        }
    } else if (isdigit(input[*h]) || input[*h] == '-' ||
               input[*h] == '+') { /* C'est un nombre */
        atom = sfs_read_number(input, h);
    } else if (input[*h] == '"') {
        atom = sfs_read_string(input, h); /* C'est une chaine de caracteres */
    } else {
        atom = sfs_read_symbol(input, h); /* C'est un symbole */
    }

    return atom;
}

object sfs_read_pair(char *input, uint *h) {
    object pair = make_object(SFS_PAIR);
    DEBUG_MSG("Reading a pair");
    DEBUG_MSG("Reading the CAR");
    pair->val.pair.car = sfs_read(input, h);

    while (input[*h] == ' ' || input[*h] == '\t') {
        (*h)++;
    }

    /* On a trouve la fin de la liste : cdr vaut nil */
    if (input[*h] == ')') {
        (*h)++;
        DEBUG_MSG("Reading the CDR: nil");
        pair->val.pair.cdr = nil;
    } else { /* On continue a lire la liste : le cdr de ce pair vaut lui meme un pair */
        DEBUG_MSG("Reading the CDR");
        pair->val.pair.cdr = sfs_read_pair(input, h);
    }

    return pair;
}

/* TODO Corriger problème avec (#t #\u) */
object sfs_read_bool(char *input, uint *h) {
    string bool_name;
    size_t p;
    for (p = 0;
            /* Ce sont les chars que peuvent finir le bool */
            input[*h + p] != ' ' && input[*h + p] != '\n' &&
            input[*h + p] != '\t' && input[*h + p] != '"' &&
            input[*h + p] != ')' && input[*h + p] != '(' && input[*h + p] != '\0'
            /* Apres la diese, on a dans un boolean maximale d'un caractere. Donc si
             * on compte deux caracteres apres la diese (p < 3), on sait ou non s'il s'agit
             * d'un boolean correcte */
            && p < 3;
            p++) {
        if (input[*h] == ')' && *h != 0 && input[*h - 1] != '\\') {
            break;
        }
        bool_name[p] = input[*h + p];
    }
    bool_name[p] = '\0';

    while (input[*h] != ' ' && input[*h] != '\n' &&
            input[*h] != '\t' && input[*h] != '"' &&
            input[*h] != '(' && input[*h] != ')' && input[*h] != '\0') {
        (*h)++;
    } /* Faut continuer jusqu'a la fin de ce faux caractere */

    if (strcmp(bool_name, "t") == 0) {
        DEBUG_MSG("Reading a SFS_BOOLEAN: #t");
        return _true;
    } else if (strcmp(bool_name, "f") == 0) {
        DEBUG_MSG("Reading a SFS_BOOLEAN: #f");
        return _false;
    } else {
        WARNING_MSG("%s is not a valid boolean", bool_name);
        return NULL;
    }
}

object sfs_read_char(char *input, uint *h) {
    if (input[*h] != '\\') {
        WARNING_MSG("Invalid call to %s", __func__);
        return NULL;
    }

    object atom = make_object(SFS_CHARACTER);

    (*h)++;
    if (input[*h] == ' ' || input[*h] == '\n' || input[*h] == '\0') {
        WARNING_MSG("Invalid character found");
        return NULL;
    } else {
        string char_name;
        size_t p;
        for (p = 0;
                input[*h] != ' ' && input[*h] != '\n' &&
                input[*h] != '\t' && input[*h] != '\0' && p < 8;
                /* Ce sont les chars que peuvent finir le char */
                p++) {
            if ((input[*h] == ')' || input[*h] == '(') &&
                    p != 0 && input[*h - 1] != '\\') {
                break;
            }
            char_name[p] = input[*h];
            (*h)++;
        }
        char_name[p] = '\0';

        /* Certains caracteres ont une representation special */
        if (strcmp(char_name, "space") == 0) {
            DEBUG_MSG("Reading a SFS_CHARACTER: space");
            atom->val.character = ' ';
        } else if (strcmp(char_name, "newline") == 0) {
            DEBUG_MSG("Reading a SFS_CHARACTER: newline");
            atom->val.character = '\n';
        } else if (strlen(char_name) == 1) {
            DEBUG_MSG("Reading a SFS_CHARACTER: %c", char_name[0]);
            atom->val.character = char_name[0];
        } else {
            WARNING_MSG("%s is not a valid character", char_name);
            return NULL;
        }
    }

    return atom;
}

object sfs_read_number(char *input, uint *h) {
    DEBUG_MSG("Reading a SFS_NUMBER");

    /* Cherche un seul i ou j pour decider si c'est un nombre complexe */
    /* Un nombre complexe doit etre ecrit comme [+|-]a{+|-}b{i|j} */
    Bool isComplex = False;
    {
        Bool foundSignal = False;
        uint i;
        for (i = *h; ; ++i) {
            if (input[i] == ' ' || input[i] == '\n' ||
                    input[i] == '\0' || input[i] == '"' ||
                    input[i] == EOF) { /* C'est la fin du nombre */
                break;
            }
            if (input[i] == '+' || input[i] == '-') {
                foundSignal = True;

                /* Found an infinity */
                if (strncasecmp(input + i + 1, "inf", 3) == 0) {
                    if (input[i + 4] != ' ' && input[i + 4] != '\0' &&
                            input[i + 4] != ')' && input[i + 4] != '(' &&
                            input[i + 4] != '\n' && input[i + 4] != '"') {
                        WARNING_MSG("Invalid number found");
                        while (input[*h] != ' ' && input[*h] != '\n' &&
                                input[*h] != '\t' && input[*h] != '"' &&
                                input[*h] != '(' && input[*h] != ')' && input[*h] != '\0') {
                            (*h)++;
                        } /* Faut continuer jusqu'a la fin de ce faux infini */
                        return NULL;
                    }
                    (*h) += 4;
                    if (input[i] == '+') {
                        return make_number(NUM_PINFTY);
                    } else {
                        return make_number(NUM_MINFTY);
                    }
                }
            }
            if (input[i] == 'i' || input[i] == 'j') {
                if (input[i + 1] != ' ' && input[i + 1] != '\0' &&
                        input[i + 1] != ')' && input[i + 1] != '(' &&
                        input[i + 1] != '\n' && input[i + 1] != '"') {
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
    if (isComplex == False) {
        uint i;
        for (i = *h; ; ++i) {
            /* C'est la fin du nombre */
            if (input[i] == ' ' || input[i] == '\n' || input[i] == ')' || input[i] == '(' ||
                    input[i] == '\0' || input[i] == '"' || input[i] == EOF) {
                if (i == *h + 1 && (input[*h] == '+' || input[*h] == '-')) {
                    /* Un seul '+' ou '-' est un symbole, pas un nombre */
                    return sfs_read_symbol(input, h);
                }
                break;
            }
            if (input[i] == '.') { /* On a trouvé un point */
                isReal = True;
                break;
            }
        }
    }

    if (isComplex == True) {
        return sfs_read_complex_number(input, h);
    } else if (isReal == True) {
        return sfs_read_real_number(input, h);
    } else {
        return sfs_read_integer_number(input, h);
    }
}

object sfs_read_string(char *input, uint *h) {
    if (input[*h] != '"') {
        WARNING_MSG("Invalid call to %s", __func__);
        return NULL;
    }

    object atom = make_object(SFS_STRING);
    (*h)++;

    size_t p;
    for (p = 0; input[*h] != '"' && p < STRLEN - 1; (*h)++, p++) {
        /* On a trouve un \". Ces guillemets n'indiquent pas la fin de la chaine */
        if (input[*h] == '\\' && input[(*h) + 1] == '"') {
            atom->val.string[p] = '\\';
            atom->val.string[++p] = '"';
            (*h)++;
            continue;
        }

        atom->val.string[p] = input[*h];
    }

    if (p == STRLEN - 1) {
        WARNING_MSG("String larger than %d characters has been truncated.", STRLEN - 1);
        while (input[*h] != '"') {
            (*h)++;
        }
    }
    (*h)++;
    atom->val.string[p] = '\0';

    DEBUG_MSG("Reading a SFS_STRING: %s", atom->val.string);

    return atom;
}

object sfs_read_symbol(char *input, uint *h) {
    object atom = make_object(SFS_SYMBOL);

    size_t p;
    for (p = 0;
            input[*h] != ' ' && input[*h] != '\n' &&
            input[*h] != '\t' && input[*h] != '\0' &&
            input[*h] != '(' && input[*h] != ')' &&
            p < STRLEN - 1; /* Ce sont les chars que peuvent finir une symbole */
            (*h)++, p++) {
        atom->val.symbol[p] = input[*h];
    }

    if (p == STRLEN - 1) {
        WARNING_MSG("Symbol name larger than %d characters has been truncated.",
                    STRLEN - 1);
        while (input[*h] != ' ' && input[*h] != '\n' &&
                input[*h] != '\t' && input[*h] != '\0' && input[*h] != '(' &&
                input[*h] != ')') {
            (*h)++;
        }
    }
    atom->val.symbol[p] = '\0';

    DEBUG_MSG("Reading a SFS_SYMBOL: %s", atom->val.symbol);

    return atom;
}

object sfs_read_integer_number(char *input, uint *h) {
    short k = 1;
    object atom = make_number(0);

    if (input[*h] == '+') {
        atom->val.number->numtype = NUM_INTEGER;
        (*h)++;
    } else if (input[*h] == '-') {
        atom->val.number->numtype = NUM_INTEGER;
        (*h)++;
        k = -1;
    } else {
        atom->val.number->numtype = NUM_UINTEGER;
    }

    long int cur_num = 0;
    do { /* Ca lit la partie entiere chiffre par chiffre */
        if (!isdigit(input[*h])) {
            WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                        input[*h]);
            return NULL;
        }
        cur_num = 10 * cur_num + (input[*h] - '0'); /* Char - '0' => l'entier */
        (*h)++;

        if (cur_num > INT_MAX) { /* Test si un overflow a eu lieu. */
            if (k == 1) {
                atom->val.number->numtype = NUM_PINFTY;
            } else {
                atom->val.number->numtype = NUM_MINFTY;
            }
        }
    } while (input[*h] != ' ' &&
             input[*h] != '\n' &&
             input[*h] != '\0' &&
             input[*h] != ')' &&
             input[*h] != '(' &&
             input[*h] != '"' &&
             input[*h] != EOF);

    /* Considere que le nombre peut etre negatif */
    atom->val.number->val.integer = cur_num * k;

    if (atom->val.number->numtype == NUM_INTEGER) {
        DEBUG_MSG("Reading a NUM_INTEGER: %d", atom->val.number->val.integer);
    } else if (atom->val.number->numtype == NUM_UINTEGER) {
        DEBUG_MSG("Reading a NUM_UINTEGER: %d", atom->val.number->val.integer);
    }
    return atom;
}

object sfs_read_complex_number(char *input, uint *h) {
    object atom = make_number(NUM_COMPLEX);

    uint real_p = 0;
    uint imag_p = 0;
    string realpart, imagpart;

    short is_negative = 0;
    if (input[*h] == '+' || input[*h] == '-') {
        is_negative = (input[*h] == '-' ? -1 : 1);
        (*h)++;
    }
    sscanf(input + *h, "%[^+-]%[^ij]", realpart, imagpart);

    atom->val.number->val.complex->real = sfs_read_number(realpart, &real_p);
    atom->val.number->val.complex->imag = sfs_read_number(imagpart, &imag_p);
    if (is_negative == -1) {
        atom->val.number->val.complex->real = prim_minus(cons(
                atom->val.number->val.complex->real, nil));
    }

    *h += real_p + imag_p + 1;

    DEBUG_MSG("Reading a NUM_COMPLEX");
    return atom;
}

object sfs_read_real_number(char *input, uint *h) {
    object atom = make_number(0);
    short k = 1;
    atom->val.number->numtype = NUM_REAL;

    if (input[*h] == '-') {
        k = -1; /* Une constante pour considerer les nombres negatifs */
        (*h)++;
    }

    long double cur_number = 0;
    do { /* Ca lit la partie entiere chiffre par chiffre */
        if (!isdigit(input[*h])) {
            WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                        input[*h]);
            return NULL;
        }
        cur_number  = 10 * cur_number  + (input[*h] - '0');
        (*h)++;
    } while (input[*h] != '.');

    (*h)++; /* Apres le point */

    double frac_constant = 0.1;
    do { /* Ca lit la partie fractionnaire chiffre par chiffre */
        if (!isdigit(input[*h])) {
            WARNING_MSG("Invalid number found. \"%c\" is not a valid character in a number",
                        input[*h]);
            return NULL;
        }
        cur_number  = cur_number  + frac_constant * (input[*h] - '0');
        frac_constant /= 10.0;
        (*h)++;
    } while (input[*h] != ' ' &&
             input[*h] != '\n' &&
             input[*h] != '\0' &&
             input[*h] != '(' &&
             input[*h] != ')' &&
             input[*h] != '"' &&
             input[*h] != EOF);

    /* Considere que le nombre peut etre negatif */
    atom->val.number->val.real = cur_number * k;

    DEBUG_MSG("Reading a NUM_REAL: %Lf", atom->val.number->val.real);
    return atom;
}
