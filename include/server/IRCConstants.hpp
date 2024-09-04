#pragma once

#define MAX_EVENTS 1024
#define CASEMAPPING "rfc1459"
#define CHANLIMIT 20
#define CHANMODES "b,k,l,ito"
#define CHANNELLEN 64
#define CHANTYPES "#"
#define KEYLEN 32
#define KICKLEN 255
#define LINELEN 512
#define MAXTARGETS 20
#define MODES 20
#define NAMELEN 128
#define NETWORK "Localnet"
#define NICKLEN 30
#define PREFIX "(o)@"
#define TOPICLEN 307
#define USERLEN 10
#define USERMODES ",,s,iow"

/**
 * :irc.local 005 test
 * CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst
 * CHANNELLEN=64 CHANTYPES=# HOSTLEN=64 KEYLEN=32
 * KICKLEN=255 LINELEN=512 :are supported by this server
 * :irc.local 005 test
 * MAXTARGETS=20 MODES=20 NAMELEN=128 NETWORK=Localnet
 * NICKLEN=30 PREFIX=(ov)@+ TOPICLEN=307
 * USERLEN=10 USERMODES=,,s,iow :are supported by this server
 */