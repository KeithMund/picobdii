/*
Copyright (C) Trampas Stern  name of author

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "obdii.h"
#include "codes.h"

UBYTE connect=UNKNOWN; 
UBYTE obdError=0;
UBYTE pidSupported[32]={0}; //allow up to 256 PIDS
UBYTE pid13;
UBYTE pid1D;
UBYTE MIL_ON=0;

UBYTE obdII_clear_codes()
{
	OBDII data[MAX_RESP];
	UBYTE cmd,ret,i;
	
	cmd=0x04;
	ret=obdII_send(&cmd,1,data,MAX_RESP);
	
	if (ret==0)
	{
		printf("No response to clear codes\n\r");
		return 0;
	}
	for (i=0; i<ret; i++)
	{
		if (data[i].data[0]==0x44)
		{
			printf("ECM %X codes cleared\n\r",data[i].addr);
		}
	}
	return ret;
}


UBYTE pidLabels(UBYTE pid, UBYTE *str, UBYTE nStr, UBYTE *str2, UBYTE nStr2)
{
 	str[0]=0;
	str2[0]=0;
 	switch (pid)
	{
		case 0x03:
			sprintf(str,"Fuel Sys1: ");
			sprintf(str2,"Fuel Sys2: ");
			break;					
		case 0x04:
			sprintf(str,"Load ");
			break;
		case 0x05: 
			sprintf(str,"ECT ");
			break;
		case 0x06: 
			sprintf(str,"SHRTFT1: ");
			if (pid1D & 0x30)
			{
				sprintf(str2,"SHRTFT3: ");
			}
			break;
		case 0x07: 
			sprintf(str,"LONGFT1: ");
			if (pid1D & 0x30)
			{
				sprintf(str2,"LONGFT3: ");
			}
			break;
		case 0x08: 
			sprintf(str,"SHRTFT2: ");
			if (pid1D & 0xC0)
			{
				sprintf(str2,"SHRTFT4: ");
			}
			break;
		case 0x09: 
			sprintf(str,"LONGFT2: ");
			if (pid1D & 0xC0)
			{
				sprintf(str2,"LONGFT4: ");
			}
			break;
		case 0x0A: 
			sprintf(str,"Fuel P: ");
			break;
		case 0x0B: 
			sprintf(str,"MAP: ");
			break;
		case 0x0C: 
			sprintf(str,"RPM: ");
			break;
		case 0x0D: 
			sprintf(str,"VSS: ");
			break;
		case 0x0E: 
			sprintf(str,"Spark Adv: ");
			break;
		case 0x0F: 
			sprintf(str,"IAT: ");
			break;
		case 0x10: 
			sprintf(str,"MAF: ");
			break;
		case 0x11: 
			sprintf(str,"TPS: ");
			break;
		case 0x12: 
   			sprintf(str,"Air Stat: ");
			break;
		case 0x14: 
			sprintf(str,"O2 B1S1: ");
			sprintf(str2,"O2 B1S1: ");
			break;
		case 0x15: 
			sprintf(str,"O2 B1S2: ");
			sprintf(str2,"O2 B1S2: ");
			break;
		case 0x16: 
			sprintf(str,"O2 B1S3: ");
			sprintf(str2,"O2 B1S3: ");
			break;
		case 0x17: 
			sprintf(str,"O2 B1S4: ");
			sprintf(str2,"O2 B1S4: ");
			break;
		case 0x18: 
			sprintf(str,"O2 B2S1: ");
			sprintf(str2,"O2 B2S1: ");
			break;
		case 0x19: 
			sprintf(str,"O2 B2S2: ");
			sprintf(str2,"O2 B2S2: ");
			break;
		case 0x1A: 
			sprintf(str,"O2 B2S3: ");
			sprintf(str2,"O2 B2S3: ");
			break;
		case 0x1B: 
			sprintf(str,"O2 B2S4: ");
			sprintf(str2,"O2 B2S4: ");
			break;
		case 0x1C: 
			sprintf(str,"OBDSUP: ");
			break;
		case 0x1E: 
			sprintf(str,"PTO_STAT: ");
			break;
		case 0x1F: 
			sprintf(str,"RunTime: ");
			break;
		case 0x21: 
			sprintf(str,"MIL_dist: ");
			break;
		case 0x22: 
			sprintf(str,"FRP: ");
			break;
		case 0x23: 
			sprintf(str,"FRP: ");
			break;
		case 0x24:
		case 0x34: 
			sprintf(str,"EQ_RAT11: ");
			sprintf(str2,"O2 S11: ");
   			break;
		case 0x25: 
		case 0x35:
			sprintf(str,"EQ_RAT12: ");
			sprintf(str2,"O2 S12: ");
   			break;	
   		case 0x26:
   		case 0x36: 
			sprintf(str,"EQ_RAT13: ");
			sprintf(str2,"O2 S13: ");
   			break;
		case 0x27: 
		case 0x37:
			sprintf(str,"EQ_RAT14: ");
			sprintf(str2,"O2 S14: ");
   			break;
		case 0x28: 
		case 0x38:
			sprintf(str,"EQ_RAT21: ");
			sprintf(str2,"O2 S21: ");
   			break;
		case 0x29:
		case 0x39:
			sprintf(str,"EQ_RAT22: ");
			sprintf(str2,"O2 S22: ");
   			break;
		case 0x2A: 
		case 0x3A:
			sprintf(str,"EQ_RAT23: ");
			sprintf(str2,"O2 S23: ");
   			break;
		case 0x2B: 
		case 0x3B:
			sprintf(str,"EQ_RAT24: ");
			sprintf(str2,"O2 S24: ");
   			break;
   		case 0x2C: 
			sprintf(str,"EGR: ");
			break;
		case 0x2D: 
			sprintf(str,"EGR_ERR: ");
			break;
		case 0x2E: 
			sprintf(str,"EVAP: ");
			break;
		case 0x2F: 
			sprintf(str,"Fuel Lvl: ");
			break;
		case 0x30: 
			sprintf(str,"Warm ups: ");
			break;
		case 0x31: 
			sprintf(str,"Clear Dist: ");
			break;
		case 0x32: 
			sprintf(str,"EVAP P: ");
			break;
		case 0x33: 
			sprintf(str,"BARO: ");
			break;
		case 0x3C: 
			sprintf(str,"CAT TEMP11: ");
			break;
		case 0x3D: 
			sprintf(str,"CAT TEMP21: ");
			break;
		case 0x3E: 
			sprintf(str,"CAT TEMP12: ");
			break;
		case 0x3F: 
			sprintf(str,"CAT TEMP22: ");
			break;
		case 0x42:
			sprintf(str,"VPWR: ");
			break;
		case 0x43:
			sprintf(str,"LOAD ABS: ");
			break;
		case 0x44:
			sprintf(str,"EQ_RAT: ");
			break;
		case 0x45:
			sprintf(str,"TP_R: ");
			break;
		case 0x46:
			sprintf(str,"AAT: ");
			break;
		case 0x47:
			sprintf(str,"TPS_B: ");
			break;
		case 0x48:
			sprintf(str,"TPS_C: ");
			break;
		case 0x49:
			sprintf(str,"APP_D: ");
			break;
		case 0x4A:
			sprintf(str,"APP_E: ");
			break;
		case 0x4B:
			sprintf(str,"APP_F: ");
			break;
		case 0x4C:
			sprintf(str,"TAC_PCT: ");
			break;
		case 0x4D:
			sprintf(str,"MIL_TIME: ");
			break;
		case 0x4E:
			sprintf(str,"CLR_TIME: ");
			break;
	}
	return 0;
}


//we have a OBDII data stream which should be a PID
UBYTE process_PID(UBYTE *data, UBYTE *str, UBYTE nStr, UBYTE *str2, UBYTE nStr2)
{
	UBYTE pid,ret;
	
	str[0]=0;
	str2[0]=0;

	if (data[0]!=0x41)
		return 0;	  //not a PID 
			
	pid=data[1]; //get PID
	ret=1;
	switch (pid)
	{
		case 0x03:
			if (data[2]==0)
				sprintf(str,"N/A");
			if (data[2] & 0x01)
				sprintf(str,"OL");
			if (data[2] & 0x02)
				sprintf(str,"CL");
			if (data[2] & 0x04)
				sprintf(str,"OL-Dr");
			if (data[2] & 0x08)
				sprintf(str,"OL_Flt");
			if (data[2] & 0x10)
				sprintf(str,"CL_Flt");
			if (data[3]==0)
				sprintf(str2,"N/A");
			if (data[3] & 0x01)
				sprintf(str2,"OL");
			if (data[3] & 0x02)
				sprintf(str2,"CL");
			if (data[3] & 0x04)
				sprintf(str2,"OL-Dr");
			if (data[3] & 0x08)
				sprintf(str2,"OL_Flt");
			if (data[3] & 0x10)
				sprintf(str2,"CL_Flt");

			break;					
		case 0x04:
			sprintf(str,"%01.1f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x05: 
			sprintf(str,"%01.0f%cC",OBDII_TEMP(data[2]),0xDF);
			break;
		case 0x06: 
		case 0x07:
		case 0x08:
		case 0x09:
			sprintf(str,"%01.0f%%",OBDII_O2PERCENT(data[2]));
			sprintf(str2,"%01.0f%%",OBDII_O2PERCENT(data[3]));
			break;
		case 0x0A: 
			sprintf(str,"%01.0fkPa",(FLOAT)(data[2])*3);
			break;
		case 0x0B: 
			sprintf(str,"%01.0fkPa",(FLOAT)(data[2]));
			break;
		case 0x0C: 
			sprintf(str,"%01.0f",(FLOAT)MAKEWORD(data[2],data[3])/4.0);
			break;
		case 0x0D: 
			sprintf(str,"%01.0fkm/h",(FLOAT)(data[2]));
			break;
		case 0x0E: 
			sprintf(str,"%01.0f",((FLOAT)(data[2])-128.0)/2.0);
			break;
		case 0x0F: 
			sprintf(str,"%01.0f%cC",OBDII_TEMP(data[2]),0xDF);
			break;
		case 0x10: 
			sprintf(str,"%01.2fg/s",(FLOAT)MAKEWORD(data[2],data[3])*0.01);
			break;
		case 0x11: 
			sprintf(str,"%01.2f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x12: 
			if (data[2] & 0x01)
				 sprintf(str,"UPS");
			if (data[2] & 0x02)
				 sprintf(str,"DNS");
			if (data[2] & 0x04)
				 sprintf(str,"OFF");
			break;
		case 0x14: 
		case 0x15: 
		case 0x16: 
		case 0x17: 
		case 0x18: 
		case 0x19: 
		case 0x1A:
		case 0x1B:  
			sprintf(str,"%01.3fV",OBDII_O2VOLTS(data[2]));
			sprintf(str2,"%01.1f%%",OBDII_O2PERCENT(data[3]));
			break;
		case 0x1C:
			if(data[2]==0x01)
				sprintf(str,"OBDII");
			if(data[2]==0x02)
				sprintf(str,"OBD");
			if(data[2]==0x03)
				sprintf(str,"OBD & II");
			if(data[2]==0x04)
				sprintf(str,"OBD I");
			if(data[2]==0x05)
				sprintf(str,"No OBD");
			if(data[2]==0x06)
				sprintf(str,"EOBD");
			if(data[2]==0x07)
				sprintf(str,"EOBD&II");
			if(data[2]==0x08)
				sprintf(str,"EOBD&OBD");
			if(data[2]==0x09)
				sprintf(str,"E OBD II");
			if(data[2]==0x0A)
				sprintf(str,"JOBD");
			if(data[2]==0x0B)
				sprintf(str,"JOBD II");
			if(data[2]==0x0C)
				sprintf(str,"JOBD EOBD");
			if(data[2]==0x0D)
				sprintf(str,"J E II");
			break;
		case 0x1F: 
			sprintf(str,"%0lus", MAKEWORD(data[2],data[3]) );
			break;
		case 0x21: 
			sprintf(str,"%0lukm", MAKEWORD(data[2],data[3]));
			break;
		case 0x22: 
			sprintf(str,"%01.2fkPA",(FLOAT)MAKEWORD(data[2],data[3])*0.079 );
			break;
		case 0x23: 
			sprintf(str,"%01.0fkPA",(FLOAT)MAKEWORD(data[2],data[3])*10.0);
			break;
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29:
		case 0x2A:
		case 0x2B: 
			sprintf(str,"%01.3f",(FLOAT)MAKEWORD(data[2],data[3])*0.0000305);
			sprintf(str2,"%01.2fV",(FLOAT)MAKEWORD(data[2],data[3])*0.000122);
			break;
		case 0x2C: 
			sprintf(str,"%01.1f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x2D: 
			sprintf(str,"%01.1f%%",OBDII_O2PERCENT(data[2]));
			break;
		case 0x2E: 
			sprintf(str,"%01.1f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x2F: 
			sprintf(str,"%01.1f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x30: 
			sprintf(str,"%0d",data[2]);
			break;
		case 0x31: 
			sprintf(str,"%0ldkm",MAKEWORD(data[2],data[3]));
			break;
		case 0x32: 
			sprintf(str,"%01.2fPa",((FLOAT)(MAKEWORD(data[2],data[3])))*0.25);
			break;
		case 0x33: 
			sprintf(str,"%0dkPa",data[2]);
			break;
		case 0x34:
		case 0x35:
		case 0x36:
		case 0x37:
		case 0x38:
		case 0x39:
		case 0x3A:
		case 0x3B: 
			sprintf(str,"%01.3f",(FLOAT)MAKEWORD(data[2],data[3])*0.0000305);
			sprintf(str2,"%01.3fmA",(FLOAT)MAKEWORD(data[2],data[3])*0.00390625);
			break;
		case 0x3C:
		case 0x3D:
		case 0x3E:
		case 0x3F: 
			sprintf(str,"%01.1f%cC",(FLOAT)MAKEWORD(data[2],data[3])*0.1,0xDF);
			break;
		case 0x42:
			sprintf(str,"%01.3fV",(((FLOAT)(MAKEWORD(data[2],data[3])))*0.001)-40.0);
			break;
		case 0x43:
			sprintf(str,"%01.1f%%abs",((FLOAT)(MAKEWORD(data[2],data[3])))*100.0/255.0);
			break;
		case 0x44:
			sprintf(str,"%01.3fs",((FLOAT)(MAKEWORD(data[2],data[3])))*0.0000305);
			break;
		case 0x45:
			sprintf(str,"%01.1f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x46:
			sprintf(str,"%01f%cC",OBDII_TEMP(data[2]),0xDF);
			break;
		case 0x47:
			sprintf(str,"%01f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x48:
			sprintf(str,"%01f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x49:
			sprintf(str,"%01f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x4A:
			sprintf(str,"%01f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x4B:
			sprintf(str,"%01f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x4C:
			sprintf(str,"%01f%%",OBDII_PERCENT(data[2]));
			break;
		case 0x4D:
			sprintf(str,"%0lds",MAKEWORD(data[2],data[3]));
			break;
		case 0x4E:
			sprintf(str,"%0lds",MAKEWORD(data[2],data[3]));
			break;
	}
	return ret;
}


UBYTE isPidSupported(UBYTE pid)
{
	return bit_test(pidSupported[pid>>3],(pid & 0x07));
}

UBYTE getNextPid(UBYTE pid)
{
	while(pid<0x4E)
	{
		pid++;
		if(isPidSupported(pid) && pid!=0x13 && pid!=0x1d )
			return pid;
	}
	return 0;
}

UBYTE PidInit()
{
	OBDII data[MAX_RESP];	
   	UBYTE cmd[2],ret,i,j;
	UBYTE mask,dataByte;
	UBYTE offset;
	static UBYTE numPids=0;
	static UBYTE ncount=0;
	
	if (ncount==0 || ncount>50)
	{
		numPids=0;
		offset=0;
		ncount=0;
	  	do {
		  	//first find supported PIDs
			cmd[0]=0x01;
			cmd[1]=offset;

			ret=obdII_send(cmd,2,data,MAX_RESP);

			if(ret==0)
			{
				printf("No response from ECM for PID number\n\r");
				return 0;
			}
			if (ret>1)
			{
				//or the data bytes together
				printf("Num ECMs %d\n\r",ret);
				for(j=0; j<ret; j++)
				{
					for(i=2; i<6; i++)
					{
						data[0].data[i]=data[0].data[i] | data[j].data[i];
					}
				}
			}
/*
			printf("pids %u: ",data[0].n);
			for(i=0; i<data[0].n;i++)
			{
				printf("%X ",data[0].data[i]);
			}
			printf("\n\r");
*/
			//else lets process PIDs.... 
			i=1+offset;
			mask=0x80;
			dataByte=2;
			while(dataByte<6 && dataByte<data[0].n)
			{
				if (data[0].data[dataByte] & mask)
				{
					bit_set(pidSupported[i>>3],(i & 0x07));
					if (i!=0x01)
					{
						numPids++;
					}
				}
				i++;
				mask=mask>>1;
				if (mask==0)
				{
					dataByte++;
					mask=0x80;
				}
			}
			offset=offset + 0x20; 

			//now handle the configuration PIDS
			if (isPidSupported(0x13))
			{
				cmd[0]=0x01;
				cmd[1]=0x13;

				ret=obdII_send(cmd,2,data,MAX_RESP);
				if (ret)
				{
					pid13=data[0].data[2];
				}
			}
			if (isPidSupported(0x1D))
			{
				cmd[0]=0x01;
				cmd[1]=0x1D;

				ret=obdII_send(cmd,2,data,MAX_RESP);
				if (ret)
				{
					pid1D=data[0].data[2];
				}
			}

		}while(data[0].data[5] & 0x01);
			
	}
   	ncount++;
/*
	printf("Supported pids: ");
	for (i=0; i<8; i++)
	{
		printf("%X ",pidSupported[i]);
	}
	printf("\n\r");	  
*/
	return numPids;
}

				

	

//gets PID from ECM and returns string. The 
// value returned from function is number of lines
// stored in string
UBYTE obdII_read_PID(UBYTE PID, UBYTE *str, UBYTE nStr,UBYTE *str2, UBYTE nStr2)
{
	OBDII data[MAX_RESP];
	UBYTE cmd[2],ret,i;
	
	cmd[0]=0x01;
	cmd[1]=PID;
	
	ret=obdII_send(cmd,2,data,MAX_RESP);

	if (ret==0)
	{
		return 0;
	}
	
	for(i=0; i<ret; i++)
	{
		if(data[i].data[1]==PID)
			process_PID(data[i].data,str,nStr,str2,nStr2);
		else
		{
			str[0]=0;
			str2[0]=0;
		}
	}
	return 1;
}

/*
UBYTE obdII_pid_get(UBYTE *str, UBYTE line)
{
	OBDII data[MAX_RESP];	
   	UBYTE cmd[2],ret,i,j;
	UBYTE mask,dataByte;
	UBYTE offset,pidNum;
	UBYTE pid;

	offset=0;
	pidNum=0;
  	do {
	  	//first find supported PIDs
		cmd[0]=0x01;
		cmd[1]=offset;

		ret=obdII_send(cmd,2,data,MAX_RESP);

		if(ret==0)
		{
			printf("No response from ECM for PID support\n\r");
			return 0;
		}
		if (ret>1)
		{
			//or the data bytes together
			printf("Num ECMs %d\n\r",ret);
			for(j=0; j<ret; j++)
			{
				for(i=2; i<6; i++)
				{
					data[0].data[i]=data[0].data[i] | data[j].data[i];
				}
			}
		}

		printf("pids ");
		for(i=0; i<data[0].n;i++)
		{
			printf("%X ",data[0].data[i]);
		}
		printf("\n\r");

		//else lets process PIDs.... 
		i=1+offset;
		mask=0x80;
		dataByte=2;
		while(dataByte<6 && dataByte<data[0].n)
		{
			if (data[0].data[dataByte] & mask)
			{
				if (i!=0x01)
				{
					pidNum++;
				}
				if (pidNum==line)
				{
					obdII_read_PID(i,str,50);
					return i;
				}
			}
			i++;
			mask=mask>>1;
			if (mask==0)
			{
				dataByte++;
				mask=0x80;
			}
		}
		offset=offset + 0x20; 
	}while(data[0].data[5] & 0x01);

	return 0;
}	
			 

//reads PIDs from ECM
UBYTE obdII_pid_test()
{
   	OBDII data[MAX_RESP];	
   	UBYTE cmd[2],ret,i,j;
   	UBYTE str[50];
	UBYTE mask,dataByte;
	UBYTE offset;

	offset=0;
  	do {
	  	//first find supported PIDs
		cmd[0]=0x01;
		cmd[1]=offset;

		ret=obdII_send(cmd,2,data,MAX_RESP);

		if(ret==0)
		{
			printf("No response from ECM for PID support\n\r");
			return 0;
		}
		if (ret>1)
		{
			//or the data bytes together
			printf("Num ECMs %d\n\r",ret);
			for(j=0; j<ret; j++)
			{
				for(i=2; i<6; i++)
				{
					data[0].data[i]=data[0].data[i] | data[j].data[i];
				}
			}
		}

		printf("pids ");
		for(i=0; i<data[0].n;i++)
		{
			printf("%X ",data[0].data[i]);
		}
		printf("\n\r");

		//else lets process PIDs.... 
		i=1+offset;
		mask=0x80;
		dataByte=2;
		while(dataByte<6 && dataByte<data[0].n)
		{
			if (data[0].data[dataByte] & mask)
			{
				obdII_read_PID(i,str,50);
			}
			i++;
			mask=mask>>1;
			if (mask==0)
			{
				dataByte++;
				mask=0x80;
			}
		}
		offset=offset + 0x20; 
	}while(data[0].data[5] & 0x01);

	return 0;
}
*/	

UBYTE obdII_leds_off()
{
	LED_CAT(0);
	LED_HEATED_CAT(0);
	LED_DTC(0);
	LED_EVAP(0);
	LED_AIR(0);
	LED_O2(0);
	LED_HEATED_O2(0);
	LED_EGR(0);
	LED_MISFIRE(0);
	LED_AC(0);
	LED_FUEL(0);
	LED_COMP(0);
	return 0;
}

UBYTE obdII_leds(OBDII *data_in,UBYTE nResp)
{
	UBYTE i,j;

	OBDII data[1];

	//Logically or multiple ECM responses for Status
	for (i=0; i<nResp; i++)
	{
		for (j=1; j<data_in[i].n; j++)
		{
			data[0].data[j]= data_in[0].data[j] | data_in[i].data[j];
		}
	}

	if (data[0].data[2] & 0x80) 
	{
		MIL_ON=1;
		//LED_DTC(1);
	}
	else
	{
		MIL_ON=0;
		//LED_DTC(0);
	}

	if ((data[0].data[3] & 0x01) && (data[0].data[3] & 0x10))
		LED_MISFIRE(0);
	else
		LED_MISFIRE(1);

	if ((data[0].data[3] & 0x02) && (data[0].data[3] & 0x20))
	{
	   	LED_FUEL(0);
	}
	else
	{
		LED_FUEL(1);	
	}
  
  	if ((data[0].data[3] & 0x04) && (data[0].data[3] & 0x40))
	{
		LED_COMP(0);
	}
	else
	{
		LED_COMP(1);
	}

	if ((data[0].data[4] & 0x01)	&& (data[0].data[5] & 0x01))
		LED_CAT(0);
	else
		LED_CAT(1);
	
	if ((data[0].data[4] & 0x02) && (data[0].data[5] & 0x02))
		LED_HEATED_CAT(0);
	else
		LED_HEATED_CAT(1);
 
 	if ((data[0].data[4] & 0x04) && (data[0].data[5] & 0x04))
		LED_EVAP(0);
	else
		LED_EVAP(1);

	if ((data[0].data[4] & 0x08) && (data[0].data[5] & 0x08))
		LED_AIR(0);
	else
		LED_AIR(1);
   
   	if ((data[0].data[4] & 0x10)	&& (data[0].data[5] & 0x10))
	 	LED_AC(0);
	else
		LED_AC(1);

	if ((data[0].data[4] & 0x20) && (data[0].data[5] & 0x20))
		LED_O2(0);
	else
		LED_O2(1);

	if ((data[0].data[4] & 0x40)	&& (data[0].data[5] & 0x40))
		LED_HEATED_O2(0);
	else
		LED_HEATED_O2(1);

	if ((data[0].data[4] & 0x80) && (data[0].data[5] & 0x80))
		LED_EGR(0);
	else
		LED_EGR(1);
  
	return 0;
}

 
UBYTE obdII_get_num_codes(UBYTE reset)
{
	OBDII data[MAX_RESP];
	UBYTE cmd[2],ret,i; 
	static UBYTE state=0;
	static UDWORD timer;
	static UBYTE nCodes=0;
	static UBYTE ncount=0;

	if(reset)
		ncount=0;
	if (ncount>50 || ncount==0)
	{
		ncount=0;

		//first find number of codes
		cmd[0]=0x01;
		cmd[1]=0x01;

		ret=obdII_send(cmd,2,data,MAX_RESP);

		if (ret==0 || ret>MAX_RESP)
		{
			printf("No response for number of codes %d\n\r",ret);
			return 0;
		}

		obdII_leds(data,ret);

		//process data
		nCodes=0;
		for (i=0; i<ret; i++)
		{
			if (data[i].data[0]==0x41)
			{
				nCodes=nCodes+(data[i].data[2] & 0x7F);
			}
		}
	}
	ncount++;

	if(nCodes && MIL_ON==0)
	{
		if (timer!=getSeconds())
		{
			state=!state;
			LED_DTC(state);
			timer=getSeconds();
		}
	}else
	{
		LED_DTC(MIL_ON);
	}	

	return nCodes;
}	 

UWORD obdII_get_code(UBYTE index, UBYTE nCodes)
{
	OBDII data[MAX_RESP];
	UBYTE cmd[2],ret,i,j,k,l;
	UWORD code;

	
	if (index>nCodes) //Opps someone passed bad parameters
	{
		return 0;
	}
	cmd[0]=0x03;
	ret=obdII_send(cmd,1,data,MAX_RESP);

	if (ret==0 || ret>MAX_RESP)
	{
		printf("No response for codes %d\n\r",ret);
		return 0;
	}
	//process codes
	
	if(index<1)
		return 0;
	l=0;
	for (i=0; i<ret; i++)
	{
		
		k=1;
		j=0;
		while (k<data[i].n)
		{
			if (data[i].data[0]==0x43)
			{
				code=(UWORD)((data[i].data[1+(j*2)] & 0xC0)>>6)*10000;
				code=code+(UWORD)((data[i].data[1+(j*2)] & 0x30)>>4)*(UWORD)1000;
				code=code+ (UWORD)((data[i].data[1+(j*2)] & 0x0F))*(UWORD)100;
				code=code+ (UWORD)((data[i].data[2+(j*2)] & 0xF0)>>4)*(UWORD)10;
				code=code+ (UWORD)((data[i].data[2+(j*2)] & 0x0F));
				//printf("code is %lu\n\r",code);
				j++;
				if (code>0)
				{	
					l++;
					if (l==index)
					{
						return code;
					}				
				}
				k=k+2; 
			}else
			{
				k=data[i].n;
			}
			
		}//while
	}
	return 0;

}
  

//we need a function to read codes
UBYTE obdII_codes()
{
	UBYTE i;
	UBYTE nCodes;

	//first find number of codes
	nCodes=obdII_get_num_codes(0);
  	printf("Found %d codes\n\r",nCodes);

	for (i=0; i<nCodes; i++)
	{
		printf("Code %lu\n\r",obdII_get_code(i,nCodes));
	}
   	printf("Codes Done!\n\r");
	return nCodes;
} 


//gets the status string by index/line number
UBYTE obdII_get_status(UBYTE *str, UBYTE line)
{
	OBDII data[MAX_RESP];
	static UBYTE piddata[7];
	static UBYTE count=0;	
	UBYTE cmd[2],ret,i,j;
	
	if (line>NUM_STATUS_LINES)
	{
		sprintf(str, "STATUS ERROR");
		return 0;
	}
	//read the status bits
	if (count==0 || count>50)
	{
		cmd[0]=0x01;
		cmd[1]=0x01;

		ret=obdII_send(cmd,2,data,MAX_RESP);

		if (ret==0 || ret>MAX_RESP)
		{
			printf("No response for status %d\n\r",ret);
			count=0;
			return 0;
		}

		//Logically or multiple ECM responses for Status
		for (i=0; i<ret; i++)
		{
			for (j=1; j<data[i].n; j++)
			{
				piddata[j]= data[0].data[j] | data[i].data[j];
			}
		}
	}
	//process data
	switch(line)
	{
		case 1:
		  if (piddata[2] & 0x80)
		  {
		  	sprintf(str, "MIL: ON");
		  }else
		  {
		  	sprintf(str, "MIL: OFF");
		  }
		  return 0;
		case 2:
			if (piddata[3] & 0x01)
			{
				if (piddata[3] & 0x10)
				{
					sprintf(str, "Missfire: NotRdy");
				}else
				{
					sprintf(str, "Missfire: Ready");
				}
			}else
			{
				sprintf(str, "Missfire: N/A");
			}
			return 0;
		 case 3:
			if (piddata[3] & 0x02)
			{
				if (piddata[3] & 0x20)
				{
					sprintf(str, "Fuel Sys: NotRdy");
				}else
				{
					sprintf(str, "Fuel Sys: Ready");
				}
			}else
			{
				sprintf(str, "Fuel Sys: N/A");
			}
			return 0;
		 case 4:
			if (piddata[3] & 0x04)
			{
				if (piddata[3] & 0x40)
				{
					sprintf(str, "Comp. Mon: NotRdy");
				}else
				{
					sprintf(str, "Comp. Mon: Ready");
				}
			}else
			{
				sprintf(str, "Comp. Mon: N/A");
			}
			return 0;
		case 5:
			if (piddata[4] & 0x01)
			{
				if (piddata[5] & 0x01)
				{
					sprintf(str, "Catalyst: NotRdy");
				}else
				{
					sprintf(str, "Catalyst: Ready");
				}
			}else
			{
				sprintf(str, "Catalyst: N/A");
			}
			return 0;
		case 6:
			if (piddata[4] & 0x02)
			{
				if (piddata[5] & 0x02)
				{
					sprintf(str, "Heated Cat: NotRdy");
				}else
				{
					sprintf(str, "Heated Cat: Ready");
				}
			}else
			{
				sprintf(str, "Heated Cat: N/A");
			}
			return 0;
		case 7:
			if (piddata[4] & 0x04)
			{
				if (piddata[5] & 0x04)
				{
					sprintf(str, "EVAP: NotRdy");
				}else
				{
					sprintf(str, "EVAP: Ready");
				}
			}else
			{
				sprintf(str, "EVAP: N/A");
			}
			return 0;
		case 8:
			if (piddata[4] & 0x08)
			{
				if (piddata[5] & 0x08)
				{
					sprintf(str, "Sec. Air: NotRdy");
				}else
				{
					sprintf(str, "Sec. Air: Ready");
				}
			}else
			{
				sprintf(str, "Sec. Air: N/A");
			}
			return 0;
		case 9:
			if (piddata[4] & 0x10)
			{
				if (piddata[5] & 0x10)
				{
					sprintf(str, "A/C Sys: NotRdy");
				}else
				{
					sprintf(str, "A/C Sys: Ready");
				}
			}else
			{
				sprintf(str, "A/C Sys: N/A");
			}
			return 0;
		case 10:
			if (piddata[4] & 0x20)
			{
				if (piddata[5] & 0x20)
				{
					sprintf(str, "O2 Sensor: NotRdy");
				}else
				{
					sprintf(str, "O2 Sensor: Ready");
				}
			}else
			{
				sprintf(str, "O2 Sensor: N/A");
			}
			return 0;
		case 11:
			if (piddata[4] & 0x40)
			{
				if (piddata[5] & 0x40)
				{
					sprintf(str, "O2 Heater: NotRdy");
				}else
				{
					sprintf(str, "O2 Heater: Ready");
				}
			}else
			{
				sprintf(str, "O2 Heater: N/A");
			}
			return 0;
		case 12:
			if (piddata[4] & 0x80)
			{
				if (piddata[5] & 0x80)
				{
					sprintf(str, "EGR Sys: NotRdy");
				}else
				{
					sprintf(str, "EGR Sys: Ready");
				}
			}else
			{
				sprintf(str, "EGR Sys: N/A");
			}
			return 0;
	}
	sprintf(str, "Status N/A");

	return 0;
}
	
UBYTE obdII_run(BYTE index, UBYTE reset)
{
	static INT line=0;
	UBYTE temp,temp2;
	UBYTE str[20];
	UBYTE str2[20];
	UBYTE str3[20];
	UBYTE str4[20];
	UBYTE str5[20];

	UBYTE nCodes;
	UBYTE nPids,i,pid;
	static UBYTE lastPid=0;
	static UWORD lastcode=0;

	//first find number of codes
	nCodes=obdII_get_num_codes(reset);
	if (obdError==0)
	{
	   //get number of PIDS
	   nPids=PidInit();	
	   if (isPidSupported(0x13))
			nPids--;
	   if (isPidSupported(0x1d))
			nPids--;

	}
  
	if (obdError)
	{
		//LCD_clear();
		sprintf(str,"No Communications");	
		LCD_print1(str,0);
		sprintf(str,"Turn Key On");
		LCD_print2(str,0);
		lastcode=0;
		obdII_leds_off();
		return 0;
	}
	
	if(index!=0)
		lastcode=0;

	line=line+index;
	if (line<0)
	{
		line=0;
	}
	
	sprintf(str,"Line=%d",line);
	if (line>(INT)(nCodes+NUM_STATUS_LINES+nPids))
	{
		line=nCodes+NUM_STATUS_LINES+nPids;
	}


	if (line<=0)
	{
		line=0;
		sprintf(str,"NumCodes %u",nCodes);
		LCD_print1(str,0);
		sprintf(str,"Scroll to view");
		LCD_print2(str,0);
		return 0;
	}


	temp=line;
	if (temp<=nCodes)
	{
		UWORD code;

		//get the code 		
		code=obdII_get_code(temp,nCodes);
		//since the code is scrolling we can not update every loop
		//so let's update when code changes...
		//printf("code %lu %u\n\r",code,temp);
		if (lastcode!=code && code!=0)
		{
			//printf("printing code %lu %u\n\r",code,temp);
			PcodePrint(code);
			lastcode=code;
		}
		//printf("printing code done\n\r");
		//PcodePrint(temp+100);
		return 0;
	}else
	{
		lastcode=0;
	}

	//Now lets handle the status monitors
	temp=line-nCodes;
	if (temp<NUM_STATUS_LINES)
	{
		//printf("status temp=%u\n\r",temp);
		obdII_get_status(str,temp);
		LCD_print1(str,0);
		obdII_get_status(str,temp+1);
		LCD_print2(str,0);
		return 0;
	}

	//now lets read the data stream... 
	temp=temp-NUM_STATUS_LINES+1;
	//printf("PID line is %d\n\r",temp);
	pid=0x02;
	for(i=0; i<temp; i++)
	{
		temp2=getNextPid(pid);
		if (temp2>0)
			pid=temp2;
	}
	//printf("PID is %u\n\r",pid);

	//get the labels for the PIDs
	pidLabels(pid,str,20,str2,20);
	temp2=0;
	if (str2[0]==0)
	{
		//printf("reading next pid\n\r");
		temp2=getNextPid(pid);
		//printf("next pid is %u\n\r",temp2);
		if (temp2!=0)
		{
			pidLabels(temp2,str2,20,str5,20);
		}
	}
	////printf("Temp2 is %u\n\r",temp2);
	if (pid!=lastPid)
	{
		LCD_print1(str,0);
		LCD_print2(str2,0);
		lastPid=pid;
	}
	obdII_read_PID(pid,str3,20,str4,20);
	//printf("str4 is %s\n\r",str4);
	if(temp2!=0)
	{
		//printf("reading second pid value %u\n\r",temp2);
		obdII_read_PID(temp2,str4,20,str5,20);
	}
	str5[0]=0;
	sprintf(str5,"%s%s",str,str3);
	str[0]=0;
	sprintf(str,"%s%s",str2,str4);
	//printf("str is %s\n\r",str);
	LCD_print1(str5,0);
	LCD_print2(str,0);
/*				
	if (temp<=nPids)
	{
		obdII_pid_get(str, temp) ;
		LCD_print1(str,0);
		obdII_pid_get(str,temp+1);
		LCD_print2(str,0);
	}
*/
    return 0;	

}	

//does CRC calculations
UBYTE crc(UBYTE *data, UBYTE len)
{
	UBYTE result;
	UBYTE i;
	UBYTE mask;
	UBYTE j;
	UBYTE temp;
	UBYTE poly;

	result=0xFF;
	for(i=0; i<len; i++)
	{
		mask=0x80;
		temp=data[i];
		for(j=0; j<8; j++)
		{
			if(temp & mask)	  //bit is 1
			{
				poly=0x1c;	
				if(result & 0x80)
				{
					poly=1;
				}
				result= ((result<<1) | 0x01) ^ poly;

			}else
			{
			 	poly=0;
				if(result & 0x80)
				{
					poly=0x1D;
				}  
				result= ((result<<1)) ^ poly;

			}
			mask=mask>>1;
		}
	}
	return ~result;
}

UBYTE checksum(UBYTE *data, UBYTE len)
{
	UBYTE i;
	UBYTE sum;

	sum=0;
	for(i=0; i<len; i++)
	{
		sum=sum+data[i];
	}
	return sum;
}

//does crc/checksum error checking
//0 no errors
UBYTE check_errors(UBYTE *data, UBYTE len, UBYTE connection)
{
	//next lets check the CRC and/or check sum
	if (connection==VPW || connection==PWM)
	{
		//we have a CRC response
		UBYTE temp;
		temp=crc(data,len-1);
		if (temp!=data[len-1])
		{
			return 1;
		}
	}else
	{
		//ISO
		//assume checksum
		UBYTE temp;
		temp=checksum(data,len-1);
		if (temp!=data[len-1])
		{
			return 2;
		}
	}
	return 0;
}

//Lets assume we have some data and now we need to parse it 
// into different ECMS. 
UBYTE process(OBDII *resp, UBYTE num_resp, UBYTE *data, UBYTE num_data, UBYTE connection)
{
	UBYTE byte1;
	UBYTE byte2;
	UBYTE done,i,j,n;
	//check vaild input
	if (num_data<5 || num_resp<1 || connection==UNKNOWN)
	{
		printf("process error %d\n\r",num_data);
		return 0;
	}

	if (connection==VPW || connection==ISO)
	{
		//first two bytes of field is 0x48 0x6B
		byte1=0x48;
		byte2=0x6B;
	}
	if (connection==PWM)
	{
		//first two bytes of field are 0x41 0x6B
		byte1=0x41;
		byte2=0x6B;
	} 

	done=0;
	i=0;
	n=0; //index in the OBDII struct
	while(!done)
	{
		//check first two bytes
		if (data[i]==byte1 && data[i+1]==byte2)
		{
			UBYTE j;
			//we can process the data for
			//first we need to know the length. 
			//to find length check CRC. 
			//printf("FOUND data i=%d\n\r",i);
			for (j=5; j<=(num_data); j++)
			{
				//printf("j=%d num_data=%d\n\r",j,num_data);
				//printf("data[j]=%X data[j+1]=%X\n\r",data[j],data[j+1]);
				if( (data[j]==byte1 && data[j+1]==byte2) || j==num_data )
				{
					//printf("checking %d %d\n\r",i,j);
					if (check_errors(&data[i],j-i,connection)==0)
					{
						UBYTE l;
						//we have vaild data
						//printf("Valid data %d %d\n\r",i,j);
						resp[n].addr=data[i+2];
						resp[n].n=j-4;
						for(l=3; l<j; l++)
						{
							resp[n].data[l-3]=data[i+l];
						}
						//i=i+j-1;
						n++;
					    if (n>num_resp || i>=num_data)
						{
							return n;
						}
					   //	j=(num_data+1);

					}
				}
			
			}
		}
		i++;
		if ((i+3)>=num_data)
		{
			done=1;
		}
	}
/*
	if (n==0)
	{
		printf("Malformed data: ");
		for(i=0; i<num_data; i++)
		{
			printf("%X ",data[i]);
		}
		printf("\n\r\n\r");
	}
*/
	return n;
}


	
//we need some way to read the codes and data from 
//obdII car
UBYTE obdII_send2(UBYTE *msg, UBYTE len_msg, OBDII *resp, UBYTE num_resp)
{
	//well we need to
	UBYTE resp_str[SIZE_RESP_STR];
	UBYTE ret,temp;
	static UBYTE cnt=0;

	ret=0;
	if (connect!=UNKNOWN)
	{
		//lets try last connection
		if (connect==ISO)
		{
			ret=iso_send(resp_str,SIZE_RESP_STR,msg,len_msg,0x33);
		}else if (connect==PWM)
		{
			ret=pwm_send(resp_str,SIZE_RESP_STR,msg,len_msg);
		}else if (connect==VPW)
		{
			ret=vpw_send(resp_str,SIZE_RESP_STR,msg,len_msg);
		} else
		{
			connect=UNKNOWN;
			ret=0;
		}
		if(ret)
		{
		   //	UBYTE i,j;
			//check data and process
			temp=process(resp,num_resp,resp_str,ret,connect);
/*
			for(j=0; j<temp; j++)
			{
				printf("ECM %X returned ",resp[j].addr);
				for(i=0; i<resp[j].n;i++)
				{
					printf("%X ",resp[j].data[i]);
				}
				printf("\n\r");
			}
*/
			return temp;
			//return 0;
		}
		cnt++;
		if (cnt<5)
			return 0;
		cnt=0;
	}
	
	//else lets try all possible connections
	printf("Checking ISO\n\r");	
   	ret=iso_send(resp_str,SIZE_RESP_STR,msg,len_msg,0x33);
	if (ret)
	{
		connect=ISO;
	}
   	if (ret==0)
	{
		printf("Checking PWM\n\r");	
		//LCD_clear();
		//sprintf(str, "Checking PWM");
		//LCD_print1(str,0);

		connect=PWM;
		ret=pwm_send(resp_str,SIZE_RESP_STR,msg,len_msg);

	}
	if (ret==0)
	{
		printf("Checking VPW\n\r");
		//LCD_clear();
		//sprintf(str, "Checking VPW");
		//LCD_print1(str,0);

		connect=VPW;
		ret=vpw_send(resp_str,SIZE_RESP_STR,msg,len_msg);
	}
	if (ret)
	{
		temp=process(resp,num_resp,resp_str,ret,connect);
/*
		for(j=0; j<temp; j++)
		{
			printf("ECM %X returned ",resp[j].addr);
			for(i=0; i<resp[j].n;i++)
			{
				printf("%X ",resp[j].data[i]);
			}
			printf("\n\r");
		}
*/
		return temp;

	}else
	{
		connect=UNKNOWN;
		return 0;
	}

}
	
UBYTE obdII_send(UBYTE *msg, UBYTE len_msg, OBDII *resp, UBYTE num_resp)
{
	UBYTE i,ret;
	UBYTE cnt;
	cnt=5;
	if (connect==PWM)
	{
		cnt=20;
	}
	i=0;
	while(i<cnt)
	{
		ret=obdII_send2(msg,len_msg,resp,num_resp);
		if(ret)
		{
			obdError=0;
			return ret;
		}
		i++;
	}
	//connect=UNKNOWN;
	obdError=1;
	return 0;
}
