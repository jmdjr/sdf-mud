/*******************************************************
/*
#define MAX_DOMAIN     10

{
   IMMORTAL_HOST_LOG *next;
   IMMORTAL_HOST_LOG *prev;
   char *host;
   char *date;
};


{
   IMMORTAL_HOST *next;
   IMMORTAL_HOST *prev;
   char *name;
   char *domain[MAX_DOMAIN];
   IMMORTAL_HOST_LOG *first_log;
   IMMORTAL_HOST_LOG *last_log;
};

IMMORTAL_HOST *last_imm_host;

IMMORTAL_HOST *fread_imm_host( FILE * fp );
IMMORTAL_HOST_LOG *fread_imm_host_log( FILE * fp );
void save_imm_host( void );
bool check_immortal_domain( CHAR_DATA * ch, char *host );
