#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "route.h"

int helpmain(void) {
    fprintf(stderr,"main help\n");
    return 1;
}

int helpbuild(void) {
    fprintf(stderr,"build help\n");
    return 1;
}

int helpnone(void) {
    fprintf(stderr,"none help\n");
    return 1;
}

/* getopt doesn't strict enforce arguments so this guards against
 * swallowing the next option as an argument by rewinding optind
 * the idea being that we should report all failures since we're
 * already failing */
#define optguard(p,c,o) if(*optarg=='-') { --optind;fprintf(stderr,"%s: option -%c needs an argument\n",p,c); return 10; } else { o }

int switchnone(context *t,int c) {
    return 0;
}

int switchbuild(context *t,int c) {
    char match[]=".";
    switch(c) {
        case 'c':
            optguard("build",c,t->config=optarg;break;)
        case 'o':
            t->optimize=1;break;
        case 'h':
        case '?':
            match[0]=optopt;
            if(strpbrk(match,"c"))
                fprintf(stderr,"build: option -%c needs an argument\n",optopt);
            else if(c=='h'||optopt=='h')
                return 20;
            else fprintf(stderr,"build: unknown option %c\n",optopt);
            return 5;
        default:
            fprintf(stderr,"build: unhandled option '%c'\n",c);
            return 6;
    }
    return 0;
}

unsigned int loaduint(char *check) {
    char *eol=0;
    int v=strtol(check,&eol,10);
    if(*eol=='\0'&&*check!='\0'&&v>0)
        return (unsigned)v;
    else
        return 0;
}

int switchmain(context *t,int c) {
    char match[]=".";
    switch(c) {
        case 'd':
            t->debug=1;break;
        case 'c':
            optguard("main",c,t->chroot=optarg;break;)
        case 'u':
            optguard("main",c,t->uid=loaduint(optarg);break;)
        case 'g':
            optguard("main",c,t->gid=loaduint(optarg);break;)
        case 'h':
        case '?':
            match[0]=optopt;
            if(strpbrk(match,"cug"))
                fprintf(stderr,"main: option -%c needs an argument\n",optopt);
            else if(c=='h'||optopt=='h')
                return 20;
            else fprintf(stderr,"main: unknown option %c\n",optopt);
            return 3;
        default:
            fprintf(stderr,"main: unhandled option '%c'\n",c);
            return 4;
    }
    return 0;
}

int route(int,char**,char*,context *t);
static command dispatcher[]={
    {"main","+dc:u:g:h",switchmain,route,helpmain,0},
    {"build","+c:oh",switchbuild,route,helpbuild,1},
    {"clean","",switchnone,route,helpnone,1},
    };

int route(int argc,char**argv,char *tool,context *t) {
    int c,r,k;
    fprintf(stderr,"enter %s\n",tool);
    for(c=0;c<argc;++c) {
        fprintf(stderr," %s",argv[c]);
    }
    fprintf(stderr,"\n");

    optind=opterr=1;
    for(r=0;r<sizeof(dispatcher)/sizeof(command);++r) {
        if(!strncmp(dispatcher[r].verb,tool,strlen(dispatcher[r].verb))) {
            while((c=getopt(argc,argv,dispatcher[r].getopt))!=-1) {
                k=dispatcher[r].switcher(t,c);
            }
            break;
        }
    }
    if(k==20) return dispatcher[r].help();
    if(r>=sizeof(dispatcher)/sizeof(command)) {
        fprintf(stderr,"unknown command: %s\n",tool);
        return 2;
    }

    printf("%s: using settings\n\
\tchroot:\t%s\n\
\tuid:\t%u\n\
\tgid:\t%u\n\
\tdebug:\t%u\n\
\toptimize:\t%u\n\
\tconfig:\t%s\n",tool,t->chroot,t->uid,t->gid,t->debug,t->optimize,t->config);

    if(optind<argc) {
        printf("invoking sub tool: %s",argv[optind]);
        for(c=optind+1;c<argc;++c) {
            printf(" %s",argv[c]);
        }
        printf("\n");
        if(!dispatcher[r].isleaf)
            return route(argc-optind,&argv[optind],argv[optind],t);
        else
            return 0;
    } else {
        return 0;
    }
}
