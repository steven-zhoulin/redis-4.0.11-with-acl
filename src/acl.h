#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef __ACL_H
#define __ACL_H

struct acl_node {
    int net;
    int len;
};

extern struct acl_node ACL_NODES[10000];
extern int ACL_NODES_TAIL;
extern int ACL_ENABLE;

char *trim(char *str);
int parse_mask(char *mask);
struct acl_node parse_acl_node(char *line);
void disploy_acl_nodes();
int acl_check(char *ip);

#endif /* __ACL_H */