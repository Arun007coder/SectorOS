#include "../Include/types.h"
#include "gdt.h"
#include "../CPU/Interrupts.h"
#include "../Drivers/IOPorts.h"
#include "../Drivers/Keyboard.h"
#include "../Shell/Shell.h"
#include "../Network/arp.h"
#include "../Include/multiboot.h"
#include "../Network/EtherFrame.h"
#include "../Network/tcp.h"
#include "../CPU/Execptions/GPF.h"
#include "../CPU/Execptions/IOC.h"
#include "../CPU/Execptions/DFault.h"
#include "../Network/ICMP.h"
#include "../loaders/JD1618.h"
#include "../Network/udp.h"
#include "../Network/InternetProtocolV4.h"
#include "../Drivers/AM79C973.h"
#include "../Include/func.h"
#include "Settings.h"
#include "../Hardcom/SerialPort.h"
#include "../CPU/syscall.h"
#include "../Drivers/Mouse.h"
#include "../Drivers/Driver.h"
#include "../Drivers/RTC.h"
#include "../Hardcom/pci.h"
#include "../Hardcom/SerialPort.h"
#include "../Include/Public_VAR.h"
#include "MultiTask.h"
#include "../Memory/MemoryManagement.h"
#include "../Include/Debug.h"
#include "../Drivers/HDD-ATA.h"
#include "../Drivers/VGADriver.h"
#include "../Filesystem/MSDOSPT.h"
#include "../Include/string.h"

using namespace std;

static uint8_t cursory;
static uint8_t cursorx;
static bool useMouse = true;
bool isused;

extern uint32_t *memupper;
extern void *allocated;

uint32_t *memupper;
void *allocated;

bool isPrefix;
bool lkl;
int index = 0;
int spind = 0;

static uint8_t x = 0, y = 0;

bool isTxtMode;
KeyboardDriver *kbrd;
DriverManager *drvMngr;
InterruptManager *intMngr;
SyscallHandler *shand;
extern const void *mb;
uint32_t mm;
inline GlobalDescriptorTable gdt;
AdvancedTechnologyAttachment ata0m(0x1F0, true);
AdvancedTechnologyAttachment ata0s(0x1F0, false);

port8BIT port43(0x43);
port8BIT port42(0x42);
port8BIT port61(0x61);
static uint16_t *VideoMemory = (uint16_t *)0xb8000;
Settings set;

char *INTTOCHARPOINT(int);
void printf(char *);

void wait(unsigned int msecs)
{
    asm("sti");
    while (msecs-- > 0)
    {
        asm("hlt");
    }
}

static void play_sound(uint32_t nFrequence)
{
    uint32_t Div;
    uint8_t tmp;

    // Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    port43.WriteToPort(0xb6);
    port42.WriteToPort((uint8_t)(Div));
    port42.WriteToPort((uint8_t)(Div >> 8));

    // And play the sound using the PC speaker
    tmp = port61.ReadFromPort();
    if (tmp != (tmp | 3))
    {
        port61.WriteToPort(tmp | 3);
    }
}

static void nosound()
{
    uint8_t tmp = port61.ReadFromPort() & 0xFC;

    port61.WriteToPort(tmp);
}

void beep()
{
    play_sound(1000);
    wait(6);
    nosound();
}

void BootSound()
{
    play_sound(1000);
    wait(4);
    nosound();
    play_sound(600);
    wait(4);
    nosound();
    play_sound(300);
    wait(4);
    nosound();
    play_sound(1300);
    wait(4);
    nosound();
}

// Can Support up to number 400
char *INTTOCHARPOINT(int num)
{
    char *res;
    switch (num)
    {
    case 0:
        res = "00";
        break;
    case 1:
        res = "01";
        break;
    case 2:
        res = "02";
        break;
    case 3:
        res = "03";
        break;
    case 4:
        res = "04";
        break;
    case 5:
        res = "05";
        break;
    case 6:
        res = "06";
        break;
    case 7:
        res = "07";
        break;
    case 8:
        res = "08";
        break;
    case 9:
        res = "09";
        break;

    case 10:
        res = "10";
        break;
    case 11:
        res = "11";
        break;
    case 12:
        res = "12";
        break;
    case 13:
        res = "13";
        break;
    case 14:
        res = "14";
        break;
    case 15:
        res = "15";
        break;
    case 16:
        res = "16";
        break;
    case 17:
        res = "17";
        break;
    case 18:
        res = "18";
        break;
    case 19:
        res = "19";
        break;
    case 20:
        res = "20";
        break;
    case 21:
        res = "21";
        break;
    case 22:
        res = "22";
        break;
    case 23:
        res = "23";
        break;
    case 24:
        res = "24";
        break;
    case 25:
        res = "25";
        break;
    case 26:
        res = "26";
        break;
    case 27:
        res = "26";
        break;
    case 28:
        res = "28";
        break;
    case 29:
        res = "29";
        break;
    case 30:
        res = "30";
        break;
    case 31:
        res = "31";
        break;
    case 32:
        res = "32";
        break;
    case 33:
        res = "33";
        break;
    case 34:
        res = "34";
        break;
    case 35:
        res = "35";
        break;
    case 36:
        res = "36";
        break;
    case 37:
        res = "37";
        break;
    case 38:
        res = "38";
        break;
    case 39:
        res = "39";
        break;
    case 40:
        res = "40";
        break;
    case 41:
        res = "41";
        break;
    case 42:
        res = "42";
        break;
    case 43:
        res = "43";
        break;
    case 44:
        res = "44";
        break;
    case 45:
        res = "45";
        break;
    case 46:
        res = "46";
        break;
    case 47:
        res = "47";
        break;
    case 48:
        res = "48";
        break;
    case 49:
        res = "49";
        break;
    case 50:
        res = "50";
        break;
    case 51:
        res = "51";
        break;
    case 52:
        res = "52";
        break;
    case 53:
        res = "53";
        break;
    case 54:
        res = "54";
        break;
    case 55:
        res = "55";
        break;
    case 56:
        res = "56";
        break;
    case 57:
        res = "57";
        break;
    case 58:
        res = "58";
        break;
    case 59:
        res = "59";
        break;
    case 60:
        res = "60";
        break;
    case 61:
        res = "61";
        break;
    case 62:
        res = "62";
        break;
    case 63:
        res = "63";
        break;
    case 64:
        res = "64";
        break;
    case 65:
        res = "65";
        break;
    case 66:
        res = "66";
        break;
    case 67:
        res = "67";
        break;
    case 68:
        res = "68";
        break;
    case 69:
        res = "69";
        break;
    case 70:
        res = "70";
        break;
    case 71:
        res = "71";
        break;
    case 72:
        res = "72";
        break;
    case 73:
        res = "73";
        break;
    case 74:
        res = "74";
        break;
    case 75:
        res = "75";
        break;
    case 76:
        res = "76";
        break;
    case 77:
        res = "77";
        break;
    case 78:
        res = "78";
        break;
    case 79:
        res = "79";
        break;
    case 80:
        res = "80";
        break;
    case 81:
        res = "81";
        break;
    case 82:
        res = "82";
        break;
    case 83:
        res = "83";
        break;
    case 84:
        res = "84";
        break;
    case 85:
        res = "85";
        break;
    case 86:
        res = "86";
        break;
    case 87:
        res = "87";
        break;
    case 88:
        res = "88";
        break;
    case 89:
        res = "89";
        break;
    case 90:
        res = "90";
        break;
    case 91:
        res = "91";
        break;
    case 92:
        res = "92";
        break;
    case 93:
        res = "93";
        break;
    case 94:
        res = "94";
        break;
    case 95:
        res = "95";
        break;
    case 96:
        res = "96";
        break;
    case 97:
        res = "97";
        break;
    case 98:
        res = "98";
        break;
    case 99:
        res = "99";
        break;

    case 100:
        res = "100";
        break;
    case 101:
        res = "101";
        break;
    case 102:
        res = "102";
        break;
    case 103:
        res = "103";
        break;
    case 104:
        res = "104";
        break;
    case 105:
        res = "105";
        break;
    case 106:
        res = "106";
        break;
    case 107:
        res = "107";
        break;
    case 108:
        res = "108";
        break;
    case 109:
        res = "109";
        break;
    case 110:
        res = "110";
        break;
    case 111:
        res = "111";
        break;
    case 112:
        res = "112";
        break;
    case 113:
        res = "113";
        break;
    case 114:
        res = "114";
        break;
    case 115:
        res = "115";
        break;
    case 116:
        res = "116";
        break;
    case 117:
        res = "117";
        break;
    case 118:
        res = "118";
        break;
    case 119:
        res = "119";
        break;
    case 120:
        res = "120";
        break;
    case 121:
        res = "121";
        break;
    case 122:
        res = "122";
        break;
    case 123:
        res = "123";
        break;
    case 124:
        res = "124";
        break;
    case 125:
        res = "125";
        break;
    case 126:
        res = "126";
        break;
    case 127:
        res = "127";
        break;
    case 128:
        res = "128";
        break;
    case 129:
        res = "129";
        break;
    case 130:
        res = "130";
        break;
    case 131:
        res = "131";
        break;
    case 132:
        res = "132";
        break;
    case 133:
        res = "133";
        break;
    case 135:
        res = "135";
        break;
    case 136:
        res = "136";
        break;
    case 137:
        res = "137";
        break;
    case 138:
        res = "138";
        break;
    case 139:
        res = "139";
        break;
    case 140:
        res = "140";
        break;
    case 141:
        res = "141";
        break;
    case 142:
        res = "142";
        break;
    case 143:
        res = "143";
        break;
    case 150:
        res = "150";
        break;
    case 151:
        res = "151";
        break;
    case 152:
        res = "152";
        break;
    case 153:
        res = "153";
        break;
    case 154:
        res = "154";
        break;
    case 155:
        res = "155";
        break;
    case 156:
        res = "156";
        break;
    case 157:
        res = "157";
        break;
    case 158:
        res = "158";
        break;
    case 159:
        res = "159";
        break;
    case 160:
        res = "160";
        break;
    case 161:
        res = "161";
        break;
    case 162:
        res = "162";
        break;
    case 163:
        res = "163";
        break;
    case 164:
        res = "164";
        break;
    case 165:
        res = "165";
        break;
    case 166:
        res = "166";
        break;
    case 167:
        res = "167";
        break;
    case 168:
        res = "168";
        break;
    case 169:
        res = "169";
        break;
    case 170:
        res = "170";
        break;
    case 171:
        res = "171";
        break;
    case 172:
        res = "172";
        break;
    case 173:
        res = "173";
        break;
    case 174:
        res = "174";
        break;
    case 175:
        res = "175";
        break;
    case 176:
        res = "176";
        break;
    case 177:
        res = "177";
        break;
    case 178:
        res = "178";
        break;
    case 179:
        res = "179";
        break;
    case 180:
        res = "180";
        break;
    case 181:
        res = "181";
        break;
    case 182:
        res = "182";
        break;
    case 183:
        res = "183";
        break;
    case 184:
        res = "184";
        break;
    case 185:
        res = "185";
        break;
    case 186:
        res = "186";
        break;
    case 187:
        res = "187";
        break;
    case 188:
        res = "188";
        break;
    case 189:
        res = "189";
        break;
    case 190:
        res = "190";
        break;
    case 191:
        res = "191";
        break;
    case 192:
        res = "192";
        break;
    case 193:
        res = "193";
        break;
    case 194:
        res = "194";
        break;
    case 195:
        res = "195";
        break;
    case 196:
        res = "196";
        break;
    case 197:
        res = "197";
        break;
    case 198:
        res = "198";
        break;
    case 199:
        res = "199";
        break;
    case 200:
        res = "200";
        break;
    case 201:
        res = "201";
        break;
    case 202:
        res = "202";
        break;
    case 203:
        res = "203";
        break;
    case 204:
        res = "204";
        break;
    case 205:
        res = "205";
        break;
    case 206:
        res = "206";
        break;
    case 207:
        res = "207";
        break;
    case 208:
        res = "208";
        break;
    case 209:
        res = "209";
        break;
    case 210:
        res = "210";
        break;
    case 211:
        res = "211";
        break;
    case 212:
        res = "212";
        break;
    case 213:
        res = "213";
        break;
    case 214:
        res = "214";
        break;
    case 215:
        res = "215";
        break;
    case 216:
        res = "216";
        break;
    case 217:
        res = "217";
        break;
    case 218:
        res = "218";
        break;
    case 219:
        res = "219";
        break;
    case 220:
        res = "220";
        break;
    case 221:
        res = "221";
        break;
    case 222:
        res = "222";
        break;
    case 223:
        res = "223";
        break;
    case 224:
        res = "224";
        break;
    case 225:
        res = "225";
        break;
    case 226:
        res = "226";
        break;
    case 227:
        res = "227";
        break;
    case 228:
        res = "228";
        break;
    case 229:
        res = "229";
        break;
    case 230:
        res = "230";
        break;
    case 231:
        res = "231";
        break;
    case 232:
        res = "232";
        break;
    case 233:
        res = "233";
        break;
    case 234:
        res = "234";
        break;
    case 235:
        res = "235";
        break;
    case 236:
        res = "236";
        break;
    case 237:
        res = "237";
        break;
    case 238:
        res = "238";
        break;
    case 239:
        res = "239";
        break;
    case 240:
        res = "240";
        break;
    case 241:
        res = "241";
        break;
    case 242:
        res = "242";
        break;
    case 243:
        res = "243";
        break;
    case 244:
        res = "244";
        break;
    case 245:
        res = "245";
        break;
    case 246:
        res = "246";
        break;
    case 247:
        res = "247";
        break;
    case 248:
        res = "248";
        break;
    case 249:
        res = "249";
        break;
    case 250:
        res = "250";
        break;
    case 251:
        res = "251";
        break;
    case 252:
        res = "252";
        break;
    case 253:
        res = "253";
        break;
    case 254:
        res = "254";
        break;
    case 255:
        res = "255";
        break;
    case 256:
        res = "256";
        break;
    case 257:
        res = "257";
        break;
    case 258:
        res = "258";
        break;
    case 259:
        res = "259";
        break;
    case 260:
        res = "260";
        break;
    case 261:
        res = "261";
        break;
    case 262:
        res = "262";
        break;
    case 263:
        res = "263";
        break;
    case 264:
        res = "264";
        break;
    case 265:
        res = "265";
        break;
    case 266:
        res = "266";
        break;
    case 267:
        res = "267";
        break;
    case 268:
        res = "268";
        break;
    case 269:
        res = "269";
        break;
    case 270:
        res = "270";
        break;
    case 271:
        res = "271";
        break;
    case 272:
        res = "272";
        break;
    case 273:
        res = "273";
        break;
    case 274:
        res = "274";
        break;
    case 275:
        res = "275";
        break;
    case 276:
        res = "276";
        break;
    case 277:
        res = "277";
        break;
    case 278:
        res = "278";
        break;
    case 279:
        res = "279";
        break;
    case 280:
        res = "280";
        break;
    case 281:
        res = "281";
        break;
    case 282:
        res = "282";
        break;
    case 283:
        res = "283";
        break;
    case 284:
        res = "284";
        break;
    case 285:
        res = "285";
        break;
    case 286:
        res = "286";
        break;
    case 287:
        res = "287";
        break;
    case 288:
        res = "288";
        break;
    case 289:
        res = "289";
        break;
    case 290:
        res = "290";
        break;
    case 291:
        res = "291";
        break;
    case 292:
        res = "292";
        break;
    case 293:
        res = "293";
        break;
    case 294:
        res = "294";
        break;
    case 295:
        res = "295";
        break;
    case 296:
        res = "296";
        break;
    case 297:
        res = "297";
        break;
    case 298:
        res = "298";
        break;
    case 299:
        res = "299";
        break;
    case 300:
        res = "300";
        break;
    case 301:
        res = "301";
        break;
    case 302:
        res = "302";
        break;
    case 303:
        res = "303";
        break;
    case 304:
        res = "304";
        break;
    case 305:
        res = "305";
        break;
    case 306:
        res = "306";
        break;
    case 307:
        res = "307";
        break;
    case 308:
        res = "308";
        break;
    case 309:
        res = "309";
        break;
    case 310:
        res = "310";
        break;
    case 311:
        res = "311";
        break;
    case 312:
        res = "312";
        break;
    case 313:
        res = "313";
        break;
    case 314:
        res = "314";
        break;
    case 315:
        res = "315";
        break;
    case 316:
        res = "316";
        break;
    case 317:
        res = "317";
        break;
    case 318:
        res = "318";
        break;
    case 319:
        res = "319";
        break;
    case 320:
        res = "320";
        break;
    case 321:
        res = "321";
        break;
    case 322:
        res = "322";
        break;
    case 323:
        res = "323";
        break;
    case 324:
        res = "324";
        break;
    case 325:
        res = "325";
        break;
    case 326:
        res = "326";
        break;
    case 327:
        res = "327";
        break;
    case 328:
        res = "328";
        break;
    case 329:
        res = "329";
        break;
    case 330:
        res = "330";
        break;
    case 331:
        res = "331";
        break;
    case 332:
        res = "332";
        break;
    case 333:
        res = "333";
        break;
    case 334:
        res = "334";
        break;
    case 335:
        res = "335";
        break;
    case 336:
        res = "336";
        break;
    case 337:
        res = "337";
        break;
    case 338:
        res = "338";
        break;
    case 339:
        res = "339";
        break;
    case 340:
        res = "340";
        break;
    case 341:
        res = "341";
        break;
    case 342:
        res = "342";
        break;
    case 343:
        res = "343";
        break;
    case 344:
        res = "344";
        break;
    case 345:
        res = "345";
        break;
    case 346:
        res = "346";
        break;
    case 347:
        res = "347";
        break;
    case 348:
        res = "348";
        break;
    case 349:
        res = "349";
        break;
    case 350:
        res = "350";
        break;
    case 351:
        res = "351";
        break;
    case 352:
        res = "352";
        break;
    case 353:
        res = "353";
        break;
    case 354:
        res = "354";
        break;
    case 355:
        res = "355";
        break;
    case 356:
        res = "356";
        break;
    case 357:
        res = "357";
        break;
    case 358:
        res = "358";
        break;
    case 359:
        res = "359";
        break;
    case 360:
        res = "360";
        break;
    case 361:
        res = "361";
        break;
    case 362:
        res = "362";
        break;
    case 363:
        res = "363";
        break;
    case 364:
        res = "364";
        break;
    case 365:
        res = "365";
        break;
    case 366:
        res = "366";
        break;
    case 367:
        res = "367";
        break;
    case 368:
        res = "368";
        break;
    case 369:
        res = "369";
        break;
    case 370:
        res = "370";
        break;
    case 371:
        res = "371";
        break;
    case 372:
        res = "372";
        break;
    case 373:
        res = "373";
        break;
    case 374:
        res = "374";
        break;
    case 375:
        res = "375";
        break;
    case 376:
        res = "376";
        break;
    case 377:
        res = "377";
        break;
    case 378:
        res = "378";
        break;
    case 379:
        res = "379";
        break;
    case 380:
        res = "380";
        break;
    case 381:
        res = "381";
        break;
    case 382:
        res = "382";
        break;
    case 383:
        res = "383";
        break;
    case 384:
        res = "384";
        break;
    case 385:
        res = "385";
        break;
    case 386:
        res = "386";
        break;
    case 387:
        res = "387";
        break;
    case 388:
        res = "388";
        break;
    case 389:
        res = "389";
        break;
    case 390:
        res = "390";
        break;
    case 391:
        res = "391";
        break;
    case 392:
        res = "392";
        break;
    case 393:
        res = "393";
        break;
    case 394:
        res = "394";
        break;
    case 395:
        res = "395";
        break;
    case 396:
        res = "396";
        break;
    case 397:
        res = "397";
        break;
    case 398:
        res = "398";
        break;
    case 399:
        res = "399";
        break;
    case 400:
        res = "400";
        break;
    case 2021:
        res = "2021";
        break;
    case 2022:
        res = "2022";
        break;
    case 2023:
        res = "2023";
        break;
    case 2024:
        res = "2024";
        break;
    case 2025:
        res = "2025";
        break;
    case 2026:
        res = "2026";
        break;
    case 2027:
        res = "2027";
        break;
    case 2028:
        res = "2028";
        break;
    case 2029:
        res = "2029";
        break;
    case 2030:
        res = "2030";
        break;
    case 2031:
        res = "2031";
        break;
    case 2032:
        res = "2032";
        break;
    case 2033:
        res = "2033";
        break;
    case 2034:
        res = "2034";
        break;
    case 2035:
        res = "2035";
        break;
    case 2036:
        res = "2036";
        break;
    case 2037:
        res = "2037";
        break;
    case 2038:
        res = "2038";
        break;
    case 2039:
        res = "2039";
        break;
    case 2040:
        res = "2040";
        break;
    case 2041:
        res = "2041";
        break;
    case 2042:
        res = "2042";
        break;
    case 2043:
        res = "2043";
        break;
    case 2044:
        res = "2044";
        break;
    case 2045:
        res = "2045";
        break;
    case 2046:
        res = "2046";
        break;
    case 2047:
        res = "2047";
        break;
    case 2048:
        res = "2048";
        break;
    case 2049:
        res = "2049";
        break;
    case 2050:
        res = "2050";
        break;
    case 2051:
        res = "2051";
        break;
    case 2052:
        res = "2052";
        break;
    default:
        res = "NaN";
        break;
    }

    return res;
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    port8BIT P1(0x3D4);
    port8BIT P2(0x3D5);

    P1.WriteToPort(0x0A);
    P2.WriteToPort((P2.ReadFromPort() & 0xC0) | cursor_start);

    P1.WriteToPort(0x0B);
    P2.WriteToPort((P2.ReadFromPort() & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
    int VGA_WIDTH = 80;
    port8BIT P1(0x3D4);
    port8BIT P2(0x3D5);

    uint16_t pos = y * VGA_WIDTH + x;

    P1.WriteToPort(0x0F);
    P2.WriteToPort((uint8_t)(pos & 0xFF));
    P1.WriteToPort(0x0E);
    P2.WriteToPort((uint8_t)((pos >> 8) & 0xFF));
}

char *hertochar(uint16_t hex2)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(hex2 >> 4) & 0xF];
    foo[1] = hex[hex2 & 0xF];
    return foo;
}

void printf(char *);

class ifkeypress : public CustomShell
{
public:
    ifkeypress()
    {
        printf("Event handler assigned surcessfully\n");
    }
    ~ifkeypress()
    {
    }
    virtual void Shell()
    {
        while (1)
        {
            printf("Key pressed\n");
        }
    }
    virtual void clearBuffer()
    {
    }

    virtual void OnKeyDown(uint8_t key)
    {
        printf("Key pressed: 0x");
        printHex(key);
        printf("\n");
        if (key == 0x01)
        {
            printf("Key Down: ESC\n");
            GlobalBool1 = true;
            KeyboardDriver *k = (KeyboardDriver *)DriverManager::ActiveDriverManager->GetDriver(UDID_KEYBOARD);
            k->ResetEventHandler();
        }
    }
};

void ColourPrint(int type)
{
    uint8_t x; // crx;
    uint8_t y; // cry;
    switch (type)
    {
    case 0:
        for (x = 0; x < 80; x++)
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | ' ';
        break;

    case 1:
        if (!isused)
        {
            for (x = 0; x < 80; x++)
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) >> 4 | ' ';
            isused = true;
        }
        else
        {
            isused = false;
        }

        break;
    }
}

void printf(char *str)
{
    uint8_t curx;
    uint8_t cury;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        cursorx = x;

        switch (str[i])
        {
        case '\n':
            y++;
            x = 0;
            break;
        case '\5':
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
            break;
        case '\3':
            for (int i = 0; i != 2; i++)
                x = cursorx;
            if (x != SPIndex)
            {
                x--;
            }
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            break;
        case '\f':
            for (int i = 0; i != 2; i++)
                x = cursorx;
            x--;
            if (x == 0 & y != 0)
            {
                x = 0;
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
                y--;
                x = 79;
            }
            else if (y == 0)
            {
                y = 1;
            }
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            break;
        case '\7':
            if (y != 0)
                y--;
            else if (y == 0)
            {
                y = 1;
            }
            break;

        case '\4':
            x--;
            break;

        case '\2':
            if (y != 25)
                y++;
            break;

        case '\6':
            if (x != 80)
                x++;
            break;

        default:
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
            break;
        }

        if (x >= 80)
        {
            y++;
            x = 0;
        }

        if (y >= 25)
        {
            // ColourPrint(1);
            char *foo = "                                                                               ";
            isused = true;
            for (int i = 0; i != 80; i++)
                foo[i] = VideoMemory[80 * 24 + i];
            for (y = 0; y < 25; y++)
                for (x = 0; x < 79; x++)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
            // ColourPrint(0);
            printf("SectorOS ");
            printf(KERNEL_VERSION);
            printf("               ");
            RTC rtclock;
            rtclock.read_rtc();
            printf(INTTOCHARPOINT(rtclock.day));
            printf("/");
            printf(INTTOCHARPOINT(rtclock.month));
            printf("/");
            printf(INTTOCHARPOINT(rtclock.year));
            printf("                            Type: Shell\n");
            for (int i = 0; i != 80; i++)
                VideoMemory[80 * 1 + i] = (VideoMemory[80 * 1 + i] & 0xFF00) | foo[i];
            printf("\n");
        }
    }

    update_cursor(x, y + 1);
}

// Uses 10 Character Space
void PrintDate()
{
    RTC rtclock;
    rtclock.read_rtc();
    printf(INTTOCHARPOINT(rtclock.day));
    printf("/");
    printf(INTTOCHARPOINT(rtclock.month));
    printf("/");
    printf(INTTOCHARPOINT(rtclock.year));
}

void PrintPrompt()
{
    // int ind = SPIndex;
    for (int i = 0; SPIndex > i; i++)
    {
        if (SP[i][0] == '%')
        {
            // ind -= 2;
            switch (SP[i + 1][0])
            {
            case 'd':
            {
                // ind += 10;
                PrintDate();
                index = 2;
                break;
            }
            case 'u':
            {
                // ind += set.UserNameLength;
                printf(set.UserName);
                index = 2;
                break;
            }
            case 'h':
            {
                // ind += set.HostnameLength;
                printf(set.Hostname);
                index = 2;
                break;
            }
            }
        }
        else if (index <= 0)
        {
            printf(SP[i]);
        }
        index--;
    }
    // SPIndex = ind;
}

void printTime()
{
    uint8_t x = 51;
    uint8_t y = 0;
    RTC rtclock;
    rtclock.read_rtc();
    char *strH = INTTOCHARPOINT(rtclock.hour);
    char dd = ':';
    char *strM = INTTOCHARPOINT(rtclock.minute);
    char *strS = INTTOCHARPOINT(rtclock.second);

    for (int i = 0; i != 3; ++i)
    {
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | strH[i];
        x++;
    }

    x--;
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | dd;
    x++;

    for (int i = 0; i != 3; ++i)
    {
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | strM[i];
        x++;
    }

    x--;
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ',';
    x++;

    for (int i = 0; i != 3; ++i)
    {
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | strS[i];
        x++;
    }
}

void PVGA()
{
    VideoGraphicsArray vga;
    vga.SetMode(320, 200, 8);
    for (int32_t y = 0; y < 200; y++)
        for (int32_t x = 0; x < 320; x++)
            vga.PutPixel(x, y, 0x00, 0x00, 0xA8);
}

void printfchar(char st)
{
    switch (st)
    {
    case '\n':
        y++;
        x = 0;
        break;
    default:
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | st;
        x++;
        break;
    }

    if (x >= 80)
    {
        y++;
        x = 0;
    }

    if (y >= 25)
    {
        for (y = 0; y < 25; y++)
            for (x = 0; x < 80; x++)
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';

        x = 0;
        y = 0;
    }

    update_cursor(x, y + 1);
}

void printHex(uint8_t Key)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(Key >> 4) & 0xF];
    foo[1] = hex[Key & 0xF];
    printf(foo);
}

void PrintPartitions()
{
    AdvancedTechnologyAttachment ata0s(0x1F0, false);
    MSDOSPartitionTable::ReadPartitions(&ata0s);
}

#define cpuid(in, a, b, c, d) __asm__("cpuid"                              \
                                      : "=a"(a), "=b"(b), "=c"(c), "=d"(d) \
                                      : "a"(in));

int do_intel(void)
{
    printf("CPU Manufacture - Genuineintel\n");
    printf("Intel Specific Features:\n");
    unsigned long eax, ebx, ecx, edx, max_eax, signature, unused;
    int model, family, type, brand;
    int extended_family = -1;
    cpuid(1, eax, ebx, unused, unused);
    model = (eax >> 4) & 0xf;
    family = (eax >> 8) & 0xf;
    type = (eax >> 12) & 0x3;
    brand = ebx & 0xff;

    printf("Type - ");
    switch (type)
    {
    case 0:
        printf("Original OEM");
        break;
    case 1:
        printf("Overdrive");
        break;
    case 2:
        printf("Dual-capable");
        break;
    case 3:
        printf("Reserved");
        break;
    }
    printf("\n");

    printf("Family - ");
    switch (family)
    {
    case 3:
        printf("i386");
        break;
    case 4:
        printf("i486");
        break;
    case 5:
        printf("Pentium");
        break;
    case 6:
        printf("Pentium Pro");
        break;
    case 15:
        printf("Pentium 4");
    }
    printf("\n");

    printf("Model - ");
    switch (family)
    {
    case 3:
        break;
    case 4:
        switch (model)
        {
        case 0:
        case 1:
            printf("DX");
            break;
        case 2:
            printf("SX");
            break;
        case 3:
            printf("487/DX2");
            break;
        case 4:
            printf("SL");
            break;
        case 5:
            printf("SX2");
            break;
        case 7:
            printf("Write-back enhanced DX2");
            break;
        case 8:
            printf("DX4");
            break;
        }
        break;
    case 5:
        switch (model)
        {
        case 1:
            printf("60/66");
            break;
        case 2:
            printf("75-200");
            break;
        case 3:
            printf("for 486 system");
            break;
        case 4:
            printf("MMX");
            break;
        }
        break;
    case 6:
        switch (model)
        {
        case 1:
            printf("Pentium Pro");
            break;
        case 3:
            printf("Pentium II Model 3");
            break;
        case 5:
            printf("Pentium II Model 5/Xeon/Celeron");
            break;
        case 6:
            printf("Celeron");
            break;
        case 7:
            printf("Pentium III/Pentium III Xeon - external L2 cache");
            break;
        case 8:
            printf("Pentium III/Pentium III Xeon - internal L2 cache");
            break;
        }
        break;
    case 15:
        break;
        printf("\n");
    }

    return 0;
}

void PrintSATA()
{
    printf("\nS-ATA primary master: ");
    AdvancedTechnologyAttachment ata0m(0x1F0, true);
    ata0m.Identify();

    printf("\nS-ATA primary slave: ");
    AdvancedTechnologyAttachment ata0s(0x1F0, false);
    ata0s.Identify();
    printf("\n");

    printf("\nS-ATA secondary master: ");
    AdvancedTechnologyAttachment ata1m(0x170, true);
    ata1m.Identify();

    printf("\nS-ATA secondary slave: ");
    AdvancedTechnologyAttachment ata1s(0x170, false);
    ata1s.Identify();
    printf("\n");
}

int detect_cpu(void)
{
    unsigned long ebx, unused;
    cpuid(0, unused, ebx, unused, unused);
    switch (ebx)
    {
    case 0x756e6547:
        do_intel();
        break;
    case 0x68747541:
        // do_amd();
        break;
    default:
        printf("Unknown x86 CPU Detected\n");
        break;
    }
    return 0;
}

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;

public:
    MouseToConsole()
    {
    }

    virtual void OnActivate()
    {
        uint16_t *VideoMemory = (uint16_t *)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }

    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t *VideoMemory = (uint16_t *)0xb8000;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);

        x += xoffset;
        if (x >= 80)
            x = 79;
        if (x < 0)
            x = 0;
        y += yoffset;
        if (y >= 25)
            y = 24;
        if (y < 0)
            y = 0;

        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }
};

void *kmalloc(size_t size)
{
    return MemoryManager::ActiveMemoryManager->MemAllocate(size);
}

void kfree(void *ptr)
{
    MemoryManager::ActiveMemoryManager->MemFree(ptr);
}

void* krealloc(void *ptr, size_t size)
{
    return MemoryManager::ActiveMemoryManager->MemReallocate(ptr, size);
}

void *kcalloc(uint32_t num, uint32_t size)
{
    void *ptr = kmalloc(num * size);
    memset(ptr, 0, num * size);
    return ptr;
}

void taskB()
{
    printf("SectorOS TASK2\nPlese restart the os manually\n");
    while (1)
        ;
}

void taskA()
{
    printf("SectorOS TASK1\nPlese restart the os manually\n");
    while (1)
        ;
}

class PrintfUDPHandler : public UserDatagramProtocolHandler
{
public:
    void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket *socket, uint8_t *data, uint16_t size)
    {
        printf("SYSMSG: GOT UDP MSG");
        char *foo = " ";
        for (int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }
    }
};

class PrintfTCPHandler : public TransmissionControlProtocolHandler
{
public:
    void TEST()
    {
        printf("TEST WORKED\n");
    }

    void TEST2()
    {
        printf("TEST2 WORKED\n");
    }

    bool HandleTransmissionControlProtocolMessage(TransmissionControlProtocolSocket *socket, uint8_t *data, uint16_t size)
    {
        printf("SYSMSG: GOT TCP MSG\n");
        char *foo = " ";
        for (int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }

        if (size > 9 && data[0] == 'G' && data[1] == 'E' && data[2] == 'T' && data[3] == ' ' && data[4] == '/' && data[5] == ' ' && data[6] == 'H' && data[7] == 'T' && data[8] == 'T' && data[9] == 'P')
        {
            printf("SYSMSG: Sending response\n");
            socket->Send((uint8_t *)"HTTP/1.1 200 OK\r\nServer: SectorOS\r\nContent-Type: text/html\r\n\r\n<html><head><title>SectorOS</title></head><body><b>This is a test webpage which is hosted on SectorOS</b> https://github.com/Arun007coder/SectorOS</body></html>\r\n", 224);
            socket->Disconnect();
        }
        return true;
    }
};

void StartWEBServer(uint16_t port)
{
    if (DriverManager::ActiveDriverManager->GetDriver(UDID_AM79C973) != 0)
    {
        printf("\5\n");
        printf("SectorOS Web Server v1.0\n");
        AM79C973 *eth0 = (AM79C973 *)DriverManager::ActiveDriverManager->GetDriver(UDID_AM79C973);
        InternetProtocolProvider *ipv4 = InternetProtocolProvider::ActiveInstance;
        TransmissionControlProtocolProvider tcp(ipv4);
        TransmissionControlProtocolSocket *tcpsocket = tcp.Listen(port);

        printf("Starting webserver\n");
        PrintfTCPHandler tcphandler;
        printf("WebServer started\n");
        printf("Listening on port ");
        printf(INTTOCHARPOINT(port));
        printf("\n");
        tcp.Bind(tcpsocket, &tcphandler);
        while (1)
            ;
    }
    else
    {
        printf("Driver for ethernet card not found\n");
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(const void *multiboot_structure, uint32_t multiboot_m)
{
    ColourPrint(0);
    printf("Initializing SectorOS Kernel ");
    printf(KERNEL_VERSION);
    printf(" ");
    printf(KERNEL_BUILD);
    printf("....\n");

    enable_cursor(0, 0);
    TaskManager taskManager;
    uint32_t mm = multiboot_m;

    GlobalDescriptorTable gdt;

    InterruptManager interrupts(0x20, &gdt);

    printf("SYSMSG: Initializing execption managers....\n");

    EXCEPTIONS_GPF gpfmanager(&interrupts);
    EXCEPTIONS_IOC iocmanager(&interrupts);
    EXECPTION_DOUBLE_FAULT doublefaultmanager(&interrupts);

    printf("SYSMSG: Execption managers initialized\n");

    SerialPort sp;
    sp.INITSerial();

    sp.logToSerialPort("\nkernel started");

    printf("Allocating Memory for the kernel....\n");
    memupper = (uint32_t *)(((size_t)multiboot_structure) + 8);
    size_t heap = 10 * 1024 * 1024;
    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printf("heap: 0x");
    printHex32(heap);

    allocated = memoryManager.MemAllocate(1024);
    printf("\nallocated: 0x");
    printHex32((size_t)allocated);
    printf("\n");

    printf("SYSMSG: Memory allocated\n");

    printf("Memory on system: 0x");
    printHex32((*memupper) * 1024);
    SyscallHandler syscalls(&interrupts, 0x80);
    DriverManager drvmgr;

    printf("\nSYSMSG: Initializing Hardwares [Stage 1]...\n");

    KeyboardDriver hexboardDriver(&interrupts);
    kbrd = &hexboardDriver;
    sp.logToSerialPort("\nHardware initialising stage 1 finished");
    drvmgr.AddDriver(&hexboardDriver);

    MouseToConsole msmgr;
    MouseDriver MouseDriver(&interrupts, &msmgr);
    if (useMouse = true)
    {
        drvmgr.AddDriver(&MouseDriver);
    }
    else
    {
        printf("\nSYSMSG: Cannot initialize mouse driver. This driver is disabled by default.");
    }

    PCI PCICONT;
    PCICONT.SelectDrivers(&drvmgr, &interrupts);
    printf("\nSYSMSG: Initializing Hardwares [Stage 2]...");
    drvmgr.activateall();

    sp.logToSerialPort("\nHardware initialising stage 2 finished");
    sp.logToSerialPort("\nDriverManager started");
    printf("\nSYSMSG: Initializing Hardwares [Stage 3]...\n");

    if (drvmgr.GetDriver(UDID_AM79C973) != 0)
    {
        interrupts.Activate();

        // IP 192.168.1.15
        uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;

        printf("\nSYSMSG: Assiging IP address: ");
        printf(INTTOCHARPOINT(ip1));
        printf(".");
        printf(INTTOCHARPOINT(ip2));
        printf(".");
        printf(INTTOCHARPOINT(ip3));
        printf(".");
        printf(INTTOCHARPOINT(ip4));
        printf("\n");

        uint32_t ip_be = ((uint32_t)ip4 << 24) | ((uint32_t)ip3 << 16) | ((uint32_t)ip2 << 8) | (uint32_t)ip1;

        // IP 192.168.1.1 GATEIP
        uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 2;

        printf("\nSYSMSG: Assiging GATEWAY IP address: ");
        printf(INTTOCHARPOINT(gip1));
        printf(".");
        printf(INTTOCHARPOINT(gip2));
        printf(".");
        printf(INTTOCHARPOINT(gip3));
        printf(".");
        printf(INTTOCHARPOINT(gip4));
        printf("\n");

        uint32_t gip_be = ((uint32_t)gip4 << 24) | ((uint32_t)gip3 << 16) | ((uint32_t)gip2 << 8) | (uint32_t)gip1;

        // 255.255.255.0 SUBNET_MASK
        uint8_t subnet1 = 255, subnet2 = 255, subnet3 = 255, subnet4 = 0;

        printf("\nSYSMSG: Assiging SUBNET_MASK: ");
        printf(INTTOCHARPOINT(subnet1));
        printf(".");
        printf(INTTOCHARPOINT(subnet2));
        printf(".");
        printf(INTTOCHARPOINT(subnet3));
        printf(".");
        printf(INTTOCHARPOINT(subnet4));
        printf("\n");

        uint32_t subnet_be = ((uint32_t)subnet4 << 24) | ((uint32_t)subnet3 << 16) | ((uint32_t)subnet2 << 8) | (uint32_t)subnet1;

        printf("\nSYSMSG: Initializing network eth0...\n");
        AM79C973 *eth0 = (AM79C973 *)(drvmgr.GetDriver(UDID_AM79C973));

        eth0->SetIPAddress(ip_be);
        printf("\nSYSMSG: IP address set\n");

        EtherFrameProvider etherframe(eth0);

        AddressResolutionProtocol arp(&etherframe);

        InternetProtocolProvider ipv4(&etherframe, &arp, gip_be, subnet_be);

        InternetControlMessageProtocol icmp(&ipv4);

        printf("Assigned MAC address: ");
        uint64_t MACADDR = eth0->GetMACAddress();
        printHex((MACADDR >> 40) & 0xFF);
        printf(":");
        printHex((MACADDR >> 32) & 0xFF);
        printf(":");
        printHex((MACADDR >> 24) & 0xFF);
        printf(":");
        printHex((MACADDR >> 16) & 0xFF);
        printf(":");
        printHex((MACADDR >> 8) & 0xFF);
        printf(":");
        printHex(MACADDR & 0xFF);

        printf("\n");

        arp.BroadcastMACAddress(gip_be);
    }

    interrupts.Activate();

    BootSound();

    sp.logToSerialPort("\nHardware initialising stage 3 finished");

    printf("\5");
    printf("\5");

    printf("Welcome to SectorOS ");
    printf(KERNEL_VERSION);
    printf("          ");
    PrintDate();
    printf("                      Type: Shell\nhttps://github.com/Arun007coder/SectorOS \n");

    /*
    printf("Initializing ");
    printf(SHELL_NAME);
    printf(" ");
    printf(SHELL_VER);
    printf("\n\n");
    */

    printf("Welcome to SectorOS Shell\nRun help to get the list of commands which is implemented \n \n");
    printf(" @@@@@@@@@@@@@@\n");
    printf("@              \n");
    printf("@              \n");
    printf("@              \n");
    printf(" @@@@@@@@@@@@@ \n");
    printf("              @\n");
    printf("              @\n");
    printf("              @\n");
    printf("@@@@@@@@@@@@@@ \n");

    sp.logToSerialPort("\nKernel initialization surcessful.\nGiving execution access to the kernel.\nAwaiting user input...");

    SPIndex = set.UserNameLength + set.HostnameLength + 5 + 11;

    Task task1(&gdt, taskA, (uint8_t *)"Test Task 1", 0);
    taskManager.AddTask(&task1);
    Task task2(&gdt, taskB, (uint8_t *)"Test Task 2", 1);
    taskManager.AddTask(&task2);

    CPUState *state = (CPUState *)OLDESP;

    taskManager.listTasks();

    taskManager.MakeDefault();

    PrintPrompt();

    while (1)
        ;
}