/*
 copyright 2020, Chen Wei <weichen302@gmail.com>
 version 0.0.3
Implement astronomical algorithms for finding solar terms and moon phases.

Truncated VSOP87D for calculate Sun's apparent longitude;

Reference:
    VSOP87: ftp://ftp.imcce.fr/pub/ephem/planets/vsop87
*/

#include <stdio.h>
#include <math.h>
#include "astro.h"


/*  Truncated VSOP87D tables */
static double earth_L0[172][3] = {
    { 1.75347045673, 0, 0 },
    { 0.03341656456, 4.66925680417, 6283.0758499914 },
    { 0.00034894275, 4.62610241759, 12566.1516999828 },
    { 0.00003417571, 2.82886579606, 3.523118349 },
    { 0.00003497056, 2.74411800971, 5753.3848848968 },
    { 0.00003135896, 3.62767041758, 77713.7714681205 },
    { 0.00002676218, 4.41808351397, 7860.4193924392 },
    { 0.00002342687, 6.13516237631, 3930.2096962196 },
    { 0.00001273166, 2.03709655772, 529.6909650946 },
    { 0.00001324292, 0.74246356352, 11506.7697697936 },
    { 0.00000901855, 2.04505443513, 26.2983197998 },
    { 0.00001199167, 1.10962944315, 1577.3435424478 },
    { 0.00000857223, 3.50849156957, 398.1490034082 },
    { 0.00000779786, 1.17882652114, 5223.6939198022 },
    { 0.0000099025, 5.23268129594, 5884.9268465832 },
    { 0.00000753141, 2.53339053818, 5507.5532386674 },
    { 0.00000505264, 4.58292563052, 18849.2275499742 },
    { 0.00000492379, 4.20506639861, 775.522611324 },
    { 0.00000356655, 2.91954116867, 0.0673103028 },
    { 0.00000284125, 1.89869034186, 796.2980068164 },
    { 0.0000024281, 0.34481140906, 5486.777843175 },
    { 0.00000317087, 5.84901952218, 11790.6290886588 },
    { 0.00000271039, 0.31488607649, 10977.078804699 },
    { 0.0000020616, 4.80646606059, 2544.3144198834 },
    { 0.00000205385, 1.86947813692, 5573.1428014331 },
    { 0.00000202261, 2.45767795458, 6069.7767545534 },
    { 0.00000126184, 1.0830263021, 20.7753954924 },
    { 0.00000155516, 0.83306073807, 213.299095438 },
    { 0.00000115132, 0.64544911683, 0.9803210682 },
    { 0.00000102851, 0.63599846727, 4694.0029547076 },
    { 0.00000101724, 4.26679821365, 7.1135470008 },
    { 0.00000099206, 6.20992940258, 2146.1654164752 },
    { 0.00000132212, 3.41118275555, 2942.4634232916 },
    { 0.00000097607, 0.6810127227, 155.4203994342 },
    { 0.00000085128, 1.29870743025, 6275.9623029906 },
    { 0.00000074651, 1.75508916159, 5088.6288397668 },
    { 0.00000101895, 0.97569221824, 15720.8387848784 },
    { 0.00000084711, 3.67080093025, 71430.69561812909 },
    { 0.00000073547, 4.67926565481, 801.8209311238 },
    { 0.00000073874, 3.50319443167, 3154.6870848956 },
    { 0.00000078756, 3.03698313141, 12036.4607348882 },
    { 0.00000079637, 1.807913307, 17260.1546546904 },
    { 0.00000085803, 5.98322631256, 161000.6857376741 },
    { 0.00000056963, 2.78430398043, 6286.5989683404 },
    { 0.00000061148, 1.81839811024, 7084.8967811152 },
    { 0.00000069627, 0.83297596966, 9437.762934887 },
    { 0.00000056116, 4.38694880779, 14143.4952424306 },
    { 0.00000062449, 3.97763880587, 8827.3902698748 },
    { 0.00000051145, 0.28306864501, 5856.4776591154 },
    { 0.00000055577, 3.47006009062, 6279.5527316424 },
    { 0.00000041036, 5.36817351402, 8429.2412664666 },
    { 0.00000051605, 1.33282746983, 1748.016413067 },
    { 0.00000051992, 0.18914945834, 12139.5535091068 },
    { 0.00000049, 0.48735065033, 1194.4470102246 },
    { 0.000000392, 6.16832995016, 10447.3878396044 },
    { 0.00000035566, 1.77597314691, 6812.766815086 },
    { 0.0000003677, 6.04133859347, 10213.285546211 },
    { 0.00000036596, 2.56955238628, 1059.3819301892 },
    { 0.00000033291, 0.59309499459, 17789.845619785 },
    { 0.00000035954, 1.70876111898, 2352.8661537718 },
    { 0.00000040938, 2.39850881707, 19651.048481098 },
    { 0.00000030047, 2.73975123935, 1349.8674096588 },
    { 0.00000030412, 0.44294464135, 83996.84731811189 },
    { 0.00000023663, 0.48473567763, 8031.0922630584 },
    { 0.00000023574, 2.06527720049, 3340.6124266998 },
    { 0.00000021089, 4.14825464101, 951.7184062506 },
    { 0.00000024738, 0.21484762138, 3.5904286518 },
    { 0.00000025352, 3.16470953405, 4690.4798363586 },
    { 0.0000002282, 5.22197888032, 4705.7323075436 },
    { 0.00000021419, 1.42563735525, 16730.4636895958 },
    { 0.00000021891, 5.55594302562, 553.5694028424 },
    { 0.00000017481, 4.56052900359, 135.0650800354 },
    { 0.00000019925, 5.22208471269, 12168.0026965746 },
    { 0.0000001986, 5.77470167653, 6309.3741697912 },
    { 0.000000203, 0.37133792946, 283.8593188652 },
    { 0.00000014421, 4.19315332546, 242.728603974 },
    { 0.00000016225, 5.98837722564, 11769.8536931664 },
    { 0.00000015077, 4.19567181073, 6256.7775301916 },
    { 0.00000019124, 3.82219996949, 23581.2581773176 },
    { 0.00000018888, 5.38626880969, 149854.40013480789 },
    { 0.00000014346, 3.72355084422, 38.0276726358 },
    { 0.00000017898, 2.21490735647, 13367.9726311066 },
    { 0.00000012054, 2.62229588349, 955.5997416086 },
    { 0.00000011287, 0.17739328092, 4164.311989613 },
    { 0.00000013971, 4.40138139996, 6681.2248533996 },
    { 0.00000013621, 1.88934471407, 7632.9432596502 },
    { 0.00000012503, 1.13052412208, 5.5229243074 },
    { 0.00000010498, 5.35909518669, 1592.5960136328 },
    { 0.00000009803, 0.99947478995, 11371.7046897582 },
    { 0.0000000922, 4.57138609781, 4292.3308329504 },
    { 0.00000010327, 6.19982566125, 6438.4962494256 },
    { 0.00000012003, 1.003514567, 632.7837393132 },
    { 0.00000010827, 0.32734520222, 103.0927742186 },
    { 0.00000008356, 4.53902685948, 25132.3033999656 },
    { 0.00000010005, 6.0291496328, 5746.271337896 },
    { 0.00000008409, 3.29946744189, 7234.794256242 },
    { 0.00000008006, 5.82145271907, 28.4491874678 },
    { 0.00000010523, 0.93871805506, 11926.2544136688 },
    { 0.00000007686, 3.12142363172, 7238.6755916 },
    { 0.00000009378, 2.62414241032, 5760.4984318976 },
    { 0.00000008127, 6.11228001785, 4732.0306273434 },
    { 0.00000009232, 0.48343968736, 522.5774180938 },
    { 0.00000009802, 5.24413991147, 27511.4678735372 },
    { 0.00000007871, 0.99590177926, 5643.1785636774 },
    { 0.00000008123, 6.2705301365, 426.598190876 },
    { 0.00000009048, 5.33686335897, 6386.16862421 },
    { 0.0000000862, 4.16538210888, 7058.5984613154 },
    { 0.00000006297, 4.71724819317, 6836.6452528338 },
    { 0.00000007575, 3.97382858911, 11499.6562227928 },
    { 0.00000007756, 2.95729056763, 23013.5395395872 },
    { 0.00000007314, 0.60652505806, 11513.8833167944 },
    { 0.00000005955, 2.87641047971, 6283.14316029419 },
    { 0.00000006534, 5.79072926033, 18073.7049386502 },
    { 0.00000007188, 3.99831508699, 74.7815985673 },
    { 0.00000007346, 4.38582365437, 316.3918696566 },
    { 0.00000005413, 5.39199024641, 419.4846438752 },
    { 0.00000005127, 2.36062848786, 10973.55568635 },
    { 0.00000007056, 0.32258441903, 263.0839233728 },
    { 0.00000006625, 3.66475158672, 17298.1823273262 },
    { 0.00000006762, 5.91132535899, 90955.5516944961 },
    { 0.00000004938, 5.73672165674, 9917.6968745098 },
    { 0.00000005547, 2.45152597661, 12352.8526045448 },
    { 0.00000005958, 3.32051344676, 6283.0085396886 },
    { 0.00000004471, 2.06385999536, 7079.3738568078 },
    { 0.00000006153, 1.45823331144, 233141.31440436149 },
    { 0.00000004348, 4.4234217548, 5216.5803728014 },
    { 0.00000006123, 1.07494905258, 19804.8272915828 },
    { 0.00000004488, 3.6528503715, 206.1855484372 },
    { 0.0000000402, 0.83995823171, 20.3553193988 },
    { 0.00000005188, 4.06503864016, 6208.2942514241 },
    { 0.00000005307, 0.38217636096, 31441.6775697568 },
    { 0.00000003785, 2.34369213733, 3.881335358 },
    { 0.00000004497, 3.27230796845, 11015.1064773348 },
    { 0.00000004132, 0.92128915753, 3738.761430108 },
    { 0.00000003521, 5.97844807108, 3894.1818295422 },
    { 0.00000004215, 1.90601120623, 245.8316462294 },
    { 0.00000003701, 5.03069397926, 536.8045120954 },
    { 0.00000003865, 1.82634360607, 11856.2186514245 },
    { 0.00000003652, 1.01838584934, 16200.7727245012 },
    { 0.0000000339, 0.97785123922, 8635.9420037632 },
    { 0.00000003737, 2.95380107829, 3128.3887650958 },
    { 0.00000003507, 3.71291946325, 6290.1893969922 },
    { 0.00000003086, 3.64646921512, 10.6366653498 },
    { 0.00000003397, 1.10590684017, 14712.317116458 },
    { 0.00000003334, 0.83684924911, 6496.3749454294 },
    { 0.00000002805, 2.58504514144, 14314.1681130498 },
    { 0.0000000365, 1.08344142571, 88860.05707098669 },
    { 0.00000003388, 3.20185096055, 5120.6011455836 },
    { 0.00000003252, 3.47859752062, 6133.5126528568 },
    { 0.00000002553, 3.94869034189, 1990.745017041 },
    { 0.0000000352, 2.05559692878, 244287.60000722769 },
    { 0.00000002565, 1.560717849, 23543.23050468179 },
    { 0.00000002621, 3.85639359951, 266.6070417218 },
    { 0.00000002955, 3.39692949667, 9225.539273283 },
    { 0.00000002876, 6.02635617464, 154717.60988768269 },
    { 0.00000002395, 1.16131956403, 10984.1923516998 },
    { 0.00000003161, 1.32798718453, 10873.9860304804 },
    { 0.00000003163, 5.08946464629, 21228.3920235458 },
    { 0.00000002361, 4.27212906992, 6040.3472460174 },
    { 0.0000000303, 1.80209931347, 35371.8872659764 },
    { 0.00000002343, 3.576898605, 10969.9652576982 },
    { 0.00000002618, 2.57870156528, 22483.84857449259 },
    { 0.00000002113, 3.71393780256, 65147.6197681377 },
    { 0.00000002019, 0.81393923319, 170.6728706192 },
    { 0.00000002003, 0.38091017375, 6172.869528772 },
    { 0.00000002506, 3.74379142438, 10575.4066829418 },
    { 0.00000002381, 0.10581361289, 7.046236698 },
    { 0.00000001949, 4.86892513469, 36.0278666774 },
    { 0.00000002074, 4.2279477457, 5650.2921106782 },
    { 0.00000001924, 5.5946054986, 6282.0955289232 },
    { 0.00000001949, 1.07002512703, 5230.807466803 },
    { 0.00000001988, 5.19736046771, 6262.300454499 },
};

static double earth_L1[165][3] = {
    { 6283.31966747491, 0, 0 },
    { 0.00206058863, 2.67823455584, 6283.0758499914 },
    { 0.0000430343, 2.63512650414, 12566.1516999828 },
    { 0.00000425264, 1.59046980729, 3.523118349 },
    { 0.00000108977, 2.96618001993, 1577.3435424478 },
    { 0.00000093478, 2.59212835365, 18849.2275499742 },
    { 0.00000119261, 5.79557487799, 26.2983197998 },
    { 0.00000072122, 1.13846158196, 529.6909650946 },
    { 0.00000067768, 1.87472304791, 398.1490034082 },
    { 0.00000067327, 4.40918235168, 5507.5532386674 },
    { 0.00000059027, 2.8879703846, 5223.6939198022 },
    { 0.00000055976, 2.17471680261, 155.4203994342 },
    { 0.00000045407, 0.39803079805, 796.2980068164 },
    { 0.00000036369, 0.46624739835, 775.522611324 },
    { 0.00000028958, 2.64707383882, 7.1135470008 },
    { 0.00000019097, 1.84628332577, 5486.777843175 },
    { 0.00000020844, 5.34138275149, 0.9803210682 },
    { 0.00000018508, 4.96855124577, 213.299095438 },
    { 0.00000016233, 0.03216483047, 2544.3144198834 },
    { 0.00000017293, 2.99116864949, 6275.9623029906 },
    { 0.00000015832, 1.43049285325, 2146.1654164752 },
    { 0.00000014615, 1.20532366323, 10977.078804699 },
    { 0.00000011877, 3.25804815607, 5088.6288397668 },
    { 0.00000011514, 2.07502418155, 4694.0029547076 },
    { 0.00000009721, 4.23925472239, 1349.8674096588 },
    { 0.00000009969, 1.30262991097, 6286.5989683404 },
    { 0.00000009452, 2.69957062864, 242.728603974 },
    { 0.00000012461, 2.83432285512, 1748.016413067 },
    { 0.00000011808, 5.2737979048, 1194.4470102246 },
    { 0.00000008577, 5.64475868067, 951.7184062506 },
    { 0.00000010641, 0.76614199202, 553.5694028424 },
    { 0.00000007576, 5.30062664886, 2352.8661537718 },
    { 0.00000005834, 1.76649917904, 1059.3819301892 },
    { 0.00000006385, 2.65033984967, 9437.762934887 },
    { 0.00000005223, 5.66135767624, 71430.69561812909 },
    { 0.00000005305, 0.90857521574, 3154.6870848956 },
    { 0.00000006101, 4.66632584188, 4690.4798363586 },
    { 0.0000000433, 0.24102555403, 6812.766815086 },
    { 0.00000005041, 1.42490103709, 6438.4962494256 },
    { 0.00000004259, 0.77355900599, 10447.3878396044 },
    { 0.00000005198, 1.85353197345, 801.8209311238 },
    { 0.00000003744, 2.00119516488, 8031.0922630584 },
    { 0.00000003558, 2.42901552681, 14143.4952424306 },
    { 0.00000003372, 3.86210700128, 1592.5960136328 },
    { 0.00000003374, 0.88776219727, 12036.4607348882 },
    { 0.00000003175, 3.18785710594, 4705.7323075436 },
    { 0.00000003221, 0.61599835472, 8429.2412664666 },
    { 0.00000004132, 5.23992859705, 7084.8967811152 },
    { 0.0000000297, 6.07026318493, 4292.3308329504 },
    { 0.000000029, 2.32464208411, 20.3553193988 },
    { 0.00000003504, 4.79975694359, 6279.5527316424 },
    { 0.0000000295, 1.43108874817, 5746.271337896 },
    { 0.00000002697, 4.80368225199, 7234.794256242 },
    { 0.00000002531, 6.22290682655, 6836.6452528338 },
    { 0.00000002745, 0.93466065396, 5760.4984318976 },
    { 0.0000000325, 3.39954640038, 7632.9432596502 },
    { 0.00000002277, 5.00277837672, 17789.845619785 },
    { 0.00000002075, 3.95534978634, 10213.285546211 },
    { 0.00000002061, 2.22411683077, 5856.4776591154 },
    { 0.00000002252, 5.67166499885, 11499.6562227928 },
    { 0.00000002148, 5.20184578235, 11513.8833167944 },
    { 0.00000001886, 0.53198320577, 3340.6124266998 },
    { 0.00000001875, 4.73511970207, 83996.84731811189 },
    { 0.0000000206, 2.54987293999, 25132.3033999656 },
    { 0.00000001794, 1.47435409831, 4164.311989613 },
    { 0.00000001778, 3.02473091781, 5.5229243074 },
    { 0.00000002029, 0.90960209983, 6256.7775301916 },
    { 0.00000002075, 2.26767270157, 522.5774180938 },
    { 0.00000001772, 3.02622802353, 5753.3848848968 },
    { 0.00000001569, 6.12410242782, 5216.5803728014 },
    { 0.0000000159, 4.63713748247, 3.2863574178 },
    { 0.00000001542, 4.20004448567, 13367.9726311066 },
    { 0.00000001427, 1.19088061711, 3894.1818295422 },
    { 0.00000001375, 3.09301252193, 135.0650800354 },
    { 0.00000001359, 4.24532506641, 426.598190876 },
    { 0.0000000134, 5.76511818622, 6040.3472460174 },
    { 0.00000001284, 3.08524663344, 5643.1785636774 },
    { 0.0000000125, 3.07748157144, 11926.2544136688 },
    { 0.00000001551, 3.07665451458, 6681.2248533996 },
    { 0.00000001268, 2.09196018331, 6290.1893969922 },
    { 0.00000001144, 3.24444699514, 12168.0026965746 },
    { 0.00000001248, 3.44504937285, 536.8045120954 },
    { 0.00000001118, 2.31829670425, 16730.4636895958 },
    { 0.00000001105, 5.31966001019, 23.8784377478 },
    { 0.00000001051, 3.75015946014, 7860.4193924392 },
    { 0.00000001025, 2.44688534235, 1990.745017041 },
    { 0.00000000962, 0.81771017882, 3.881335358 },
    { 0.0000000091, 0.41727865299, 7079.3738568078 },
    { 0.00000000883, 5.16833917651, 11790.6290886588 },
    { 0.00000000957, 4.07673573735, 6127.6554505572 },
    { 0.0000000111, 3.90096793825, 11506.7697697936 },
    { 0.00000000802, 3.88778875582, 10973.55568635 },
    { 0.0000000078, 2.39934293755, 1589.0728952838 },
    { 0.00000000758, 1.30034364248, 103.0927742186 },
    { 0.00000000749, 4.962758033, 6496.3749454294 },
    { 0.00000000765, 3.36312388424, 36.0278666774 },
    { 0.00000000915, 5.41543742089, 206.1855484372 },
    { 0.00000000776, 2.57589093871, 11371.7046897582 },
    { 0.00000000772, 3.98369209464, 955.5997416086 },
    { 0.00000000749, 5.17890001805, 10969.9652576982 },
    { 0.00000000806, 0.34218864254, 9917.6968745098 },
    { 0.00000000728, 5.20962563787, 38.0276726358 },
    { 0.00000000685, 2.77592961854, 20.7753954924 },
    { 0.00000000636, 4.28242193632, 28.4491874678 },
    { 0.00000000608, 5.63278508906, 10984.1923516998 },
    { 0.00000000704, 5.60738823665, 3738.761430108 },
    { 0.00000000685, 0.38876148682, 15.252471185 },
    { 0.00000000601, 0.73489602442, 419.4846438752 },
    { 0.00000000716, 2.65279791438, 6309.3741697912 },
    { 0.00000000584, 5.54502568227, 17298.1823273262 },
    { 0.0000000065, 1.13379656406, 7058.5984613154 },
    { 0.00000000688, 2.59683891779, 3496.032826134 },
    { 0.00000000485, 0.44467180946, 12352.8526045448 },
    { 0.00000000528, 2.74936967681, 3930.2096962196 },
    { 0.00000000597, 5.27668281777, 10575.4066829418 },
    { 0.00000000583, 3.1892906781, 4732.0306273434 },
    { 0.00000000526, 5.01697321546, 5884.9268465832 },
    { 0.0000000054, 1.29175137075, 640.8776073822 },
    { 0.00000000473, 5.4995330697, 5230.807466803 },
    { 0.00000000406, 5.21248452189, 220.4126424388 },
    { 0.00000000395, 1.87474483222, 16200.7727245012 },
    { 0.0000000037, 3.84921354713, 18073.7049386502 },
    { 0.00000000367, 0.88533542778, 6283.14316029419 },
    { 0.00000000379, 0.37983009325, 10177.2576795336 },
    { 0.00000000356, 3.84145204913, 11712.9553182308 },
    { 0.00000000374, 5.01577520608, 7.046236698 },
    { 0.00000000381, 4.30250406634, 6062.6632075526 },
    { 0.00000000471, 0.86381834647, 6069.7767545534 },
    { 0.00000000367, 1.32943839763, 6283.0085396886 },
    { 0.0000000046, 5.19667219575, 6284.0561710596 },
    { 0.00000000333, 5.54256205741, 4686.8894077068 },
    { 0.00000000341, 4.36522989934, 7238.6755916 },
    { 0.00000000336, 4.00205876835, 3097.88382272579 },
    { 0.00000000359, 6.22679790284, 245.8316462294 },
    { 0.00000000307, 2.35299010924, 170.6728706192 },
    { 0.00000000343, 3.77164927143, 6076.8903015542 },
    { 0.00000000296, 5.44152227481, 17260.1546546904 },
    { 0.00000000328, 0.13837875384, 11015.1064773348 },
    { 0.00000000268, 1.1390455063, 12569.6748183318 },
    { 0.00000000263, 0.00538633678, 4136.9104335162 },
    { 0.00000000282, 5.0439983748, 7477.522860216 },
    { 0.00000000288, 3.13401177517, 12559.038152982 },
    { 0.00000000259, 0.93882269387, 5642.1982426092 },
    { 0.00000000292, 1.98420020514, 12132.439962106 },
    { 0.00000000247, 3.84244798532, 5429.8794682394 },
    { 0.00000000245, 5.70467521726, 65147.6197681377 },
    { 0.00000000241, 0.99480969552, 3634.6210245184 },
    { 0.00000000246, 3.06168069935, 110.2063212194 },
    { 0.00000000239, 6.11855909114, 11856.2186514245 },
    { 0.00000000263, 0.66348415419, 21228.3920235458 },
    { 0.00000000262, 1.51070507866, 12146.6670561076 },
    { 0.0000000023, 1.75927314884, 9779.1086761254 },
    { 0.00000000223, 2.00967043606, 6172.869528772 },
    { 0.00000000246, 1.10411690865, 6282.0955289232 },
    { 0.00000000221, 3.03945240854, 8635.9420037632 },
    { 0.00000000214, 4.03840869663, 14314.1681130498 },
    { 0.00000000236, 5.4691507058, 13916.0191096416 },
    { 0.00000000224, 4.68408089456, 24072.9214697764 },
    { 0.00000000212, 2.13695625494, 5849.3641121146 },
    { 0.00000000207, 3.07724246401, 11.729352836 },
    { 0.00000000207, 6.10306282747, 23543.23050468179 },
    { 0.00000000266, 1.00709566823, 2388.8940204492 },
    { 0.00000000217, 6.27837036335, 17267.26820169119 },
    { 0.00000000204, 2.34615348695, 266.6070417218 },
    { 0.00000000195, 5.55015549753, 6133.5126528568 },
};

static double earth_L2[93][3] = {
    { 0.0005291887, 0, 0 },
    { 0.00008719837, 1.07209665242, 6283.0758499914 },
    { 0.00000309125, 0.86728818832, 12566.1516999828 },
    { 0.00000027339, 0.05297871691, 3.523118349 },
    { 0.00000016334, 5.18826691036, 26.2983197998 },
    { 0.00000015752, 3.6845788943, 155.4203994342 },
    { 0.00000009541, 0.75742297675, 18849.2275499742 },
    { 0.00000008937, 2.05705419118, 77713.7714681205 },
    { 0.00000006952, 0.8267330541, 775.522611324 },
    { 0.00000005064, 4.66284525271, 1577.3435424478 },
    { 0.00000004061, 1.03057162962, 7.1135470008 },
    { 0.00000003463, 5.14074632811, 796.2980068164 },
    { 0.00000003169, 6.05291851171, 5507.5532386674 },
    { 0.0000000302, 1.19246506441, 242.728603974 },
    { 0.00000002886, 6.11652627155, 529.6909650946 },
    { 0.0000000381, 3.4405080349, 5573.1428014331 },
    { 0.00000002714, 0.30637881025, 398.1490034082 },
    { 0.00000002371, 4.38118838167, 5223.6939198022 },
    { 0.00000002538, 2.27992810679, 553.5694028424 },
    { 0.00000002079, 3.75435330484, 0.9803210682 },
    { 0.00000001675, 0.90216407959, 951.7184062506 },
    { 0.00000001534, 5.75900462759, 1349.8674096588 },
    { 0.00000001224, 2.97328088405, 2146.1654164752 },
    { 0.00000001449, 4.3641591397, 1748.016413067 },
    { 0.00000001341, 3.72061130861, 1194.4470102246 },
    { 0.00000001254, 2.94846826628, 6438.4962494256 },
    { 0.00000000999, 5.98640014468, 6286.5989683404 },
    { 0.00000000917, 4.79788687522, 5088.6288397668 },
    { 0.00000000828, 3.31321076572, 213.299095438 },
    { 0.00000001103, 1.27104454479, 161000.6857376741 },
    { 0.00000000762, 3.41582762988, 5486.777843175 },
    { 0.00000001044, 0.60409577691, 3154.6870848956 },
    { 0.00000000887, 5.23465144638, 7084.8967811152 },
    { 0.00000000645, 1.60096192515, 2544.3144198834 },
    { 0.00000000681, 3.43155669169, 4694.0029547076 },
    { 0.00000000605, 2.47806340546, 10977.078804699 },
    { 0.00000000706, 6.19393222575, 4690.4798363586 },
    { 0.00000000643, 1.98042503148, 801.8209311238 },
    { 0.00000000502, 1.44394375363, 6836.6452528338 },
    { 0.0000000049, 2.34129524194, 1592.5960136328 },
    { 0.00000000458, 1.30876448575, 4292.3308329504 },
    { 0.00000000431, 0.03526421494, 7234.794256242 },
    { 0.00000000379, 3.17030522615, 6309.3741697912 },
    { 0.00000000348, 0.99049550009, 6040.3472460174 },
    { 0.00000000386, 1.57019797263, 71430.69561812909 },
    { 0.00000000347, 0.67013291338, 1059.3819301892 },
    { 0.00000000458, 3.81499443681, 149854.40013480789 },
    { 0.00000000302, 1.91760044838, 10447.3878396044 },
    { 0.00000000307, 3.55343347416, 8031.0922630584 },
    { 0.00000000395, 4.93701776616, 7632.9432596502 },
    { 0.00000000314, 3.18093696547, 2352.8661537718 },
    { 0.00000000282, 4.41936437052, 9437.762934887 },
    { 0.00000000276, 2.71314254553, 3894.1818295422 },
    { 0.00000000298, 2.5203747421, 6127.6554505572 },
    { 0.0000000023, 1.37790215549, 4705.7323075436 },
    { 0.00000000252, 0.55330133471, 6279.5527316424 },
    { 0.00000000255, 5.26570187369, 6812.766815086 },
    { 0.00000000275, 0.67264264272, 25132.3033999656 },
    { 0.00000000178, 0.92820785174, 1990.745017041 },
    { 0.00000000221, 0.63897368842, 6256.7775301916 },
    { 0.00000000155, 0.77319790838, 14143.4952424306 },
    { 0.0000000015, 2.40470465561, 426.598190876 },
    { 0.00000000196, 6.06877865012, 640.8776073822 },
    { 0.00000000137, 2.21679460145, 8429.2412664666 },
    { 0.00000000127, 3.26094223174, 17789.845619785 },
    { 0.00000000128, 5.47237279946, 12036.4607348882 },
    { 0.00000000122, 2.16291082757, 10213.285546211 },
    { 0.00000000118, 0.45789822268, 7058.5984613154 },
    { 0.00000000141, 2.34932647403, 11506.7697697936 },
    { 0.000000001, 0.85621569847, 6290.1893969922 },
    { 0.00000000092, 5.10587476002, 7079.3738568078 },
    { 0.00000000126, 2.65428307012, 88860.05707098669 },
    { 0.00000000106, 5.85646710022, 7860.4193924392 },
    { 0.00000000084, 3.57457554262, 16730.4636895958 },
    { 0.00000000089, 4.21433259618, 83996.84731811189 },
    { 0.00000000097, 5.57938280855, 13367.9726311066 },
    { 0.00000000102, 2.05853060226, 87.30820453981 },
    { 0.0000000008, 4.73792651816, 11926.2544136688 },
    { 0.0000000008, 5.41418965044, 10973.55568635 },
    { 0.00000000106, 4.10978997399, 3496.032826134 },
    { 0.00000000102, 3.62650006043, 244287.60000722769 },
    { 0.00000000075, 4.89483161769, 5643.1785636774 },
    { 0.00000000087, 0.42863750683, 11015.1064773348 },
    { 0.00000000069, 1.8890876072, 10177.2576795336 },
    { 0.00000000089, 1.35567273119, 6681.2248533996 },
    { 0.00000000066, 0.99455837265, 6525.8044539654 },
    { 0.00000000067, 5.5124099707, 3097.88382272579 },
    { 0.00000000076, 2.72016814799, 4164.311989613 },
    { 0.00000000063, 1.4434990254, 9917.6968745098 },
    { 0.00000000078, 3.51469733747, 11856.2186514245 },
    { 0.00000000085, 0.50956043858, 10575.4066829418 },
    { 0.00000000067, 3.62043033405, 16496.3613962024 },
    { 0.00000000055, 5.24637517308, 3340.6124266998 },
};

static double earth_L3[8][3] = {
    { 0.00000289226, 5.84384198723, 6283.0758499914 },
    { 0.00000034955, 0, 0 },
    { 0.00000016819, 5.48766912348, 12566.1516999828 },
    { 0.00000002962, 5.19577265202, 155.4203994342 },
    { 0.00000001288, 4.72200252235, 3.523118349 },
    { 0.00000000635, 5.96925937141, 242.728603974 },
    { 0.00000000714, 5.30045809128, 18849.2275499742 },
    { 0.00000000402, 3.78682982419, 553.5694028424 },
};

static double earth_L4[4][3] = {
    { 0.00000114084, 3.14159265359, 0 },
    { 0.00000007717, 4.13446589358, 6283.0758499914 },
    { 0.00000000765, 3.83803776214, 12566.1516999828 },
    { 0.0000000042, 0.41925861858, 155.4203994342 },
};

static double earth_L5[4][3] = {
    { 0.00000000878, 3.14159265359, 0 },
    { 0.00000000172, 2.7657906951, 6283.0758499914 },
    { 0.0000000005, 2.01353298182, 155.4203994342 },
    { 0.00000000028, 2.21496423926, 12566.1516999828 },
};

/*
static double earth_B0[20][3] = {
    { 0.0000027962, 3.19870156017, 84334.66158130829 },
    { 0.00000101643, 5.42248619256, 5507.5532386674 },
    { 0.00000080445, 3.88013204458, 5223.6939198022 },
    { 0.00000043806, 3.70444689758, 2352.8661537718 },
    { 0.00000031933, 4.00026369781, 1577.3435424478 },
    { 0.00000022724, 3.9847383156, 1047.7473117547 },
    { 0.00000016392, 3.56456119782, 5856.4776591154 },
    { 0.00000018141, 4.98367470263, 6283.0758499914 },
    { 0.00000014443, 3.70275614914, 9437.762934887 },
    { 0.00000014304, 3.41117857525, 10213.285546211 },
    { 0.00000011246, 4.8282069053, 14143.4952424306 },
    { 0.000000109, 2.08574562327, 6812.766815086 },
    { 0.00000009714, 3.47303947752, 4694.0029547076 },
    { 0.00000010367, 4.05663927946, 71092.88135493269 },
    { 0.00000008775, 4.44016515669, 5753.3848848968 },
    { 0.00000008366, 4.9925151218, 7084.8967811152 },
    { 0.00000006921, 4.32559054073, 6275.9623029906 },
    { 0.00000009145, 1.14182646613, 6620.8901131878 },
    { 0.00000007194, 3.60193205752, 529.6909650946 },
    { 0.00000007698, 5.55425745881, 167621.57585086189 },
};

static double earth_B1[12][3] = {
    { 0.0000000903, 3.8972906189, 5507.5532386674 },
    { 0.00000006177, 1.73038850355, 5223.6939198022 },
    { 0.000000038, 5.24404145734, 2352.8661537718 },
    { 0.00000002834, 2.4734503745, 1577.3435424478 },
    { 0.00000001817, 0.41874743765, 6283.0758499914 },
    { 0.00000001499, 1.83320979291, 5856.4776591154 },
    { 0.00000001466, 5.69401926017, 5753.3848848968 },
    { 0.00000001301, 2.18890066314, 9437.762934887 },
    { 0.00000001233, 4.95222451476, 10213.285546211 },
    { 0.00000001021, 0.12866660208, 7860.4193924392 },
    { 0.00000000982, 0.09005453285, 14143.4952424306 },
    { 0.00000000865, 1.73949953555, 3930.2096962196 },
};

static double earth_B2[4][3] = {
    { 0.00000001662, 1.62703209173, 84334.66158130829 },
    { 0.00000000492, 2.41382223971, 1047.7473117547 },
    { 0.00000000344, 2.24353004539, 5507.5532386674 },
    { 0.00000000258, 6.00906896311, 5223.6939198022 },
};

static double earth_B3[1][3] = {
    { 0, 0, 0 },
};

static double earth_B4[1][3] = {
    { 0, 0, 0 },
};

static double earth_R0[72][3] = {
    { 1.00013988799, 0, 0 },
    { 0.01670699626, 3.09846350771, 6283.0758499914 },
    { 0.00013956023, 3.0552460962, 12566.1516999828 },
    { 0.0000308372, 5.19846674381, 77713.7714681205 },
    { 0.00001628461, 1.17387749012, 5753.3848848968 },
    { 0.00001575568, 2.84685245825, 7860.4193924392 },
    { 0.00000924799, 5.45292234084, 11506.7697697936 },
    { 0.00000542444, 4.56409149777, 3930.2096962196 },
    { 0.0000047211, 3.66100022149, 5884.9268465832 },
    { 0.0000032878, 5.89983646482, 5223.6939198022 },
    { 0.00000345983, 0.96368617687, 5507.5532386674 },
    { 0.00000306784, 0.29867139512, 5573.1428014331 },
    { 0.00000174844, 3.01193636534, 18849.2275499742 },
    { 0.00000243189, 4.27349536153, 11790.6290886588 },
    { 0.00000211829, 5.84714540314, 1577.3435424478 },
    { 0.00000185752, 5.02194447178, 10977.078804699 },
    { 0.00000109835, 5.05510636285, 5486.777843175 },
    { 0.00000098316, 0.88681311277, 6069.7767545534 },
    { 0.00000086499, 5.68959778254, 15720.8387848784 },
    { 0.00000085825, 1.27083733351, 161000.6857376741 },
    { 0.00000062916, 0.92177108832, 529.6909650946 },
    { 0.00000057056, 2.01374292014, 83996.84731811189 },
    { 0.00000064903, 0.27250613787, 17260.1546546904 },
    { 0.00000049384, 3.24501240359, 2544.3144198834 },
    { 0.00000055736, 5.24159798933, 71430.69561812909 },
    { 0.00000042515, 6.01110242003, 6275.9623029906 },
    { 0.00000046963, 2.57805070386, 775.522611324 },
    { 0.00000038968, 5.36071738169, 4694.0029547076 },
    { 0.00000044661, 5.53715807302, 9437.762934887 },
    { 0.0000003566, 1.67468058995, 12036.4607348882 },
    { 0.00000031921, 0.18368229781, 5088.6288397668 },
    { 0.00000031846, 1.77775642085, 398.1490034082 },
    { 0.00000033193, 0.24370300098, 7084.8967811152 },
    { 0.00000038245, 2.39255343974, 8827.3902698748 },
    { 0.00000028464, 1.21344868176, 6286.5989683404 },
    { 0.0000003749, 0.82952922332, 19651.048481098 },
    { 0.00000036957, 4.90107591914, 12139.5535091068 },
    { 0.00000034537, 1.84270693282, 2942.4634232916 },
    { 0.00000026275, 4.58896850401, 10447.3878396044 },
    { 0.00000024596, 3.78660875483, 8429.2412664666 },
    { 0.00000023587, 0.26866117066, 796.2980068164 },
    { 0.00000027793, 1.89934330904, 6279.5527316424 },
    { 0.00000023927, 4.99598548138, 5856.4776591154 },
    { 0.00000020349, 4.65267995431, 2146.1654164752 },
    { 0.00000023287, 2.80783650928, 14143.4952424306 },
    { 0.00000022103, 1.95004702988, 3154.6870848956 },
    { 0.00000019506, 5.38227371393, 2352.8661537718 },
    { 0.00000017958, 0.19871379385, 6812.766815086 },
    { 0.00000017174, 4.43315560735, 10213.285546211 },
    { 0.0000001619, 5.23160507859, 17789.845619785 },
    { 0.00000017314, 6.15200787916, 16730.4636895958 },
    { 0.00000013814, 5.18962074032, 8031.0922630584 },
    { 0.00000018833, 0.67306674027, 149854.40013480789 },
    { 0.00000018331, 2.25348733734, 23581.2581773176 },
    { 0.00000013641, 3.68516118804, 4705.7323075436 },
    { 0.00000013139, 0.65289581324, 13367.9726311066 },
    { 0.00000010414, 4.33285688538, 11769.8536931664 },
    { 0.00000009978, 4.20126336355, 6309.3741697912 },
    { 0.00000010169, 1.59390681369, 4690.4798363586 },
    { 0.00000007564, 2.6256059739, 6256.7775301916 },
    { 0.00000009661, 3.6758679122, 27511.4678735372 },
    { 0.00000006743, 0.56270332741, 3340.6124266998 },
    { 0.00000008743, 6.06359123461, 1748.016413067 },
    { 0.00000007786, 3.67371235637, 12168.0026965746 },
    { 0.00000006633, 5.66149277792, 11371.7046897582 },
    { 0.00000007712, 0.31242577789, 7632.9432596502 },
    { 0.00000006592, 3.13576266188, 801.8209311238 },
    { 0.0000000746, 5.64757188143, 11926.2544136688 },
    { 0.00000006933, 2.923845864, 6681.2248533996 },
    { 0.00000006802, 1.4232980642, 23013.5395395872 },
    { 0.00000006115, 5.13393615454, 1194.4470102246 },
    { 0.00000006477, 2.64986648492, 19804.8272915828 },
};

static double earth_R1[10][3] = {
    { 0.00103018608, 1.10748969588, 6283.0758499914 },
    { 0.00001721238, 1.06442301418, 12566.1516999828 },
    { 0.00000702215, 3.14159265359, 0 },
    { 0.00000032346, 1.02169059149, 18849.2275499742 },
    { 0.00000030799, 2.84353804832, 5507.5532386674 },
    { 0.00000024971, 1.31906709482, 5223.6939198022 },
    { 0.00000018485, 1.42429748614, 1577.3435424478 },
    { 0.00000010078, 5.91378194648, 10977.078804699 },
    { 0.00000008634, 0.27146150602, 5486.777843175 },
    { 0.00000008654, 1.42046854427, 6275.9623029906 },
};

static double earth_R2[6][3] = {
    { 0.00004359385, 5.78455133738, 6283.0758499914 },
    { 0.00000123633, 5.57934722157, 12566.1516999828 },
    { 0.00000012341, 3.14159265359, 0 },
    { 0.00000008792, 3.62777733395, 77713.7714681205 },
    { 0.00000005689, 1.86958905084, 5573.1428014331 },
    { 0.00000003301, 5.47027913302, 18849.2275499742 },
};

static double earth_R3[2][3] = {
    { 0.00000144595, 4.27319435148, 6283.0758499914 },
    { 0.00000006729, 3.91697608662, 12566.1516999828 },
};

static double earth_R4[1][3] = {
    { 0.00000003858, 2.56384387339, 6283.0758499914 },
};

static double earth_R5[1][3] = {
    { 0, 0, 0 },
};
*/


/* helper function for calculate VSOP87 */
double vsopLx(double vsopterms[][3], size_t rowcount, double t)
{
    double lx = 0;
    int i;
    for (i = 0; i < rowcount; i++) {
        lx += vsopterms[i][0] * cos(vsopterms[i][1] + vsopterms[i][2] * t);
    }

    return lx;
}


/* Calculate ecliptical longitude of earth in heliocentric coordinates,
 * use VSOP87D table, heliocentric spherical, coordinates referred to the mean
 * equinox of the date,
 *
 * In A&A, Meeus said while the complete VSOP87 yields an accuracy of 0.01",
 * the A&A abridge VSOP87 has an accuracy  of 1" for -2000 - +6000.
 *
 * The VSOP87D table used here is a truncated version, done by the
 * vsoptrunc-sph.c from Celestia.
 *
 * Arg:
 *   jd: in JDTT
 * Return:
 *   earth longitude in radians, referred to mean dynamical ecliptic and
 *   equinox of the date
 */
double vsop(double jd)
{
    double t, lon;
    double L0, L1, L2, L3, L4, L5;
    t = (jd - J2000) / 365250.0;

    L0 = vsopLx(earth_L0, sizeof(earth_L0) / 24, t);
    L1 = vsopLx(earth_L1, sizeof(earth_L1) / 24, t);
    L2 = vsopLx(earth_L2, sizeof(earth_L2) / 24, t);
    L3 = vsopLx(earth_L3, sizeof(earth_L3) / 24, t);
    L4 = vsopLx(earth_L4, sizeof(earth_L4) / 24, t);
    L5 = vsopLx(earth_L5, sizeof(earth_L5) / 24, t);

    lon = (L0 + t * (L1 + t * (L2 + t * (L3 + t * (L4 + t * L5)))));

    /* adjust FK5  */
    lon += -4.379321981462438e-07;

    return lon;
}

/* calculate the apprent place of the Sun.
 * Arg:
 *     jd as jd
 * Return:
 *     geocentric longitude in radians
 */

double apparentsun(double jd, int ignorenutation)
{
    double geolon;
    geolon = vsop(jd) + PI;

    /* compensate nutation */
    if (!ignorenutation)
        geolon += nutation(jd);

    geolon += lightabbr_high(jd);
    return geolon;
}
