
#include <stdbool.h>
#include <tamtypes.h>
#include <string.h>
#include <kernel.h>
#include <sifrpc.h>
#include <stdio.h>

#include "../sd2psxman_common.h"

static SifRpcClientData_t sd2psxman_RPC;
static int rpc_initialized = false;

#define DPRINTF(fmt, x...) printf(fmt, ##x)

#define CHECK_RPC_INIT() if (!rpc_initialized) {DPRINTF("ERROR: Cannot call %s if RPC server is not initialized\n", __FUNCTION__); return -2;}

int sd2psxman_init(void)
{
    if (rpc_initialized)
        {return -2;}
    
    int E;
	while(1)
	{
		if((E = SifBindRpc(&sd2psxman_RPC, SD2PSXMAN_IRX, 0)) < 0)
        {
            DPRINTF("Failed to bind RPC server for SD2PSXMAN (%d)\n", E);
			return -1;
        }

		if(sd2psxman_RPC.server != NULL)
			break;

		nopdelay();
	}

	rpc_initialized = true;

	return 0;
}


int sd2psxman_ping(int port, int slot)
{
    CHECK_RPC_INIT();

    sd2psxman_rpc_pkt_t pkt;
    memset(&pkt, 0, SD2PSXMAN_RPC_PKT_SIZE);

    pkt.port = port;
    pkt.slot = slot;
    
    if (SifCallRpc(&sd2psxman_RPC, SD2PSXMAN_PING, 0, &pkt, SD2PSXMAN_RPC_PKT_SIZE, &pkt, SD2PSXMAN_RPC_PKT_SIZE, NULL, NULL) < 0)
    {
        DPRINTF("%s: RPC ERROR\n", __FUNCTION__);
        return -1;
    }

    //bits 24-16: protocol ver
    //bits 16-8:  product id
    //bits 8-0:   revision id
    return pkt.ret;
}

int sd2psxman_get_status(int port, int slot)
{
    CHECK_RPC_INIT();

    sd2psxman_rpc_pkt_t pkt;
    memset(&pkt, 0, SD2PSXMAN_RPC_PKT_SIZE);

    pkt.port = port;
    pkt.slot = slot;
    
    if (SifCallRpc(&sd2psxman_RPC, SD2PSXMAN_PING, 0, &pkt, SD2PSXMAN_RPC_PKT_SIZE, &pkt, SD2PSXMAN_RPC_PKT_SIZE, NULL, NULL) < 0)
    {
        DPRINTF("%s: RPC ERROR\n", __FUNCTION__);
        return -1;
    }

    return pkt.ret;
}

int sd2psxman_get_card(int port, int slot)
{
    CHECK_RPC_INIT();

    sd2psxman_rpc_pkt_t pkt;
    memset(&pkt, 0, SD2PSXMAN_RPC_PKT_SIZE);

    pkt.port = port;
    pkt.slot = slot;
    
    if (SifCallRpc(&sd2psxman_RPC, SD2PSXMAN_GET_CARD, 0, &pkt, SD2PSXMAN_RPC_PKT_SIZE, &pkt, SD2PSXMAN_RPC_PKT_SIZE, NULL, NULL) < 0)
    {
        DPRINTF("%s: RPC ERROR\n", __FUNCTION__);
        return -1;
    }

    return pkt.ret;
}

int sd2psxman_set_card(int port, int slot, int mode, int cnum)
{
    CHECK_RPC_INIT();

    sd2psxman_rpc_pkt_t pkt;
    memset(&pkt, 0, SD2PSXMAN_RPC_PKT_SIZE);

    pkt.port = port;
    pkt.slot = slot;

    pkt.mode = mode;
    pkt.cnum = cnum;
    
    if (SifCallRpc(&sd2psxman_RPC, SD2PSXMAN_SET_CARD, 0, &pkt, SD2PSXMAN_RPC_PKT_SIZE, NULL, 0, NULL, NULL) < 0)
    {
        DPRINTF("%s: RPC ERROR\n", __FUNCTION__);
        return -1;
    }

    return pkt.ret;
}

int sd2psxman_get_channel(int port, int slot)
{
    CHECK_RPC_INIT();

    sd2psxman_rpc_pkt_t pkt;
    memset(&pkt, 0, SD2PSXMAN_RPC_PKT_SIZE);

    pkt.port = port;
    pkt.slot = slot;
    
    if (SifCallRpc(&sd2psxman_RPC, SD2PSXMAN_GET_CHANNEL, 0, &pkt, SD2PSXMAN_RPC_PKT_SIZE, &pkt, SD2PSXMAN_RPC_PKT_SIZE, NULL, NULL) < 0)
    {
        DPRINTF("%s: RPC ERROR\n", __FUNCTION__);
        return -1;
    }

    return pkt.ret;
}

int sd2psxman_set_channel(int port, int slot, int mode, int cnum)
{
    CHECK_RPC_INIT();

    sd2psxman_rpc_pkt_t pkt;
    memset(&pkt, 0, SD2PSXMAN_RPC_PKT_SIZE);

    pkt.port = port;
    pkt.slot = slot;

    pkt.mode = mode;
    pkt.cnum = cnum;
    
    if (SifCallRpc(&sd2psxman_RPC, SD2PSXMAN_SET_CHANNEL, 0, &pkt, SD2PSXMAN_RPC_PKT_SIZE, NULL, 0, NULL, NULL) < 0)
    {
        DPRINTF("%s: RPC ERROR\n", __FUNCTION__);
        return -1;
    }

    return pkt.ret;
}

int sd2psxman_get_gameid(int port, int slot, char *gameid)
{
    if (gameid == NULL)
        return -1;

    CHECK_RPC_INIT();

    sd2psxman_gameid_rpc_pkt_t pkt;
    memset(&pkt, 0, SD2PSXMAN_GAMEID_RPC_PKT_SIZE);

    pkt.port = port;
    pkt.slot = slot;
    
    if (SifCallRpc(&sd2psxman_RPC, SD2PSXMAN_GET_GAMEID, 0, &pkt, SD2PSXMAN_GAMEID_RPC_PKT_SIZE, &pkt, SD2PSXMAN_GAMEID_RPC_PKT_SIZE, NULL, NULL) < 0)
    {
        DPRINTF("%s: RPC ERROR\n", __FUNCTION__);
        return -1;
    }

    if (pkt.ret > -1) {
        strcpy(gameid, pkt.gameid);
    }

    return pkt.ret;
}

int sd2psxman_set_gameid(int port, int slot, const char *gameid)
{
    if (gameid == NULL)
        return -1;

    if (strlen(gameid) > 0x10)
        return -1; 

    CHECK_RPC_INIT();

    sd2psxman_gameid_rpc_pkt_t pkt;
    memset(&pkt, 0, SD2PSXMAN_GAMEID_RPC_PKT_SIZE);

    pkt.port = port;
    pkt.slot = slot;
    strcpy(pkt.gameid, gameid);

    if (SifCallRpc(&sd2psxman_RPC, SD2PSXMAN_SET_GAMEID, 0, &pkt, SD2PSXMAN_GAMEID_RPC_PKT_SIZE, &pkt, SD2PSXMAN_GAMEID_RPC_PKT_SIZE, NULL, NULL) < 0)
    {
        DPRINTF("%s: RPC ERROR\n", __FUNCTION__);
        return -1;
    }

    return pkt.ret;
}