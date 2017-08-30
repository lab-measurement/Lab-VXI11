#include <stdio.h>
#include <stdlib.h>

#include "vxi11.h"

#include <error.h>

int
main (int argc, char **argv)
{
  CLIENT *VXI11Client;
  if((VXI11Client=clnt_create("132.199.98.2",
                              DEVICE_CORE,DEVICE_CORE_VERSION,"tcp"))==NULL)
    {
      error(1, 0, "clnt_create");
      /* Do error handling here */
    }

  Create_LinkParms MyCreate_LinkParms;
  MyCreate_LinkParms.clientId = 0; // Not used
  MyCreate_LinkParms.lockDevice = 0; // No exclusive access
  MyCreate_LinkParms.lock_timeout = 0;
  MyCreate_LinkParms.device = "inst0"; // Logical device name
  Create_LinkResp *MyCreate_LinkResp;
  if ((MyCreate_LinkResp=create_link_1(&MyCreate_LinkParms,VXI11Client))
      ==NULL)
    {
      /* Do error handling here */
      error(1, 0, "create_link_1");
    }

  Device_Link MyLink;
  MyLink = MyCreate_LinkResp->lid; // Save link ID for further use

int i;
for (i = 0; i < 1000; ++i) {
  Device_WriteParms MyDevice_WriteParms;
  MyDevice_WriteParms.lid = MyLink;
  MyDevice_WriteParms.io_timeout = 10000; // in ms
  MyDevice_WriteParms.lock_timeout = 10000; // in ms
  MyDevice_WriteParms.flags = 0;
  MyDevice_WriteParms.data.data_val = ":read?\n";
  MyDevice_WriteParms.data.data_len = strlen(MyDevice_WriteParms.data.data_val);
  Device_WriteResp *MyDevice_WriteResp;
  if((MyDevice_WriteResp=device_write_1(&MyDevice_WriteParms,VXI11Client))
     ==NULL) {
    /* Do error handling here */
    error(1, 0, "device_write_1");
  }

  Device_ReadParms MyDevice_ReadParms;
  MyDevice_ReadParms.lid = MyLink;
  MyDevice_ReadParms.requestSize = 200;
  MyDevice_ReadParms.io_timeout = 10000;
  MyDevice_ReadParms.lock_timeout = 10000;
  MyDevice_ReadParms.flags = 0;
  MyDevice_ReadParms.termChar = '\n';
  Device_ReadResp *MyDevice_ReadResp;
  if((MyDevice_ReadResp=device_read_1(&MyDevice_ReadParms,VXI11Client))==NULL) {
    /* Do error handling here */
    error(1, 0, "device_read_1");
  }
  char DataRead[200];
  strncpy(DataRead,MyDevice_ReadResp->data.data_val,
          MyDevice_ReadResp->data.data_len);
  DataRead[MyDevice_ReadResp->data.data_len]=0;
  printf("Instrument ID string: %s\n",DataRead);
}


  if(destroy_link_1(&MyLink,VXI11Client)==NULL)
    {
      /* Do error handling here */
      error(1, 0, "destroy_link_1");
    }
  clnt_destroy(VXI11Client);
  return 0;
}
