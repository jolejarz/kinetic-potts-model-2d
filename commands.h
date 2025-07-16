/**************************************************************
***************************************************************
*** 
***   commands.h
***
***   Jason Olejarz
***   2011-01-25
***
***   This file sets up the command arrays.
***
***************************************************************
**************************************************************/

enum {type_double, type_int, type_short_int, type_string, type_unsigned_long_int};

double arg_double[32];
int arg_int[32];
short int arg_short_int[32];
char arg_string[32][32];
unsigned long int arg_unsigned_long_int[32];

// data
void cmd_add();
void cmd_bins();
void cmd_cnt();
void cmd_dd();
void cmd_dindex();
void cmd_disp();
void cmd_dt();
void cmd_flt();
void cmd_index();
void cmd_minmax();
void cmd_moveflips();
void cmd_movelat();
void cmd_receive();
void cmd_sd();
void cmd_send();
void cmd_tag();
void cmd_testinit();
// file
void cmd_load();
void cmd_loadtext();
void cmd_load2d();
void cmd_numclust();
void cmd_save();
void cmd_savetext();
void cmd_save2d();
void cmd_sizefile();
void cmd_xy();
void cmd_xyrange();
void cmd_xyyerror();
// list
void cmd_dl();
void cmd_sl();
// loop
void cmd_lp();
void cmd_lpseed();
void cmd_randdiff();
void cmd_randq();
void cmd_randq3();
void cmd_randq4();
void cmd_randq5();
void cmd_randq6();
void cmd_randq7();
void cmd_randq8();
void cmd_randq9();
void cmd_tsc();
// plot
void cmd_anrt();
void cmd_anrt2();
void cmd_anstep();
void cmd_brplot();
void cmd_clspins();
void cmd_spins();
// run
void cmd_run();
// save
void cmd_forcesave();
void cmd_fa();
void cmd_fbr();
void cmd_fbr2();
void cmd_fcl();
void cmd_fe();
void cmd_fm();
void cmd_fmabs();
void cmd_fnum0();
void cmd_fnum1();
void cmd_fnum2();
void cmd_fnum3();
void cmd_fnum4();
void cmd_fnum5();
void cmd_fnum6();
void cmd_fnum7();
void cmd_fnum8();
void cmd_fperc();
void cmd_fs();
void cmd_ft();
void cmd_fTvert();
void cmd_fTvert2();
void cmd_svseed();
// set
void cmd_block();
void cmd_cpcur();
void cmd_cpinit();
void cmd_rnglcg();
void cmd_rngmt();
void cmd_seed();
void cmd_set();
void cmd_settsc();
// step
void cmd_check();
void cmd_check2();
void cmd_energy();
void cmd_energy2();
void cmd_energyavg();
void cmd_Tvertavg();
void cmd_Tvert2avg();
void cmd_flips();
void cmd_flips2();
void cmd_flipsb2();
void cmd_force();
void cmd_force2();
void cmd_mag();
void cmd_steplat();
void cmd_Tvert();
void cmd_Tvert2();
void cmd_time();
void cmd_time2();
void cmd_trigs();
void cmd_trigt();
void cmd_trigtonce();

int main();

char *cmd_name[]=
{
	/* data */
	"add",
	"bins",
	"cnt",
	"dd",
	"dindex",
	"disp",
	"dt",
	"flt",
	"index",
	"minmax",
	"moveflips",
	"movelat",
	"receive",
	"sd",
	"send",
	"tag",
	"testinit",
	/* file */
	"load",
	"loadtext",
	"load2d",
	"numclust",
	"save",
	"savetext",
	"save2d",
	"sizefile",
	"xy",
	"xyrange",
	"xyyerror",
	/* list */
	"dl",
	"sl",
	/* loop */
	"lp",
	"lpseed",
	"randdiff",
	"randq",
	"randq3",
	"randq4",
	"randq5",
	"randq6",
	"randq7",
	"randq8",
	"randq9",
	"tsc",
	/* plot */
	"anrt",
	"anrt2",
	"anstep",
	"brplot",
	"clspins",
	"spins",
	/* run */
	"run",
	/* save */
	"forcesave",
	"fa",
	"fbr",
	"fbr2",
	"fcl",
	"fe",
	"fm",
	"fmabs",
	"fnum0",
	"fnum1",
	"fnum2",
	"fnum3",
	"fnum4",
	"fnum5",
	"fnum6",
	"fnum7",
	"fnum8",
	"fperc",
	"fs",
	"ft",
	"fTvert",
	"fTvert2",
	"svseed",
	/* set */
	"block",
	"cpcur",
	"cpinit",
	"rnglcg",
	"rngmt",
	"seed",
	"set",
	"settsc",
	/* step */
	"check",
	"check2",
	"energy",
	"energy2",
	"energyavg",
	"Tvertavg",
	"Tvert2avg",
	"flips",
	"flips2",
	"flipsb2",
	"force",
	"force2",
	"mag",
	"steplat",
	"Tvert",
	"Tvert2",
	"time",
	"time2",
	"trigs",
	"trigt",
	"trigtonce",
	/* exit / quit */
	"exit",
	"quit"
};
void (*functions[])()=
{
	/* data */
	cmd_add,
	cmd_bins,
	cmd_cnt,
	cmd_dd,
	cmd_dindex,
	cmd_disp,
	cmd_dt,
	cmd_flt,
	cmd_index,
	cmd_minmax,
	cmd_moveflips,
	cmd_movelat,
	cmd_receive,
	cmd_sd,
	cmd_send,
	cmd_tag,
	cmd_testinit,
	/* file */
	cmd_load,
	cmd_loadtext,
	cmd_load2d,
	cmd_numclust,
	cmd_save,
	cmd_savetext,
	cmd_save2d,
	cmd_sizefile,
	cmd_xy,
	cmd_xyrange,
	cmd_xyyerror,
	/* list */
	cmd_dl,
	cmd_sl,
	/* loop */
	cmd_lp,
	cmd_lpseed,
	cmd_randdiff,
	cmd_randq,
	cmd_randq3,
	cmd_randq4,
	cmd_randq5,
	cmd_randq6,
	cmd_randq7,
	cmd_randq8,
	cmd_randq9,
	cmd_tsc,
	/* plot */
	cmd_anrt,
	cmd_anrt2,
	cmd_anstep,
	cmd_brplot,
	cmd_clspins,
	cmd_spins,
	/* run */
	cmd_run,
	/* save */
	cmd_forcesave,
	cmd_fa,
	cmd_fbr,
	cmd_fbr2,
	cmd_fcl,
	cmd_fe,
	cmd_fm,
	cmd_fmabs,
	cmd_fnum0,
	cmd_fnum1,
	cmd_fnum2,
	cmd_fnum3,
	cmd_fnum4,
	cmd_fnum5,
	cmd_fnum6,
	cmd_fnum7,
	cmd_fnum8,
	cmd_fperc,
	cmd_fs,
	cmd_ft,
	cmd_fTvert,
	cmd_fTvert2,
	cmd_svseed,
	/* set */
	cmd_block,
	cmd_cpcur,
	cmd_cpinit,
	cmd_rnglcg,
	cmd_rngmt,
	cmd_seed,
	cmd_set,
	cmd_settsc,
	/* step */
	cmd_check,
	cmd_check2,
	cmd_energy,
	cmd_energy2,
	cmd_energyavg,
	cmd_Tvertavg,
	cmd_Tvert2avg,
	cmd_flips,
	cmd_flips2,
	cmd_flipsb2,
	cmd_force,
	cmd_force2,
	cmd_mag,
	cmd_steplat,
	cmd_Tvert,
	cmd_Tvert2,
	cmd_time,
	cmd_time2,
	cmd_trigs,
	cmd_trigt,
	cmd_trigtonce,
	/* exit / quit */
	0,
	0
};
int num_args[]=
{
	/* data */
		/* add */	3,
		/* bins */	2,
		/* cnt */	3,
		/* dd */	1,
		/* dindex */	0,
		/* disp */	3,
		/* dt */	0,
		/* flt */	3,
		/* index */	2,
		/* minmax */	1,
		/* moveflips */	4,
		/* movelat */	3,
		/* receive */	0,
		/* sd */	0,
		/* send */	0,
		/* tag */	2,
		/* testinit */	1,
	/* file */
		/* load */	2,
		/* loadtext */	2,
		/* load2d */	2,
		/* numclust */	6,
		/* save */	2,
		/* savetext */	2,
		/* save2d */	2,
		/* sizefile */	4,
		/* xy */	3,
		/* xyrange */	5,
		/* xyyerror */	4,
	/* list */
		/* dl */	2,
		/* sl */	1,
	/* loop */
		/* lp */	3,
		/* lpseed */	5,
		/* randdiff */	2,
		/* randq */	2,
		/* randq3 */	6,
		/* randq4 */	8,
		/* randq5 */	10,
		/* randq6 */	12,
		/* randq7 */	14,
		/* randq8 */	16,
		/* randq9 */	18,
		/* tsc */	2,
	/* plot */
		/* anrt */	14,
		/* anrt2 */	11,
		/* anstep */	10,
		/* brplot */	7,
		/* clspins */	4,
		/* spins */	5,
	/* run */
		/* run */	1,
	/* save */
		/* forcesave */ 12,
		/* fa */	4,
		/* fbr */	5,
		/* fbr2 */	12,
		/* fcl */	8,
		/* fe */	4,
		/* fm */	4,
		/* fmabs */	4,
		/* fnum0 */	4,
		/* fnum1 */	4,
		/* fnum2 */	4,
		/* fnum3 */	4,
		/* fnum4 */	4,
		/* fnum5 */	4,
		/* fnum6 */	4,
		/* fnum7 */	4,
		/* fnum8 */	4,
		/* fperc */	14,
		/* fs */	4,
		/* ft */	4,
		/* fTvert */	4,
		/* fTvert2 */	4,
		/* svseed */	5,
	/* set */
		/* block */	6,
		/* cpcur */	1,
		/* cpinit */	1,
		/* rnglcg */	1,
		/* rngmt */	1,
		/* seed */	3,
		/* set */	7,
		/* settsc */	1,
	/* step */
		/* check */	3,
		/* check2 */	3,
		/* energy */	4,
		/* energy2 */	5,
		/* energyavg */	7,
		/* Tvertavg */	7,
		/* Tvert2avg */	7,
		/* flips */	4,
		/* flips2 */	5,
		/* flipsb2 */	5,
		/* force */	4,
		/* force2 */	12,
		/* mag */	4,
		/* steplat */	6,
		/* Tvert */	4,
		/* Tvert2 */	4,
		/* time */	4,
		/* time2 */	5,
		/* trigs */	5,
		/* trigt */	5,
		/* trigtonce */	5,
	/* exit / quit */
		/* exit */	0,
		/* quit */	0
};
int arg_types[][32]=
{
	/* data */
		/* add */	{type_short_int, type_short_int, type_double},
		/* bins */	{type_short_int, type_int},
		/* cnt */	{type_short_int, type_double, type_double},
		/* dd */	{type_short_int},
		/* dindex */	{},
		/* disp */	{type_short_int, type_int, type_int},
		/* dt */	{},
		/* flt */	{type_short_int, type_double, type_double},
		/* index */	{type_short_int, type_int},
		/* minmax */	{type_short_int},
		/* moveflips */	{type_short_int, type_short_int, type_int, type_int},
		/* movelat */	{type_short_int, type_int, type_int},
		/* receive */	{},
		/* sd */	{},
		/* send */	{},
		/* tag */	{type_short_int, type_short_int},
		/* testinit */	{type_short_int},
	/* file */
		/* load */	{type_short_int, type_string},
		/* loadtext */	{type_short_int, type_string},
		/* load2d */	{type_short_int, type_string},
		/* numclust */	{type_short_int, type_short_int, type_short_int, type_int, type_double, type_string},
		/* save */	{type_short_int, type_string},
		/* savetext */	{type_short_int, type_string},
		/* save2d */	{type_short_int, type_string},
		/* sizefile */	{type_short_int, type_int, type_double, type_string},
		/* xy */	{type_short_int, type_short_int, type_string},
		/* xyrange */	{type_short_int, type_short_int, type_string, type_int, type_int},
		/* xyyerror */	{type_short_int, type_int, type_double, type_string},
	/* list */
		/* dl */	{type_short_int, type_short_int},
		/* sl */	{type_short_int},
	/* loop */
		/* lp */	{type_short_int, type_int, type_short_int},
		/* lpseed */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* randdiff */	{type_short_int, type_short_int},
		/* randq */	{type_short_int, type_short_int},
		/* randq3 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int},
		/* randq4 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_int},
		/* randq5 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int},
		/* randq6 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int},
		/* randq7 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int},
		/* randq8 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int},
		/* randq9 */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int, type_int},
		/* tsc */	{type_short_int, type_short_int},
	/* plot */
		/* anrt */	{type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_double, type_double, type_double, type_double, type_short_int, type_string, type_string, type_string, type_string},
		/* anrt2 */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_double, type_double, type_short_int, type_string, type_string},
		/* anstep */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_string, type_string},
		/* brplot */	{type_short_int, type_string, type_string, type_string, type_string, type_string, type_string},
		/* clspins */	{type_short_int, type_int, type_string, type_string},
		/* spins */	{type_short_int, type_short_int, type_int, type_string, type_string},
	/* run */
		/* run */	{type_short_int},
	/* save */
		/* forcesave */	{type_short_int, type_short_int, type_double, type_double, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* fa */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fbr */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* fbr2 */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int, type_short_int, type_int},
		/* fcl */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int, type_int},
		/* fe */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fm */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fmabs */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum0 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum1 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum2 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum3 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum4 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum5 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum6 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum7 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fnum8 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fperc */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int, type_short_int, type_short_int, type_short_int, type_int},
		/* fs */	{type_short_int, type_short_int, type_short_int, type_int},
		/* ft */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fTvert */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fTvert2 */	{type_short_int, type_short_int, type_short_int, type_int},
		/* svseed */    {type_short_int, type_short_int, type_short_int, type_short_int, type_int},
	/* set */
		/* block */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int},
		/* cpcur */	{type_short_int},
		/* cpinit */	{type_short_int},
		/* rnglcg */	{type_short_int},
		/* rngmt */	{type_short_int},
		/* seed */	{type_short_int, type_double, type_double},
		/* set */	{type_short_int, type_short_int, type_int, type_int, type_unsigned_long_int, type_double, type_short_int},
		/* settsc */	{type_short_int},
	/* step */
		/* check */	{type_short_int, type_short_int, type_double},
		/* check2 */	{type_short_int, type_short_int, type_double},
		/* energy */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* energy2 */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int, type_unsigned_long_int},
		/* energyavg */	{type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_double, type_double},
		/* Tvertavg */	{type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_double, type_double},
		/* Tvert2avg */	{type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_double, type_double},
		/* flips */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int},
		/* flips2 */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int, type_unsigned_long_int},
		/* flipsb2 */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int, type_unsigned_long_int},
		/* force */	{type_short_int, type_short_int, type_double, type_double},
		/* force2 */	{type_short_int, type_short_int, type_double, type_double, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* mag */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* steplat */	{type_short_int, type_short_int, type_unsigned_long_int, type_short_int, type_string, type_string},
		/* Tvert */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* Tvert2 */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* time */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* time2 */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int, type_unsigned_long_int},
		/* trigs */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int, type_short_int},
		/* trigt */	{type_short_int, type_short_int, type_short_int, type_double, type_short_int},
		/* trigtonce */	{type_short_int, type_short_int, type_short_int, type_double, type_int},
	/* exit / quit */
		/* exit */	{},
		/* quit */	{}
};

