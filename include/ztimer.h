#ifndef ZTIMER
#define ZTIMER

#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
/*Porting gettimeofday to Windows,
source: https://www.codefull.net/2015/12/systime-h-replacement-for-windows/
TODO: Consider simply using std::chrono for timing operations
*/
#include <winsock2.h>

#define __need_clock_t
#include <sys/timeb.h>
#include <time.h>
typedef long long suseconds_t;
/* Structure describing CPU time used by a process and its children.  */
struct tms {
  clock_t tms_utime; /* User CPU time.  */
  clock_t tms_stime; /* System CPU time.  */

  clock_t tms_cutime; /* User CPU time of dead children.  */
  clock_t tms_cstime; /* System CPU time of dead children.  */
};

/* Store the CPU time used by this process and all its
   dead children (and their dead children) in BUFFER.
   Return the elapsed real time, or (clock_t) -1 for errors.
   All times are in CLK_TCKths of a second.  */
clock_t times(struct tms *__buffer) {

  __buffer->tms_utime = clock();
  __buffer->tms_stime = 0;
  __buffer->tms_cstime = 0;
  __buffer->tms_cutime = 0;
  return __buffer->tms_utime;
}

int gettimeofday(struct timeval *t, void *timezone) {
  struct _timeb timebuffer;
  _ftime(&timebuffer);
  t->tv_sec = timebuffer.time;
  t->tv_usec = 1000 * timebuffer.millitm;
  return 0;
}

#else
#include <sys/time.h>
#endif
class ZTimer {
public:
  struct timeval t1, t2;

public:
  ZTimer() {
    gettimeofday(&t1, 0);
    t2 = t1;
  }
  void reset() {
    gettimeofday(&t1, 0);
    t2 = t1;
  }
  int elapsed() {
    return ((t2.tv_sec - t1.tv_sec) * 1000) +
           ((t2.tv_usec - t1.tv_usec) / 1000);
  }
  int split() {
    gettimeofday(&t2, 0);
    return elapsed();
  }
};

#endif
