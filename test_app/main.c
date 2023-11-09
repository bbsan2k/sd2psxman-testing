#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <loadfile.h>

#include "sd2psxman_rpc.h"
#include "../sd2psxman_common.h"

#define PORT PORT_MEMCARD2
#define MAX_GAMEID_LEN 251

#define IRX_DEFINE(mod)                     \
    extern unsigned char mod##_irx[];       \
    extern unsigned int size_##mod##_irx

#define IRX_LOAD(mod)                                                           \
    if (SifExecModuleBuffer(mod##_irx, size_##mod##_irx, 0, NULL, NULL) < 0)    \
    printf("Could not load ##mod##\n")

IRX_DEFINE(mcman);
IRX_DEFINE(sd2psxman);
IRX_DEFINE(sio2man);

inline void delay(int seconds)
{
    struct timespec tv;
    tv.tv_sec = seconds;
    tv.tv_nsec = 0;
    nanosleep(&tv, NULL);
}

void test_ping(void)
{
    int rv;

    printf("Testing: 0x01 - Ping\n");
    rv = sd2psxman_ping(PORT, 0);
    
    if (rv > -1)
        printf("[PASS] proto ver: 0x%x, prod id: 0x%x, rev id: 0x%x\n", rv >> 16, (rv >> 8) & 0xff, rv & 0xff);
    else
        printf("[Fail] error: %i\n", rv);

    printf("\n");
}

void test_get_status(void)
{}

void test_get_card(void)
{
    int rv;

    printf("Testing: 0x3 - Get Card\n");
    rv = sd2psxman_get_card(PORT, 0);

    if (rv > -1)
        printf("[PASS] current card: %i\n", rv);
    else
        printf("[FAIL] error: %i\n", rv);

    printf("\n");
}

void test_set_card_next(void)
{
    int rv;
    int temp = -1;

    printf("Testing: 0x4 - Set Card [NEXT]\n");
    rv = sd2psxman_get_card(PORT, 0);

    if (rv > -1) {
        sd2psxman_set_card(PORT, 0, SD2PSXMAN_MODE_NEXT, 0);
        delay(7); //give card time to switch
        
        temp = sd2psxman_get_card(PORT, 0);

        if (temp > -1 && temp != rv) {
            printf("[PASS] card: %i -> %i\n", rv, temp);
            printf("\n");
            return;
        }
    }
    printf("[FAIL] %i %i\n", rv, temp);
    printf("\n");
}

void test_set_card_prev(void)
{
    int rv;
    int temp = -1;

    printf("Testing: 0x4 - Set Card [PREV]\n");
    rv = sd2psxman_get_card(PORT, 0);

    if (rv > -1) {
        sd2psxman_set_card(PORT, 0, SD2PSXMAN_MODE_PREV, 0);
        delay(7); //give card time to switch
        temp = sd2psxman_get_card(PORT, 0);

        if (temp > -1 && temp != rv) {
            printf("[PASS] card: %i -> %i\n", rv, temp);
            printf("\n");
            return;
        }
    }
    printf("[FAIL] %i %i\n", rv, temp);
    printf("\n");
}

void test_set_card_num(int num)
{
    int rv;
    int temp = -1;

    printf("Testing: 0x4 - Set Card [NUM] (%i)\n", num);
    rv = sd2psxman_get_card(PORT, 0);

    if (rv > -1) {
        sd2psxman_set_card(PORT, 0, SD2PSXMAN_MODE_NUM, num);
        delay(7); //give card time to switch
        temp = sd2psxman_get_card(PORT, 0);

        if (temp > -1 && temp != rv) {
            printf("[PASS] card: %i -> %i\n", rv, temp);
            printf("\n");
            return;
        }
    }
    printf("[FAIL] %i %i\n", rv, temp);
    printf("\n");
}

void test_get_channel(void)
{
    int rv;

    printf("Testing: 0x5 - Get Channel\n");
    rv = sd2psxman_get_channel(PORT, 0);

    if (rv > -1)
        printf("[PASS] current channel: %i\n", rv);
    else
        printf("[FAIL] error: %i\n", rv);

    printf("\n");
}

void test_set_channel_next(void)
{
    int rv;
    int temp = -1;

    printf("Testing: 0x6 - Set Channel [NEXT]\n");
    rv = sd2psxman_get_channel(PORT, 0);

    if (rv > -1) {
        sd2psxman_set_channel(PORT, 0, SD2PSXMAN_MODE_NEXT, 0);
        delay(7); //give card time to switch
        temp = sd2psxman_get_channel(PORT, 0);

        if (temp > -1 && temp != rv) {
            printf("[PASS] card: %i -> %i\n", rv, temp);
            printf("\n");
            return;
        }
    }
    printf("[FAIL] %i %i\n", rv, temp);
    printf("\n");
}

void test_set_channel_prev(void)
{
    int rv;
    int temp = -1;

    printf("Testing: 0x6 - Set Channel [PREV]\n");
    rv = sd2psxman_get_channel(PORT, 0);
    
    if (rv > -1) {
        sd2psxman_set_channel(PORT, 0, SD2PSXMAN_MODE_PREV, 0);
        delay(7); //give card time to switch        
        temp = sd2psxman_get_channel(PORT, 0);

        if (temp > -1 && temp != rv) {
            printf("[PASS] card: %i -> %i\n", rv, temp);
            printf("\n");
            return;
        }
    }
    printf("[FAIL] %i %i\n", rv, temp);
    printf("\n");
}

void test_set_channel_num(int num)
{
    int rv;
    int temp = -1;

    printf("Testing: 0x6 - Set Channel [NUM] (%i)\n", num);
    rv = sd2psxman_get_channel(PORT, 0);

    if (rv > -1) {
        sd2psxman_set_channel(PORT, 0, SD2PSXMAN_MODE_NUM, num);
        delay(7); //give card time to switch
        temp = sd2psxman_get_channel(PORT, 0);

        if (temp > -1 && temp != rv) {
            printf("[PASS] card: %i -> %i\n", rv, temp);
            printf("\n");
            return;
        }
    }
    printf("[FAIL] %i %i\n", rv, temp);
    printf("\n");
}

void test_get_gameid(void)
{
    int rv;
    char gameid[MAX_GAMEID_LEN];

    printf("Testing: 0x7 - Get GameID\n");
    rv = sd2psxman_get_gameid(PORT, 0, gameid);
    
    if (rv > -1)
        printf("[PASS] gameid: %s\n", gameid);
    else
        printf("[FAIL] error: %i\n", rv);

    printf("\n");
}

void test_set_gameid(char *gameid)
{
    int rv;
    char old_gameid[MAX_GAMEID_LEN];
    char new_gameid[MAX_GAMEID_LEN];

    printf("Testing: 0x8 - Set GameID (%s)\n", gameid);
    rv = sd2psxman_get_gameid(PORT, 0, old_gameid);
    
    if (rv > -1) {
        sd2psxman_set_gameid(PORT, 0, gameid);
        delay(1);
        rv = sd2psxman_get_gameid(PORT, 0, new_gameid);

        if (rv > -1) {
            if (strcmp(old_gameid, new_gameid) != 0){
                printf("[PASS] gameid: %s -> %s\n", old_gameid, new_gameid);
                return;
            }
        }
    }
    printf("[FAIL] %i\n", rv);
    printf("\n");
}

void test_unmount_bootcard(void)
{
    int rv;
    int temp;

    printf("Testing: 0x30 - Unmount Bootcard\n");
    rv = sd2psxman_get_card(PORT, 0);
    if (rv == 0) {
        sd2psxman_unmount_bootcard(PORT, 0);
        delay(7); //give card time to switch
        temp = sd2psxman_get_card(PORT, 0);

        if (temp > -1 && temp != rv) {
            printf("[PASS] bootcard unmounted: %i -> %i\n", rv, temp);
            printf("\n");
        } else {
            printf("[FAIL] %i %i\n", rv, temp);
            printf("\n");
        }
        return;
    }
    printf("[FAIL] bootcard not mounted\n");
    printf("\n");
}

int main()
{
    printf("Loading sio2man\n");
    IRX_LOAD(sio2man);
    delay(1);

    printf("Loading sd2psxman\n");
    IRX_LOAD(sd2psxman);
    delay(1);
    
    printf("Loading mcman\n");
    IRX_LOAD(mcman);

    sd2psxman_init();

    time_t t;
    srand((unsigned) time(&t));
    
    char gameid[0x250] = {'S', 'L', 'U', 'S', '-', '2', '0', '9', '1', '5', '\0'};

    delay(2);
    test_ping();
    delay(2);
    test_get_card();
    delay(2);
    test_set_card_next();
    delay(2);
    test_set_card_prev();
    delay(2);
    test_set_card_num(rand() % 1024);
    delay(2);
    test_get_channel();
    delay(2);
    test_set_channel_next();
    delay(2);
    test_set_channel_prev();
    delay(2);
    test_set_channel_num((rand() % 7) + 1);
    delay(2);
    test_get_gameid();
    delay(2);
    test_set_gameid(gameid);

    printf("Testing Complete\n");

    return 0;
}
