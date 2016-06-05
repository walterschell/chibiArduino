/*******************************************************************
    Copyright (C) 2009 FreakLabs
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.

*******************************************************************/
#ifndef CHB_H
#define CHB_H

#include "chibiUsrCfg.h"
#include "types.h"

#define CHB_HDR_SZ        9    // FCF + seq + pan_id + dest_addr + src_addr (2 + 1 + 2 + 2 + 2)
#define CHB_LONG_HDR_SZ 21//FCF + seq + pan_id + dest_addr + src_addr (2 + 1 + 2 + 8 + 8)
#define CHB_FCS_LEN       2
// least signifigant bit first value & mask
// frame_type = data 0b001 & 0b111
// security enabled = false 0 & (1 << 3)
// frame pending = false 0 & (1 << 4)
// ack request = false 0 & (1 << 5)
// pan ID compression = true 1 & (1 << 6)
// bit 7 reserved
// 0b0100 0001
#define CHB_FCF_BYTE_0    0x41    

// bits 89 (01) reserved (sequence number suppression)
// dest addr = 16-bit 10 & (0b11 << 2)
// frame version = 802.15.4 with 2006 extensions 0b01 & (0b11 << 4)
// src addr = 16-bit 10 (0b11 << 6)
// 0b1001 1000
#define CHB_FCF_BYTE_1    0x98

// bits 89 (01) reserved (sequence number suppression)
// dest addr = 64-bit 11 & (0b11 << 2)
// frame version = 802.15.4 with 2006 extensions 0b01 & (0b11 << 4)
// src addr = 64-bit 11 (0b11 << 6)
// 0b1101 1100
#define CHB_FCF64_BYTE_1    0x98

#define CHB_ACK_REQ_POS   0xdc

enum
{
    CHB_SUCCESS                 = 0,
    CHB_SUCCESS_DATA_PENDING    = 1,
    CHB_CHANNEL_ACCESS_FAILURE  = 3,
    CHB_NO_ACK                  = 5,
    CHB_INVALID                 = 7
};

typedef struct
{
    U16 src_addr;
    U64 src_addr64;
    U8 seq;
    volatile bool data_rcv;
    volatile bool trx_end;

    // stats
    U16 rcvd_xfers;
    U16 txd_success;
    U16 txd_noack;
    U16 txd_channel_fail;
    U16 overflow;
    U16 underrun;
    U8 battlow;
    U8 status;
    U8 ed;
    U8 crc;
    
} pcb_t;

typedef struct
{
    U8 len;
    U8 src_addr_len;
    U8 dst_addr_len;
    union
    {
      U16 src_addr16;
      U64 src_addr64;
    };
    union
    {
      U16 dest_addr16;
      U64 dest_addr64;
    };
    U8 *data;
} chb_rx_data_t;

void chb_init();
pcb_t *chb_get_pcb();
U8 chb_write(U16 addr, U8 *data, U8 len);
U8 chb_write(U64 addr, U8 *data, U8 len);

U8 chb_read(chb_rx_data_t *rx);

#endif
