#ifndef SD2PSXMAN_RPC_H
#define SD2PSXMAN_RPC_H

int sd2psxman_init(void);

int sd2psxman_ping(int port, int slot);
int sd2psxman_get_status(int port, int slot);

int sd2psxman_get_card(int port, int slot);
int sd2psxman_set_card(int port, int slot, int mode, int num);

int sd2psxman_get_channel(int port, int slot);
int sd2psxman_set_channel(int port, int slot, int mode, int num);

int sd2psxman_get_gameid(int port, int slot, char *gameid);
int sd2psxman_set_gameid(int port, int slot, const char *gameid);

#endif