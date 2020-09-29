/*
    KTop, a taskmanager and cpu load monitor
   
	Copyright (c) 1999 Chris Schlaeger
	                   cs@kde.org
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

// $Id$

/*
 * ATTENTION: PORTING INFORMATION!
 * 
 * If you plan to port KTop to a new platform please follow these instructions.
 * For general porting information please look at the file OSStatus.cpp!
 *
 * To keep this file readable and maintainable please keep the number of
 * #ifdef _PLATFORM_ as low as possible. Ideally you dont have to make any
 * platform specific changes in the header files. Please do not add any new
 * features. This is planned for KTop versions after 1.0.0!
 */

#include <config.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif      
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_USER_H
#include <sys/user.h>
#endif
#include <signal.h>
#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <kapp.h>

#include "OSProcess.h"
#include "TimeStampList.h"

#ifdef __linux__

// Code for Linux 2.x

OSProcess::OSProcess(const void* info, TimeStampList* lastTStamps,
					 TimeStampList* newTStamps)
{
	error = false;

	userName = "????";

	if (!read((const char*) info))
		return;

	TimeStamp* ts = new TimeStamp(pid, userTime, sysTime);
	newTStamps->inSort(ts);

	if (lastTStamps->find(ts) >= 0)
	{
		int lastCentStamp = lastTStamps->current()->getCentStamp();
		int lastUserTime = lastTStamps->current()->getUserTime();
		int lastSysTime = lastTStamps->current()->getSysTime();

		int timeDiff = ts->getCentStamp() - lastCentStamp;
		int userDiff = userTime - lastUserTime;
		int sysDiff =  sysTime - lastSysTime;

		if (timeDiff > 0)
		{
			userLoad = ((double) userDiff / timeDiff) * 100.0;
			sysLoad = ((double) sysDiff / timeDiff) * 100.0;
		}
		else
			sysLoad = userLoad = 0.0;
	}
	else
		sysLoad = userLoad = 0.0;
}

OSProcess::OSProcess(int pid_)
{
	error = false;

	QString pidStr;
	pidStr.sprintf("%d", pid_);

	if (!read((const void *)pidStr))
		return;

	userLoad = sysLoad = 0.0;
}

bool
OSProcess::read(const void* info)
{
	FILE* fd;

	QString buf;
	buf.sprintf("/proc/%s/status", (const char*) info);
	if((fd = fopen(buf, "r")) == 0)
	{
		error = true;
		errMessage.sprintf(i18n("Cannot open %s!\n"), buf.data());
		return (false);
	}

    char status;
    int nstgid;
    char *line = NULL;
    name[0] = 0;
    for (size_t len = 0; getline(&line, &len, fd) != -1;) {
	sscanf(line, "Name: %[^\n]", name);
	sscanf(line, "State: %c %*s", &status);
	sscanf(line, "Uid: %d %*d %*d %*d", (int*) &uid);
	sscanf(line, "Gid: %d %*d %*d %*d", (int*) &gid);
	sscanf(line, "VmSize: %d kB", &vm_size);
	sscanf(line, "VmRSS: %d kB", &vm_rss);
	sscanf(line, "VmLib: %d kB", &vm_lib);
	sscanf(line, "Pid: %d", &pid);
	sscanf(line, "PPid: %d", &ppid);
    }
    free(line);

	fclose(fd);

    buf.sprintf("/proc/%s/stat", (const char *)info);
	if ((fd = fopen(buf, "r")) == 0)
	{
		error = true;
		errMessage.sprintf(i18n("Cannot open %s!\n"), buf.data());
		return (false);
	}

        for (size_t i=0; i<strlen(name); i++) {
            if (name[i] == '%') {
                name[i] = ' '; // better safe than sorry, I guess
            }
        }

        QString fmtBuf;
	fmtBuf.sprintf(
                "%%*d " // PID
                "(%s) " // Comm
                "%%*c " // State
                "%%*d " // Ppid
                "%%*d " // Pgrp
                "%%*d " // Session
                "%%d "  // ttyno
                "%%*d " // tpgid
                "%%*u " // flags
                "%%*u " // minflt
                "%%*u " // cminflt
                "%%*u " // majflt
                "%%*u " // cmajflt
                "%%d "  // utime
                "%%d "  // stime
		"%%*d " // cutime
                "%%*d " // cstime
                "%%*d " // priority
                "%%d "  // nice
                "%%*u " // numthreads
                "%%*u " // itrealvalue
                "%%*d " // starttime
                "%%*u", // vsize
            name);
	int read = fscanf(fd,
                fmtBuf.data(),
		   &ttyNo, &userTime, &sysTime, &niceLevel);

        if (read != 4) {
            ttyNo = 0;
            userTime = 0;
            sysTime = 0;
            niceLevel = 0;
        }
	vm_rss = (vm_rss + 3) * getpagesize();

	fclose(fd);

    buf.sprintf("/proc/%s/cmdline", (const char *)info);
	if ((fd = fopen(buf, "r")) == 0)
	{
		error = true;
		errMessage.sprintf(i18n("Cannot open %s!\n"), buf.data());
		return (false);
	}
	char cbuf[1024];
	cbuf[0] = '\0';
	fscanf(fd, "%1023[^\n]", cbuf);
	cbuf[1023] = '\0';
	cmdline = cbuf;
	fclose(fd);

	switch (status)
	{
	case 'R':
		statusTxt = i18n("Run");
		break;
	case 'S':
		statusTxt = i18n("Sleep");
		break;
	case 'D': 
		statusTxt = i18n("Disk");
		break;
	case 'Z': 
		statusTxt = i18n("Zombie");
		break;
	case 'T': 
		statusTxt = i18n("Stop");
		break;
	case 'W': 
		statusTxt = i18n("Swap");
		break;
	default:
		statusTxt = i18n("????");
		break;
	}

	// find out user name with the process uid
	struct passwd* pwent = getpwuid(uid);
	if (pwent)
		userName = pwent->pw_name;

	return (true);
}

#elif __FreeBSD__
/* Port to FreeBSD by Hans Petter Bieker <zerium@webindex.no>.
 *
 * Copyright 1999 Hans Petter Bieker <zerium@webindex.no>.
 */

#include <sys/param.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <sys/user.h>                    

OSProcess::OSProcess(const void* info, TimeStampList* /*lastTStamps*/,
                                         TimeStampList* /*newTStamps*/)
{
	error = false;

	read(info);
}

OSProcess::OSProcess(int pid_)
{
	error = false;

	int mib[4];
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PID;
        mib[3] = pid_;

	struct kinfo_proc p;
	size_t len = sizeof (struct kinfo_proc);
	if (sysctl(mib, 4, &p, &len, NULL, 0) == -1 || !len)
		return;

	read((char *)&p);
}

bool
OSProcess::read(const void* info)
{
	struct kinfo_proc *p = (struct kinfo_proc *)info;

	pid = p->kp_proc.p_pid;
	ppid = p->kp_eproc.e_ppid;
	strcpy(name, p->kp_proc.p_comm);
	uid = p->kp_eproc.e_ucred.cr_uid;
	gid = p->kp_eproc.e_pgid;

	// find out user name with the process uid
	struct passwd* pwent = getpwuid(uid);
	userName = pwent ? pwent->pw_name : "????";
	priority = p->kp_proc.p_priority;
	niceLevel = p->kp_proc.p_nice;

	// this isn't usertime -- it's total time (??)
#if __FreeBSD_version >= 300000
	userTime = p->kp_proc.p_runtime / 10000;
#else
	userTime = p->kp_proc.p_rtime.tv_sec*100+p->kp_proc.p_rtime.tv_usec/10000;
#endif
	sysTime = 0;
	userLoad = p->kp_proc.p_pctcpu / 100;
	sysLoad = 0;

	// memory
	vm_size =  (p->kp_eproc.e_vm.vm_tsize +
                    p->kp_eproc.e_vm.vm_dsize +
                    p->kp_eproc.e_vm.vm_ssize) * getpagesize();
	vm_rss = p->kp_eproc.e_vm.vm_rssize * getpagesize();

	statusTxt = p->kp_eproc.e_wmesg; // i18n() this FIXME

	return (true);
}

#else
#warning "No supported platform"

OSProcess::OSProcess(const void* info, TimeStampList* lastTStamps,
                                         TimeStampList* newTStamps)
{
}

OSProcess::OSProcess(int pid_)
{
}

bool
OSProcess::read(const char* info)
{
}
#endif

/*
 * Hopefully the following functions work on all platforms.
 */

bool
OSProcess::exists(void) const
{
	return ((kill(pid, 0) == 0) || (errno == EPERM));
}

bool
OSProcess::setNiceLevel(int newNiceLevel)
{
	if (setpriority(PRIO_PROCESS, pid, newNiceLevel) == -1)
	{
		error = true;
		errMessage.sprintf(i18n("Could not set new nice level for process %d"),
						   pid);
		return (false);
	}

	return (true);
}

bool
OSProcess::sendSignal(int sig)
{
	if (kill(pid, sig))
	{
		error = true;
		errMessage.sprintf(i18n("Cound not send signal %d to process %d"),
						   sig, pid);
		return (false);
	}

	return (true);
}
