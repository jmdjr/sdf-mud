CC      = gcc
#PROF    = -p
NOCRYPT =

# Alter the directory below and mud.h to set the debugger.
#DMALLOC = -ldmalloclp -ldmalloc -ldl

# Uncomment the two lines below if compiling on a Solaris box.
#SOLARIS_FLAG = -Dsun -DSYSV
#SOLARIS_LINK = -lnsl -lsocket

# Uncomment the line below if you are getting undefined crypt errors.
NEED_CRYPT = -lcrypt

# Uncomment the line below if you are getting a line like:
# interp.c:757: warning: int format, time_t arg (arg 7)
TIMEFLAG = -DTIMEFORMAT

# Comment out below if you don't want compression.
MCCP = -DMCCP

# Comment below if you do not want to tax players holding 10m or 500k in bank.
#TAXPLAYERS = -DTAX_PLAYERS

# Comment out below if you don't want ispell.
#ISPELL = -DISPELL

# Comment the line below to prevent the Websvr code from being compiled in
WEBSVR = yes

#COMPRESSION = -DCOMPRESSION_TEST

DBUGFLG = -DREQUESTS

WARN_FLAGS = -Wall #-Werror

# Uncomment the line below if you want a performance increase though beware
# your core files may not be as much of a benefit if you do.
#OPT_FLAG = -finline-functions -funroll-loops -fdefer-pop -fstrength-reduce

C_FLAGS = $(OPT_FLAG) -ggdb3 $(PROF) $(TAXPLAYERS) $(TIMEFLAG) $(NOCRYPT) \
	$(DBUGFLG) $(SOLARIS_FLAG) $(ISPELL) $(MCCP) $(I3) $(WARN_FLAGS)
L_FLAGS = $(OPT_FLAG) $(PROF) $(SOLARIS_LINK) $(NEED_CRYPT) $(DMALLOC) -lz

IMC_FILES = ice.c icec.c icec-mercbase.c imc.c imc-color.c imc-config.c \
	imc-events.c imc-interp.c imc-mail.c imc-mercbase.c imc-util.c \
	imc-version.c crwrapper.c minilzo.c

C_FILES = act_comm.c act_info.c act_move.c act_obj.c act_wiz.c antitank.c \
	arena.c assa.c axsmaug.c ban.c bank.c base64.c bits.c boards.c \
	bounty.c build.c buffer.c clans.c comm.c comments.c const.c db.c \
	deity.c disable.c do.c fear.c fight.c gboards.c grub.c handler.c \
	hashstr.c healer.c hiscores.c house.c hskelter.c i3.c ibuild.c ident.c \
	imm_host.c immscore.c interp.c ispell.c locker.c lzo.c magic.c \
	makeobjs.c map.c marry.c mccp.c medit.c misc.c mobchat.c motd.c mpxset.c \
	mud_comm.c mud_prog.c news.c oedit.c planes.c player.c polymorph.c quest.c \
	redit.c remort.c requests.c reset.c save.c services.c sharpen.c \
	shops.c skills.c slay.c special.c starmap.c stat_obj.c suicide.c \
	tables.c track.c update.c familiar.c $(IMC_FILES)

# Modify the file lists if the webserver has been defined to be compiled.
ifdef WEBSVR
    C_FILES := $(C_FILES) websvr.c web-imc.c
    C_FLAGS := $(C_FLAGS) -DWEBSVR
endif

O_FILES := $(patsubst %.c,o/%.o,$(C_FILES))

H_FILES = $(wildcard *.h)

all:
	make -s rmexe

rmexe: $(O_FILES)
	rm -f rmexe
	$(CC) $(L_FLAGS) -o rmexe $(O_FILES) $(DMALLOC)
	echo "Done compiling rmexe...";

# Check in all files
checkin:
	ci -l $(C_FILES) $(H_FILES) Makefile
	rm -f *~

# Make a unified diff of all files.
diff:
	rcsdiff -u $(C_FILES) $(H_FILES) Makefile > ../diff.new
	gzip -9 ../diff.new

ftp:
	scripts/ftp-diff
	rm ../diff.new.gz
        
patch:
	gunzip diff.new.gz
	patch < diff.new
	rm diff.new

o/%.o: %.c
	echo "	Now compiling... $@";
	$(CC) -c $(C_FLAGS) $< -o $@

clean:
	rm -f o/*.o rmexe *~
