#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <vxi11.h>

#include "const-c.inc"

typedef CLIENT *Lab__VXI11;

static void
do_not_warn_unused(void *x __attribute__((__unused__)))
{
}

static SV *
pointer_object(pTHX_ const char *class_name, void *pv)
{
    SV *rv = newSV(0);
    sv_setref_pv(rv, class_name, pv);
    return rv;
}

MODULE = Lab::VXI11		PACKAGE = Lab::VXI11		

INCLUDE: const-xs.inc

Lab::VXI11
new(char *class, char *host, unsigned long prog, unsigned long vers, char *proto)
CODE:
    do_not_warn_unused(class);
    RETVAL = clnt_create(host, prog, vers, proto);
OUTPUT:
    RETVAL


void
DESTROY(Lab::VXI11 client)
CODE:
    clnt_destroy(client);


void
create_link(Lab::VXI11 client, long clientId, bool_t lockDevice, unsigned long lock_timeout, char *device)
PPCODE:
    Create_LinkParms link_parms;
    link_parms.clientId = clientId;
    link_parms.lockDevice = lockDevice;
    link_parms.lock_timeout = lock_timeout;
    link_parms.device = device;
    
    Create_LinkResp *link_resp;
    link_resp = create_link_1(&link_parms, client);
    mXPUSHi(link_resp->error);
    mXPUSHi(link_resp->lid);
    mXPUSHu(link_resp->abortPort);
    mXPUSHu(link_resp->maxRecvSize);



void
device_write(Lab::VXI11 client, long lid, unsigned long io_timeout, unsigned long lock_timeout, long flags, SV *data)
PPCODE:
    const char *bytes;
    STRLEN len;
    bytes = SvPV(data, len);
    Device_WriteParms write_parms;
    write_parms.lid = lid;
    write_parms.io_timeout = io_timeout;
    write_parms.lock_timeout = lock_timeout;
    write_parms.flags = flags;
    write_parms.data.data_len = len;
    write_parms.data.data_val = (char *) bytes;

    Device_WriteResp *write_resp;
    write_resp = device_write_1(&write_parms, client);
    mXPUSHi(write_resp->error);
    mXPUSHu(write_resp->size);


void
device_read(Lab::VXI11 client, long lid, unsigned long requestSize, unsigned long io_timeout, unsigned long lock_timeout, long flags, char termChar)
PPCODE:
    Device_ReadParms read_parms;
    read_parms.lid = lid;
    read_parms.requestSize = requestSize;
    read_parms.io_timeout = io_timeout;
    read_parms.lock_timeout = lock_timeout;
    read_parms.flags = flags;
    read_parms.termChar = termChar;

    Device_ReadResp *read_resp;
    read_resp = device_read_1(&read_parms, client);
    mXPUSHi(read_resp->error);
    mXPUSHi(read_resp->reason);
    mXPUSHp(read_resp->data.data_val, read_resp->data.data_len);
    
    

   

