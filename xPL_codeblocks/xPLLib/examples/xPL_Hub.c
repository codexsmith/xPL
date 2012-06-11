/* xPL_Hub.c -- Implementation of an xPL Hub using xPLLib */
/* Copyright (c) 2004, Gerald R Duprey Jr. */

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <stdarg.h>


#include "../xPL.h"

#define MAX_HUB_RESTARTS 10000

static Bool daemonMode = TRUE;
static Bool debugMode = FALSE;

/* static buffer to create log messages */
#define LOG_BUFF_MAX 512
static char logMessageBuffer[LOG_BUFF_MAX];

/* Used to track hubs */
static pid_t hubPid = 0;


/* Private wrapper for messages */
static void writeMessage(int msgType, String theFormat, va_list theParms) {
  time_t rightNow;

  /* Write a time stamp */
  if (!daemonMode) {
    time(&rightNow);
    strftime(logMessageBuffer, 40, "%y/%m/%d %H:%M:%S ", localtime(&rightNow));

    switch(msgType) {
    case LOG_ERR:
      strcat(logMessageBuffer, "ERROR");
      break;
    case LOG_WARNING:
      strcat(logMessageBuffer, "WARNING");
      break;
    case LOG_INFO:
      strcat(logMessageBuffer, "INFO");
      break;
    case LOG_DEBUG:
      strcat(logMessageBuffer, "DEBUG");
      break;
    }

    strcat(logMessageBuffer, ": ");
  } else
    logMessageBuffer[0] = '\0';

  /* Convert formatted message */
  vsprintf(&logMessageBuffer[strlen(logMessageBuffer)], theFormat, theParms);

  /* Write to the console or system log file */
  if (daemonMode)
    syslog(msgType, logMessageBuffer);
  else {
    strcat(logMessageBuffer, "\n");
    fprintf(stderr, logMessageBuffer);
  }
}

/* Write an information message out */
void writeInfo(String theFormat, ...) {
  va_list theParms;
  va_start(theParms, theFormat);
  writeMessage(LOG_INFO, theFormat, theParms);
  va_end(theParms);
}

/* Write an error message out */
static void writeError(String theFormat, ...) {
  va_list theParms;
  va_start(theParms, theFormat);
  writeMessage(LOG_ERR, theFormat, theParms);
  va_end(theParms);
}

/* Write a debug message out */
static void writeDebug(String theFormat, ...) {
  va_list theParms;

  if (debugMode) {
    va_start(theParms, theFormat);
    writeMessage(LOG_DEBUG, theFormat, theParms);
    va_end(theParms);
  }
}

/* parseCmdLine will handles command line switches.  Valid switches are: */
/*  -interface x - set interface to use */
/*  -debug - set debugging mode */
/*  -xpldebug - set debugging and enable xPL debugging */
/*  -nodaemon - Dosn't disconnect from the console */
static Bool parseCmdLine( int *argc, char *argv[]) {
  int swptr;
  int newcnt = 0;

  /* Handle each item of the command line.  If it starts with a '-', then */
  /* process it as a switch.  If not, then copy it to a new position in   */
  /* the argv list and up the new parm counter.                           */
  for(swptr = 0; swptr < *argc; swptr++) {
    /* If it doesn't begin with a '-', it's not a switch. */
    if (argv[swptr][0] != '-') {
      if (swptr != newcnt) argv[++newcnt] = argv[swptr];
    }
    else {
       /* Check for debug mode */
      if (!strcmp(argv[swptr],"-debug")) {
	debugMode = TRUE;
	daemonMode = FALSE;
	writeDebug("Debuging mode enabled");
	continue;
      }

      /* Check for debug mode */
      if (!strcmp(argv[swptr],"-xpldebug")) {
	debugMode = TRUE;
	daemonMode = FALSE;
	xPL_setDebugging(TRUE);
	xPL_Debug("xPL Debug mode enabled");
	continue;
      }

      /* Check for daemon mode */
      if (!strcmp(argv[swptr],"-nodaemon")) {
	daemonMode = FALSE;
	writeInfo("Running on console");
	continue;
      }

      /* Anything left is unknown */
      writeError("Unknown switch `%s'", argv[swptr] );
      return FALSE;
    }
  }

  /* Set in place the new argument count and exit */
  *argc = newcnt + 1;
  return TRUE;
}

/* Print command usage info out */
void printUsage(String ourName) {
  fprintf(stderr, "%s - xPL Hub\n", ourName);
  fprintf(stderr, "Copyright (c) 2005, Gerald Duprey\n\n");
  fprintf(stderr, "Usage: %s [-debug] [-xpldebug] [-nodaemon] [-ip x] [-interface x]\n", ourName);
  fprintf(stderr, "  -debug -- enable hub debug messages\n");
  fprintf(stderr, "  -xpldebug -- enable hub and xPLLib debugging messagaes\n");
  fprintf(stderr, "  -nodaemon -- don't detach -- run from the console\n");
  fprintf(stderr, "  -interface x -- Use interface named x (i.e. eth0) as network interface\n");
  fprintf(stderr, "  -ip x -- Bind to specified IP address for xPL\n");
}


/* Shutdown gracefully if user hits ^C or received TERM signal */
static void hubShutdownHandler(int onSignal) {
  xPL_stopHub();
  xPL_shutdown();
  exit(0);
}

/* This is the hub code itself.  */
static void runHub() {
  /* Start xPLLib */
  if (!xPL_initialize(xcStandAlone)) {
    writeError("Unable to start xPLLib -- an xPL hub appears to already be running");
    exit(1);
  }
  xPL_Debug("xPLLib started");

  /* Start xPL Hub */
  xPL_startHub();

  /* Install signal traps for proper shutdown */
  signal(SIGTERM, hubShutdownHandler);
  signal(SIGINT, hubShutdownHandler);

  /* Hand control over to xPLLib */
  writeInfo("xPL Hub now running");
  xPL_processMessages(-1);
  exit(0);
}

/* Shutdown gracefully if user hits ^C or received TERM signal */
static void supervisorShutdownHandler(int onSignal) {
  int childStatus;

  writeInfo("Received termination signal -- starting shutdown");

  /* Stop the child and wait for it */
  if ((hubPid != 0) && !kill(hubPid, SIGTERM)) {
    waitpid(hubPid, &childStatus, 0);
  }

  /* Exit */
  exit(0);
}

/* Start the hub and monitor it.  If it stops for any reason, restart it */
static void superviseHub() {
  int hubRestartCount = 0;
  int childStatus;

  /* Begin hub supervision */

  /* Install signal traps for proper shutdown */
  signal(SIGTERM, supervisorShutdownHandler);
  signal(SIGINT, supervisorShutdownHandler);

  /* To supervise the hub, we repeatedly fork ourselves each time we */
  /* determine the hub is not running.  We maintain a circuit breaker*/
  /* to prevent an endless spawning if the hub just can't run        */
  for(;;) {
    /* See if we can still do this */
    if (hubRestartCount == MAX_HUB_RESTARTS) {
      writeError("xPLHub has died %d times -- something may be wrong -- terminating supervisor", MAX_HUB_RESTARTS);
      exit(1);
    }

    /* Up the restart count */
    hubRestartCount++;

    /* Fork off the hub */
    switch (hubPid = fork()) {
    case 0:            /* child */
      /* Close standard I/O and become our own process group */
      close(fileno(stdin));
      close(fileno(stdout));
      close(fileno(stderr));
      setpgrp();
      
      /* Run the hub */
      runHub();

      /* If we come back, something bad likely happened */
      exit(1);
      
      break;
    default:           /* parent */
      writeDebug("Spawned xPLHub process, PID=%d, Spawn count=%d", hubPid, hubRestartCount);
      break;
    case -1:           /* error */
      writeError("Unable to spawn xPLHub supervisor, %s (%d)", strerror(errno), errno);
      exit(1);
    }


    /* Now we just wait for something bad to happen to our hub */
    waitpid(hubPid, &childStatus, 0);
    if (WIFEXITED(childStatus)) {
      writeInfo("xPLHub exited normally with status %d -- restarting...", WEXITSTATUS(childStatus));
      continue;
    }
    if (WIFSIGNALED(childStatus)) {
      writeInfo("xPLHub died from by receiving unexpected signal %d -- restarting...", WTERMSIG(childStatus));
      continue;
    }
    writeInfo("xPLHub died from unknown causes -- restarting...");
    continue;
  }
}


int main(int argc, String argv[]) {
  /* Check for xPL command parameters */
  xPL_parseCommonArgs(&argc, argv, TRUE);

  /* Parse Hub command arguments */
  if (!parseCmdLine(&argc, argv)) {
    printUsage(argv[0]);
    exit(1);
  }

  /* Now we detach (daemonize ourself) */
  if (daemonMode) {
    /* Fork ourselves */
    switch (fork()) {
    case 0:            /* child */
      /* Close standard I/O and become our own process group */
      close(fileno(stdin));
      close(fileno(stdout));
      close(fileno(stderr));
      setpgrp();

      /* Start he hub and keep it running */
      superviseHub();      

      break;
    default:           /* parent */
      exit(0);
      break;
    case -1:           /* error */
      writeError("Unable to spawn xPLHub supervisor, %s (%d)", strerror(errno), errno);
      exit(1);
    }
  } else {
    /* When running non-detached, just do the hub work */
    runHub();
  }


  return 0;
}
