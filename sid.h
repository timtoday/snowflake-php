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

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <linux/types.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>

struct snowflake_data
{
  uint64_t lastTimestamp;
  long datacenterId;
  long workerId;
  int sequence;
} mData;


void set_workid(long myworkid);
void set_datacenterId(long datacenterId);
uint64_t timeGen();
uint64_t tilNextMillis(uint64_t lastStamp);
pid_t get_pid(void);
int atomic_incr(int id);
uint64_t get_twitter_id();
