import SCons.cpp

# component options

# make all component false
RT_USING_FINSH      = False
RT_USING_DFS        = False
RT_USING_DFS_ELMFAT = False
RT_USING_DFS_YAFFS2 = False
RT_USING_DFS_NFS    = False
RT_USING_DFS_ROMFS  = False
RT_USING_DFS_DEVFS  = False
RT_USING_LWIP       = False
RT_USING_WEBSERVER  = False
RT_USING_RTGUI      = False
RT_USING_MODBUS     = False
RT_USING_MODULE     = False
RT_USING_FTK        = False
RT_USING_NEWLIB     = False
RT_USING_PTHREAD    = False

# parse rtconfig.h to get used component
PreProcessor = SCons.cpp.PreProcessor()
f = file('rtconfig.h', 'r')
contents = f.read()
f.close()
PreProcessor.process_contents(contents)
rtconfig_ns = PreProcessor.cpp_namespace

# libc options
if rtconfig_ns.has_key('RT_USING_NEWLIB'):
    RT_USING_NEWLIB = True

if rtconfig_ns.has_key('RT_USING_PTHREAD'):
    RT_USING_PTHREAD = True

# finsh shell options
if rtconfig_ns.has_key('RT_USING_FINSH'):
    RT_USING_FINSH = True

# device virtual filesystem options
if rtconfig_ns.has_key('RT_USING_DFS'):
    RT_USING_DFS = True

    if rtconfig_ns.has_key('RT_USING_DFS_ELMFAT'):
        RT_USING_DFS_ELMFAT = True
    if rtconfig_ns.has_key('RT_DFS_ELM_USE_LFN'):
        RT_DFS_ELM_USE_LFN = True           
    if rtconfig_ns.has_key('RT_USING_DFS_YAFFS2'):
        RT_USING_DFS_YAFFS2 = True
    if rtconfig_ns.has_key('RT_USING_DFS_NFS'):
        RT_USING_DFS_NFS    = True
    if rtconfig_ns.has_key('RT_USING_DFS_ROMFS'):
        RT_USING_DFS_ROMFS    = True
    if rtconfig_ns.has_key('RT_USING_DFS_DEVFS'):
        RT_USING_DFS_DEVFS    = True

# lwip options
if rtconfig_ns.has_key('RT_USING_LWIP'):
    RT_USING_LWIP = True
    if rtconfig_ns.has_key('RT_USING_WEBSERVER'):
        RT_USING_WEBSERVER = True

# rtgui options
if rtconfig_ns.has_key('RT_USING_RTGUI'):
    RT_USING_RTGUI = True

if rtconfig_ns.has_key('RT_USING_FTK'):
    RT_USING_FTK = True

# module options
if rtconfig_ns.has_key('RT_USING_MODULE'):
    RT_USING_MODULE = True
		
# panel options
# 'PNL_A70','PNL_N35', 'PNL_T35'
RT_USING_LCD_TYPE = 'PNL_T35'

# toolchains options
ARCH     = 'arm'
CPU      = 's3c24x0'
TextBase = '0x30000000'

CROSS_TOOL 	= 'gcc'

if  CROSS_TOOL == 'gcc':
	PLATFORM 	= 'gcc'
	EXEC_PATH 	= 'E:/Program Files/CodeSourcery/Sourcery G++ Lite/bin'
elif CROSS_TOOL == 'keil':
	PLATFORM 	= 'armcc'
	EXEC_PATH 	= 'E:/Keil'
BUILD = 'debug'

if PLATFORM == 'gcc':
    # toolchains
    PREFIX = 'arm-none-eabi-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'axf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'

    DEVICE = ' -mcpu=arm920t'
    CFLAGS = DEVICE
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp' + ' -DTEXT_BASE=' + TextBase
    LFLAGS = DEVICE + ' -Wl,--gc-sections,-Map=rtthread_mini2440.map,-cref,-u,_start -T mini2440_ram.ld' + ' -Ttext ' + TextBase

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -gdwarf-2'
        AFLAGS += ' -gdwarf-2'
    else:
        CFLAGS += ' -O2'

    if RT_USING_WEBSERVER:
        CFLAGS += ' -DWEBS -DUEMF -DRTT -D__NO_FCNTL=1 -DRT_USING_WEBSERVER'

    POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'

elif PLATFORM == 'armcc':
    # toolchains
    CC = 'armcc'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'
    TARGET_EXT = 'axf'

    DEVICE = ' --device DARMSS9'
    CFLAGS = DEVICE + ' --apcs=interwork --diag_suppress=870'
    AFLAGS = DEVICE
    LFLAGS = DEVICE + ' --strict --info sizes --info totals --info unused --info veneers --list rtthread-mini2440.map --ro-base 0x30000000 --entry Entry_Point --first Entry_Point'

    CFLAGS += ' -I"' + EXEC_PATH + '/ARM/RV31/INC"'
    LFLAGS += ' --libpath "' + EXEC_PATH + '/ARM/RV31/LIB"'

    EXEC_PATH += '/arm/bin40/'

    if BUILD == 'debug':
        CFLAGS += ' -g -O0'
        AFLAGS += ' -g'
    else:
        CFLAGS += ' -O2'

    RT_USING_MINILIBC = False
    if RT_USING_WEBSERVER:
        CFLAGS += ' -DWEBS -DUEMF -DRTT -D__NO_FCNTL=1 -DRT_USING_WEBSERVER'
    POST_ACTION = 'fromelf --bin $TARGET --output rtthread.bin \nfromelf -z $TARGET'

elif PLATFORM == 'iar':
    # toolchains
    CC = 'armcc'
    AS = 'armasm'
    AR = 'armar'
    LINK = 'armlink'

    CFLAGS = ''
    AFLAGS = ''
    LFLAGS = ''