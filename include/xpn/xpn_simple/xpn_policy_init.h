#ifndef _XPN_POLICY_INIT_H
#define _XPN_POLICY_INIT_H

#ifdef ENABLE_MXML
	#include "mxml.h"
#endif

#include "xpn_err.h"
#include "xpn.h"
#include "xpn_init.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


enum type_conf_connection{
	CONF_FILE = 0,
	CONF_RPC = 1,
	CONF_DESC = 2,
	CONF_MXML = 3,
};




#define XPN_DATA_SERVER         0
#define XPN_META_SERVER         1


#ifdef ENABLE_MXML
	#define	CONF_DEFAULT  CONF_MXML
#else
	#define	CONF_DEFAULT  CONF_FILE
#endif

#ifdef ENABLE_MXML

#define XML_TAG_ELEM_PARTITION		"partition"
#define XML_TAG_ELEM_DATANODE		"data_node"
#define XML_TAG_ELEM_METADATANODE	"metadata_node"

#define XML_TAG_ATTR_NAME		"name"
#define XML_TAG_ATTR_TYPE		"type"
#define XML_TAG_ATTR_BLOCKSIZE		"bsize"
#define XML_TAG_ATTR_THREADS		"threads"
#define XML_TAG_ATTR_TH_THRESHOLD	"th_limit"
#define XML_TAG_ATTR_URL		"url"
#define XML_TAG_ATTR_ID		"id"

#define XML_DEFAULT_ATTR_TYPE		"NORMAL"
#define XML_DEFAULT_ATTR_BLOCKSIZE	"256K"
#define XML_DEFAULT_ATTR_THREADS	"no"
#define XML_DEFAULT_ATTR_TH_THRESHOLD	"-1"


struct xml_connect_st{
	FILE *conf_f;
	FILE *profile_f;

	char conf_n[255];
	char profile_n[255];

	mxml_node_t* profile_tree;
	mxml_node_t* conf_tree;

	mxml_node_t* conf_partition_node;
	mxml_node_t* conf_meta_node;
	mxml_node_t* conf_data_node;

	mxml_node_t* profile_partition_node;
	mxml_node_t* profile_meta_node;
	mxml_node_t* profile_data_node;
};
#endif

struct conf_connect_st{
	int type;
	union{
	  FILE *f;
	  CLIENT *cl;
	  int fd;
#ifdef ENABLE_MXML
	  struct xml_connect_st xml;
#endif
	/* more types */
	}connect_u;
};


#define XPN_CONF		"XPN_CONF"

#ifdef ENABLE_MXML
	#define XPN_PROFILE		"XPN_PROFILE"
	#define XPN_CONF_DEFAULT 	"/etc/xpn/xpn.conf.xml"
	#define XPN_PROFILE_DEFAULT 	"/etc/xpn/profile.conf.xml"
#else
	#define XPN_CONF_DEFAULT 	"/etc/xpn/xpn.conf"
#endif



struct conf_connect_st * XpnPartitionOpen();
void XpnPartitionClose(struct conf_connect_st *fconf);
int XpnGetNextPartition(struct conf_connect_st *fconf, char *name);
int XpnGetIdPartition(struct conf_connect_st *fconf, char *name);
int XpnGetInfoPartition(struct conf_connect_st *fconf, struct xpn_partition *part);
int XpnGetNumServersPartition(struct conf_connect_st *fconf, struct xpn_partition *part, int type);
struct xpn_partition* XpnSearchPart(int pd);

int XpnGetServer(struct conf_connect_st *fconf, struct xpn_partition *part, struct nfi_server *serv, int type);
int XpnGetPartition(char *path);


 #ifdef  __cplusplus
     }
 #endif


#endif

