SD2PSX Protocol Draft v0.1

Possible command ranges:
- 0x01-0x2F = General purpose memory card cmds (ping, get card, set card, gameid, etc)
- 0x30-0x40 = Exploit related cmds, FMCB, PS2BBL, FunTuna, OpenTuna 
- 0x40-0x60 = FileIO cmds

out = ps2 -> sd2psx\
in  = ps2 <- sd2psx

### Base packet structure:
|      offset       | out  |  in  |        description        |
|-------------------|------|------|---------------------------|
| 0x00              | 0x8b | 0xff | sd2psxman identifier byte |
| 0x01              | var  | 0xaa | cmd                       |
| 0x02              | 0xff | 0x00 | reserved byte             |
| 0x03-var          | var  | var  | cmd specific bytes        |
| last cmd byte + 1 | 0xff | 0xff | termination byte          |

Note:
0xaa is used as a weak validation check. sd2psxman checks rdbuf[0x1] for 0xaa,
if it's not present the data is considered invalid

### 0x01 - Ping [Implemented]
| offset  | out  |  in  |     description      |
|---------|------|------|----------------------|
| 0x03    | 0xff | var  | product version      |
| 0x04    | 0xff | var  | product id           |
| 0x05    | 0xff | var  | product revision     |
| 0x06    | 0xff | 0xff | termination byte     |

SD2PSX:
protocol version: 0x1
product id: 0x1
product revision: 0x1

### 0x02 - Get Status [Partially Implemented]
| offset | out  |  in  |          description          |
|--------|------|------|-------------------------------|
| 0x03   | var  | 0x0  | operation to return status of |
| 0x04   | 0xff | var  | status                        |
| 0x05   | 0xff | 0xff | termination byte              |

Note:
Some operations such as set card and set channel take several
seconds to complete. The SIO2 cannot wait this long in the middle
of a transfer to return whether or not the operation was successful.

This command addresses this limitation by returning the last
operation performed and whether or not it was successful.

### 0x3 - Get Card [Implemented]
| offset | out  |  in  |        description        |
|--------|------|------|---------------------------|
| 0x03   | 0xff | var  | current card upper 8 bits |
| 0x04   | 0xff | var  | current card lower 8 bits |
| 0x05   | 0xff | 0xff | termination byte          |

### 0x4 - Set Card [Implemented]
| offset | out  |  in  |         description         |
|--------|------|------|-----------------------------|
| 0x03   | var  | 0x0  | mode                        |
| .      |      |      | 0x0 = set card to number    |
| .      |      |      | 0x1 = set card to next card |
| .      |      |      | 0x2 = set card to prev card |
| 0x04   | var  | 0x0  | card upper 8 bits           |
| 0x05   | var  | 0x0  | card lower 8 bits           |
| 0x06   | 0xff | 0xff | termination byte            |

### 0x5 - Get Channel [Implemented]
| offset | out  |  in  |         description          |
|--------|------|------|------------------------------|
| 0x03   | 0xff | var  | current channel upper 8 bits |
| 0x04   | 0xff | var  | current channel lower 8 bits |
| 0x05   | 0xff | 0xff | termination byte             |

### 0x6 - Set Channel [Implemented]
| offset | out  |  in  |            description            |
|--------|------|------|-----------------------------------|
| 0x03   | var  | 0x0  | mode                              |
| .      |      |      | 0x0 = set channel to number       |
| .      |      |      | 0x1 = set channel to next channel |
| .      |      |      | 0x2 = set channel to prev channel |
| 0x04   | var  | 0x0  | channel upper 8 bits              |
| 0x05   | var  | 0x0  | channel lower 8 bits              |
| 0x06   | 0xff | 0xff | termination byte                  |

### 0x7 - Get GameID [Implemented]
|  offset  | out  |  in  |      description       |
|----------|------|------|------------------------|
| 0x03     | 0xff | var  | gameid len             |
| 0x04-var | 0xff | var  | gameid (max 250 bytes) |
| 0xff     | 0xff | 0xff | termination byte       |

Note: SIO2 side expects to receive 255 bytes of data.
If gameid is shorter than this, padding bytes must be sent.

### 0x8 - Set GameID [Implemented]
|  offset  | out  |  in  |      description       |
|----------|------|------|------------------------|
| 0x03     | var  | 0x00 | gameid len             |
| 0x04-var | var  | 0x00 | gameid (max 250 bytes) |
| var + 1  | 0xff | 0xff | termination byte       |

### 0x30 [Temporary Assignment] - Unmount Bootcard [Implemented]
| offset | out  |  in  |   description    |
|--------|------|------|------------------|
| 0x03   | 0xff | 0xff | termination byte |