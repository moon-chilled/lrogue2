#define _POSIX_SOURCE
/*
 * machdep.c
 *
 * This source herein may be modified and/or distributed by anybody who
 * so desires, with the following restrictions:
 *    1.)  No portion of this notice shall be removed.
 *    2.)  Credit shall not be taken for the creation of this source.
 *    3.)  This code is not to be traded, sold, or used for personal
 *         gain or profit.
 *
 */

#ifdef UNIX

#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

/* so struct timezone is defined */
#define __USE_MISC
#include <sys/time.h>
#undef __USE_MISC

#include <signal.h>
#include <time.h>

/* getlogin_r() */
#define __USE_REENTRANT
#include <unistd.h>
#undef __USE_REENTRANT

#include "rogue.h"
#include "extern.h"

#ifdef _POSIX_SOURCE
#include <unistd.h>
#include <termios.h>
#endif

/* md_control_keyboard():
 *
 * This routine is much like md_cbreak_no_echo_nonl() above.  It sets up the
 * keyboard for appropriate input.  Specifically, it prevents the tty driver
 * from stealing characters.  For example, ^Y is needed as a command
 * character, but the tty driver intercepts it for another purpose.  Any
 * such behavior should be stopped.  This routine could be avoided if
 * we used RAW mode instead of CBREAK.  But RAW mode does not allow the
 * generation of keyboard signals, which the program uses.
 *
 * The parameter 'mode' when true, indicates that the keyboard should
 * be set up to play rogue.  When false, it should be restored if
 * necessary.
 *
 * This routine is not strictly necessary and may be stubbed.  This may
 * cause certain command characters to be unavailable.
 */

void md_control_keyboard(int mode) {
	static boolean called_before = 0;
#ifdef _POSIX_SOURCE
        static struct termios tc_orig ;
        static struct termios tc_temp ;
#else
	static struct ltchars ltc_orig;
	static struct tchars tc_orig;
	struct ltchars ltc_temp;
	struct tchars tc_temp;
#endif
	signal(SIGTSTP,SIG_IGN);

	if (!called_before) {
		called_before = 1;
#ifdef _POSIX_SOURCE
        tcgetattr(0,&tc_orig);	
#else
		ioctl(0, TIOCGETC, &tc_orig);
		ioctl(0, TIOCGLTC, &ltc_orig);
#endif
	}
#ifndef _POSIX_SOURCE
	ltc_temp = ltc_orig;
#endif
	tc_temp = tc_orig;

	if (!mode) {
#ifdef _POSIX_SOURCE
	/* tc_temp.c_cc[VSUSP] = -1; */
	tc_temp.c_cc[VERASE] = -1;
	tc_temp.c_cc[VSTART] = -1;
	tc_temp.c_cc[VSTOP] = -1;
#endif
#ifndef _POSIX_SOURCE
		ltc_temp.t_rprntc = ltc_temp.t_flushc = -1;
		ltc_temp.t_werasc = ltc_temp.t_lnextc = -1;
		tc_temp.t_startc = tc_temp.t_stopc = -1;
#endif
	}
#ifdef _POSIX_SOURCE
       tcsetattr(0,TCSANOW,&tc_temp);
#else
	ioctl(0, TIOCSETC, &tc_temp);
	ioctl(0, TIOCSLTC, &ltc_temp);
#endif
}

/* md_heed_signals():
 *
 * This routine tells the program to call particular routines when
 * certain interrupts/events occur:
 *
 *      SIGINT: call onintr() to interrupt fight with monster or long rest.
 *      SIGQUIT: call byebye() to check for game termination.
 *      SIGHUP: call error_save() to save game when terminal hangs up.
 *
 *		On VMS, SIGINT and SIGQUIT correspond to ^C and ^Y.
 *
 * This routine is not strictly necessary and can be stubbed.  This will
 * mean that the game cannot be interrupted properly with keyboard
 * input, this is not usually critical.
 */

void md_heed_signals() {
	signal(SIGINT, onintr);
	signal(SIGQUIT, byebye);
	signal(SIGHUP, error_save);
}

/* md_ignore_signals():
 *
 * This routine tells the program to completely ignore the events mentioned
 * in md_heed_signals() above.  The event handlers will later be turned on
 * by a future call to md_heed_signals(), so md_heed_signals() and
 * md_ignore_signals() need to work together.
 *
 * This function should be implemented or the user risks interrupting
 * critical sections of code, which could cause score file, or saved-game
 * file, corruption.
 */

void md_ignore_signals() {
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGTSTP,SIG_IGN);	/* No ^Z ! */
}

/* md_get_file_id():
 *
 * This function returns an integer that uniquely identifies the specified
 * file.  It need not check for the file's existence.  In UNIX, the inode
 * number is used.
 *
 * This function need not be implemented.  To stub the routine, just make
 * it return 0.  This will make the game less able to prevent users from
 * modifying saved-game files.  This is probably no big deal.
 */

int md_get_file_id(char *fname) {
	struct stat sbuf;

	if (stat(fname, &sbuf)) {
		return(-1);
	}
	return((int) sbuf.st_ino);
}

/* md_link_count():
 *
 * This routine returns the number of hard links to the specified file.
 *
 * This function is not strictly necessary.  On systems without hard links
 * this routine can be stubbed by just returning 1.
 */

int md_link_count(char *fname) {
	struct stat sbuf;

	stat(fname, &sbuf);
	return((int) sbuf.st_nlink);
}

/* md_gct(): (Get Current Time)
 *
 * This function returns the current year, month(1-12), day(1-31), hour(0-23),
 * minute(0-59), and second(0-59).  This is used for identifying the time
 * at which a game is saved.
 *
 * This function is not strictly necessary.  It can be stubbed by returing
 * zeros instead of the correct year, month, etc.  If your operating
 * system doesn't provide all of the time units requested here, then you
 * can provide only those that it does, and return zeros for the others.
 * If you cannot provide good time values, then users may be able to copy
 * saved-game files and play them.  
 */

void md_gct(rogue_time *rt_buf) {
	struct timeval tv;
	struct timezone tzp;
	struct tm *t;
	long seconds;

	gettimeofday(&tv, &tzp);
	seconds = (long) tv.tv_sec;
	t = localtime(&seconds);

	rt_buf->year = t->tm_year;
	rt_buf->month = t->tm_mon + 1;
	rt_buf->day = t->tm_mday;
	rt_buf->hour = t->tm_hour;
	rt_buf->minute = t->tm_min;
	rt_buf->second = t->tm_sec;
}

/* md_gfmt: (Get File Modification Time)
 *
 * This routine returns a file's date of last modification in the same format
 * as md_gct() above.
 *
 * This function is not strictly necessary.  It is used to see if saved-game
 * files have been modified since they were saved.  If you have stubbed the
 * routine md_gct() above by returning constant values, then you may do
 * exactly the same here.
 * Or if md_gct() is implemented correctly, but your system does not provide
 * file modification dates, you may return some date far in the past so
 * that the program will never know that a saved-game file being modified.  
 * You may also do this if you wish to be able to restore games from
 * saved-games that have been modified.
 */

void md_gfmt(char *fname, rogue_time *rt_buf) {
	struct stat sbuf;
	long seconds;
	struct tm *t;

	stat(fname, &sbuf);
	seconds = (long) sbuf.st_mtime;
	t = localtime(&seconds);

	rt_buf->year = t->tm_year;
	rt_buf->month = t->tm_mon + 1;
	rt_buf->day = t->tm_mday;
	rt_buf->hour = t->tm_hour;
	rt_buf->minute = t->tm_min;
	rt_buf->second = t->tm_sec;
}

/* md_df: (Delete File)
 *
 * This function deletes the specified file, and returns true (1) if the
 * operation was successful.  This is used to delete saved-game files
 * after restoring games from them.
 *
 * Again, this function is not strictly necessary, and can be stubbed
 * by simply returning 1.  In this case, saved-game files will not be
 * deleted and can be replayed.
 */

boolean md_df(char *fname) {
	if (unlink(fname)) {
		return(0);
	}
	return(1);
}

/* md_gln: (Get login name)
 *
 * This routine returns the login name of the user.  This string is
 * used mainly for identifying users in score files.
 *
 * A dummy string may be returned if you are unable to implement this
 * function, but then the score file would only have one name in it.
 */

char *md_gln() {
	char *t = malloc(sizeof(char)*30);
	getlogin_r(t, 30);
	return t;
}

/* md_sleep:
 *
 * This routine causes the game to pause for the specified number of
 * seconds.
 *
 * This routine is not necessary at all, and can be stubbed with no ill
 * effects.
 */

void md_sleep(int nsecs) {
	(void) sleep(nsecs);
}

/* md_getenv()
 *
 * This routine gets certain values from the user's environment.  These
 * values are strings, and each string is identified by a name.  The names
 * of the values needed, and their use, is as follows:
 *
 *   ROGUEOPTS
 *     A string containing the various game options.  This need not be
 *     defined.
 *   HOME
 *     The user's home directory.  This is only used when the user specifies
 *     '~' as the first character of a saved-game file.  This string need
 *     not be defined.
 *
 * If your system does not provide a means of searching for these values,
 * you will have to do it yourself.  None of the values above really need
 * to be defined, however.
 */

char *md_getenv(char *name) {
	char *value;
	char *getenv();

	value = getenv(name);

	return(value);
}


/* md_gseed() (Get Seed)
 *
 * This function returns a seed for the random number generator (RNG).  This
 * seed causes the RNG to begin generating numbers at some point in it's
 * sequence.  Without a random seed, the RNG will generate the same set
 * of numbers, and every game will start out exactly the same way.  A good
 * number to use is the process id, given by getpid() on most UNIX systems.
 *
 * You need to find some single random integer, such as:
 *   process id.
 *   current time (minutes + seconds) returned from md_gct(), if implemented.
 *   
 * It will not help to return "get_rand()" or "rand()" or the return value of
 * any pseudo-RNG.  If you cannot a random number, you can just return 1,
 * but this means you games will ALWAYS start the same way, and will play
 * exactly the same way given the same input.
 */

int md_gseed() {
	return(getpid());
}

#endif /* UNIX */
