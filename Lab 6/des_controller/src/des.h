#ifndef DES_H_
#define DES_H_
#include <iostream>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/dispatch.h>

enum Inputs {
	ls, rs, _ws, lo, ro, lc, rc, gru, grl, gll, glu, ex, invalid
};

enum States {
	SS,
	LS,
	GLU,
	LO,
	LWS,
	LC,
	GLL,
	IGRU,
	IRO,
	IRC,
	RS,
	GRU,
	RO,
	RWS,
	RC,
	GRL,
	IGLU,
	ILO,
	ILC,
	ER,
	EXIT
};

const char *toDisplay[13] = { "Person scanned ID, ID = ",
		"Person scanned ID, ID = ", "Person weighed, Weight = ",
		"Person opened left door", "Person opened Right door",
		"Left door closed (automatically)", "Right door closed (automatically)",
		"Right door unlocked by Guard", "Right door locked by Guard",
		"Left door locked by Guard", "Left door unlocked by Guard",
		"EXITING...", "INVALID" };

States transitionTable[][19] = {
/*STATE     SS   LS   GLU  LO   LWS  LC   GLL  IGRU IRO  IRC  RS   GRU  RO   RWS  RC   GRL  IGLU ILO  ILC*/
/*ls */{ LS, ER, ER, ER, ER, ER, LS, ER, ER, ER, ER, ER, ER, ER, ER, LS, ER, ER,
		ER },
/*rs */{ RS, ER, ER, ER, ER, ER, RS, ER, ER, ER, ER, ER, ER, ER, ER, RS, ER, ER,
		ER },
/*ws */{ ER, ER, ER, LWS, ER, ER, ER, ER, ER, ER, ER, ER, RWS, ER, ER, ER, ER,
		ER, ER },
/*lo */{ ER, ER, LO, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ILO,
		ER, ER },
/*ro */{ ER, ER, ER, ER, ER, ER, ER, IRO, ER, ER, ER, RO, ER, ER, ER, ER, ER,
		ER, ER },
/*lc */{ ER, ER, ER, ER, LC, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER,
		ILC, ER },
/*rc */{ ER, ER, ER, ER, ER, ER, ER, ER, IRC, ER, ER, ER, ER, RC, ER, ER, ER,
		ER, ER },
/*gru*/{ ER, ER, ER, ER, ER, ER, IGRU, ER, ER, ER, GRU, ER, ER, ER, ER, ER, ER,
		ER, ER },
/*grl*/{ ER, ER, ER, ER, ER, ER, ER, ER, ER, GRL, ER, ER, ER, ER, GRL, ER, ER,
		ER, ER },
/*gll*/{ ER, ER, ER, ER, ER, GLL, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER,
		ER, GLL },
/*glu*/{ ER, GLU, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, IGLU, ER,
		ER, ER },
/*ex */{ EXIT, EXIT, EXIT, EXIT, EXIT, EXIT, EXIT, EXIT, EXIT, EXIT, EXIT, EXIT,
		EXIT, EXIT, EXIT, EXIT, EXIT, EXIT, EXIT } };

typedef struct _pulse msg_header_t;

struct message {
	msg_header_t hdr;
	enum States state;
	enum Inputs input;
	int id;
	int weight;
};

#endif
