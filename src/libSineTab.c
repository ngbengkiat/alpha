#include "project.h"
#include "libTrigo.h"
const int16_t sineTab[] = {
    0,   17,   34,   52,   69,   87,  104,  122,  139,  157,
  174,  191,  209,  226,  244,  261,  279,  296,  314,  331,
  348,  366,  383,  401,  418,  436,  453,  471,  488,  505,
  523,  540,  558,  575,  593,  610,  627,  645,  662,  680,
  697,  714,  732,  749,  767,  784,  801,  819,  836,  854,
  871,  888,  906,  923,  941,  958,  975,  993, 1010, 1027,
 1045, 1062, 1079, 1097, 1114, 1132, 1149, 1166, 1184, 1201,
 1218, 1236, 1253, 1270, 1287, 1305, 1322, 1339, 1357, 1374,
 1391, 1409, 1426, 1443, 1460, 1478, 1495, 1512, 1529, 1547,
 1564, 1581, 1598, 1616, 1633, 1650, 1667, 1684, 1702, 1719,
 1736, 1753, 1770, 1788, 1805, 1822, 1839, 1856, 1873, 1890,
 1908, 1925, 1942, 1959, 1976, 1993, 2010, 2027, 2044, 2062,
 2079, 2096, 2113, 2130, 2147, 2164, 2181, 2198, 2215, 2232,
 2249, 2266, 2283, 2300, 2317, 2334, 2351, 2368, 2385, 2402,
 2419, 2436, 2453, 2469, 2486, 2503, 2520, 2537, 2554, 2571,
 2588, 2605, 2621, 2638, 2655, 2672, 2689, 2706, 2722, 2739,
 2756, 2773, 2789, 2806, 2823, 2840, 2856, 2873, 2890, 2907,
 2923, 2940, 2957, 2973, 2990, 3007, 3023, 3040, 3056, 3073,
 3090, 3106, 3123, 3139, 3156, 3173, 3189, 3206, 3222, 3239,
 3255, 3272, 3288, 3305, 3321, 3338, 3354, 3370, 3387, 3403,
 3420, 3436, 3452, 3469, 3485, 3502, 3518, 3534, 3551, 3567,
 3583, 3599, 3616, 3632, 3648, 3665, 3681, 3697, 3713, 3729,
 3746, 3762, 3778, 3794, 3810, 3826, 3842, 3859, 3875, 3891,
 3907, 3923, 3939, 3955, 3971, 3987, 4003, 4019, 4035, 4051,
 4067, 4083, 4099, 4115, 4131, 4146, 4162, 4178, 4194, 4210,
 4226, 4241, 4257, 4273, 4289, 4305, 4320, 4336, 4352, 4368,
 4383, 4399, 4415, 4430, 4446, 4461, 4477, 4493, 4508, 4524,
 4539, 4555, 4570, 4586, 4601, 4617, 4632, 4648, 4663, 4679,
 4694, 4710, 4725, 4740, 4756, 4771, 4786, 4802, 4817, 4832,
 4848, 4863, 4878, 4893, 4909, 4924, 4939, 4954, 4969, 4984,
 5000, 5015, 5030, 5045, 5060, 5075, 5090, 5105, 5120, 5135,
 5150, 5165, 5180, 5195, 5210, 5224, 5239, 5254, 5269, 5284,
 5299, 5313, 5328, 5343, 5358, 5372, 5387, 5402, 5417, 5431,
 5446, 5461, 5475, 5490, 5504, 5519, 5533, 5548, 5562, 5577,
 5591, 5606, 5620, 5635, 5649, 5664, 5678, 5692, 5707, 5721,
 5735, 5750, 5764, 5778, 5792, 5807, 5821, 5835, 5849, 5863,
 5877, 5891, 5906, 5920, 5934, 5948, 5962, 5976, 5990, 6004,
 6018, 6032, 6045, 6059, 6073, 6087, 6101, 6115, 6129, 6142,
 6156, 6170, 6184, 6197, 6211, 6225, 6238, 6252, 6266, 6279,
 6293, 6306, 6320, 6333, 6347, 6360, 6374, 6387, 6401, 6414,
 6427, 6441, 6454, 6467, 6481, 6494, 6507, 6520, 6534, 6547,
 6560, 6573, 6586, 6600, 6613, 6626, 6639, 6652, 6665, 6678,
 6691, 6704, 6717, 6730, 6743, 6755, 6768, 6781, 6794, 6807,
 6819, 6832, 6845, 6858, 6870, 6883, 6896, 6908, 6921, 6934,
 6946, 6959, 6971, 6984, 6996, 7009, 7021, 7033, 7046, 7058,
 7071, 7083, 7095, 7107, 7120, 7132, 7144, 7156, 7169, 7181,
 7193, 7205, 7217, 7229, 7241, 7253, 7265, 7277, 7289, 7301,
 7313, 7325, 7337, 7349, 7360, 7372, 7384, 7396, 7408, 7419,
 7431, 7443, 7454, 7466, 7477, 7489, 7501, 7512, 7524, 7535,
 7547, 7558, 7569, 7581, 7592, 7604, 7615, 7626, 7637, 7649,
 7660, 7671, 7682, 7693, 7705, 7716, 7727, 7738, 7749, 7760,
 7771, 7782, 7793, 7804, 7815, 7826, 7836, 7847, 7858, 7869,
 7880, 7890, 7901, 7912, 7922, 7933, 7944, 7954, 7965, 7975,
 7986, 7996, 8007, 8017, 8028, 8038, 8048, 8059, 8069, 8079,
 8090, 8100, 8110, 8120, 8131, 8141, 8151, 8161, 8171, 8181,
 8191, 8201, 8211, 8221, 8231, 8241, 8251, 8260, 8270, 8280,
 8290, 8300, 8309, 8319, 8329, 8338, 8348, 8358, 8367, 8377,
 8386, 8396, 8405, 8415, 8424, 8433, 8443, 8452, 8461, 8471,
 8480, 8489, 8498, 8508, 8517, 8526, 8535, 8544, 8553, 8562,
 8571, 8580, 8589, 8598, 8607, 8616, 8625, 8633, 8642, 8651,
 8660, 8668, 8677, 8686, 8694, 8703, 8712, 8720, 8729, 8737,
 8746, 8754, 8763, 8771, 8779, 8788, 8796, 8804, 8813, 8821,
 8829, 8837, 8845, 8853, 8862, 8870, 8878, 8886, 8894, 8902,
 8910, 8917, 8925, 8933, 8941, 8949, 8957, 8964, 8972, 8980,
 8987, 8995, 9003, 9010, 9018, 9025, 9033, 9040, 9048, 9055,
 9063, 9070, 9077, 9085, 9092, 9099, 9106, 9114, 9121, 9128,
 9135, 9142, 9149, 9156, 9163, 9170, 9177, 9184, 9191, 9198,
 9205, 9211, 9218, 9225, 9232, 9238, 9245, 9252, 9258, 9265,
 9271, 9278, 9284, 9291, 9297, 9304, 9310, 9316, 9323, 9329,
 9335, 9342, 9348, 9354, 9360, 9366, 9372, 9378, 9384, 9390,
 9396, 9402, 9408, 9414, 9420, 9426, 9432, 9438, 9443, 9449,
 9455, 9460, 9466, 9472, 9477, 9483, 9488, 9494, 9499, 9505,
 9510, 9515, 9521, 9526, 9531, 9537, 9542, 9547, 9552, 9557,
 9563, 9568, 9573, 9578, 9583, 9588, 9593, 9598, 9602, 9607,
 9612, 9617, 9622, 9626, 9631, 9636, 9640, 9645, 9650, 9654,
 9659, 9663, 9668, 9672, 9677, 9681, 9685, 9690, 9694, 9698,
 9702, 9707, 9711, 9715, 9719, 9723, 9727, 9731, 9735, 9739,
 9743, 9747, 9751, 9755, 9759, 9762, 9766, 9770, 9774, 9777,
 9781, 9785, 9788, 9792, 9795, 9799, 9802, 9806, 9809, 9812,
 9816, 9819, 9822, 9826, 9829, 9832, 9835, 9838, 9841, 9845,
 9848, 9851, 9854, 9857, 9859, 9862, 9865, 9868, 9871, 9874,
 9876, 9879, 9882, 9884, 9887, 9890, 9892, 9895, 9897, 9900,
 9902, 9905, 9907, 9909, 9912, 9914, 9916, 9918, 9921, 9923,
 9925, 9927, 9929, 9931, 9933, 9935, 9937, 9939, 9941, 9943,
 9945, 9947, 9948, 9950, 9952, 9953, 9955, 9957, 9958, 9960,
 9961, 9963, 9964, 9966, 9967, 9969, 9970, 9971, 9973, 9974,
 9975, 9976, 9978, 9979, 9980, 9981, 9982, 9983, 9984, 9985,
 9986, 9987, 9988, 9988, 9989, 9990, 9991, 9991, 9992, 9993,
 9993, 9994, 9995, 9995, 9996, 9996, 9997, 9997, 9997, 9998,
 9998, 9998, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
10000,
};

const int16_t tangentTab[] = {
    0,   17,   34,   52,   69,   87,  104,  122,  139,  157,
  174,  192,  209,  226,  244,  261,  279,  296,  314,  331,
  349,  366,  384,  401,  419,  436,  454,  471,  489,  506,
  524,  541,  559,  576,  594,  611,  629,  646,  664,  681,
  699,  716,  734,  751,  769,  787,  804,  822,  839,  857,
  874,  892,  910,  927,  945,  962,  980,  998, 1015, 1033,
 1051, 1068, 1086, 1104, 1121, 1139, 1157, 1174, 1192, 1210,
 1227, 1245, 1263, 1281, 1298, 1316, 1334, 1352, 1369, 1387,
 1405, 1423, 1441, 1458, 1476, 1494, 1512, 1530, 1548, 1565,
 1583, 1601, 1619, 1637, 1655, 1673, 1691, 1709, 1727, 1745,
 1763, 1781, 1799, 1817, 1835, 1853, 1871, 1889, 1907, 1925,
 1943, 1961, 1980, 1998, 2016, 2034, 2052, 2070, 2089, 2107,
 2125, 2143, 2162, 2180, 2198, 2216, 2235, 2253, 2271, 2290,
 2308, 2327, 2345, 2363, 2382, 2400, 2419, 2437, 2456, 2474,
 2493, 2511, 2530, 2548, 2567, 2586, 2604, 2623, 2642, 2660,
 2679, 2698, 2716, 2735, 2754, 2773, 2792, 2810, 2829, 2848,
 2867, 2886, 2905, 2924, 2943, 2962, 2981, 3000, 3019, 3038,
 3057, 3076, 3095, 3114, 3133, 3152, 3172, 3191, 3210, 3229,
 3249, 3268, 3287, 3307, 3326, 3345, 3365, 3384, 3404, 3423,
 3443, 3462, 3482, 3501, 3521, 3541, 3560, 3580, 3600, 3619,
 3639, 3659, 3679, 3699, 3718, 3738, 3758, 3778, 3798, 3818,
 3838, 3858, 3878, 3898, 3918, 3939, 3959, 3979, 3999, 4019,
 4040, 4060, 4080, 4101, 4121, 4142, 4162, 4183, 4203, 4224,
 4244, 4265, 4286, 4306, 4327, 4348, 4368, 4389, 4410, 4431,
 4452, 4473, 4494, 4515, 4536, 4557, 4578, 4599, 4620, 4641,
 4663, 4684, 4705, 4726, 4748, 4769, 4791, 4812, 4834, 4855,
 4877, 4898, 4920, 4942, 4964, 4985, 5007, 5029, 5051, 5073,
 5095, 5117, 5139, 5161, 5183, 5205, 5227, 5250, 5272, 5294,
 5317, 5339, 5361, 5384, 5406, 5429, 5452, 5474, 5497, 5520,
 5543, 5565, 5588, 5611, 5634, 5657, 5680, 5703, 5727, 5750,
 5773, 5796, 5820, 5843, 5866, 5890, 5913, 5937, 5961, 5984,
 6008, 6032, 6056, 6080, 6104, 6128, 6152, 6176, 6200, 6224,
 6248, 6272, 6297, 6321, 6346, 6370, 6395, 6419, 6444, 6469,
 6494, 6518, 6543, 6568, 6593, 6618, 6643, 6669, 6694, 6719,
 6745, 6770, 6795, 6821, 6847, 6872, 6898, 6924, 6950, 6976,
 7002, 7028, 7054, 7080, 7106, 7132, 7159, 7185, 7212, 7238,
 7265, 7292, 7318, 7345, 7372, 7399, 7426, 7453, 7480, 7508,
 7535, 7562, 7590, 7617, 7645, 7673, 7701, 7728, 7756, 7784,
 7812, 7841, 7869, 7897, 7925, 7954, 7982, 8011, 8040, 8068,
 8097, 8126, 8155, 8184, 8214, 8243, 8272, 8302, 8331, 8361,
 8390, 8420, 8450, 8480, 8510, 8540, 8571, 8601, 8631, 8662,
 8692, 8723, 8754, 8785, 8816, 8847, 8878, 8909, 8941, 8972,
 9004, 9035, 9067, 9099, 9131, 9163, 9195, 9227, 9260, 9292,
 9325, 9357, 9390, 9423, 9456, 9489, 9522, 9556, 9589, 9623,
 9656, 9690, 9724, 9758, 9792, 9826, 9861, 9895, 9930, 9965,
10000,
};
