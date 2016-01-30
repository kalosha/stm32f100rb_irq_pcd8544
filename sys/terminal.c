/*
 * File: terminal.c
 * Author: George Eliozov
 * Date: 2013-11-26
 */

#include "stm32f10x.h"
//#include "parser.h"
#include "terminal.h"

static term_rxhook_t _term_hook = NULL;
static prs_cmd_t _term_commands[TERM_MAX_COMMANDS];
static prs_config_t _prs_config;
static char _term_buf[TERM_BUF_LEN];
static int _term_buf_pos = 0;

void term_putchar(char c) {
	if (c == '\n'){
		TERM_PUTC_USART1('\r');
	}
	TERM_PUTC_USART1(c);
}

void term_puts(char *s) {
	while (*s){
		term_putchar(*s++);
	}
}

int term_cmd_list(char *s) {
	int i;
	term_puts("Listing commands available:\n");
	for (i = 0; i < TERM_MAX_COMMANDS; i++) {
		if (_term_commands[i].cmd != NULL && _term_commands[i].callback != NULL) {
			term_puts("\ - ");
			term_puts(_term_commands[i].cmd);
			term_puts("\t\t");
			term_puts(_term_commands[i].help);
			term_puts("\n");
		}
	}
	return 1;
}

int term_register_cmd(char *name, prs_cmd_callback_t callback, char *help) {
	return prs_reg_cmd(name, callback, &_prs_config, help);
}

void term_put_cmd_prefix() {
	term_puts("\ncmd: ");
}

void term_init(char *welcome) {
	_prs_config.cmdlist = _term_commands;
	_prs_config.listsize = TERM_MAX_COMMANDS;
	_prs_config.validchars = prs_vc_not_whitespace;
	prs_reg_cmd("help", term_cmd_list, &_prs_config, "\tPrint help List");
	term_puts(welcome);
	term_put_cmd_prefix();
}

void term_process_received() {
	int i;
	char c;

	i = TERM_GETC_USART1();
	if (i < 0){
		return;
	}
	c = (char) i;

	if (_term_hook) {
		if (_term_hook(c)) {
			_term_hook = 0;
			term_put_cmd_prefix();
		}
		return;
	}

	if (c == '\r'){
		c = '\n';
	}
	if (c == '\n') {
		term_putchar(c);
		_term_buf[_term_buf_pos] = 0;
		if (_term_buf_pos > 0) {
			//xprintf("cmd: %s\n",_prs_term_buf);

			i = prs_lookup(_term_buf, &_prs_config);
			if (!_term_hook) {
				/*
				 if(i< 0)
				 xprintf("ERROR, code: %d\n",i);
				 else
				 xprintf("OK, code: %d\n",i);
				 */
				term_put_cmd_prefix();
			}
		} else {
			term_put_cmd_prefix();
		}
		_term_buf_pos = 0;
		return;
	} else if (c == '\t'){
		c = ' ';
	}
	if ((c >= (uint8_t) ' ' && c <= (uint8_t) '\x7e')
			|| c >= (uint8_t) '\xa0') {
		if (_term_buf_pos >= (TERM_BUF_LEN - 1)){
			term_putchar('\a');
		} else {
			_term_buf[_term_buf_pos++] = c;
			term_putchar(c); // Echo
		}
		return;
	}

	switch (c) {
	case 'c' & 0x1f: // CTRL+Z (EOF)
		return;

	case '\b':
	case '\x7f': // del
		if (_term_buf_pos > 0) {
			term_putchar('\b');
			term_putchar(' ');
			term_putchar('\b');
			_term_buf_pos--;
		}
		break;

	case 'r' & 0x1f:
		term_putchar('\r');
		for (i = 0; i < _term_buf_pos; i++) {
			term_putchar(_term_buf[i]);
		}
		break;

	case 'u' & 0x1f:
		term_puts("[chr: u&0x1f]");
		break;

	case 'w' & 0x1f:
		term_puts("[chr: w&0x1f]");

		break;
	}
}
