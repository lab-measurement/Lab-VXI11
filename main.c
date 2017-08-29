#include <stdio.h>
#include <stdlib.h>

#include "vxi11.h"

int
main (int argc, char **argv)
{
  CLIENT *VXI11Client;
  if((VXI11Client=clnt_create("169.254.9.80",
                              DEVICE_CORE,DEVICE_CORE_VERSION,"tcp"))==NULL)
    {
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
    }
  
  return 0;
}
