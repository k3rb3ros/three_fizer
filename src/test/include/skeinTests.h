#ifndef SKEINTESTS_H
#define SKEINTESTS_H

#include <assert.h>                     //assert()
#include <stdint.h>                     //uintxx_t types
#include <stdio.h>                      //printf()
#include "../../include/debug.h"        //pdebug()
#include "print.h"                      //showBuff()
#include "../../include/skeinApi.h"     //skeinCtx type, all skein hash functions and init functions

/*
* K3rb3ros 10.12.2014
* These values were taken from a freshly initialized SkeinCtx on my test machine
*/

static const uint64_t ExpectedState256[] = { 18202890402666165321ULL, 3443677322885453875ULL,
                                             12915131351309911055ULL, 7662005193972177513ULL };

static const uint64_t ExpectedState512[] = { 5261240102383538638ULL, 978932832955457283ULL,
                                             10363226125605772238ULL, 11107378794354519217ULL,
                                             6752626034097301424ULL, 16915020251879818228ULL,
                                             11029617608758768931ULL, 12544957130904423475ULL };

static const uint64_t ExpectedState1024[] = { 15389884076360409941ULL, 1564408309651595276ULL,
                                              5872946452714669296ULL, 269443931446226863ULL,
                                              2066808069366720664ULL, 7949147512366979231ULL,
                                              8638675932900051674ULL, 13915592562466140323ULL,
                                              15479235830881622709ULL, 7681746768227630605ULL,
                                              10539485315553235762ULL, 1882256010534388436ULL,
                                              690868493690407864ULL, 7310148287346284186ULL,
                                              7060852991742072730ULL, 2152812356051330361ULL };

static const uint64_t Skein256NullHash[] = { 8724517391939661071ULL, 13370976164479673572ULL,
                                             17406185901391511763ULL, 5767528745175948617ULL };

static const uint64_t Skein512NullHash[] = { 16674971110164789043ULL, 16271238091670380495ULL,
                                             13753278159638688213ULL, 15640160688208848212ULL,
                                             6426357076001091602ULL, 7868118233575035598ULL,
                                             13409637217154661878ULL, 15823918759942544260ULL };

static const uint64_t Skein1024NullHash[] = { 5767348195165280385ULL, 13313145644582907738ULL,
                                              15445495210649757637ULL, 784299716472470282ULL,
                                              6258770670021293714ULL, 8003395179891192994ULL,
                                              5536180265455278323ULL, 3682281098039611423ULL,
                                              950856614975086330ULL, 11517112629637956673ULL,
                                              4305894531452524800ULL, 16828010516203810237ULL,
                                              16645466546792041892ULL, 8220960308264854982ULL,
                                              1879382561755265108ULL, 13388187221246470230ULL };

static const uint64_t TestMsg[] = { 5077073466687252581ULL , 2327889596035104768ULL };

/*
* K3rb3ros 10.19.2014
* These values were taken from Skein digests on my test machine
*/

static const uint64_t ExpectedDigest256[] = { 5913355494111548831ULL, 14545365143941776789ULL,
                                              3101499150589447885ULL, 14330232381075572585ULL };

static const uint64_t ExpectedDigest512[] = { 17820824062134001381ULL, 1920143778435347109ULL,
                                              11915273695160806815ULL, 4539088631920594701ULL,
                                              14090049249442769627ULL, 14822485233854683647ULL,
                                              11956900317171193513ULL, 17778993175431006280ULL };

static const uint64_t ExpectedDigest1024[] = { 8465303633664212295ULL, 4846274381220427229ULL,
                                               12614322184485712757ULL, 7503119550539696887ULL,
                                               7005327482526261793ULL, 13756209870218924162ULL,
                                               476568214109261173ULL, 1867171129410923627ULL,
                                               16099196659237491457ULL, 589659456053222171ULL,
                                               10255270865036076450ULL, 17448666692730491597ULL,
                                               9165797820039646890ULL, 10872441060414999887ULL,
                                               9011713385788166076ULL, 3434631984923120672ULL };

void runSkeinTests();

#endif