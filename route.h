#ifndef __ROUTE_H
#define __ROUTE_H
typedef struct {
    char *chroot;
    unsigned int uid;
    unsigned int gid;
    unsigned int debug;
    char *config;
    unsigned int optimize;
} context;
typedef struct {
    char *verb;
    char *getopt;
    int (*switcher)(context*,int);
    int (*tool)(int,char**,char*,context *);
    int (*help)(void);
    int isleaf;
} command;
extern int route(int,char**,char*,context*);
#endif
