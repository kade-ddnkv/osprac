#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main() {
    uid_t user_id = getuid();
    gid_t group_id = getgid();
    printf("user_id: %d\ngroup_id: %d\n", user_id, group_id);
    return 0;
}
