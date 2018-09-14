/********************************************************************************
 *
 * Copyright (c) 2000 Easysoft Ltd
 *
 * The contents of this file are subject to the Easysoft Public License 
 * Version 1.0 (the "License"); you may not use this file except in compliance 
 * with the License. 
 *
 * You may obtain a copy of the License at http://www.easysoft.org/EPL.html 
 *
 * Software distributed under the License is distributed on an "AS IS" basis, 
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for 
 * the specific language governing rights and limitations under the License. 
 *
 * The Original Code was created by Easysoft Limited and its successors. 
 *
 * Contributor(s): Tom Fosdick (Easysoft) 
 *
 ********************************************************************************
 *
 * $Id: diagnostics.h,v 1.2 2004/08/06 20:45:16 dbox Exp $
 *
 * $Log: diagnostics.h,v $
 * Revision 1.2  2004/08/06 20:45:16  dbox
 * add error code sqlerror_07002, wrong number of bound parameters or conversion error in binding parameter
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.7  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.6  2000/05/04 14:56:32  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.5  2000/05/03 16:04:17  tom
 * Added error
 *
 * Revision 1.4  2000/04/26 10:11:28  tom
 *
 *
 * Various fixes
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 *
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:30:11  tom
 *
 * First Functional Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 *
 * Added files
 *
 *
 ********************************************************************************/

#ifndef _DIAGNOSTICS_H
#define _DIAGNOSTICS_H

/* 
 * Function Prototypes
 */

void ood_init_hgeneric(void *hH, SQLINTEGER htype);

/*
 * Error node type
 */

struct error_node_tag
{
    SQLCHAR *origin;
    SQLINTEGER column_number;
    SQLCHAR *connection_name;
    SQLCHAR message[512];
    SQLINTEGER native;
    SQLINTEGER row_number;
    SQLCHAR *server_name;
    SQLCHAR *sqlstate;

    struct error_node_tag *next;
};
typedef struct error_node_tag error_node;

typedef struct 
{
    SQLINTEGER cusor_row_count;
    SQLCHAR dynamic_fn[32];
    SQLINTEGER dynamic_fn_code;
    SQLINTEGER diag_number;
    SQLINTEGER return_code;
    SQLINTEGER row_count;
}error_header;

extern char *error_messages[];

enum error_codes { 
 sqlerror_01004=0,
 sqlerror_01S02,
 sqlerror_01S07,
 sqlerror_01S09,
 sqlerror_07002,
 sqlerror_07005,
 sqlerror_07006,
 sqlerror_07009,
 sqlerror_08001,
 sqlerror_08002,
 sqlerror_08003,
 sqlerror_08004,
 sqlerror_08S01,
 sqlerror_21S02,
 sqlerror_22001,
 sqlerror_22002,
 sqlerror_22003,
 sqlerror_22007,
 sqlerror_22008,
 sqlerror_22012,
 sqlerror_22015,
 sqlerror_23000,
 sqlerror_24000,
 sqlerror_25000,
 sqlerror_25S01,
 sqlerror_34000,
 sqlerror_3C000,
 sqlerror_40001,
 sqlerror_42000,
 sqlerror_42S02,
 sqlerror_42S02a,
 sqlerror_42S22,
 sqlerror_HY000,
 sqlerror_HY001,
 sqlerror_HY003,
 sqlerror_HY004,
 sqlerror_HY007,
 sqlerror_HY008,
 sqlerror_HY009,
 sqlerror_HY010,
 sqlerror_HY011,
 sqlerror_HY012,
 sqlerror_HY017,
 sqlerror_HY019,
 sqlerror_HY020,
 sqlerror_HY024,
 sqlerror_HY090,
 sqlerror_HY091,
 sqlerror_HY092,
 sqlerror_HY096,
 sqlerror_HY106,
 sqlerror_HY109,
 sqlerror_HY111,
 sqlerror_HYC00,
 sqlerror_HYT00,
 sqlerror_IM001,
 sqlerror_IM002,
 sqlerror_S1107,
 sqlerror_S1108,
 sqlerror_S1C00
};

#define ERROR_MESSAGE_01004 error_messages[sqlerror_01004]
#define ERROR_MESSAGE_01S02 error_messages[sqlerror_01S02]
#define ERROR_MESSAGE_01S07 error_messages[sqlerror_01S07]
#define ERROR_MESSAGE_01S09 error_messages[sqlerror_01S09]
#define ERROR_MESSAGE_40001 error_messages[sqlerror_40001]
#define ERROR_MESSAGE_42000 error_messages[sqlerror_42000]
#define ERROR_MESSAGE_07002 error_messages[sqlerror_07002]
#define ERROR_MESSAGE_07005 error_messages[sqlerror_07005]
#define ERROR_MESSAGE_07006 error_messages[sqlerror_07006]
#define ERROR_MESSAGE_07009 error_messages[sqlerror_07009]
#define ERROR_MESSAGE_08001 error_messages[sqlerror_08001]
#define ERROR_MESSAGE_08002 error_messages[sqlerror_08002]
#define ERROR_MESSAGE_08003 error_messages[sqlerror_08003]
#define ERROR_MESSAGE_08004 error_messages[sqlerror_08004]
#define ERROR_MESSAGE_08S01 error_messages[sqlerror_08S01]
#define ERROR_MESSAGE_21S02 error_messages[sqlerror_21S02]
#define ERROR_MESSAGE_22001 error_messages[sqlerror_22001]
#define ERROR_MESSAGE_22002 error_messages[sqlerror_22002]
#define ERROR_MESSAGE_22003 error_messages[sqlerror_22003]
#define ERROR_MESSAGE_22007 error_messages[sqlerror_22007]
#define ERROR_MESSAGE_22008 error_messages[sqlerror_22008]
#define ERROR_MESSAGE_22012 error_messages[sqlerror_22012]
#define ERROR_MESSAGE_22015 error_messages[sqlerror_22015]
#define ERROR_MESSAGE_23000 error_messages[sqlerror_23000]
#define ERROR_MESSAGE_24000 error_messages[sqlerror_24000]
#define ERROR_MESSAGE_25000 error_messages[sqlerror_25000]
#define ERROR_MESSAGE_25S01 error_messages[sqlerror_25S01]
#define ERROR_MESSAGE_34000 error_messages[sqlerror_34000]
#define ERROR_MESSAGE_3C000 error_messages[sqlerror_3C000]
#define ERROR_MESSAGE_42000 error_messages[sqlerror_42000]
#define ERROR_MESSAGE_42S02 error_messages[sqlerror_42S02]
#define ERROR_MESSAGE_42S02a error_messages[sqlerror_42S02a]
#define ERROR_MESSAGE_42S22 error_messages[sqlerror_42S22]
#define ERROR_MESSAGE_HY000 error_messages[sqlerror_HY000]
#define ERROR_MESSAGE_HY001 error_messages[sqlerror_HY001]
#define ERROR_MESSAGE_HY003 error_messages[sqlerror_HY003]
#define ERROR_MESSAGE_HY004 error_messages[sqlerror_HY004]
#define ERROR_MESSAGE_HY007 error_messages[sqlerror_HY007]
#define ERROR_MESSAGE_HY008 error_messages[sqlerror_HY008]
#define ERROR_MESSAGE_HY009 error_messages[sqlerror_HY009]
#define ERROR_MESSAGE_HY010 error_messages[sqlerror_HY010]
#define ERROR_MESSAGE_HY011 error_messages[sqlerror_HY011]
#define ERROR_MESSAGE_HY012 error_messages[sqlerror_HY012]
#define ERROR_MESSAGE_HY017 error_messages[sqlerror_HY017]
#define ERROR_MESSAGE_HY019 error_messages[sqlerror_HY019]
#define ERROR_MESSAGE_HY020 error_messages[sqlerror_HY020]
#define ERROR_MESSAGE_HY024 error_messages[sqlerror_HY024]
#define ERROR_MESSAGE_HY090 error_messages[sqlerror_HY090]
#define ERROR_MESSAGE_HY091 error_messages[sqlerror_HY091]
#define ERROR_MESSAGE_HY092 error_messages[sqlerror_HY092]
#define ERROR_MESSAGE_HY096 error_messages[sqlerror_HY096]
#define ERROR_MESSAGE_HY106 error_messages[sqlerror_HY106]
#define ERROR_MESSAGE_HY109 error_messages[sqlerror_HY109]
#define ERROR_MESSAGE_HY111 error_messages[sqlerror_HY111]
#define ERROR_MESSAGE_HYC00 error_messages[sqlerror_HYC00]
#define ERROR_MESSAGE_HYT00 error_messages[sqlerror_HYT00]
#define ERROR_MESSAGE_IM001 error_messages[sqlerror_IM001]
#define ERROR_MESSAGE_IM002 error_messages[sqlerror_IM002]
#define ERROR_MESSAGE_S1107 error_messages[sqlerror_S1107]
#define ERROR_MESSAGE_S1108 error_messages[sqlerror_S1108]
#define ERROR_MESSAGE_S1C00 error_messages[sqlerror_S1C00]

extern char *error_states[];

#define ERROR_STATE_01004 error_states[sqlerror_01004]
#define ERROR_STATE_01S02 error_states[sqlerror_01S02]
#define ERROR_STATE_01S07 error_states[sqlerror_01S07]
#define ERROR_STATE_01S09 error_states[sqlerror_01S09]
#define ERROR_STATE_40001 error_states[sqlerror_40001]
#define ERROR_STATE_42000 error_states[sqlerror_42000]
#define ERROR_STATE_07002 error_states[sqlerror_07002]
#define ERROR_STATE_07005 error_states[sqlerror_07005]
#define ERROR_STATE_07006 error_states[sqlerror_07006]
#define ERROR_STATE_07009 error_states[sqlerror_07009]
#define ERROR_STATE_08001 error_states[sqlerror_08001]
#define ERROR_STATE_08002 error_states[sqlerror_08002]
#define ERROR_STATE_08003 error_states[sqlerror_08003]
#define ERROR_STATE_08004 error_states[sqlerror_08004]
#define ERROR_STATE_08S01 error_states[sqlerror_08S01]
#define ERROR_STATE_21S02 error_states[sqlerror_21S02]
#define ERROR_STATE_22001 error_states[sqlerror_22001]
#define ERROR_STATE_22002 error_states[sqlerror_22002]
#define ERROR_STATE_22003 error_states[sqlerror_22003]
#define ERROR_STATE_22007 error_states[sqlerror_22007]
#define ERROR_STATE_22008 error_states[sqlerror_22008]
#define ERROR_STATE_22012 error_states[sqlerror_22012]
#define ERROR_STATE_22015 error_states[sqlerror_22015]
#define ERROR_STATE_23000 error_states[sqlerror_23000]
#define ERROR_STATE_24000 error_states[sqlerror_24000]
#define ERROR_STATE_25000 error_states[sqlerror_25000]
#define ERROR_STATE_25S01 error_states[sqlerror_25S01]
#define ERROR_STATE_34000 error_states[sqlerror_34000]
#define ERROR_STATE_3C000 error_states[sqlerror_3C000]
#define ERROR_STATE_42000 error_states[sqlerror_42000]
#define ERROR_STATE_42S02 error_states[sqlerror_42S02]
#define ERROR_STATE_42S02a error_states[sqlerror_42S02a]
#define ERROR_STATE_42S22 error_states[sqlerror_42S22]
#define ERROR_STATE_HY000 error_states[sqlerror_HY000]
#define ERROR_STATE_HY001 error_states[sqlerror_HY001]
#define ERROR_STATE_HY003 error_states[sqlerror_HY003]
#define ERROR_STATE_HY004 error_states[sqlerror_HY004]
#define ERROR_STATE_HY007 error_states[sqlerror_HY007]
#define ERROR_STATE_HY008 error_states[sqlerror_HY008]
#define ERROR_STATE_HY009 error_states[sqlerror_HY009]
#define ERROR_STATE_HY010 error_states[sqlerror_HY010]
#define ERROR_STATE_HY011 error_states[sqlerror_HY011]
#define ERROR_STATE_HY012 error_states[sqlerror_HY012]
#define ERROR_STATE_HY017 error_states[sqlerror_HY017]
#define ERROR_STATE_HY019 error_states[sqlerror_HY019]
#define ERROR_STATE_HY020 error_states[sqlerror_HY020]
#define ERROR_STATE_HY024 error_states[sqlerror_HY024]
#define ERROR_STATE_HY090 error_states[sqlerror_HY090]
#define ERROR_STATE_HY091 error_states[sqlerror_HY091]
#define ERROR_STATE_HY092 error_states[sqlerror_HY092]
#define ERROR_STATE_HY096 error_states[sqlerror_HY096]
#define ERROR_STATE_HY106 error_states[sqlerror_HY106]
#define ERROR_STATE_HY109 error_states[sqlerror_HY109]
#define ERROR_STATE_HY111 error_states[sqlerror_HY111]
#define ERROR_STATE_HYC00 error_states[sqlerror_HYC00]
#define ERROR_STATE_HYT00 error_states[sqlerror_HYT00]
#define ERROR_STATE_IM001 error_states[sqlerror_IM001]
#define ERROR_STATE_IM002 error_states[sqlerror_IM002]
#define ERROR_STATE_S1107 error_states[sqlerror_S1107]
#define ERROR_STATE_S1108 error_states[sqlerror_S1108]
#define ERROR_STATE_S1C00 error_states[sqlerror_S1C00]

extern char *error_origins[];

enum origin_codes {
    sqloriginISO9075=0,
    sqloriginODBC3_0,
    sqloriginODBC2_0
};

#define ERROR_ORIGIN_01004 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_01S02 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_01S07 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_01S09 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_40001 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_42000 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_07002 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_07005 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_07006 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_07009 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_08001 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_08002 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_08003 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_08004 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_08S01 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_21S02 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_22001 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_22002 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_22003 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_22007 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_22008 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_22012 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_22015 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_23000 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_24000 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_25000 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_25S01 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_34000 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_3C000 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_42000 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_42S02 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_42S02a error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_42S22 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY000 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY001 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY003 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY004 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY007 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY008 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY009 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY010 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY011 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY012 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY013 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY017 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY019 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY020 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY024 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY090 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY091 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY092 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY096 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY106 error_origins[sqloriginISO9075]
#define ERROR_ORIGIN_HY109 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_HY111 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_HYC00 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_HYT00 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_IM001 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_IM002 error_origins[sqloriginODBC3_0]
#define ERROR_ORIGIN_S1107 error_origins[sqloriginODBC2_0]
#define ERROR_ORIGIN_S1108 error_origins[sqloriginODBC2_0]
#define ERROR_ORIGIN_S1C00 error_origins[sqloriginODBC2_0]


/*
 * ood_post_diag puts another diag rec on the SQLHANDLE
 */
void ood_post_diag(struct hgeneric_TAG *hH,
        char* origin,
        SQLINTEGER column_number,
        char *connection_name,
        char *message,
        SQLINTEGER native,
        SQLINTEGER row_number,
        char *server_name,
        char *sqlstate,
        char *file,
        int line);

void ood_free_diag(struct hgeneric_TAG *hH);
void ood_clear_diag(struct hgeneric_TAG *hH);
#endif
