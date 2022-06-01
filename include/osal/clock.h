#ifndef OSAL_CLOCK_H_
#define OSAL_CLOCK_H_

/** Nanoseconds per second. */
#define OSAL_CLOCK_NANOSPERSEC     1000000000LL

/** Nanoseconds per millisecond. */
#define OSAL_CLOCK_NANOSPERMSEC    1000000L

/** Nanoseconds per microsecond. */
#define OSAL_CLOCK_NANOSPERUSEC    1000

/** Timespec. */
typedef struct {
	/** Seconds. */
	long s;
	/** Nanoseconds. */
	long ns;
} osal_timespec_t;

/** Performance counter. */
typedef struct osal_clock_perf osal_clock_perf_t;

/**
 * Create a performance counter.
 *
 * @return
 *	Performance counter instance (NULL if it could not be created).
 */
osal_clock_perf_t *osal_clock_perf_create(void);

/**
 * Destroy a performance counter.
 *
 * @param [in] perf
 *	Performance counter instance.
 */
void osal_clock_perf_destroy(osal_clock_perf_t *perf);

/**
 * Reset a performance counter start time reference.
 *
 * @param [in] perf
 *	Performance counter instance.
 *
 * @return
 *	0 on success, error code otherwise.
 */
int osal_clock_perf_reset(osal_clock_perf_t *perf);

/**
 * Obtain performance counter using the highest available resolution clock on
 * the system.
 *
 * @note
 *      The counter is monotonic.
 *      The obtained time is relative to counter creation or reset.
 *
 * @param [in] perf
 *	Performance counter instance.
 * @param [out] ts
 *	Where current performance counter value will be stored.
 *
 * @return
 *	0 on success, error code otherwise.
 */
int osal_clock_perf_get(osal_clock_perf_t *perf, osal_timespec_t *ts);

/**
 * Obtain current (monotonic) time.
 *
 * @param [out] ts
 *	Where current time will be stored.
 *
 * @return
 *	0 on success, error code otherwise.
 */
int osal_clock_gettime(osal_timespec_t *ts);

/**
 * Sleep (ms).
 *
 * @param [in] ms
 *	Number of milliseconds to sleep.
 */
void osal_clock_sleep_ms(int ms);

#endif
