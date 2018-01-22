/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:tim.today@gmail.com                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */
#include "sid.h"
#include <sys/types.h>
#include "php.h"

//2010/11/4 9:42:54 twitter版本时间
#define twepoch 1288834974657L

#define workerIdBits 5L
#define datacenterIdBits 5L
#define maxWorkerId (-1L ^ (-1L << workerIdBits))
#define maxDatacenterId (-1L ^ (-1L << datacenterIdBits))
#define sequenceBits 12L

#define workerIdShift sequenceBits
#define datacenterIdShift (sequenceBits + workerIdBits)
#define timestampLeftShift (sequenceBits + workerIdBits + datacenterIdBits)
#define sequenceMask (-1L ^ (-1L << sequenceBits))
///#define lastTimestamp -1L

static uint64_t last_id;

void set_workid(long myworkid)
{
    mData.workerId = myworkid;
}
void set_datacenterId(long datacenterId)
{
    mData.datacenterId = datacenterId;
}
pid_t get_pid(void)
{
    return syscall(__NR_gettid);
}
uint64_t timeGen()
{
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    uint64_t ms_time = time_now.tv_sec * 1000 + time_now.tv_usec / 1000;
    return ms_time;
}
uint64_t tilNextMillis(uint64_t lastStamp)
{

    uint64_t cur = 0;
    do
    {
        cur = timeGen();
    } while (cur <= lastStamp);

    return cur;
}
int atomic_incr(int id)
{
    __sync_add_and_fetch(&id, 1);
    return id;
}

uint64_t get_twitter_id()
{
    uint64_t snowflake_id;
    uint64_t timestamp = timeGen();
    int seq;
    if (timestamp < mData.lastTimestamp)
    {
        perror("error");
        exit(-1);
    }
    if (timestamp == mData.lastTimestamp)
    {
        mData.sequence = atomic_incr(mData.sequence) & sequenceMask;
        if (mData.sequence == 0)
        {
            timestamp = tilNextMillis(mData.lastTimestamp);
        }
    }
    else
    {
        mData.sequence = 0;
    }

    mData.lastTimestamp = timestamp;
    //return ((timestamp - twepoch) << timestampLeftShift) | (mData.datacenterId << datacenterIdShift) | (mData.workerId << workerIdShift) | mData.sequence;
    snowflake_id = ((timestamp - twepoch) << timestampLeftShift) | (mData.datacenterId << datacenterIdShift) | (mData.workerId << workerIdShift) | mData.sequence;
    if (snowflake_id <= last_id)
    {
        snowflake_id = get_twitter_id();
    }
    last_id = snowflake_id;
    return snowflake_id;
}