/****************************************************************
@filename:data_process.c
@time:08-25
@author:pengkunfan
****************************************************************/
#ifndef davince_ipncam
#define davince_ipncam
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data_define.h"
#include "data_update.h"
#include "server_api.h"
#include <sys/stat.h>
#include <unistd.h>
#include "./webparam/xml_op.h"


#include <sys/socket.h>
#include <sys/un.h>
#include "msg_type.h"
/*lishi start*/
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>


#define RYK_TEST      1


#if RYK_TEST
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>

#define OSD_FILE     "/home/ubuntu/var/www/cgi-bin/osd.xml"
#define ARITHMETIC_FILE     "/home/ubuntu/var/www/cgi-bin/arithmetic.xml"

#define STR_LEN        16
#define OSD_STR_LEN         100

#endif

static int fifoFd = 0;
static int _have_init = 0;
void usage(){
	printf("./writeFifo 0  recv decode audio\n"
		   "./writeFifo 1  recv PC audio\n"
		   "./writeFifo 2  recv Android audio\n");
}
#if RYK_TEST
typedef struct osd_t{
		char str[OSD_STR_LEN];
		char size[STR_LEN];
		char color[STR_LEN];
		char x[STR_LEN];
		char y[STR_LEN];
		char device_addr[OSD_STR_LEN];
		char time[STR_LEN];
}osd;



 static checknodeornull(char * str)
 {
	  int IsEmpty = 0;
	  int length = strlen(str);
	  int n;
	 
	  for(n=0;n<length;n++)
	  {
			  if(str[n]==32)
					  IsEmpty=1;
			  else
			  {
					  IsEmpty=0;
					  break;
			  }
	  }
	  if(length == 0)
	  {
			  return  0;
	  }
	  else if(IsEmpty)
	  {
			  return 0;
	  }
	  else
	  {
			  return 1;
	  }
  
 
 
 }
 

 static void set_osd_item(osd *osd_item,osd *osd_setting) 
 {
 
		 assert(osd_item); 
 
		 /*osd_item->num = 1; 
		 snprintf(osd_item->str, OSD_STR_LEN, "%s", "³õÊ¼»¯ÖÐ"); 
		 snprintf(osd_item->size, STR_LEN, "%s", "4"); 
		 snprintf(osd_item->color, STR_LEN, "%s", "1"); 
		 snprintf(osd_item->x, STR_LEN, "%s", "50"); 
		 snprintf(osd_item->y, STR_LEN, "%s", "50"); */

		 
		

		 
		 snprintf(osd_item->str, OSD_STR_LEN, "%s", osd_setting->str);		 
		 snprintf(osd_item->size, STR_LEN, "%s", osd_setting->size);		 
		 snprintf(osd_item->color, STR_LEN, "%s",osd_setting->color);
		 snprintf(osd_item->x, STR_LEN, "%s", osd_setting->x);		 
		 snprintf(osd_item->y, STR_LEN, "%s", osd_setting->y);
		 snprintf(osd_item->device_addr, OSD_STR_LEN, "%s", osd_setting->device_addr);		 
		 snprintf(osd_item->time, STR_LEN, "%s", osd_setting->time);


		 if(checknodeornull(osd_item->str)==0)
		 {
			snprintf(osd_item->str, OSD_STR_LEN, "%s", "null");
		 }

		 if(checknodeornull(osd_item->size)==0)
		 {
			snprintf(osd_item->size, OSD_STR_LEN, "%s", "null");
		 }

		 if(checknodeornull(osd_item->color)==0)
		 {
			snprintf(osd_item->color, OSD_STR_LEN, "%s", "null");
		 }

		 if(checknodeornull(osd_item->x)==0)
		 {
			snprintf(osd_item->x, OSD_STR_LEN, "%s", "null");
		 }

		  if(checknodeornull(osd_item->y)==0)
		 {
			snprintf(osd_item->y, OSD_STR_LEN, "%s", "null");
		 }

		  if(checknodeornull(osd_item->device_addr)==0)
		 {
			snprintf(osd_item->device_addr, OSD_STR_LEN, "%s", "null");
		 }

		  if(checknodeornull(osd_item->time)==0)
		 {
			snprintf(osd_item->time, OSD_STR_LEN, "%s", "null");
		 }
		
		 
 }



 
 static xmlNodePtr create_osd_node(const osd *osd_item)
 {
		assert(osd_item);

		char num[STR_LEN] = {0};
		xmlNodePtr osd_node = NULL;

		osd_node = xmlNewNode(NULL, BAD_CAST"osd");
		if (osd_node == NULL)
		{
				fprintf(stderr, "Failed to create new node.\n");
				return NULL;
		}



		

		xmlNewChild(osd_node, NULL, BAD_CAST"str", (xmlChar *)osd_item->str);
		xmlNewChild(osd_node, NULL, BAD_CAST"size", (xmlChar *)osd_item->size);
		xmlNewChild(osd_node, NULL, BAD_CAST"color", (xmlChar *)osd_item->color);
		xmlNewChild(osd_node, NULL, BAD_CAST"x", (xmlChar *)osd_item->x);
		xmlNewChild(osd_node, NULL, BAD_CAST"y", (xmlChar *)osd_item->y);
		xmlNewChild(osd_node, NULL, BAD_CAST"device_addr", (xmlChar *)osd_item->device_addr);
		xmlNewChild(osd_node, NULL, BAD_CAST"time", (xmlChar *)osd_item->time);
		

		return osd_node;
}

 static int add_osd_node_to_root(xmlNodePtr root_node,osd *osd_setting) 
 {
 
		 xmlNodePtr osd_node = NULL; 
		 osd *osd_item = NULL; 
		
 
		 osd_item = (osd *)malloc(sizeof(osd)); 
		 if (osd_item == NULL) 
		 { 
				 fprintf(stderr, "Failed to malloc memory.\n"); 
				 return -1; 
		 }
		

		
 
		 set_osd_item(osd_item,osd_setting);
		 osd_node = create_osd_node(osd_item);  
		 if (osd_node == NULL) 
		 {
 
				 fprintf(stderr, "Failed to create osd node.\n"); 
				 goto FAILED; 
		 } 
 
		 xmlAddChild(root_node, osd_node); 
		 free(osd_item); 
 
		 return 0;
 
 FAILED:
 
		 if (osd_item) 
		 { 
				 free(osd_item); 
		 }
 
		 return -1;
 
 }


 
 static int create_osd_block(const char *osd_block_file,osd *osd_setting) 
 {
 
		 assert(osd_block_file); 
 
		 xmlDocPtr doc = NULL; 
		 xmlNodePtr root_node = NULL; 
 
		 // create xml file
 
		 doc = xmlNewDoc(BAD_CAST"1.0");
 
		 if (doc == NULL) 
		 { 
				 fprintf(stderr, "Failed to new doc.\n"); 
				 return -1; 
		 }
 
 
 
		 // create root node
 
		 root_node = xmlNewNode(NULL, BAD_CAST"osd_block"); 
		 if (root_node == NULL) 
		 { 
				 fprintf(stderr, "Failed to new root node.\n"); 
				 goto FAILED; 
		 }
 
		 // add root node to doc
 
		 xmlDocSetRootElement(doc, root_node); 
 
		 if (add_osd_node_to_root(root_node,osd_setting) != 0) 
		 {
 
				 fprintf(stderr, "Failed to add a new osd node.\n"); 
				 goto FAILED;
 
		 }
 
			/*
 
			xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"son");
 
			xmlAddChild(root_node,node);
 
			xmlNodePtr grandson = xmlNewNode(NULL,BAD_CAST"grandson");
 
			xmlAddChild(node,grandson);
 
			xmlAddChild(grandson, xmlNewText(BAD_CAST"This is a grandson node"));
 
			*/
 
 
 
		 // add doc to xml file 
 
		 xmlSaveFormatFileEnc(osd_block_file, doc, "UTF-8", 1); 
		 //xmlSaveFile("test.xml", doc); 
		 xmlFreeDoc(doc); 
 
		 return 0; 
 
 FAILED:
 
		 if (doc) 
		 { 
				 xmlFreeDoc(doc); 
		 }
  
		 return -1;
 
 }
 
 
 static int add_osd_node(const char *osd_block_file,osd *osd_setting) 
 { 
		 assert(osd_block_file); 
 
		 xmlDocPtr doc = NULL; 
		 xmlNodePtr root_node = NULL; 
		 xmlNodePtr osd_node = NULL; 
		 osd *osd_item = NULL; 
 
		 xmlKeepBlanksDefault(0) ;//libxml2 global variable . 
		 xmlIndentTreeOutput = 1 ;// indent .with \n 
	 
 
		 //doc = xmlParseFile(osd_block_file);
 
		 doc = xmlReadFile(osd_block_file, NULL, XML_PARSE_NOBLANKS); 
		 if (doc == NULL) 
		 { 
				 fprintf(stderr, "Failed to parser xml file:%s\n", osd_block_file); 
				 return -1; 
		 }
 
 
		 root_node = xmlDocGetRootElement(doc); 
		 if (root_node == NULL) 
		 { 
				 fprintf(stderr, "Failed to get root node.\n"); 
				 goto FAILED; 
		 } 
 
		 if (add_osd_node_to_root(root_node,osd_setting) != 0) 
		 { 
				 fprintf(stderr, "Failed to add a new osd node.\n"); 
				 goto FAILED; 
		 }
 
 
		 xmlSaveFormatFileEnc(osd_block_file, doc, "UTF-8", 1); 
		 xmlFreeDoc(doc); 
 
		 return 0;
 
 FAILED:
 
		 if (doc) 
		 { 
				 xmlFreeDoc(doc); 
		 } 
		 
		 return -1;
 
 }

static xmlXPathObjectPtr getNodeset(xmlDocPtr doc,const xmlChar *xpath)
{
		xmlXPathContextPtr context;
		xmlXPathObjectPtr result;
		context = xmlXPathNewContext(doc);

		if(context == NULL)
		{
				printf("context is NULL\n");
				return NULL;
		}

		result = xmlXPathEvalExpression(xpath,context);
		xmlXPathFreeContext(context);
		if(result == NULL)
		{
				printf("xmlXPathEvalExpression return NULL\n");
		}

		if(xmlXPathNodeSetIsEmpty(result->nodesetval))
		{
				xmlXPathFreeObject(result);
				printf("nodeset is empty\n");
				return NULL;	
		}

		return result;
}


static int modify_osd_node(const char *osd_block_file,osd *osd_setting)
{

		assert(osd_block_file);



		xmlDocPtr doc = NULL;
		xmlNodePtr root_node = NULL;
		xmlNodePtr osd_node = NULL;
		osd *osd_item = NULL;




		xmlKeepBlanksDefault(0) ;//libxml2 global variable .
		xmlIndentTreeOutput = 1 ;// indent .with \n 	

	

		doc = xmlReadFile(osd_block_file, NULL, XML_PARSE_NOBLANKS);

		if (doc == NULL)
		{
				fprintf(stderr, "Failed to parser xml file:%s\n", osd_block_file);
				return -1;
		}



		root_node = xmlDocGetRootElement(doc);
		if (root_node == NULL)
		{
				fprintf(stderr, "Failed to get root node.\n");
				goto FAILED;
		}


/********************ii*/

		
	   

		osd_item = (osd *)malloc(sizeof(osd)); 
		if (osd_item == NULL) 
		{ 
				fprintf(stderr, "Failed to malloc memory.\n"); 
				return -1; 
		}
	   

	   

		set_osd_item(osd_item,osd_setting);



/********************ii**/





		/***************/
#if 1
		xmlChar *xpath = ("/osd_block/osd");
		xmlXPathObjectPtr  app_result = getNodeset(doc,xpath);
		if(app_result ==NULL)
		{
				printf("app_result is NULL\n");
				return ;
		}

		int i =0;

		xmlChar *values;
		if(app_result)
		{
				xmlNodeSetPtr nodeset = app_result->nodesetval;
				xmlNodePtr cur2;

				for(i=0;i<nodeset->nodeNr;i++)
				{
						root_node = nodeset->nodeTab[i];
						root_node = root_node->xmlChildrenNode;
						while(root_node != NULL)
						{



								if(!xmlStrcmp(root_node->name, (const xmlChar *)"str"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)osd_item->str);
								}
								else
								{
										//printf("error !!\n");
								}	

								if(!xmlStrcmp(root_node->name, (const xmlChar *)"size"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)osd_item->size);
								}
								else
								{
										//printf("error !!\n");
								}
								
								if(!xmlStrcmp(root_node->name, (const xmlChar *)"color"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)osd_item->color);
								}
								else
								{
										//printf("error !!\n");
								}	
									if(!xmlStrcmp(root_node->name, (const xmlChar *)"x"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)osd_item->x);
								}
								else
								{
										//printf("error !!\n");
								}	
									if(!xmlStrcmp(root_node->name, (const xmlChar *)"y"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)osd_item->y);
								}
								else
								{
										//printf("error !!\n");
								}	
									if(!xmlStrcmp(root_node->name, (const xmlChar *)"device_addr"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)osd_item->device_addr);
								}
								else
								{
										//printf("error !!\n");
								}	
									if(!xmlStrcmp(root_node->name, (const xmlChar *)"time"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)osd_item->time);
								}
								else
								{
										//printf("error !!\n");
								}	
								

								root_node = root_node->next;
						}
				}

				xmlXPathFreeObject(app_result);
		}


#endif	

		/********************/


		xmlSaveFormatFileEnc(osd_block_file, doc, "UTF-8", 1);
		xmlFreeDoc(doc);

		return 0;

FAILED:

		if (doc)
		{
				xmlFreeDoc(doc);
		}

		return -1;  

}




 






#endif  


#if RYK_TEST  // arithmetic
typedef struct arithmetic_t{		
		char arithmeticpara1[STR_LEN];
		char arithmeticpara2[STR_LEN];
		char arithmeticpara3[STR_LEN];
		char arithmetic_recvideo[STR_LEN];

}arithmetic;

 static void set_arithmetic_item(arithmetic *arithmetic_item,arithmetic *arithmetic_setting) 
 {
 
		 assert(arithmetic_item); 
 
		 
		 snprintf(arithmetic_item->arithmeticpara1, STR_LEN, "%s",arithmetic_setting->arithmeticpara1);
		 snprintf(arithmetic_item->arithmeticpara2, STR_LEN, "%s", arithmetic_setting->arithmeticpara2);		 
		 snprintf(arithmetic_item->arithmeticpara3, STR_LEN, "%s", arithmetic_setting->arithmeticpara3);
		 snprintf(arithmetic_item->arithmetic_recvideo, STR_LEN, "%s", arithmetic_setting->arithmetic_recvideo);


		 
		 if(checknodeornull(arithmetic_item->arithmeticpara1)==0)
		 {
				snprintf(arithmetic_item->arithmeticpara1, STR_LEN, "%s", "null");
		 }

		 if(checknodeornull(arithmetic_item->arithmeticpara2)==0)
		 {
				snprintf(arithmetic_item->arithmeticpara2, STR_LEN, "%s", "null");
		 }

		   if(checknodeornull(arithmetic_item->arithmeticpara3)==0)
		 {
				snprintf(arithmetic_item->arithmeticpara3, STR_LEN, "%s", "null");
		 }

		    if(checknodeornull(arithmetic_item->arithmetic_recvideo)==0)
		 {
				snprintf(arithmetic_item->arithmetic_recvideo, STR_LEN, "%s", "null");
		 }
		 
		 
		
		 
 }
 
 static xmlNodePtr create_arithmetic_node(const arithmetic *arithmetic_item)
 {
		assert(arithmetic_item);

		
		xmlNodePtr arithmetic_node = NULL;

		arithmetic_node = xmlNewNode(NULL, BAD_CAST"arithmetic");
		if (arithmetic_node == NULL)
		{
				fprintf(stderr, "Failed to create new node.\n");
				return NULL;
		}		

		xmlNewChild(arithmetic_node, NULL, BAD_CAST"arithmeticpara1", (xmlChar *)arithmetic_item->arithmeticpara1);
		xmlNewChild(arithmetic_node, NULL, BAD_CAST"arithmeticpara2", (xmlChar *)arithmetic_item->arithmeticpara2);
		xmlNewChild(arithmetic_node, NULL, BAD_CAST"arithmeticpara3", (xmlChar *)arithmetic_item->arithmeticpara3);
		xmlNewChild(arithmetic_node, NULL, BAD_CAST"arithmetic_recvideo", (xmlChar *)arithmetic_item->arithmetic_recvideo);


		return arithmetic_node;
}

 static int add_arithmetic_node_to_root(xmlNodePtr root_node,arithmetic *arithmetic_setting) 
 {
 
		 xmlNodePtr arithmetic_node = NULL; 
		 arithmetic *arithmetic_item = NULL; 
		
 
		 arithmetic_item = (arithmetic *)malloc(sizeof(arithmetic)); 
		 if (arithmetic_item == NULL) 
		 { 
				 fprintf(stderr, "Failed to malloc memory.\n"); 
				 return -1; 
		 }
		

		
 
		 set_arithmetic_item(arithmetic_item,arithmetic_setting);
		 arithmetic_node = create_arithmetic_node(arithmetic_item);  
		 if (arithmetic_node == NULL) 
		 {
 
				 fprintf(stderr, "Failed to create arithmetic node.\n"); 
				 goto FAILED; 
		 } 
 
		 xmlAddChild(root_node, arithmetic_node); 
		 free(arithmetic_item); 
 
		 return 0;
 
 FAILED:
 
		 if (arithmetic_item) 
		 { 
				 free(arithmetic_item); 
		 }
 
		 return -1;
 
 }


 
 static int create_arithmetic_block(const char *arithmetic_block_file,arithmetic *arithmetic_setting) 
 {
 
		 assert(arithmetic_block_file); 
 
		 xmlDocPtr doc = NULL; 
		 xmlNodePtr root_node = NULL; 
 
		 // create xml file
 
		 doc = xmlNewDoc(BAD_CAST"1.0");
 
		 if (doc == NULL) 
		 { 
				 fprintf(stderr, "Failed to new doc.\n"); 
				 return -1; 
		 }
 
 
 
		 // create root node
 
		 root_node = xmlNewNode(NULL, BAD_CAST"arithmetic_block"); 
		 if (root_node == NULL) 
		 { 
				 fprintf(stderr, "Failed to new root node.\n"); 
				 goto FAILED; 
		 }
 
		 // add root node to doc
 
		 xmlDocSetRootElement(doc, root_node); 
 
		 if (add_arithmetic_node_to_root(root_node,arithmetic_setting) != 0) 
		 {
 
				 fprintf(stderr, "Failed to add a new arithmetic node.\n"); 
				 goto FAILED;
 
		 }
 
 
 
		 // add doc to xml file 
 
		 xmlSaveFormatFileEnc(arithmetic_block_file, doc, "UTF-8", 1); 
		 //xmlSaveFile("test.xml", doc); 
		 xmlFreeDoc(doc); 
 
		 return 0; 
 
 FAILED:
 
		 if (doc) 
		 { 
				 xmlFreeDoc(doc); 
		 }
  
		 return -1;
 
 }
 
 
 static int add_arithmetic_node(const char *arithmetic_block_file,arithmetic *arithmetic_setting) 
 { 
		 assert(arithmetic_block_file); 
 
		 xmlDocPtr doc = NULL; 
		 xmlNodePtr root_node = NULL; 
		 xmlNodePtr arithmetic_node = NULL; 
		 arithmetic *arithmetic_item = NULL; 
 
		 xmlKeepBlanksDefault(0) ;//libxml2 global variable . 
		 xmlIndentTreeOutput = 1 ;// indent .with \n 
	 
 
		 
		 doc = xmlReadFile(arithmetic_block_file, NULL, XML_PARSE_NOBLANKS); 
		 if (doc == NULL) 
		 { 
				 fprintf(stderr, "Failed to parser xml file:%s\n", arithmetic_block_file); 
				 return -1; 
		 }
 
 
		 root_node = xmlDocGetRootElement(doc); 
		 if (root_node == NULL) 
		 { 
				 fprintf(stderr, "Failed to get root node.\n"); 
				 goto FAILED; 
		 } 
 
		 if (add_arithmetic_node_to_root(root_node,arithmetic_setting) != 0) 
		 { 
				 fprintf(stderr, "Failed to add a new arithmetic node.\n"); 
				 goto FAILED; 
		 }
 
 
		 xmlSaveFormatFileEnc(arithmetic_block_file, doc, "UTF-8", 1); 
		 xmlFreeDoc(doc); 
 
		 return 0;
 
 FAILED:
 
		 if (doc) 
		 { 
				 xmlFreeDoc(doc); 
		 } 
		 
		 return -1;
 
 }



static int modify_arithmetic_node(const char *arithmetic_block_file,osd *arithmetic_setting)
{

		assert(arithmetic_block_file);



		xmlDocPtr doc = NULL;
		xmlNodePtr root_node = NULL;
		xmlNodePtr arithmetic_node = NULL;
		arithmetic *arithmetic_item = NULL;




		xmlKeepBlanksDefault(0) ;//libxml2 global variable .
		xmlIndentTreeOutput = 1 ;// indent .with \n 	

	

		doc = xmlReadFile(arithmetic_block_file, NULL, XML_PARSE_NOBLANKS);

		if (doc == NULL)
		{
				fprintf(stderr, "Failed to parser xml file:%s\n", arithmetic_block_file);
				return -1;
		}



		root_node = xmlDocGetRootElement(doc);
		if (root_node == NULL)
		{
				fprintf(stderr, "Failed to get root node.\n");
				goto FAILED;
		}


/********************ii*/

		
	   

		arithmetic_item = (arithmetic *)malloc(sizeof(arithmetic)); 
		if (arithmetic_item == NULL) 
		{ 
				fprintf(stderr, "Failed to malloc memory.\n"); 
				return -1; 
		}
	   

	   

		set_arithmetic_item(arithmetic_item,arithmetic_setting);



/********************ii**/





		/***************/
#if 1
		xmlChar *xpath = ("/arithmetic_block/arithmetic");
		xmlXPathObjectPtr  app_result = getNodeset(doc,xpath);
		if(app_result ==NULL)
		{
				printf("app_result is NULL\n");
				return ;
		}

		int i =0;

		xmlChar *values;
		if(app_result)
		{
				xmlNodeSetPtr nodeset = app_result->nodesetval;
				xmlNodePtr cur2;

				for(i=0;i<nodeset->nodeNr;i++)
				{
						root_node = nodeset->nodeTab[i];
						root_node = root_node->xmlChildrenNode;
						while(root_node != NULL)
						{



								if(!xmlStrcmp(root_node->name, (const xmlChar *)"arithmeticpara1"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)arithmetic_item->arithmeticpara1);
								}
								else
								{
										//printf("error !!\n");
								}	

								if(!xmlStrcmp(root_node->name, (const xmlChar *)"arithmeticpara2"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)arithmetic_item->arithmeticpara2);
								}
								else
								{
										//printf("error !!\n");
								}

								if(!xmlStrcmp(root_node->name, (const xmlChar *)"arithmeticpara3"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)arithmetic_item->arithmeticpara3);
								}
								else
								{
										//printf("error !!\n");
								}


								if(!xmlStrcmp(root_node->name, (const xmlChar *)"arithmetic_recvideo"))
								{
										printf("%s\n",((char*)XML_GET_CONTENT(root_node->xmlChildrenNode)));
										xmlNodeSetContent(root_node,(const xmlChar *)arithmetic_item->arithmetic_recvideo);
								}
								else
								{
										//printf("error !!\n");
								}

								root_node = root_node->next;
						}
				}

				xmlXPathFreeObject(app_result);
		}


#endif	

		/********************/


		xmlSaveFormatFileEnc(arithmetic_block_file, doc, "UTF-8", 1);
		xmlFreeDoc(doc);

		return 0;

FAILED:

		if (doc)
		{
				xmlFreeDoc(doc);
		}

		return -1;  

}




 


#endif  //end test  



 static int init_fifo(char *fifoName){
	if(_have_init <= 0){
        fifoFd = open(fifoName,O_WRONLY|O_NONBLOCK);
        if(fifoFd < 0){
          printf("write open error\n");
          return -1;
        }
        _have_init=1;
      }
    return 1;
}

int send_client_type_cmd(char *type)
{
	int ret = 0;
	char *fifoName="/home/ubuntu/var/config_fifo";
	/*if(argc != 2){
		usage();
		return -1;
	}*/
	if(init_fifo(fifoName)<0){
		return -1;
	}

	ret = write(fifoFd,type,1);
	if(ret != 1){
		perror("write");
	}
	return 0;
}

/*lishi end*/

#define UNIX_DOMAIN "/tmp/UNIX.domain"


#define R_DB(fmt, args...)	fprintf(stderr, fmt, ##args)
extern url_option url_option_table_time [];
extern url_option url_option_table_ipaddr [];
#endif

//static void write_param(system_param my_param);


int send_set_bitrate_cmd(void)
{

  int connect_fd;
  struct sockaddr_un srv_addr;
  char snd_buf[1024];
  char rcv_buf[1024];
  int ret;
  int i;
  MSG_BUF msg_buf;
  

  //create client socket

  connect_fd = socket(PF_UNIX, SOCK_STREAM, 0);

  if(connect_fd < 0)
  {
    perror("client create socket failed");
    return 1;

  }

  

  //set server sockaddr_un

  srv_addr.sun_family = AF_UNIX;
  strcpy(srv_addr.sun_path, UNIX_DOMAIN);

  

  //connect to server

  ret = connect(connect_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));

  if(ret == -1)
  {
    perror("connect to server failed!");
    close(connect_fd);
    unlink(UNIX_DOMAIN);
    return 1;

  }

  

  //receive message from server

  memset((void *)&msg_buf, 0, sizeof(msg_buf));
 // int rcv_num = read(connect_fd, rcv_buf, sizeof(rcv_buf));
//  printf("receive message from server (%d) :%s\n", rcv_num, rcv_buf);

  //send message to server
  //(snd_buf, 0, 1024);
  //strcpy(snd_buf, "qjq message from client");
  //printf("sizeof(snd_buf): %d\n", sizeof(snd_buf));

  //sleep(2000);



  //send message to server

 // for(i = 0; i < 4; i++)
 // {
 	msg_buf.m_type = MSG_SET_BIT_RATE;
 	msg_buf.buf_len = 8;
	int bit_rate = decoder_param.en_param.bit_rate;
	int bit_ctrl = decoder_param.en_param.bit_ctrl;
	memcpy((void *)msg_buf.buf, (void *)&bit_rate,sizeof(bit_rate));
	memcpy((void *)msg_buf.buf + sizeof(bit_rate), (void *)&bit_ctrl,sizeof(bit_ctrl));
    write(connect_fd, &msg_buf, sizeof(msg_buf));
 //}

  close(connect_fd);

  return 0;

}





/*
static unsigned char *get_section_name (char *section_line)
{
	char *current;
	unsigned char *name;

	if (!section_line)
		return NULL;

	current = section_line;

	while (*current == ' ' || *current == '\t')
		current++;

	if (*current == ';' || *current == '#')
		return NULL;

	if (*current++ == '[')
		while (*current == ' ' || *current == '\t')
			current++;
	else
		return NULL;

	name = current;
	while (*current != ']' && *current != '\n' &&
		   *current != ';' && *current != '#' && *current != '\0')
		current++;
	*current = '\0';
	while (*current == ' ' || *current == '\t')
	  {
		  *current = '\0';
		  current--;
	  }

	return name;
}*/


static int get_ulr_addr(char *dest_addr,char *net_addr,int buffersize)
{
	unsigned char *current;
	unsigned char *dest;
	int ret = 0;
 
	if (!net_addr)
		return -1;

	current = (unsigned char *)net_addr;
	dest = (unsigned char *)dest_addr;
	while (*current == ' ' || *current == '\t')
		current++;
	//printf("current = %s \n",current);
	//rtsp://192.168.1.101:8558/h264
	
	// rtsp%3A%2F%2F192.168.1.101%3A8558%2Fh264
	while(buffersize>0)
	{
		if(*current=='%')
		{
			//rtsp %3A %2F%2F 192.168.0.102 %2F 1.264 %3a=(:)  %2f=(/)  ---%3f=(?)  %3d=(=) %26=(&)
			if(*(current+1)=='3'&& *(current+2)=='A')
			{
				*dest_addr++ = ':';
				current = current + 3;
				buffersize = buffersize -3;
			}
			else if(*(current+1)=='2'&&*(current+2)=='F')
			{
				*dest_addr++ = '/';
				current = current + 3;
				buffersize = buffersize -3;
			}
			else if(*(current+1)=='3'&&*(current+2)=='F')
			{
				*dest_addr++ = '?';
				current = current + 3;
				buffersize = buffersize -3;
			}
			else if(*(current+1)=='3'&&*(current+2)=='D')
			{
				*dest_addr++ = '=';
				current = current + 3;
				buffersize = buffersize -3;
			}
			else if(*(current+1)=='2'&&*(current+2)=='6')
			{
				*dest_addr++ = '&';
				current = current + 3;
				buffersize = buffersize -3;
			}
			else if(*(current+1)=='4'&&*(current+2)=='0')
			{
				*dest_addr++ = '@';
				current = current + 3;
				buffersize = buffersize -3;
			}
			
			continue;
		}
		//printf("%s",dest);
		*dest_addr++ = *current++;
		buffersize--;
		
	}


	
	dest_addr = (char *)dest;
	return ret;
}

static void read_param();

/****************************************************************
@function:reply_display
@brief:display html
@time:08-25
@author:pengkunfan
****************************************************************/
void reply_display(int html_id)
{
	int s_id=html_id;
	switch(s_id)
	{
		case 0:
		
		break;
		break;
		case 4:
			get_ipaddr();
		break;
		case 5:
			
		break;
		case 6:
			get_rtspaddr();
		break;
		default:
		break;
	}
	return;
}

/****************************************************************
@function:date_process
@brief:process date and time
@time:08-25
@author:
****************************************************************/
int date_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;
	int hour,min,sec;
	
	COMMAND_ARGUMENT *argm=(COMMAND_ARGUMENT*)malloc(sizeof(COMMAND_ARGUMENT));
	
		argm->value=strdup(in_buf[0].value);
		
		hour=atoi(in_buf[1].value);
		min=atoi(in_buf[2].value);
		sec=atoi(in_buf[3].value);//6ge
		
		url_option_table_time[0].handler(in_buf[0].name, argm);
	
		/*
		argm->value=strdup(in_buf[4].value);
		url_option_table_time[2].handler(in_buf[4].name, argm);
		argm->value=strdup(in_buf[5].value);
		url_option_table_time[3].handler(in_buf[5].name, argm);
		*/
		sys_set_time(hour,min,sec);
	return ret;
}



/****************************************************************
@function:enc_param_process
@brief:process enc
@time:
@author:
****************************************************************/
int enc_param_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;
	char value[32] = {0};
	
	//read_param();

	if(strcmp(in_buf[1].value,"one")==0)
	{
		//h_print();
		//printf("1080p  !!!\n");
		decoder_param.en_param.resolution = 0;//for 1080p
	}
	else if(strcmp(in_buf[1].value,"two")==0)
	{
		//printf("720p  !!!\n");
		decoder_param.en_param.resolution = 1;//for 720p
	}
	else if(strcmp(in_buf[1].value,"three")==0)
	{
		//printf("720p  !!!\n");
		decoder_param.en_param.resolution = 2;//for 1080i
	}
	else if(strcmp(in_buf[1].value,"four")==0)
	{
		//printf("480p  !!!\n");
		decoder_param.en_param.resolution = 3;//for 480p
	}


	if(strcmp(in_buf[2].value,"one")==0)
	{
		//printf("  60fps  !!!\n");
		decoder_param.en_param.fps = 0;//for 60fsp
	}
	else if(strcmp(in_buf[2].value,"two")==0)
	{
		//printf("  30fps  !!!\n");
		decoder_param.en_param.fps = 1;//for 30fps
	}
	else if(strcmp(in_buf[2].value,"three")==0)
	{
		decoder_param.en_param.fps = 2;//for 25fps
	}


	if(strcmp(in_buf[3].value,"one")==0)
	{
		decoder_param.en_param.bit_ctrl = 0;//
	}
	else if(strcmp(in_buf[3].value,"two")==0)
	{
		decoder_param.en_param.bit_ctrl = 1;
	}
	

	decoder_param.en_param.bit_rate = atoi(in_buf[0].value);
#if 0
	FILE *p_fd;
	p_fd=fopen("sys.cfg","wb");
	ret=fwrite(&decoder_param,1,sizeof(system_param),p_fd);
	fclose(p_fd);
	system("sync");
	ret=0;

#else
	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.en_param.bit_rate);
	save_to_xml("enc.bitrate",value);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.en_param.resolution);
	save_to_xml("enc.resolution",value);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.en_param.fps);
	save_to_xml("enc.fps",value);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.en_param.bit_ctrl);
	save_to_xml("enc.bitctrl",value);
	send_set_bitrate_cmd();
#endif
	return ret;
}


/****************************************************************
@function:client_param_process
@brief:process 
@time:
@author:
****************************************************************/
int client_param_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;

	char value[32] = {0};
	//read_param();

	if(strcmp(in_buf[0].value,"one")==0)
	{
		//printf("PC client\n");
		decoder_param.client_type= 0;
		send_client_type_cmd("0");
	}
	else if(strcmp(in_buf[0].value,"two")==0)
	{
		//printf("decode client\n");
		decoder_param.client_type= 1;
		send_client_type_cmd("1");
	}
	
	else if(strcmp(in_buf[0].value,"three")==0)
	{
		//printf("Android client\n");
		decoder_param.client_type= 2;
		send_client_type_cmd("2");
	}
#if 0
	FILE *p_fd;
	p_fd=fopen("sys.cfg","wb");
	ret=fwrite(&decoder_param,1,sizeof(system_param),p_fd);
	fclose(p_fd);
	system("sync");
	ret=0;
#else
	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.client_type);
	save_to_xml("client.talk_back_type",value);
#endif
	return ret;
}




/****************************************************************
@function:audio_param_process
@brief:process audio
@time:
@author:
****************************************************************/
int audio_param_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;
	int pb_volume = 0;
	int r_volume = 0;
	char cmd[64] = {0};

	//read_param();
	char value[32] = {0};
	
	pb_volume = atoi(in_buf[0].value);
	r_volume = atoi(in_buf[1].value);
	
	if(pb_volume > 100)
		pb_volume = 100;
	if(pb_volume < 0)
		pb_volume = 0;

	if(r_volume > 100)
		r_volume = 100;
	if(r_volume < 0)
		r_volume = 0;


	decoder_param.pb_volume = pb_volume;
	decoder_param.r_volume= r_volume;
#if 0

	FILE *p_fd;
	p_fd=fopen("sys.cfg","wb");
	ret=fwrite(&decoder_param,1,sizeof(system_param),p_fd);
	fclose(p_fd);
	system("sync");
#else
	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.pb_volume);
	save_to_xml("audio.pb_volume",value);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.r_volume);
	save_to_xml("audio.r_volume",value);
#endif
	

	sprintf(cmd, "amixer cset name='PCM Playback Volume' %d%%", pb_volume);
	system(cmd);

	memset(cmd, 0 , sizeof(cmd));
	r_volume = (r_volume * 12) / 10;
	sprintf(cmd, "amixer  cset numid=37 %d", r_volume);
	system(cmd);
	
	printf("%s\n", cmd);
	return 0;
}


int display_param_process(entry_buf in_buf[],int cont_value)
{


	//read_param();
	char value[32] = {0};
	unsigned char file_buffer[1024*512];
	int start_opencv =0;


	if(strcmp(in_buf[1].value,"one")==0)
	{
		printf(" opencv is open !!!\n");
		decoder_param.start_opencv= 1;
	}
	else if(strcmp(in_buf[1].value,"two")==0)
	{
		printf("opencv is off  !!!\n");
		decoder_param.start_opencv= 0;
	}


	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.start_opencv);
	save_to_xml("rtsp.opencv",value);

	
	//memset(value, 0 ,sizeof(value));
	//sprintf(value, "%d", decoder_param.rtsp_url);
	//save_to_xml("rtsp.rtsp_url",in_buf[0].value);




	//strcpy(decoder_param.rtsp_url, (in_buf[0].value));

	//printf("%s\n",decoder_param.rtsp_url);
	
	//save_to_xml("rtsp.rtsp_url",(char * )decoder_param.rtsp_url);

	

	COMMAND_ARGUMENT *argm=(COMMAND_ARGUMENT*)malloc(sizeof(COMMAND_ARGUMENT));
		
	argm->value=strdup(in_buf[0].value);
	get_ulr_addr((char *)file_buffer,argm->value,strlen(argm->value));
	printf("%s \n",file_buffer);
	printf("%s \n",file_buffer);
	strcpy((char *)decoder_param.rtsp_url,(char *)file_buffer);


	

	printf("%s\n",decoder_param.rtsp_url);
	
	save_to_xml("rtsp.rtsp_url",(char * )decoder_param.rtsp_url);


	return 0;
}

/****************************************************************
@function:arithmetic_param_process
@brief:process arithmetic
@time:
@author:
****************************************************************/
int arithmetic_param_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;
	int arithmeticpara1 = 0;
	int arithmeticpara2 = 0;
	int arithmeticpara3 = 0;
	int arithmetic_recvideo = 0;
	char cmd[64] = {0};
	char srecvideo[STR_LEN]={0};
	//read_param();
	char value[32] = {0};



	
	if(strcmp(in_buf[3].value,"one")==0)
	{	
		decoder_param.arithmetic_recvideo= 1;//for rec video  is open
	}
	else if(strcmp(in_buf[3].value,"two")==0)
	{
		decoder_param.arithmetic_recvideo = 0;//for rec video  is off
	}




	
	arithmeticpara1 = atoi(in_buf[0].value);
	arithmeticpara2 = atoi(in_buf[1].value);
	arithmeticpara3 = atoi(in_buf[2].value);	


	decoder_param.arithmeticpara1 = arithmeticpara1;
	decoder_param.arithmeticpara2= arithmeticpara2;
	decoder_param.arithmeticpara3= arithmeticpara3;
#if 0

	FILE *p_fd;
	p_fd=fopen("sys.cfg","wb");
	ret=fwrite(&decoder_param,1,sizeof(system_param),p_fd);
	fclose(p_fd);
	system("sync");
#else
	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.arithmeticpara1);
	save_to_xml("arithmetic.arithmeticpara1",value);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.arithmeticpara2);
	save_to_xml("arithmetic.arithmeticpara2",value);

	
	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.arithmeticpara3);
	save_to_xml("arithmetic.arithmeticpara3",value);


	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.arithmetic_recvideo);
	save_to_xml("arithmetic.arithmetic_recvideo",value);
	
#endif
	


/******/
#if RYK_TEST
	
		char *arithmetic_block_file = ARITHMETIC_FILE;
		arithmetic *arithmetic_setting =NULL;
		
		arithmetic_setting = (arithmetic *)malloc(sizeof(arithmetic));
		
			if (arithmetic_setting == NULL)
			{	
				fprintf(stderr, "Failed to malloc memory setting.\n");	
				return -1;
		
			}	
			

			sprintf(srecvideo, "%d", decoder_param.arithmetic_recvideo);
			
			snprintf(arithmetic_setting->arithmeticpara1, STR_LEN, "%s", in_buf[0].value);
			snprintf(arithmetic_setting->arithmeticpara2, STR_LEN, "%s", in_buf[1].value);
			snprintf(arithmetic_setting->arithmeticpara3, STR_LEN, "%s", in_buf[2].value);
			snprintf(arithmetic_setting->arithmetic_recvideo, STR_LEN, "%s", srecvideo);
		
	
		if (access(arithmetic_block_file, F_OK) == 0)
		{
				//add_arithmetic_node(arithmetic_block_file,arithmetic_setting);

				modify_arithmetic_node(arithmetic_block_file,arithmetic_setting);
				
		}
		else 
		{
				create_arithmetic_block(arithmetic_block_file,arithmetic_setting);
		}
#endif
/******/	



	
	return 0;
}



/****************************************************************
@function:osd_param_process
@brief:process osd
@time:
@author:
****************************************************************/
int osd_param_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;

	unsigned char osd_str[100]="";
	int osd_size=0;
	int  osd_color=0;
	int osd_x=0;
	int osd_y=0;
	unsigned char osd_device_addr[100]="";
	int osd_time =0;

	//read_param();
	char value[100] = {0};




	if(strcmp(in_buf[2].value,"one")==0)
	{
		//printf("red  !!!\n");
		decoder_param.osd_color = 1;//for red
	}
	else if(strcmp(in_buf[2].value,"two")==0)
	{
		//printf("green  !!!\n");
		decoder_param.osd_color = 2;//for green
	}
	else if(strcmp(in_buf[2].value,"three")==0)
	{
		//printf("blue  !!!\n");
		decoder_param.osd_color = 3;//for blue
	}
	else if(strcmp(in_buf[2].value,"four")==0)
	{
		//printf("yellow  !!!\n");
		decoder_param.osd_color = 4;//for yellow
	}	
	else if(strcmp(in_buf[2].value,"five")==0)
	{
		//printf("black	!!!\n");
		decoder_param.osd_color = 5;//for black
	}
	else if(strcmp(in_buf[2].value,"six")==0)
	{
		//printf("whiter	!!!\n");
		decoder_param.osd_color = 6;//for whiter
	}


	
	if(strcmp(in_buf[6].value,"one")==0)
	{
		//printf(" time is open !!!\n");
		decoder_param.osd_time= 1;//for time is open
	}
	else if(strcmp(in_buf[6].value,"two")==0)
	{
		//printf("time is off  !!!\n");
		decoder_param.osd_time = 0;//for time is off
	}




	
	
	
	//osd_str = in_buf[0].value;
	osd_size = atoi(in_buf[1].value);
	//osd_color = in_buf[2].value;
	osd_x = atoi(in_buf[3].value);
	osd_y= atoi(in_buf[4].value);
//	osd_device_addr =  atoi(in_buf[5].value);

//	osd_time =  atoi(in_buf[6].value);






	



	//decoder_param.osd_str= osd_str;
	decoder_param.osd_size = osd_size;
	//decoder_param.osd_color = osd_color;
	decoder_param.osd_x = osd_x;
	decoder_param.osd_y= osd_y;
//	decoder_param.device_addr = osd_device_addr;

	//decoder_param.osd_time = osd_time;

	strcpy(decoder_param.osd_str,(in_buf[0].value));
	strcpy(decoder_param.device_addr,(in_buf[5].value));

#if 0

	FILE *p_fd;
	p_fd=fopen("sys.cfg","wb");
	ret=fwrite(&decoder_param,1,sizeof(system_param),p_fd);
	fclose(p_fd);
	system("sync");
#else


	//memset(value, 0 ,sizeof(value));
	//sprintf(value, "%d", decoder_param.osd_str);
	save_to_xml("osd.str",decoder_param.osd_str);

	
	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.osd_size);
	save_to_xml("osd.size",value);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.osd_color);
	save_to_xml("osd.color",value);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.osd_x);
	save_to_xml("osd.x",value);

	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.osd_y);
	save_to_xml("osd.y",value);

	//memset(value, 0 ,sizeof(value));
	//sprintf(value, "%d", decoder_param.device_addr);
	save_to_xml("osd.device_addr",decoder_param.device_addr);


	memset(value, 0 ,sizeof(value));
	sprintf(value, "%d", decoder_param.osd_time);
	save_to_xml("osd.time",value);



/******/
#if RYK_TEST

	char *osd_block_file = OSD_FILE;
	osd *osd_setting =NULL;
	char sosdcolor[STR_LEN] = {0};
	char sosdtime[STR_LEN]={0};
	char sosddeletecmd[STR_LEN] = {0};
	
	osd_setting = (osd *)malloc(sizeof(osd));
	
		if (osd_setting == NULL)
		{	
			fprintf(stderr, "Failed to malloc memory setting.\n");	
			return -1;
	
		}	
		/*
		osd_setting->num = 9;
		snprintf(osd_setting->str, OSD_STR_LEN, "%s", "this is one osd!");
		snprintf(osd_setting->size, STR_LEN, "%s", "6");
		snprintf(osd_setting->color, STR_LEN, "%s", "4");	
		snprintf(osd_setting->x, STR_LEN, "%s", "22");	
		snprintf(osd_setting->y, STR_LEN, "%s", "33");*/
		

		sprintf(sosdcolor, "%d", decoder_param.osd_color);
		sprintf(sosdtime, "%d", decoder_param.osd_time);
	
		
	
		snprintf(osd_setting->str, OSD_STR_LEN, "%s", in_buf[0].value);
		snprintf(osd_setting->size, STR_LEN, "%s", in_buf[1].value);
		snprintf(osd_setting->color, STR_LEN, "%s", sosdcolor);	
		snprintf(osd_setting->x, STR_LEN, "%s", in_buf[3].value);	
		snprintf(osd_setting->y, STR_LEN, "%s", in_buf[4].value);
		snprintf(osd_setting->device_addr, OSD_STR_LEN, "%s", in_buf[5].value);	
		snprintf(osd_setting->time, STR_LEN, "%s", sosdtime);

	

	if (access(osd_block_file, F_OK) == 0)
	{
			//add_osd_node(osd_block_file,osd_setting);
			//add_osd_node_2(osd_block_file);
			modify_osd_node(osd_block_file,osd_setting);
	}
	else 
	{
			create_osd_block(osd_block_file,osd_setting);
	}
#endif
/******/	



#endif
system("/bin/sync");	
/*
	sprintf(cmd, "amixer cset name='PCM Playback Volume' %d%%", pb_volume);
	system(cmd);

	memset(cmd, 0 , sizeof(cmd));
	r_volume = (r_volume * 12) / 10;
	sprintf(cmd, "amixer  cset numid=37 %d", r_volume);
	system(cmd);
	
	printf("%s\n", cmd);
*/
	return 0;
}



/****************************************************************
@function:ipaddr_process
@brief:process ipaddr
@time:08-25
@author:
****************************************************************/
int ipaddr_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;

	
	//read_param();
#if 0
	decoder_param.camera_net.ip_add = inet_addr(in_buf[0].value);
	decoder_param.camera_net.net_mask = inet_addr(in_buf[1].value);
	decoder_param.camera_net.gateway = inet_addr(in_buf[2].value);
	//net_set_ifaddr("eth0", inet_addr(in_buf[0].value));
	//net_set_netmask("eth0", inet_addr(in_buf[1].value));
#else
	strcpy(decoder_param.camera_net.ip_add , (in_buf[0].value));
	strcpy(decoder_param.camera_net.net_mask , (in_buf[1].value));
	strcpy(decoder_param.camera_net.gateway , (in_buf[2].value));
	//net_set_ifaddr("eth0", inet_addr(in_buf[0].value));
	//net_set_netmask("eth0", inet_addr(in_buf[1].value));
#endif
	save_to_xml("network.ip",decoder_param.camera_net.ip_add);
	save_to_xml("network.netmask",decoder_param.camera_net.net_mask);
	save_to_xml("network.gateway",decoder_param.camera_net.gateway);
#if 0
	//get_from_xml("network.ip",decoder_param.camera_net.ip_add);
	printf("ip:%s", decoder_param.camera_net.ip_add);


	FILE *p_fd;
	p_fd=fopen("sys.cfg","wb");
	ret=fwrite(&decoder_param,1,sizeof(system_param),p_fd);
	fclose(p_fd);
	system("sync");
#endif
	system("/bin/sync");
	system("/sbin/reboot");
	
	ret=0;
	return ret;
}



int serverip_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;

	int serverport=0;

	char value[32] = {0};
	strcpy(decoder_param.serverip, (in_buf[0].value));
	

	serverport =  atoi(in_buf[1].value);
	decoder_param.serverport = serverport;


	
	
	save_to_xml("server.ip",decoder_param.serverip);

	
	     memset(value, 0 ,sizeof(value));
		sprintf(value, "%d", decoder_param.serverport);
		save_to_xml("server.port",value);

	
	ret=0;
	return ret;
}



/****************************************************************
@function:pwd_process
@brief:process ipaddr
@time:08-25
@author:
****************************************************************/
int pwd_process(entry_buf in_buf[],int cont_value)
{
	int ret=0;

	
	//read_param();
	save_to_xml("user.name",in_buf[0].value);
	save_to_xml("user.pwd",in_buf[1].value);
	return 0;
}

/****************************************************************
@function:rtspaddr_process
@brief:process rtspaddr
@time:08-25
@author:pengkunfan
****************************************************************/
int rtspaddr_process(entry_buf in_buf[],int cont_value)
{
	COMMAND_ARGUMENT *argm=(COMMAND_ARGUMENT*)malloc(sizeof(COMMAND_ARGUMENT));

	argm->value=strdup(in_buf[0].value);
	
	unsigned char file_buffer[1024*512];
	unsigned char char_buf[1024*512];
	unsigned char outPut[100];
	//printf("length =%d \n",cont_value);//12-all;10-cutOsd;9-cutTime;
	//exit(1);

	FILE* p_fd;
	//FILE* p_fp;//6
	int ret=0;
	int temp = 0;
	int temp_value1 = 0;
	int temp_value2 = 0;
	int temp_value = 0;
	int i;
	
	memset(file_buffer,0,sizeof(file_buffer));
	memset(char_buf,0,sizeof(char_buf));
	memset(outPut,0,sizeof(outPut));
	
	read_param();
	//add
	if(strcmp(in_buf[0].value,"one")==0)
	{
		decoder_param.p_rtspaddr.batch = 0;
		decoder_param.p_rtspaddr.chrtsp[0].flag = atoi(in_buf[1].value);
		decoder_param.p_rtspaddr.chrtsp[1].flag = atoi(in_buf[2].value);
		decoder_param.p_rtspaddr.chrtsp[2].flag = atoi(in_buf[3].value);
		
		//decoder_param.p_rtspaddr.flag = atoi(in_buf[8].value);//5->7->10
		//if(strcmp(in_buf[4].value,"one")==0)
		
		if(strcmp(in_buf[4].value,"one")==0)
		decoder_param.p_rtspaddr.channel = 1;
		else if(strcmp(in_buf[4].value,"two")==0)
		decoder_param.p_rtspaddr.channel = 2;
		else if(strcmp(in_buf[4].value,"thid")==0)
		decoder_param.p_rtspaddr.channel = 3;
		//else if(strcmp(in_buf[4].value,"four")==0)
		//decoder_param.p_rtspaddr.channel = 4;
		
		temp_value = decoder_param.p_rtspaddr.channel;
		

		if(temp_value>=1 &&temp_value <=4)
		{
			//decoder_param.p_rtspaddr.chrtsp[temp].
			for(i=0;i<temp_value; i++)
			{
				argm->value=strdup(in_buf[5+i].value);
				get_ulr_addr((char *)file_buffer,argm->value,strlen(argm->value));
				//printf("file_buffer = %s \n",file_buffer);
				strcpy((char *)decoder_param.p_rtspaddr.chrtsp[i].rtsp,(char *)file_buffer);
			}
			decoder_param.p_rtspaddr.mult_flag = temp_value;
			//printf("file_buffer = %d \n",decoder_param.p_rtspaddr.mult_flag);
		}
		
		if(cont_value==10)
		{///9		
			decoder_param.reserve[1]= 0;//for osd
			decoder_param.reserve[2]= 0;//for time
			
			decoder_param.p_rtspaddr.flag = atoi(in_buf[9].value);
	
		}
		else if(cont_value==11)
		{
			temp = atoi(in_buf[9].value);
			if(temp==11)
			{
				decoder_param.reserve[1]= 1;//for osd
				decoder_param.reserve[2]= 0;//for time
				argm->value=strdup(in_buf[8].value);
				
				DeBaiFenStrin2Int(outPut,argm->value,strlen(argm->value));//decoder_param.p_rtspaddr.reserve,strlen(decoder_param.p_rtspaddr.reserve));
	
				memset(decoder_param.p_rtspaddr.reserve,0,strlen(decoder_param.p_rtspaddr.reserve));
				memcpy(decoder_param.p_rtspaddr.reserve,outPut,strlen(outPut));
			}
			else if(temp==12)
			{
				decoder_param.reserve[1]= 0;//for osd
				decoder_param.reserve[2]= 1;//for time
			}
			decoder_param.p_rtspaddr.flag = atoi(in_buf[10].value);
		}
		else if(cont_value==12)
		{	
			decoder_param.reserve[1]= 1;//for osd
			decoder_param.reserve[2]= 1;//for time
			argm->value=strdup(in_buf[8].value);
			
			DeBaiFenStrin2Int(outPut,argm->value,strlen(argm->value));//decoder_param.p_rtspaddr.reserve,strlen(decoder_param.p_rtspaddr.reserve));

			memset(decoder_param.p_rtspaddr.reserve,0,strlen(decoder_param.p_rtspaddr.reserve));
			memcpy(decoder_param.p_rtspaddr.reserve,outPut,strlen(outPut));
			decoder_param.p_rtspaddr.flag = atoi(in_buf[11].value);
		}
	
	}
	else 
	{
		if(strcmp(in_buf[0].value,"two")==0)
		{	
			decoder_param.p_rtspaddr.batch = 1;
		}
		if(strcmp(in_buf[0].value,"thid")==0)
		{
			decoder_param.p_rtspaddr.batch = 2;
		}
	
		
		//decoder_param.p_rtspaddr.flag = atoi(in_buf[8].value);//5->7->10
		//if(strcmp(in_buf[4].value,"one")==0)
		
		if(strcmp(in_buf[1].value,"one")==0)
		decoder_param.p_rtspaddr.channel = 1;
		else if(strcmp(in_buf[1].value,"two")==0)
		decoder_param.p_rtspaddr.channel = 2;
		else if(strcmp(in_buf[1].value,"thid")==0)
		decoder_param.p_rtspaddr.channel = 3;
		//else if(strcmp(in_buf[4].value,"four")==0)
		//decoder_param.p_rtspaddr.channel = 4;
		
		temp_value = decoder_param.p_rtspaddr.channel;
		

		if(temp_value>=1 &&temp_value <=4)
		{
			//decoder_param.p_rtspaddr.chrtsp[temp].
			for(i=0;i<temp_value; i++)
			{
				argm->value=strdup(in_buf[2+i].value);
				get_ulr_addr((char *)file_buffer,argm->value,strlen(argm->value));
				//printf("file_buffer = %s \n",file_buffer);
				strcpy((char *)decoder_param.p_rtspaddr.chrtsp[i].rtsp,(char *)file_buffer);
			}
			decoder_param.p_rtspaddr.mult_flag = temp_value;
		}
		
		if(cont_value==7)
		{
			decoder_param.reserve[1]= 0;//for osd
			decoder_param.reserve[2]= 0;//for time
			
			decoder_param.p_rtspaddr.flag = atoi(in_buf[6].value);
		}
		else if(cont_value==8)
		{
			temp = atoi(in_buf[6].value);
			if(temp==11)
			{
				decoder_param.reserve[1]= 1;//for osd
				decoder_param.reserve[2]= 0;//for time
				argm->value=strdup(in_buf[5].value);
				
				DeBaiFenStrin2Int(outPut,argm->value,strlen(argm->value));//decoder_param.p_rtspaddr.reserve,strlen(decoder_param.p_rtspaddr.reserve));
	
				memset(decoder_param.p_rtspaddr.reserve,0,strlen(decoder_param.p_rtspaddr.reserve));
				memcpy(decoder_param.p_rtspaddr.reserve,outPut,strlen(outPut));
			}
			else if(temp==12)
			{
				decoder_param.reserve[1]= 0;//for osd
				decoder_param.reserve[2]= 1;//for time
			}
			decoder_param.p_rtspaddr.flag = atoi(in_buf[7].value);
		}
		else if(cont_value==9)
		{
			decoder_param.reserve[1]= 1;//for osd
			decoder_param.reserve[2]= 1;//for time

			argm->value=strdup(in_buf[5].value);
			
			DeBaiFenStrin2Int(outPut,argm->value,strlen(argm->value));//decoder_param.p_rtspaddr.reserve,strlen(decoder_param.p_rtspaddr.reserve));

			memset(decoder_param.p_rtspaddr.reserve,0,strlen(decoder_param.p_rtspaddr.reserve));
			memcpy(decoder_param.p_rtspaddr.reserve,outPut,strlen(outPut));
	
			decoder_param.p_rtspaddr.flag = atoi(in_buf[8].value);
		}
	}
	
	decoder_param.p_rtspaddr.change_time=0;
	//
	/*
	decoder_param.p_rtspaddr.change_time  = atoi(in_buf[1].value);
	
	if(decoder_param.p_rtspaddr.change_time==0)
	{
		if(strcmp(in_buf[2].value,"one")==0)
		decoder_param.p_rtspaddr.channel = 1;
		else if(strcmp(in_buf[2].value,"two")==0)
		decoder_param.p_rtspaddr.channel = 2;
		else if(strcmp(in_buf[2].value,"thid")==0)
		decoder_param.p_rtspaddr.channel = 3;
		else if(strcmp(in_buf[2].value,"four")==0)
		decoder_param.p_rtspaddr.channel = 4;
		
		temp_value = decoder_param.p_rtspaddr.channel;
		

		if(temp_value>=1 &&temp_value <=4)
		{
			//decoder_param.p_rtspaddr.chrtsp[temp].
			for(i=0;i<temp_value; i++)
			{
				argm->value=strdup(in_buf[3+i].value);
				get_ulr_addr((char *)file_buffer,argm->value,strlen(argm->value));
				//printf("file_buffer = %s \n",file_buffer);
				strcpy((char *)decoder_param.p_rtspaddr.chrtsp[i].rtsp,(char *)file_buffer);
			}
			decoder_param.p_rtspaddr.mult_flag = temp_value;
		}
	
			
	}
	else if(decoder_param.p_rtspaddr.change_time >0)
	{
		
		if(strcmp(in_buf[0].value,"one")==0)
		decoder_param.p_rtspaddr.batch = 1;
		else if(strcmp(in_buf[0].value,"two")==0)
		decoder_param.p_rtspaddr.batch = 2;
		else if(strcmp(in_buf[0].value,"thid")==0)
		decoder_param.p_rtspaddr.batch = 3;
		else if(strcmp(in_buf[0].value,"four")==0)
		decoder_param.p_rtspaddr.batch = 4;
		else if(strcmp(in_buf[0].value,"five")==0)
		decoder_param.p_rtspaddr.batch = 5;
		else if(strcmp(in_buf[0].value,"six")==0)
		decoder_param.p_rtspaddr.batch = 6;
		else if(strcmp(in_buf[0].value,"seven")==0)
		decoder_param.p_rtspaddr.batch = 7;
		else if(strcmp(in_buf[0].value,"eight")==0)
		decoder_param.p_rtspaddr.batch = 8;
		temp_value1 = decoder_param.p_rtspaddr.batch;
		
		if(temp_value1>0||temp_value1<8)
		{
			
			if(strcmp(in_buf[2].value,"one")==0)
			decoder_param.p_rtspaddr.channel = 1;
			else if(strcmp(in_buf[2].value,"two")==0)
			decoder_param.p_rtspaddr.channel = 2;
			else if(strcmp(in_buf[2].value,"thid")==0)
			decoder_param.p_rtspaddr.channel = 3;
			else if(strcmp(in_buf[2].value,"four")==0)
			decoder_param.p_rtspaddr.channel = 4;
			
			temp_value2 = decoder_param.p_rtspaddr.channel;

		
			if(temp_value2>=1 &&temp_value2 <=4)
			{
				//decoder_param.p_rtspaddr.chrtsp[temp].
				//get_ulr_addr((char *)file_buffer,argm->value,strlen(argm->value));
				//strcpy((char *)decoder_param.p_rtspaddr.chrtsp[(temp_value1-1)*temp+temp_value2-1].rtsp,(char *)file_buffer);
				
				for(i=0;i<temp_value2; i++)
				{
					argm->value=strdup(in_buf[3+i].value);
					get_ulr_addr((char *)file_buffer,argm->value,strlen(argm->value));
					//printf("file_buffer = %s \n",file_buffer);
					strcpy((char *)decoder_param.p_rtspaddr.chrtsp[(temp_value1-1)*temp_value2+i].rtsp,(char *)file_buffer);
					memset(file_buffer,0,sizeof(file_buffer));
				}
				
				decoder_param.p_rtspaddr.mult_flag = temp_value2;
			}
			decoder_param.p_rtspaddr.channel_numbs = (temp_value1-1)*temp_value2+temp_value2;
			
		}
	
	}
	argm->value=strdup(in_buf[7].value);
	DeBaiFenStrin2Int(outPut,argm->value,strlen(argm->value));//decoder_param.p_rtspaddr.reserve,strlen(decoder_param.p_rtspaddr.reserve));
	
	memset(decoder_param.p_rtspaddr.reserve,0,strlen(decoder_param.p_rtspaddr.reserve));
	memcpy(decoder_param.p_rtspaddr.reserve,outPut,strlen(outPut));
	//memcpy(decoder_param.p_rtspaddr.reserve,argm->value,strlen(argm->value));

	decoder_param.p_rtspaddr.flag = atoi(in_buf[8].value);//5->7->10 for reboot!
	*/
	//rtsp %3A %2F%2F 192.168.0.102 %2F 1.264--------------> %3a=(:)  %2f=(/) 
	//rtsp://192.168.0.168:8557/PSIA/Streaming/channels/2?videoCodecType=H.264 -------->%3f=(?)  %3d=(=) %26=(&)
	//rtsp%3A%2F%2F192.168.0.168%3A8557%2FPSIA%2FStreaming%2Fchannels%2F2%3FvideoCodecType%3DH.264
	
	//get_ulr_addr((char *)file_buffer,argm->value,strlen(argm->value));
	//strcpy((char *)decoder_param.p_rtspaddr.rtsp,(char *)file_buffer);
	
	p_fd=fopen("sys.cfg","wb");
	ret=fwrite(&decoder_param,1,sizeof(system_param),p_fd);
	fclose(p_fd);
	reply_display(6);//have bug!!!!!! loop!!!
	ret=0;
	return ret;
	
}

/*
static void write_param(system_param my_param)
{
	system_param ka_param=my_param;
	int p_fd;
	p_fd=fopen("sys.cfg","wb");
	int ret=fwrite(&ka_param,1,sizeof(system_param),p_fd);
	memset(&ka_param,0,sizeof(system_param));	
}
*/

static void read_param()
{
	FILE* p_fd;
	int ret;
	p_fd=fopen("sys.cfg","rb");
	
	if(p_fd==NULL)
	{
		
		p_fd=fopen("sys.cfg","wb");
		memset(&decoder_param,0,sizeof(decoder_param));
		decoder_param.camera_time.year=2014;
		decoder_param.camera_time.month=8;
		decoder_param.camera_time.day=1;
		decoder_param.camera_time.hour=12;
		decoder_param.camera_time.min=0;
		decoder_param.camera_time.sec=0;
	
	
		
		strcpy(decoder_param.camera_net.ip_add , ("192.168.0.168"));
		strcpy(decoder_param.camera_net.net_mask , ("255.255.255.0"));
		strcpy(decoder_param.camera_net.gateway , ("192.168.0.254"));
		strcpy(decoder_param.camera_net.nameserver , ("192.168.0.1"));
		
		
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[0].rtsp,"rtsp://192.168.0.102/1.264");
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[1].rtsp,"rtsp://192.168.0.102/1.264");
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[2].rtsp,"rtsp://192.168.0.102/1.264");
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[3].rtsp,"rtsp://192.168.0.102/1.264");
		strcpy((char *)decoder_param.p_rtspaddr.chrtsp[4].rtsp,"rtsp://192.168.0.102/1.264");
		
		decoder_param.p_rtspaddr.channel_numbs = 1;
		decoder_param.p_rtspaddr.change_time = 0;
		decoder_param.p_rtspaddr.flag = 0;
	
		ret=fwrite((void *)&decoder_param,1,sizeof(decoder_param),p_fd);
		
		
	}
	else 
	{
		fread(&decoder_param,1,sizeof(system_param),p_fd);
	}
#if 1
	get_from_xml("network.ip",decoder_param.camera_net.ip_add);
	get_from_xml("network.netmask",decoder_param.camera_net.net_mask);
	get_from_xml("network.gateway",decoder_param.camera_net.gateway);

	
#endif
	
		
}

/****************************************************************
@function:reset_value(
@brief:reset tags of html
@time:08-25
@author:pengkunfan
****************************************************************/
int reset_value(unsigned char *inbuf,unsigned char *outbuf,unsigned char  *data_char,int flag)
{
	const char *in_put=(char *)inbuf;
	 char *temp_dest=NULL;
	 char *temp_end=NULL;
	 char *my_data=(char *)data_char;
	int ptr_id=0;
	int temp_id=0;
	int ptr_len=strlen((char *)inbuf);
	//unsigned char *out = outbuf;
	//
	memset(outbuf,'\0',strlen((char*)inbuf)+strlen((char*)data_char));
	//printf("ptr_len = %d \n",ptr_len);
	
	switch(flag)
	{
	
		
		/*********ipaddr start************/
		case 12:
			temp_dest=strstr(in_put,"camera_ipaddr");
			temp_end=strstr(temp_dest,"><");
			temp_dest=temp_dest+22;
			temp_end=temp_end-2;
		break;
		case 13:
			temp_dest=strstr(in_put,"camera_netmask");
			temp_end=strstr(temp_dest,"><");
			temp_dest=temp_dest+23;
			temp_end=temp_end-2;
		break;
		case 14:
			temp_dest=strstr(in_put,"camera_gateway");
			temp_end=strstr(temp_dest,"><");
			temp_dest=temp_dest+23;
			temp_end=temp_end-2;
		break;
		case 15:
			temp_dest=strstr(in_put,"exp_gian_e");
			temp_end=strstr(temp_dest,">8~");
			temp_dest=temp_dest+19;
			temp_end=temp_end-2;
		break;
		/*********time start************/
	
		/*****************set rtsp addr**************************/
		case 18:
			temp_dest=strstr(in_put,"rtspaddr_h");
			temp_end=strstr(temp_dest,"><");
			temp_dest=temp_dest+20;//r---start
			temp_end=temp_end-3;//end rtsp
		break;
		default:
		break;
	}
	
	int new_dlen=strlen(my_data);
	if(new_dlen<=0)exit(-1);
	
	int old_dlen=temp_end-temp_dest+1;
	
	ptr_id=temp_dest-(char *)inbuf;//
	temp_id=ptr_len-ptr_id;//addr to end len
	
	int m_len=0;
	//printf("new_dlen =%d ,old_dlen = %d, temp_id =%d \n",new_dlen,old_dlen,temp_id);//12 662
	
	if(new_dlen>old_dlen)
	{
		m_len=new_dlen-old_dlen;//move length
		int i=temp_id-1;
		*(temp_dest+i+m_len+1)='\0';
		while(i>0)
		{
			*(temp_dest+i+m_len)=*(temp_dest+i);
			i--;
		
		}

		int j;
		for(j=0;j<new_dlen;j++)
		{
			*(temp_dest+j)=data_char[j];

		}
		
	}
	else if(new_dlen<old_dlen)
	{
		m_len=old_dlen-new_dlen;
		int k=0;
		
		while(k<temp_id)
		{
			*(temp_dest+new_dlen+k)=*(temp_dest+new_dlen+k+m_len);
			k++;
		}
		//
		int j;
		for(j=0;j<new_dlen;j++)
		{
			*(temp_dest+j)=data_char[j];
		}
	}
	else if(new_dlen==old_dlen)
	{
		int j;
		for(j=0;j<new_dlen;j++)
		{
			*(temp_dest+j)=data_char[j];
		}
	}
	
	int n=ptr_len+m_len;
	//printf("%s",inbuf);//the old data why???????????
	while(n>0)
	{
		(*outbuf++)=(*inbuf++);	
		//printf("%c",(*outbuf));
		n--;
	}
	//printf("%s\n",out);
	
	*outbuf='\0';

	return 0;
}

/****************************************************************
@function:add_check_selected
@brief:add check and selected of tags for html
@time:08-25
@author:pengkunfan
****************************************************************/
int add_check_selected(unsigned char *inbuf,unsigned char *outbuf,int flag)
{
	char *in_put=(char *)inbuf;
	char *temp_dest=NULL;

	int ch_se_flag=0;//check:0 ,select:1;

	int pri_id=0;
	int left_id=0;
	
	int htm_len=strlen((char *)inbuf);
	
	switch(flag)
	{
	
		case 2:
			temp_dest=strstr(in_put,">1-");
			temp_dest=temp_dest-10;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 3:
			temp_dest=strstr(in_put,">2-");
			temp_dest=temp_dest-10;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 4:
			temp_dest=strstr(in_put,">3-");
			temp_dest=temp_dest-10;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 5:
			temp_dest=strstr(in_put,">4-");
			temp_dest=temp_dest-10;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 6:
			temp_dest=strstr(in_put,">5-");
			temp_dest=temp_dest-10;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 7:
			temp_dest=strstr(in_put,">6-");
			temp_dest=temp_dest-10;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 8:
			temp_dest=strstr(in_put,">7-");
			temp_dest=temp_dest-10;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 9:
			temp_dest=strstr(in_put,">8-");
			temp_dest=temp_dest-10;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 10:
			temp_dest=strstr(in_put,">9-");
			temp_dest=temp_dest-10;
		
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 11:
			temp_dest=strstr(in_put,">10-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 12:
			temp_dest=strstr(in_put,">11-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 13:
			temp_dest=strstr(in_put,">12-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 14:
			temp_dest=strstr(in_put,">13-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 15:
			temp_dest=strstr(in_put,">14-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 16:
			temp_dest=strstr(in_put,">15-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 17:
			temp_dest=strstr(in_put,">16-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 18:
			temp_dest=strstr(in_put,">17-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 19:
			temp_dest=strstr(in_put,">18-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 20:
			temp_dest=strstr(in_put,">19-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 21:
			temp_dest=strstr(in_put,">20-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 22:
			temp_dest=strstr(in_put,">21-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 23:
			temp_dest=strstr(in_put,">22-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 24:
			temp_dest=strstr(in_put,">23-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 25:
			temp_dest=strstr(in_put,">24-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 26:
			temp_dest=strstr(in_put,">25-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 27:
			temp_dest=strstr(in_put,">26-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 28:
			temp_dest=strstr(in_put,">27-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 29:
			temp_dest=strstr(in_put,">28-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 30:
			temp_dest=strstr(in_put,">29-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 31:
			temp_dest=strstr(in_put,">30-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			break;
		case 32:
			temp_dest=strstr(in_put,">31-");
			temp_dest=temp_dest-11;
			
			pri_id=temp_dest-in_put;
	
			left_id=htm_len-pri_id;//delease one
			ch_se_flag=1;
			
		break;
	
		case 44:
			temp_dest=strstr(in_put,"camera_lens");
			
			temp_dest=temp_dest+11;
			pri_id=temp_dest-in_put;
			left_id=htm_len-pri_id;//1c
			ch_se_flag=0;
			
		break;
		case 45:
			temp_dest=strstr(in_put,"camera_lens");
			temp_dest=temp_dest+2;
			temp_dest=strstr(temp_dest,"camera_lens");
			
			temp_dest=temp_dest+11;
			pri_id=temp_dest-in_put;
			left_id=htm_len-pri_id;//1c
			ch_se_flag=0;
			
		break;
		case 46:
		temp_dest=strstr(in_put,"alg_plr_reboot");
		
		temp_dest=temp_dest+25;
		pri_id=temp_dest-in_put;
		left_id=htm_len-pri_id;//1c
		ch_se_flag=0;
			
		break;
		case 47:
		temp_dest=strstr(in_put,"alg_plr_reboot");
		temp_dest=temp_dest+2;
		temp_dest=strstr(temp_dest,"alg_plr_reboot");
		
		temp_dest=temp_dest+25;
		pri_id=temp_dest-in_put;
		left_id=htm_len-pri_id;//1c
		ch_se_flag=0;
			
		break;
/////////////////////////////////rtsp start////////////////////////////////////////		
		case 48:
			temp_dest=strstr(in_put,"Channel_level");
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest-1;
	
			pri_id=temp_dest-in_put;
			left_id=htm_len-pri_id;//0
			ch_se_flag=1;
			
		break;
		case 49:
			temp_dest=strstr(in_put,"Channel_level");
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest+2;
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest-1;
	
			pri_id=temp_dest-in_put;
			left_id=htm_len-pri_id;//0
			ch_se_flag=1;
			
			
		break;
		case 50:
			temp_dest=strstr(in_put,"Channel_level");
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest+2;
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest+2;
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest-1;

			pri_id=temp_dest-in_put;
			left_id=htm_len-pri_id;//2
			ch_se_flag=1;
			break;
		case 51:
			temp_dest=strstr(in_put,"Channel_level");
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest+2;
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest+2;
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest+2;
			temp_dest=strstr(temp_dest,"value");
			temp_dest=temp_dest-1;
			
			pri_id=temp_dest-in_put;
			left_id=htm_len-pri_id;//2
			ch_se_flag=1;
			
		break;
		default:
		break;
	}
	
	int m_len=0;
	if(ch_se_flag==0)
	{
		m_len=17;
	}
	else if(ch_se_flag==1)
	{
		m_len=19;
		
	}
	
	int i=left_id-1;
	
	while(i>0)
	{
		*(temp_dest+i+m_len)=*(temp_dest+i);
		i--;
		
	}

	int j;
	char check_char[20];
	int n=0;
	memset(check_char,0,sizeof(check_char));
	if(ch_se_flag==0)
	{
		strcpy(check_char,"checked='checked'");
		for(j=0;j<17;j++)
		{
			*(temp_dest+j+1)=check_char[j];
			if(j==8||j==16)*(temp_dest+j+1)='"';
		}
	
		n=htm_len+17;
		
	}
	else if(ch_se_flag==1)
	{
		strcpy(check_char,"selected='selected'");
		for(j=0;j<19;j++)
		{
			*(temp_dest+j+1)=check_char[j];
			if(j==9||j==18)*(temp_dest+j+1)='"';
		}
	
		n=htm_len+19;
	}
	
	while(n>0)
	{
		(*outbuf++)=(*inbuf++);	
		n--;
	}
	
	*outbuf='\0';
	return 0;
}
/*******************************************************
@function:reset_check_selceted
@brief:reset check flag
@param:
@modyfy:09-16 all delet select
********************************************************/
int reset_check_selected(unsigned char *inbuf,unsigned char *outbuf,int flag)
{
	char *in_put=(char*)inbuf;
	int htm_len=strlen((char *)in_put);
	int m=htm_len;
	int left_id=0;
	int prim_id=0;
	int m_len=0;
	char *temp_ptr=NULL;
	if(flag==0)
	{
		m_len=17;
	}
	else if(flag==1)
	{
		m_len=19;
	}
	while(m)
	{
		if(flag==0)
		{
			temp_ptr=strstr(in_put,"checked");
		}
		else if(flag==1)
		{
			temp_ptr=strstr(in_put,"selected");
		}
		if(temp_ptr!=NULL)
		{
			prim_id=temp_ptr-(char*)in_put;
			left_id=htm_len-prim_id;//size of select  -1
			int j=0;
			//*(temp_ptr+m_len+j)=' ';//end " " "
			while(j<left_id)
			{
				*(temp_ptr+j)=*(temp_ptr+m_len+j);//start:s is replace by ' ' end: 
				j++;
			}
			htm_len-=m_len;
			m=m-(m_len-1);
		}
		else break;
		m--;	
	}
	
	int n=htm_len;
	while(n>0)
	{
		(*outbuf++)=(*inbuf++);	
		n--;
	}
	*outbuf='\0';
	return 0;
}

void get_data(char *word,char *line,char stop)//cut word
{
	int x=0,y=0;
	for (x=0;((line[x])&&(line[x]!=stop));x++)
	word[x]=line[x];
	word[x]='\0';
	if (line[x])++x;
	while((line[y++]=line[x++]));
}

char *make_data(char *line,char stop)//cut word '='
{
	int x=0,y;
	char *word=(char *)malloc(sizeof(char)*(strlen(line)));
	for(x=0;((line[x])&&(line[x]!=stop));x++)
	{
		
		word[x]=line[x];
	}
	word[x]='\0';
	if (line[x])++x;
	y=0;
	while((line[y++]=line[x++]));
	return word;//=name
}

char *fmake_data(FILE *f,char stop,int *len)//get name =value
{
	int wsize;
	char *word;
	int ll;
	ll=0;
	wsize=102400;//100kb;
	word=(char *)malloc(sizeof(char)*(wsize+1));
	while(1)
	{
		word[ll]=(char)fgetc(f);//!EOF
		if(ll==wsize)
		{
			word[ll+1]='\0';
			wsize+=102400;
			word=(char*)realloc(word,sizeof(char)*(wsize+1));
		}
		 
		if (word[ll]==stop||feof(f))//feof=0
		{
			if(word[ll]!=stop)ll++;
			word[ll]='\0';
			return word;
		}
		++ll;
	}
}	
char u2c(char *what)//url encode
{
	register char digit;
	digit=(what[0]>='A'?((what[0]&0xdf)-'A')+10:(what[0]-'0'));
	digit*=16;//<-4
	digit+=(what[0]>='A'?((what[1]&0xdf)-'A')+10:(what[1]-'0'));//(&)-32,-48
	return(digit);
}
void unescape_url(char *url)//url encode//tempery not have 
{
	
	//printf("my1:%s\n",url);
	/*
	for (x=0,y=0;strlen(p_url);++x,++y)//url[y]
	{
		if (p_url[x]=p_url[y]=='%')
		{
			printf("oooooooooox=%d\n",x);
			p_url[x]=x2c(&p_url[y+1]);
			y+=2;
		}
		printf("%c-%d",p_url[y],y);//and hack!!!
	}*/
	//printf("my2:%s\n",url);
	//url[x]='\0';
	
}
void plustospace(char *str)//+ to ''
{
	register int x;
	for (x=0;str[x];x++)
	if(str[x]=='+')str[x]=' ';
}
char *get_value(char *s,int cont_value,entry_buf in_buf[])//get form's  one value
{
	int x,length,i=0;
	char *buf = NULL;
	for (x=0;x<=cont_value;x++)//num_value static
	{
		if (strcmp(in_buf[x].name,s)==0)//entries static
		{	
			if ((length=strlen(in_buf[x].value))==0)return NULL;
			if ((buf=(char *)malloc(length))==NULL)
			{
				printf("not enough memory to allocate buffer");
				exit(0);
			}
			memset(buf,0,sizeof(buf));
			while((in_buf[x].value[i])!='\0')
			{
				buf[i]=in_buf[x].value[i];
				i++;
			}
			
			buf[i]='\0';
			
		}
	}
	return buf;
}
int  get_index(entry_buf in_buf[])
{
	int ret=0;
	
	
	if(strcmp(in_buf[0].name,"date_h")==0)
	{
		in_buf[0].index=4;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"camera_ipaddr")==0)
	{
		in_buf[0].index=5;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"Batch_level")==0)
	{
		in_buf[0].index=6;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"bit_rate0")==0)
	{
		in_buf[0].index=7;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"select_client_type")==0)
	{
		in_buf[0].index=8;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"playbak_volume")==0)
	{
		in_buf[0].index=9;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"user_name")==0)
	{
		in_buf[0].index=10;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"name")==0)
	{
		in_buf[0].index=11;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"camera_osdsetting_str_name")==0)
	{
		in_buf[0].index=50;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"arithmeticpara1")==0)
	{
		in_buf[0].index=51;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"rtspurl")==0)
	{
		in_buf[0].index=52;
		ret=1;
	}
	else if(strcmp(in_buf[0].name,"server_ipaddr_name")==0)
	{
		in_buf[0].index=55;
		ret=1;
	}
	else {in_buf[0].index=0;ret=0;printf("%s",in_buf[0].name);}
	return ret;
}

int netsplit( char *pAddress, void *ip )
{
	unsigned int ret;
	NET_IPV4 *ipaddr = (NET_IPV4 *)ip;

	if ((ret = atoi(pAddress + 9)) > 255)
		return FALSE;
	ipaddr->str[3] = ret;

	*( pAddress + 9 ) = '\x0';
	if ((ret = atoi(pAddress + 6)) > 255)
		return FALSE;
	ipaddr->str[2] = ret;

	*( pAddress + 6 ) = '\x0';
	if ((ret = atoi(pAddress + 3)) > 255)
		return FALSE;
	ipaddr->str[1] = ret;

	*( pAddress + 3 ) = '\x0';
	if ((ret = atoi(pAddress + 0)) > 255)
		return FALSE;
	ipaddr->str[0] = ret;

	return TRUE;
}

//hex 's string to sign char (0x printf)
int tolower(int c) 
{ 
    if (c >= 'A' && c <= 'Z') 
    { 
        return c + 'a' - 'A'; 
    } 
    else 
    { 
        return c; 
    } 
} 

//将十六进制的字符串转换成整数 
int htoi(char s[]) 
{ 
    int i; 
    int n = 0; 
    if(s[0] == '0' && (s[1]=='x' || s[1]=='X')) //判断是否有前导0x或者0X
    { 
        i = 2; 
    } 
    else 
    { 
        i = 0; 
    } 
    for(; (s[i] >= '0' && s[i] <= '9') 
|| (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i) 
    {   
        if(tolower(s[i]) > '9') 
        { 
            n = 16 * n + (10 + tolower(s[i]) - 'a'); 
        } 
        else 
        { 
            n = 16 * n + (tolower(s[i]) - '0'); 
        } 
    } 
    return n; 
} 

int DeBaiFenStrin2Int(unsigned char *des,unsigned char *src,int size)
{
	
	unsigned char *pSrc = src;
	unsigned char *pDes = des;
	unsigned char outPut[100]={0};
	unsigned char inPut[100];
	
	unsigned char *pOut=outPut;
	unsigned char in[10]={0};//in[3];
	unsigned char out[4]={0};
	memset(in,0,sizeof(in));
	memset(outPut,0,sizeof(outPut));
	memset(inPut,0,sizeof(inPut));
	
	int bufSize = size;
	int i,j;
	unsigned char *pOutPut =outPut;
	memset(pDes,0,strlen(pDes));
	while(bufSize>0)
	{
		memset(in,0,sizeof(in));
		memset(outPut,0,sizeof(outPut));
		if(*pSrc=='%'&&(pSrc+9)!=NULL&&*(pSrc+1)<0x80)
		{
			pSrc++;
			in[0]=*(pSrc);
			pSrc++;
			in[1]=*(pSrc);
			pSrc++;
			in[2]='\0';
			out[0]=htoi(in);
			
			pSrc++;
			in[0]=*(pSrc);
			pSrc++;
			in[1]=*(pSrc);
			pSrc++;
			in[2]='\0';
			out[1]=htoi(in);
			
			
			pSrc++;
			in[0]=*(pSrc);
			pSrc++;
			in[1]=*(pSrc);
			pSrc++;
			in[2]='\0';
			out[2]=htoi(in);
			
			UTF8ToGB2312(out, pOut, strlen(out));
			*pDes=pOut[0];
			pDes++;
			*pDes=pOut[1];
			pDes++;
			bufSize-=9;
		}
		else
		{
			*pDes=0xa3;
			pDes++;
			*pDes=(*pSrc)+0x80;
			pDes++;
			pSrc++; 
			bufSize--;
		}
		
		
	}
/*
	for(i=0;i<strlen(des);i++)
	{
		printf("a%d:%0x <br>",i,*(des+i));	
		if(*(des+i)>=0x0&&*(des+i)<=0x9)
		{
			//*(des+i) =0x80+*(des+i);
			*pOutPut=0xa3;
			pOutPut++;
			*pOutPut = *(des+i)+0x80+0x30;
		}
		else
		{
			*pOutPut=*(des+i);
		}
		pOutPut++;
	}
	memcpy(des,outPut,strlen(outPut));
	*/
	//printf("\nlen =%d\n",strlen(des));

	return 0;
}

/***************************************************************************
 *                                                                         *
 ***************************************************************************/
int ipv4_str_to_num(char *data, struct in_addr *ipaddr)
{
	if ( strchr(data, '.') == NULL )
		return netsplit(data, ipaddr);
	return inet_aton(data, ipaddr);
}
