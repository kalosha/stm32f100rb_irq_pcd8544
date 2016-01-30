/*
 * parser.h
 *
 *  Created on: May 17, 2011
 *      Author: nerozero
 */

#ifndef PARSER_H_
#define PARSER_H_

#ifndef NULL
#define NULL		((void *)0)
#endif

typedef int (*prs_cmd_callback_t)(char *s);
typedef int (*prs_rxhook_t)(char c);

typedef struct {
	prs_cmd_callback_t callback;
	char *cmd;
	char *help;
} prs_cmd_t;

typedef enum{
	prs_vc_a_z = 0x01,
	prs_vc_A_Z = 0x02,
	prs_vc_0_9 = 0x04,
	prs_vc_not_whitespace = 0x10,
}prs_validchars_e;

typedef struct{
	prs_cmd_t *cmdlist;
	int	listsize;
	prs_validchars_e validchars;
}prs_config_t;

/* Clears and initialize command list*/
void prs_init(prs_config_t *cfg);

/*
* Lookup for the appropriate command in the list and execute it,
* returns callback result value or -1 if not found
*/
int prs_lookup(char *s, prs_config_t *cfg);

/*
 * Registers name and callback in cmd list
 * returns -1 if existed or -2 if there is no empty slot found
 * note, be careful, command name stored in "name" used as pointer
 */
int prs_reg_cmd(char *name, prs_cmd_callback_t callback, prs_config_t *cfg, char *help);

/*
 * Clears all commands in the list
 */
void prs_clear_cmd(prs_config_t *cfg);


/* Argument parsing */
int prs_args_i(char **s, int *args, int alng, char delim);
int prs_args_l(char **s, long *args, int alng, char delim);
int prs_args_f(char **s, double *args, int alng, char delim);


#endif /* PARSER_H_ */
