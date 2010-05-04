CC      = gcc
#PROF    = -p

# Uncomment the two lines below if compiling on a Solaris box.
#SOLARIS_FLAG = -Dsun -DSYSV
#SOLARIS_LINK = -lnsl -lsocket

# Comment out below if you don't want compression.
MCCP = -DMCCP

# Comment below if you do not want to tax players holding 10m or 500k in bank.
TAXPLAYERS = -DTAX_PLAYERS

# Comment out below if you don't want ispell.
ISPELL = -DISPELL

#IMC2 - Comment out to disable IMC2 support
IMC = 1

#Intermud-3 - Comment out to disable I3 support in your code
I3 = 1

# Comment the line below to prevent the Websvr code from being compiled in
WEBSVR = yes

#COMPRESSION = -DCOMPRESSION_TEST

DBUGFLG = -DREQUESTS

WARN_FLAGS = -Wall -Wshadow -Wredundant-decls -Wpointer-arith -Wcast-align -Wformat-security #-Werror

C_FLAGS = -g3 $(PROF) $(TAXPLAYERS) $(TIMEFLAG) $(DBUGFLG) $(SOLARIS_FLAG) $(ISPELL) $(MCCP) $(WARN_FLAGS)
L_FLAGS = $(PROF) $(SOLARIS_LINK) -lcrypt -lz

C_FILES = act_comm.c act_info.c act_move.c act_obj.c act_wiz.c antitank.c \
	arena.c assa.c ban.c bank.c base64.c bits.c boards.c \
	bounty.c build.c buffer.c changes.c channels.c cbackup.c clans.c \
	comm.c comments.c const.c db.c deity.c disable.c do.c familiar.c \
	fear.c fight.c gboards.c grub.c handler.c hashstr.c healer.c \
	hiscores.c hometowns.c house.c hskelter.c imm_host.c immscore.c \
	interp.c ispell.c locker.c magic.c makeobjs.c map.c marry.c mccp.c \
	medit.c misc.c mobchat.c motd.c mpxset.c mud_comm.c mud_prog.c \
	mudmsg.c news.c oedit.c pfiles.c  planes.c player.c polymorph.c quest.c \
	redit.c remort.c requests.c reset.c save.c services.c sharpen.c \
	shops.c skills.c slay.c special.c starmap.c stat_obj.c suicide.c \
	tables.c track.c trivia.c update.c

#ifdef IMC
#   C_FILES := imc.c sha256.c $(C_FILES)
#   C_FLAGS := $(C_FLAGS) -DIMC -DIMCSMAUG
#endif
#
#ifdef I3
#   C_FILES := i3.c $(C_FILES)
#   C_FLAGS := $(C_FLAGS) -DI3 -DI3SMAUG
#endif
#
# Modify the file lists if the webserver has been defined to be compiled.
ifdef WEBSVR
   C_FILES := $(C_FILES) websvr.c
   C_FLAGS := $(C_FLAGS) -DWEBSVR
endif

O_FILES := $(patsubst %.c,o/%.o,$(C_FILES))

H_FILES = $(wildcard *.h)

all:
	$(MAKE) -s rmexe

# pull in dependency info for *existing* .o files
-include dependencies.d

rmexe: $(O_FILES)
	rm -f rmexe
	$(CC) -o rmexe $(O_FILES) $(L_FLAGS)
	echo "Generating dependency file ...";
	$(CC) -MM $(C_FLAGS) $(C_FILES) > dependencies.d
	perl -pi -e 's.^([a-z]).o/$$1.g' dependencies.d
	echo "Done compiling rmexe...";
	chmod 770 rmexe

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

indent:
	indent -ts3 -nut -nsaf -nsai -nsaw -npcs -npsl -ncs -nbc -bls -prs -bap -cbi0 -cli3 -bli0 -l125 -lp -i3 -cdb -c1 -cd1 -sc -pmt $(C_FILES)
	indent -ts3 -nut -nsaf -nsai -nsaw -npcs -npsl -ncs -nbc -bls -prs -bap -cbi0 -cli3 -bli0 -l125 -lp -i3 -cdb -c1 -cd1 -sc -pmt $(H_FILES)

indentclean:
	rm *.c~ *.h~

o/%.o: %.c
	echo "	Now compiling... $@";
	$(CC) -c $(C_FLAGS) $< -o $@

clean:
	rm -f o/*.o rmexe dependencies.d *~
	@echo "Generating dependency file ...";
	@$(CC) -MM $(C_FLAGS) $(C_FILES) > dependencies.d
	@perl -pi -e 's.^([a-z]).o/$$1.g' dependencies.d
