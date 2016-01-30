/*
 * parser.c
 *
 *  Created on: May 17, 2011
 *      Author: nerozero
 */
#include "stm32f10x.h"
#include "utils.h"
#include "parser.h"


int _prs_cmd_compare(char *cmd, char *s,prs_config_t *cfg);

void prs_init(prs_config_t *cfg)
{
	prs_clear_cmd(cfg);
}

uint8_t _prs_validate_chr(char c,prs_config_t *cfg)
{
	uint8_t result = 0;
	if(c==0) return 0; // do not accept "\0"
	if(cfg->validchars & prs_vc_0_9)
		result |= (c>='0' && c<='9');
	if(cfg->validchars & prs_vc_A_Z)
		result |= (c>='A' && c<='Z');
	if(cfg->validchars & prs_vc_a_z)
		result |= (c>='a' && c<='z');
	if(cfg->validchars & prs_vc_not_whitespace)
		result |= (c!=' ' && c!='\t');
	return result;
}

/*
 * Compares command string cmd and string s, if match, returns offset of the rest arguments
 */
int _prs_cmd_compare(char *cmd, char *s,prs_config_t *cfg)
{
	char *_cmd, *_s;
	_cmd = cmd; _s = s;

	while(1)
	{
		if(*_cmd==0 && !_prs_validate_chr(*_s,cfg)) // reach the end of the cmd
		{
			return *_s == 0?_s - s: _s - s +1 ;  // ok, return arguments offset
		}
		if(*_s==0 || *_cmd==0) break;// Reach the end of the line;
		if(!(_prs_validate_chr(*_cmd,cfg) && _prs_validate_chr(*_s,cfg))) break; // not valid char
		if(*_s != *_cmd) break; // check if chars is the same
		_s++; _cmd++;
	}
	return -1;
}

/*
 * Lookup for the appropriate command in the list and execute it,
 * returns callback result value or -1 if not found
 */
int prs_lookup(char *s, prs_config_t *cfg)
{
	int i,p;
	for(i=0;i<cfg->listsize;i++)
	{
		if(cfg->cmdlist[i].callback==NULL) continue;
		if(cfg->cmdlist[i].cmd==NULL) continue;
		if((p = _prs_cmd_compare(cfg->cmdlist[i].cmd,s,cfg))>0)
		{
			return cfg->cmdlist[i].callback((char *)(s+p));
		}
	}
	return -1;
}

/*
 * Registers name and callback in cmd list
 * returns -1 if existed or -2 if there is no empty slot found
 * note, command name "name" used as pointer
 */
int prs_reg_cmd(char *name, prs_cmd_callback_t callback, prs_config_t *cfg, char *help)
{
	int i;
	char *_s;
	_s =name;
	while(*_s!=0)
	{
		if(!_prs_validate_chr(*_s++,cfg))
			return -2;// not valid command name
	}

	// find out if the command already existed
	for(i=0;i<cfg->listsize;i++)
	{
		if(cfg->cmdlist[i].callback!=NULL) continue;
		if(cfg->cmdlist[i].cmd!= NULL) continue;
		if(_prs_cmd_compare(cfg->cmdlist[i].cmd,name,cfg)>0) return -1;
	}

	// find first empty slot
	for(i=0;i<cfg->listsize;i++)
	{
		if(cfg->cmdlist[i].callback!=NULL) continue;
		if(cfg->cmdlist[i].cmd!=NULL) continue;
		cfg->cmdlist[i].cmd = name;
		cfg->cmdlist[i].help = help;
		cfg->cmdlist[i].callback = callback;
		return i;
	}
	return -2;
}

void prs_clear_cmd(prs_config_t *cfg)
{
	int i;
	for(i=0; i<cfg->listsize; i++)
	{
		cfg->cmdlist[i].callback = NULL;
		cfg->cmdlist[i].cmd = NULL;
		cfg->cmdlist[i].help = NULL;
	}
}

/* Argument parsers */


/*
 * return list *s offset after last arg, or -1 if fail
 */
int prs_args_i(char **s, int *args, int alng, char delim)
{
	int i = 0;
	int result = -1;
	while(i<alng && **s)
	{
		if(!( result = xatoi(s,&args[i++])) )
			break;
		while(**s==' ')(*s)++; // left trim
		if (**s!=delim)
		{// next argument delimiter not found
			result = (i==alng)?1:0;
			break;
		}
		(*s)++;
	}
	return result;
}

int prs_args_l(char **s, long *args, int alng, char delim)
{
	int i = 0;
	int result = -1;
	while(i<alng && **s)
	{
		if(!( result = xatoi(s,&args[i++])) )
			break;
		while(**s==' ')(*s)++; // left trim
		if (**s!=delim)
		{// next argument delimiter not found
			result = (i==alng)?1:0;
			break;
		}
		(*s)++;
	}
	return result;
}

/*
 * return list *s offset after last arg, or -1 if fail
 */
int prs_args_f(char **s, double *args, int alng, char delim)
{
	int i = 0;
	int result = -1;
	while(i<alng && **s)
	{
		if(!( result = xatof(s,&args[i++])) )
			break;
		while(**s==' ')(*s)++; // left trim
		if (**s!=delim)
		{// next argument delimiter not found
			result = (i==alng)?1:0;
			break;
		}
		(*s)++;
	}
	return result;
}

