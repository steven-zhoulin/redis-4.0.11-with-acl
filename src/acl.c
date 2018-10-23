#include "acl.h"

struct acl_node ACL_NODES[10000];
int ACL_NODES_TAIL = 0;
int ACL_ENABLE = 0;

char *trim(char *str) {
    char *start = str;
    char *end;
    if (start) {
        end = start + strlen(str) - 1;
        while(*start && isspace(*start)) start++;
        while(end > start && isspace(*end)) *end-- = '\0';
    }
    return start;
}

int parse_mask(char *mask) {

    int imask = atoi(mask);
    int rtn = 0;
    int i;
    
    for (i = 31; i >= (32 - imask); i--) {
        rtn += (0x1 << i);
    }
    
    return rtn;
}

struct acl_node parse_acl_node(char *line) {

    char *addr = strtok(line, "/");
    char *mask = strtok(NULL, "/");
    if (NULL == mask) {
        mask = "32";
    }

    int net = 0;
    char *p = strtok(addr, ".");
    while (NULL != p) {
        net = (net << 8) + atoi(p);
        p = strtok(NULL, ".");
    }
    
    net = net & parse_mask(mask);
    
    struct acl_node netNode;
    netNode.net = net;
    netNode.len = atoi(mask);

    return netNode;
}

void disploy_acl_nodes() {
    int i;
    for (i = 0; i < ACL_NODES_TAIL; i++) {
        printf("                               ");
        int k;
        for (k = 31; k >= 0; k--) {
            printf("%c", ((0x1 << k) & ACL_NODES[i].net) ? '1' : '0');
            if (k % 8 == 0)  printf(" ");
        }
        printf("    mask_len = %2d \n", ACL_NODES[i].len);
    }  
}

int parse_address(char *ip) {

    char addr[100];
    int rtn = 0;

    strcpy(addr, ip);
    char *result = strtok(addr, ".");
    while (NULL != result) {
        int i = atoi(result);
        rtn = (rtn << 8) + i;
        result = strtok(NULL, ".");
    }
    return rtn;
}

int acl_check(char *ip) {
    int rtn = 0;
    int addr = parse_address(ip);
    
    int i;
    for (i = 0; i < ACL_NODES_TAIL; i++) {
        int k;
        int len = 32 - ACL_NODES[i].len;
        for (k = 31; k >= len; k--) {
            if (((0x1 << k) & ACL_NODES[i].net) != ((0x1 << k) & addr)) {
                break;
            }
        }
        
        if (k == (len - 1)) {
            return 1;
        }
    }

    return rtn;
}